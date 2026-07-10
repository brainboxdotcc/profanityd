#include "utf8.h"
#include "homoglyphs.h"
#include <cstdint>
#include <string>
#include <unordered_map>
#include "confusables.h"

void fold_homoglyphs(std::string& text) {
	std::string output;
	output.reserve(text.size());

	for (std::size_t i = 0; i < text.size();) {
		const auto length = utf8_length(
			static_cast<unsigned char>(text[i])
		);

		if (i + length > text.size()) {
			output.append(text, i, text.size() - i);
			break;
		}

		const auto codepoint = decode_utf8(text, i, length);
		const auto mapping = confusables.find(codepoint);

		if (mapping != confusables.end()) {
			output += mapping->second;
		} else {
			output.append(text, i, length);
		}

		i += length;
	}

	text.swap(output);
}
