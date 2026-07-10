#pragma once
#include <string>
#include <cstdint>

std::size_t utf8_length(unsigned char c);
uint32_t decode_utf8(const std::string& text, std::size_t offset, std::size_t length);

