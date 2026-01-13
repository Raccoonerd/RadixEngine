#include "../include/DictionaryLoader.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

DictionaryLoader::DictionaryLoader(const std::string& path){
  std::ifstream file;
  file.open(path , std::ifstream::in);
  
  if(!file.is_open()){
    std::cerr << "File not found: " << path << '\n';
    return;
  }

  size_t fileSize = std::filesystem::file_size(path);

  m_words.reserve(fileSize / 4);

  std::string line;

  while(std::getline(file, line)){
    if(line.ends_with('\r')){
      line.pop_back();
    }

    m_words.emplace_back(line);
  }

  file.close();
}

auto DictionaryLoader::getWords() -> std::vector<std::string>&{
  return m_words;
}
