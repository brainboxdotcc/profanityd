#include "strip_zero_width.h"

void strip_zero_width(std::string& s)
{
    std::string out;

    for (size_t i = 0; i < s.size();) {

        if (i + 2 < s.size()) {

            unsigned char a = s[i];
            unsigned char b = s[i + 1];
            unsigned char c = s[i + 2];

            // U+200B..U+200D
            if (a == 0xE2 && b == 0x80 &&
                (c == 0x8B || c == 0x8C || c == 0x8D)) {
                i += 3;
                continue;
            }

            // U+FEFF
            if (a == 0xEF && b == 0xBB && c == 0xBF) {
                i += 3;
                continue;
            }
        }

        out += s[i++];
    }

    s.swap(out);
}
