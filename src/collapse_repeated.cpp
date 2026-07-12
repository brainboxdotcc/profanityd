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
#include "collapse_repeated.h"
#include "utf8.h"

void collapse_repeated(std::string& s) {
	std::string out;

	for (size_t i = 0; i < s.size();) {
		size_t j = i + 1;

		while (j < s.size() && s[j] == s[i])
			++j;

		size_t run = j - i;

		out.append(run >= 3 ? 1 : run, s[i]);

		i = j;
	}

	s.swap(out);
}

std::vector<character_run_t> character_runs(const std::string& text) {
	std::vector<character_run_t> runs;

	for (std::size_t offset = 0; offset < text.size();) {
		std::size_t length = utf8_length(static_cast<unsigned char>(text[offset]));

		if (offset + length > text.size()) {
			length = 1;
		}

		const std::string character = text.substr(offset, length);

		if (!runs.empty() && runs.back().character == character) {
			++runs.back().count;
		} else {
			runs.push_back({character, 1});
		}

		offset += length;
	}

	return runs;
}

std::vector<std::string> reduced_repeat_variants(const std::string& text) {
	const auto runs = character_runs(text);
	std::vector<std::string> variants = {""};

	for (const auto& run : runs) {
		std::vector<std::string> next;
		next.reserve(variants.size() * (run.count > 1 ? 2 : 1));

		for (const auto& variant : variants) {
			std::string unchanged = variant;

			for (std::size_t i = 0; i < run.count; ++i) {
				unchanged += run.character;
			}

			next.push_back(std::move(unchanged));

			if (run.count > 1) {
				next.push_back(variant + run.character);
			}
		}

		variants = std::move(next);
	}

	return variants;
}

bool contains_with_reduced_repeats(const std::unordered_set<std::string>& dictionary, const std::string& word) {
	if (dictionary.find(word) != dictionary.end()) {
		return true;
	}

	for (const auto& candidate : reduced_repeat_variants(word)) {
		if (candidate == word) {
			continue;
		}

		if (dictionary.find(candidate) != dictionary.end()) {
			return true;
		}
	}

	return false;
}
