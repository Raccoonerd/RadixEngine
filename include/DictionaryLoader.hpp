#pragma once
#include <string>
#include <vector>

class DictionaryLoader{
  std::vector<std::string> m_words;

public:
  DictionaryLoader(const std::string& path);
  auto getWords() -> std::vector<std::string>&;
};
