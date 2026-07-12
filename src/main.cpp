#include <drogon/drogon.h>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "dictionary.h"
#include "normalise.h"
#include "collapse_repeated.h"

using namespace drogon;

std::unordered_set<std::string> all_words;
std::unordered_map<std::string, std::unordered_set<std::string>> lang_words;

bool contains(const std::string& word, const std::string& english_word, const std::vector<std::string>& langs) {
	if (langs.empty()) {
		if (all_words.find(word) != all_words.end()) {
			return true;
		}
		const auto english = lang_words.find("en");
		return english != lang_words.end() && contains_with_reduced_repeats(english->second, english_word);
	}

	for (const auto& lang : langs) {
		const auto dictionary = lang_words.find(lang);

		if (dictionary == lang_words.end()) {
			continue;
		}

		const auto& candidate = lang == "en" ? english_word : word;

		if (dictionary->second.find(candidate) != dictionary->second.end()) {
			return true;
		}

		if (lang == "en" && contains_with_reduced_repeats(dictionary->second, candidate)) {
			return true;
		}
	}

	return false;
}

std::string censor(const std::string& text, char replacement, const std::vector<std::string>& langs) {
	std::string out, token;

	auto flush = [&]() {
		if (token.empty()) {
			return;
		}

		auto normalised = token;
		normalise(normalised);

		auto english = token;
		normalise_english(english);

		if (!normalised.empty() && contains(normalised, english, langs)) {
			out.append(english.size(), replacement);
		} else {
			out += token;
		}

		token.clear();
	};

	for (unsigned char c : text) {
		if (c < 0x80 && std::isspace(c)) {
			flush();
			out += static_cast<char>(c);
		} else {
			token += static_cast<char>(c);
		}
	}

	flush();

	return out;
}

int main() {

	load_dictionaries(all_words, lang_words);

	app()
	.enableRunAsDaemon()
	.setThreadNum(std::thread::hardware_concurrency())
	.setClientMaxBodySize(128 * 1024)
	.registerHandler("/bad-word-filter",
		[](const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& cb) {
			Json::Value body;

			if (req->getJsonObject()) {
				body = *req->getJsonObject();
			}

			std::string content = body.get("content", "").asString();

			char repl = '#';
			auto censor_char = body.get("censor-character", "#").asString();
			if (!censor_char.empty()) {
				repl = censor_char[0];
			}

			std::vector<std::string> langs;

			if (body.isMember("languages")) {
				for (const auto& l : body["languages"]) {
					langs.push_back(l.asString());
				}
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
