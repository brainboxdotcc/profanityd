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
#include "strip_zalgo.h"
#include "utf8.h"

static bool is_combining_mark(char32_t codepoint) {
	return
		(codepoint >= 0x0300 && codepoint <= 0x036f) ||
		(codepoint >= 0x1ab0 && codepoint <= 0x1aff) ||
		(codepoint >= 0x1dc0 && codepoint <= 0x1dff) ||
		(codepoint >= 0x20d0 && codepoint <= 0x20ff) ||
		(codepoint >= 0xfe20 && codepoint <= 0xfe2f);
}

void strip_zalgo(std::string& text) {
	std::string output;
	output.reserve(text.size());

	for (std::size_t i = 0; i < text.size();) {
		const auto length = utf8_length(static_cast<unsigned char>(text[i]));

		if (i + length > text.size()) {
			output.append(text, i, text.size() - i);
			break;
		}

		const auto codepoint = decode_utf8(text, i, length);

		if (!is_combining_mark(codepoint)) {
			output.append(text, i, length);
		}

		i += length;
	}

	text.swap(output);
}
