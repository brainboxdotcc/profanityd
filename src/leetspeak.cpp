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
