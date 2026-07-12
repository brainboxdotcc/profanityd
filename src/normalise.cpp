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
#include "normalise.h"
#include "lowercase.h"
#include "strip_zero_width.h"
#include "leetspeak.h"
#include "collapse_repeated.h"
#include "accent_flatten.h"
#include "homoglyphs.h"
#include "strip_zalgo.h"
#include "strip_formatting.h"

void normalise_english(std::string& text) {
	strip_zero_width(text);
	strip_zalgo(text);
	strip_formatting(text);
	fold_homoglyphs(text);
	flatten_accents(text);
	lowercase(text);
	leetspeak(text);
	collapse_repeated(text);
}

void normalise(std::string& s) {
	lowercase(s);
	strip_zero_width(s);
	strip_formatting(s);
	leetspeak(s);
	collapse_repeated(s);
}
