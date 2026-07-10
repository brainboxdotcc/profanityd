#include "leetspeak.h"

void leetspeak(std::string& s) {
	for (char& c : s) {

		switch (c) {

			case '@':
			case '4':
				c = 'a';
				break;

			case '3':
				c = 'e';
				break;

			case '0':
				c = 'o';
				break;

			case '$':
			case '5':
				c = 's';
				break;

			case '7':
			case '+':
				c = 't';
				break;

			default:
				break;
		}
	}
}
