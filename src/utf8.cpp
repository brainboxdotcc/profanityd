#include "utf8.h"
#include <string>

std::size_t utf8_length(unsigned char c) {
	if ((c & 0x80) == 0) {
		return 1;
	}
	if ((c & 0xE0) == 0xC0) {
		return 2;
	}
	if ((c & 0xF0) == 0xE0) {
		return 3;
	}
	if ((c & 0xF8) == 0xF0) {
		return 4;
	}
	return 1;
}

uint32_t decode_utf8(const std::string& text, std::size_t offset, std::size_t length) {
	const auto first = static_cast<unsigned char>(text[offset]);

	if (length == 1) {
		return first;
	}
	uint32_t cp = first & (0x7F >> length);
	for (std::size_t i = 1; i < length; ++i) {
		cp = (cp << 6) | (static_cast<unsigned char>(text[offset + i]) & 0x3F);
	}

	return cp;
}
