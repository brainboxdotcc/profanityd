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
#include "strip_formatting.h"
#include <cctype>
#include <cstdint>
#include <string>
#include <utility>
#include "utf8.h"

bool is_unicode_punctuation(uint32_t codepoint)
{
	return
		(codepoint >= 0x2000 && codepoint <= 0x206F) ||
		(codepoint >= 0x2E00 && codepoint <= 0x2E7F) ||
		(codepoint >= 0x3000 && codepoint <= 0x303F) ||
		(codepoint >= 0xFE10 && codepoint <= 0xFE1F) ||
		(codepoint >= 0xFE30 && codepoint <= 0xFE4F) ||
		(codepoint >= 0xFF01 && codepoint <= 0xFF0F) ||
		(codepoint >= 0xFF1A && codepoint <= 0xFF20) ||
		(codepoint >= 0xFF3B && codepoint <= 0xFF40) ||
		(codepoint >= 0xFF5B && codepoint <= 0xFF65);
}

bool is_emoji(uint32_t codepoint)
{
	return
		(codepoint >= 0x2600 && codepoint <= 0x27BF) ||
		(codepoint >= 0x1F000 && codepoint <= 0x1FAFF) ||
		(codepoint >= 0x1F3FB && codepoint <= 0x1F3FF) ||
		(codepoint >= 0xFE00 && codepoint <= 0xFE0F) ||
		codepoint == 0x200D;
}

bool should_strip(uint32_t codepoint)
{
	if (codepoint < 0x80) {
		return std::ispunct(static_cast<unsigned char>(codepoint)) != 0;
	}

	return is_unicode_punctuation(codepoint) || is_emoji(codepoint);
}

void strip_formatting(std::string& text)
{
	std::string output;
	output.reserve(text.size());

	for (std::size_t offset = 0; offset < text.size();) {
		std::size_t length = utf8_length(
			static_cast<unsigned char>(text[offset])
		);

		if (offset + length > text.size()) {
			length = 1;
		}

		const uint32_t codepoint = decode_utf8(
			text,
			offset,
			length
		);

		if (!should_strip(codepoint)) {
			output.append(text, offset, length);
		}

		offset += length;
	}

	text = std::move(output);
}
