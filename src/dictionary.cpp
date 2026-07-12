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
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void load_dictionaries(std::unordered_set<std::string> all_words, std::unordered_map<std::string, std::unordered_set<std::string>> lang_words) {
	size_t count = 0;

	for (const auto& f : fs::directory_iterator("../dictionaries")) {
		if (!f.is_regular_file() || f.path().extension() != ".txt") {
			continue;
		}
		std::ifstream in(f.path());
		if (!in) {
			continue;
		}
		std::string language = f.path().stem().string();
		auto& dict = lang_words[language];
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
			dict.insert(line);
			all_words.insert(line);
			++count;
		}
	}

	std::cout << "Loaded " << count << " words in " << lang_words.size() << " languages\n";
}
