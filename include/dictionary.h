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
#pragma once
#include <csignal>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

extern std::shared_mutex dictionaries_mutex;

bool load_dictionaries(std::unordered_set<std::string>& all_words, std::unordered_map<std::string, std::unordered_set<std::string>>& lang_words);

bool install_dictionary_reload_handler();

void process_dictionary_reload(std::unordered_set<std::string>& all_words, std::unordered_map<std::string, std::unordered_set<std::string>>& lang_words);
