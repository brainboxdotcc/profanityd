#include <drogon/drogon.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "lowercase.h"
#include "strip_zero_width.h"
#include "leetspeak.h"
#include "collapse_repeated.h"

using namespace drogon;
namespace fs = std::filesystem;

std::unordered_set<std::string> all_words;
std::unordered_map<std::string, std::unordered_set<std::string>> lang_words;

void normalise(std::string& s)
{
	lowercase(s);
	strip_zero_width(s);
	leetspeak(s);
	collapse_repeated(s);
}

void load_dictionaries()
{
	size_t count = 0;

	for (const auto& f : fs::directory_iterator("../dictionaries")) {
		if (!f.is_regular_file() || f.path().extension() != ".txt")
			continue;

		std::ifstream in(f.path());
		if (!in)
			continue;

		auto& dict = lang_words[f.path().stem().string()];
		std::string line;

		while (std::getline(in, line)) {
			if (line.empty())
				continue;

			normalise(line);
			dict.insert(line);
			all_words.insert(line);
			++count;
		}
	}

	std::cout << "Loaded " << count << " words in "
	          << lang_words.size() << " languages\n";
}

bool contains(const std::string& word, const std::vector<std::string>& langs)
{
	if (langs.empty())
		return all_words.contains(word);

	for (const auto& l : langs) {
		auto i = lang_words.find(l);
		if (i != lang_words.end() && i->second.contains(word))
			return true;
	}

	return false;
}

std::string censor(const std::string& text, char replacement, const std::vector<std::string>& langs)
{
	std::string out, token;

	auto flush = [&]() {
		if (!token.empty()) {
			auto n = token;
			normalise(n);

			if (contains(n, langs))
				out.append(token.size(), replacement);
			else
				out += token;

			token.clear();
		}
	};

	for (unsigned char c : text) {
		if (c < 0x80 && (std::isspace(c) || std::ispunct(c))) {
			flush();
			out += (char)c;
		} else {
			token += (char)c;
		}
	}

	flush();

	return out;
}

int main()
{
	load_dictionaries();

	app()
		.enableRunAsDaemon()
		.setThreadNum(std::thread::hardware_concurrency())
		.setClientMaxBodySize(128 * 1024)
		.registerHandler("/bad-word-filter",
		[](const HttpRequestPtr& req,
		   std::function<void(const HttpResponsePtr&)>&& cb)
		{
			Json::Value body;

			if (req->getJsonObject())
				body = *req->getJsonObject();

			std::string content = body.get("content", "").asString();

			char repl = '#';
			auto censor_char = body.get("censor-character", "#").asString();
			if (!censor_char.empty())
				repl = censor_char[0];

			std::vector<std::string> langs;

			if (body.isMember("languages")) {
				for (const auto& l : body["languages"])
					langs.push_back(l.asString());
			}

			auto censored = censor(content, repl, langs);

			Json::Value reply;
			reply["is-bad"] = censored != content;
			reply["censored-content"] = censored;

			auto resp = HttpResponse::newHttpJsonResponse(reply);
			cb(resp);
		},
		{Post});

	app().addListener("127.0.0.1", 6970);
	app().run();
}
