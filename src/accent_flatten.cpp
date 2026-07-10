#include "accent_flatten.h"

#include <cstdint>

static char flatten_latin(uint32_t cp)
{
    switch (cp) {
        case U'À': case U'Á': case U'Â': case U'Ã': case U'Ä': case U'Å':
        case U'Ā': case U'Ă': case U'Ą':
            return 'A';

        case U'à': case U'á': case U'â': case U'ã': case U'ä': case U'å':
        case U'ā': case U'ă': case U'ą':
            return 'a';

        case U'Ç': case U'Ć': case U'Ĉ': case U'Ċ': case U'Č':
            return 'C';

        case U'ç': case U'ć': case U'ĉ': case U'ċ': case U'č':
            return 'c';

        case U'Ð': case U'Ď': case U'Đ':
            return 'D';

        case U'ð': case U'ď': case U'đ':
            return 'd';

        case U'È': case U'É': case U'Ê': case U'Ë':
        case U'Ē': case U'Ĕ': case U'Ė': case U'Ę': case U'Ě':
            return 'E';

        case U'è': case U'é': case U'ê': case U'ë':
        case U'ē': case U'ĕ': case U'ė': case U'ę': case U'ě':
            return 'e';

        case U'Ĝ': case U'Ğ': case U'Ġ': case U'Ģ':
            return 'G';

        case U'ĝ': case U'ğ': case U'ġ': case U'ģ':
            return 'g';

        case U'Ĥ': case U'Ħ':
            return 'H';

        case U'ĥ': case U'ħ':
            return 'h';

        case U'Ì': case U'Í': case U'Î': case U'Ï':
        case U'Ĩ': case U'Ī': case U'Ĭ': case U'Į': case U'İ':
            return 'I';

        case U'ì': case U'í': case U'î': case U'ï':
        case U'ĩ': case U'ī': case U'ĭ': case U'į': case U'ı':
            return 'i';

        case U'Ĵ':
            return 'J';

        case U'ĵ':
            return 'j';

        case U'Ķ':
            return 'K';

        case U'ķ':
            return 'k';

        case U'Ĺ': case U'Ļ': case U'Ľ': case U'Ŀ': case U'Ł':
            return 'L';

        case U'ĺ': case U'ļ': case U'ľ': case U'ŀ': case U'ł':
            return 'l';

        case U'Ñ': case U'Ń': case U'Ņ': case U'Ň':
            return 'N';

        case U'ñ': case U'ń': case U'ņ': case U'ň':
            return 'n';

        case U'Ò': case U'Ó': case U'Ô': case U'Õ': case U'Ö': case U'Ø':
        case U'Ō': case U'Ŏ': case U'Ő':
            return 'O';

        case U'ò': case U'ó': case U'ô': case U'õ': case U'ö': case U'ø':
        case U'ō': case U'ŏ': case U'ő':
            return 'o';

        case U'Ŕ': case U'Ŗ': case U'Ř':
            return 'R';

        case U'ŕ': case U'ŗ': case U'ř':
            return 'r';

        case U'Ś': case U'Ŝ': case U'Ş': case U'Š':
            return 'S';

        case U'ś': case U'ŝ': case U'ş': case U'š':
            return 's';

        case U'Ţ': case U'Ť': case U'Ŧ':
            return 'T';

        case U'ţ': case U'ť': case U'ŧ':
            return 't';

        case U'Ù': case U'Ú': case U'Û': case U'Ü':
        case U'Ũ': case U'Ū': case U'Ŭ': case U'Ů': case U'Ű': case U'Ų':
            return 'U';

        case U'ù': case U'ú': case U'û': case U'ü':
        case U'ũ': case U'ū': case U'ŭ': case U'ů': case U'ű': case U'ų':
            return 'u';

        case U'Ŵ':
            return 'W';

        case U'ŵ':
            return 'w';

        case U'Ý': case U'Ŷ': case U'Ÿ':
            return 'Y';

        case U'ý': case U'ÿ': case U'ŷ':
            return 'y';

        case U'Ź': case U'Ż': case U'Ž':
            return 'Z';

        case U'ź': case U'ż': case U'ž':
            return 'z';

        default:
            return 0;
    }
}

static std::size_t utf8_length(unsigned char c)
{
    if ((c & 0x80) == 0)
        return 1;
    if ((c & 0xE0) == 0xC0)
        return 2;
    if ((c & 0xF0) == 0xE0)
        return 3;
    if ((c & 0xF8) == 0xF0)
        return 4;
    return 1;
}

static uint32_t decode_utf8(const std::string& text, std::size_t offset, std::size_t length)
{
    const auto first = static_cast<unsigned char>(text[offset]);

    if (length == 1)
        return first;

    uint32_t cp = first & (0x7F >> length);

    for (std::size_t i = 1; i < length; ++i) {
        cp = (cp << 6) |
            (static_cast<unsigned char>(text[offset + i]) & 0x3F);
    }

    return cp;
}

void flatten_accents(std::string& text)
{
    std::string output;
    output.reserve(text.size());

    for (std::size_t i = 0; i < text.size();) {
        const std::size_t length = utf8_length(
            static_cast<unsigned char>(text[i])
        );

        if (i + length > text.size()) {
            output.append(text, i, text.size() - i);
            break;
        }

        const uint32_t cp = decode_utf8(text, i, length);
        const char flattened = flatten_latin(cp);

        if (flattened != 0)
            output += flattened;
        else
            output.append(text, i, length);

        i += length;
    }

    text.swap(output);
}
