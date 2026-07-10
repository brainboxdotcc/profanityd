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
