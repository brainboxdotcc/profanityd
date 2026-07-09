#include "collapse_repeated.h"

void collapse_repeated(std::string& s)
{
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
