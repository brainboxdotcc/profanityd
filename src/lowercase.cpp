#include "lowercase.h"

#include <cctype>

void lowercase(std::string& s) {
	for (char& c : s) {
		c = (char)std::tolower((unsigned char)c);
	}
}
