#include "../include/DictionaryLoader.hpp"

DictionaryLoader::DictionaryLoader(const std::string& path){
  m_file.open(path , std::ifstream::in);


}

auto DictionaryLoader::getWords() const -> std::vector<std::string>{
  return m_words;
}
