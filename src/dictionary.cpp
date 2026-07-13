/************************************************************************************
 *
 * profanityd, a neutrino API compatible multi language censor API
 *
 * Copyright 2026 Craig Edwards
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#include "dictionary.h"
#include "normalise.h"
#include <drogon/drogon.h>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>

namespace fs = std::filesystem;

std::shared_mutex dictionaries_mutex;
volatile std::sig_atomic_t dictionary_reload_requested = 0;

void dictionary_reload_signal(int signal) {
	if (signal == SIGHUP) {
		dictionary_reload_requested = 1;
	}
}

bool install_dictionary_reload_handler() {
	struct sigaction action{};

	action.sa_handler = dictionary_reload_signal;
	action.sa_flags = SA_RESTART;
	sigemptyset(&action.sa_mask);

	if (sigaction(SIGHUP, &action, nullptr) == -1) {
		std::cerr << "Unable to install SIGHUP handler: " << std::strerror(errno);
		return false;
	}

	return true;
}

bool load_dictionaries(std::unordered_set<std::string>& all_words, std::unordered_map<std::string, std::unordered_set<std::string>>& lang_words) {
	std::unordered_set<std::string> new_all_words;
	std::unordered_map<std::string, std::unordered_set<std::string>> new_lang_words;
	size_t count = 0;

	try {
		for (const auto& f : fs::directory_iterator("../dictionaries")) {
			if (!f.is_regular_file() || f.path().extension() != ".txt") {
				continue;
			}

			std::ifstream in(f.path());

			if (!in) {
				return false;
			}

			const std::string language = f.path().stem().string();
			auto& dict = new_lang_words[language];
			std::string line;

			while (std::getline(in, line)) {
				if (line.empty()) {
					continue;
				}

				if (language == "en") {
					normalise_english(line);
				} else {
					normalise(line);
				}

				if (line.empty()) {
					continue;
				}

				dict.insert(line);
				new_all_words.insert(line);
				++count;
			}
		}
	} catch (const std::exception& error) {
		return false;
	}

	if (new_lang_words.empty()) {
		return false;
	}

	{
		std::unique_lock<std::shared_mutex> lock(dictionaries_mutex);
		all_words.swap(new_all_words);
		lang_words.swap(new_lang_words);
	}

	std::cout << "Loaded " << count << " words in " << lang_words.size() << " languages" << std::endl;
	return true;
}

void process_dictionary_reload(std::unordered_set<std::string>& all_words, std::unordered_map<std::string, std::unordered_set<std::string>>& lang_words) {
	if (dictionary_reload_requested == 0) {
		return;
	}
	dictionary_reload_requested = 0;
	load_dictionaries(all_words, lang_words);
}