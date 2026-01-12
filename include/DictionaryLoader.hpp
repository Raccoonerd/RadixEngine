#pragma once
#include <fstream>
#include <string>
#include <vector>

class DictionaryLoader{
  std::ifstream m_file;
  std::vector<std::string> m_words;

public:
  DictionaryLoader(const std::string& path);
  auto getWords() const -> std::vector<std::string>;
};
