#include "../include/RadixTree.hpp"
#include <memory>

RadixTree::RadixTree(){
  m_root = std::make_unique<RadixNode>();
}

void RadixTree::insertWorker(RadixNode* node, std::string_view word){
  RadixNode* currentNode = node;

  for(auto& child : currentNode->m_nextNodes){
    if(child.first == word.at(0)){
      auto* childNode = child.second.get();
      std::string_view label = childNode->m_prefix;

      size_t matchingLen = 0;
      while (matchingLen < word.length() && matchingLen < label.length() &&
             word[matchingLen] == label[matchingLen]) {
        matchingLen++;
      }

      std::string_view end = word.substr(matchingLen);     

      if(matchingLen == label.length()){
        if(end.empty()){
          childNode->m_isWord = true;
        } else {
          insertWorker(childNode, end);
        }
      } else {
        auto splitNode = std::make_unique<RadixNode>();

        splitNode->m_prefix = label.substr(matchingLen);
        splitNode->m_isWord = childNode->m_isWord;
        splitNode->m_nextNodes = std::move(childNode->m_nextNodes);
        
        childNode->m_prefix = label.substr(0, matchingLen);
        childNode->m_isWord = false;
        childNode->m_nextNodes.emplace_back(splitNode->m_prefix.at(0), std::move(splitNode));
        
        if(end.empty()){
          childNode->m_isWord = true;
        } else {
          auto newNode = std::make_unique<RadixNode>();

          newNode->m_prefix = end;
          newNode->m_isWord = true;
          childNode->m_nextNodes.emplace_back(end.at(0), std::move(newNode));
        }
      }
      
      return;
    }
  }

  auto newNode = std::make_unique<RadixNode>();

  newNode->m_prefix = word;

  newNode->m_isWord = true;

  currentNode->m_nextNodes.emplace_back(word.at(0), std::move(newNode));
}

void RadixTree::insert(std::string_view word){
  if(word.empty()){
    return;
  }

  insertWorker(m_root.get(), word);
}

auto RadixTree::containsWorker(const RadixNode* node, std::string_view word) const -> bool{
  for(const auto& child : node->m_nextNodes){
    if(child.first == word.at(0)){
      RadixNode* childNode = child.second.get();
      std::string_view label = childNode->m_prefix;

       size_t matchingLen = 0;
      while (matchingLen < word.length() && matchingLen < label.length() &&
             word[matchingLen] == label[matchingLen]) {
        matchingLen++;
      }

      if(matchingLen < label.length()){
        return false;
      }

      std::string_view remainingWord = word.substr(matchingLen);

      if(remainingWord.empty()){
        return childNode->m_isWord;
      }          

      return containsWorker(childNode, remainingWord);
     
    }
  }

  return false;
}

auto RadixTree::contains(std::string_view word) const -> bool{
  return containsWorker(m_root.get(), word);
}

void RadixTree::collectWords(const RadixNode* node, 
                             const std::string& currentString, 
                             std::vector<std::string>& results) const{
  if(node->m_isWord){
    results.push_back(currentString);
  }

  for(const auto& child : node->m_nextNodes){
    collectWords(child.second.get(), currentString+child.second->m_prefix, results);
  }
}

auto RadixTree::getPredictions(std::string_view prefix) 
  const -> std::vector<std::string>{
  std::vector<std::string> results;
  const RadixNode* currentNode = m_root.get();

  size_t currentLen = 0;

  while(currentLen < prefix.length()){
    bool foundNext = false;

    for(const auto& child : currentNode->m_nextNodes){
      if(child.first == prefix[currentLen]){
        const RadixNode* childNode = child.second.get();
        std::string_view label = childNode->m_prefix;

        size_t matchingLen = 0;
        while(matchingLen < label.length()&&
              currentLen + matchingLen < prefix.length()&&
              label[matchingLen] == prefix[currentLen+matchingLen]){
          matchingLen++;
        }

        if(currentLen + matchingLen == prefix.length()){
          std::string baseString = std::string(prefix);
          baseString += label.substr(matchingLen);

          collectWords(childNode, baseString, results);
          return results;
        }

        if(matchingLen == label.length()){
          currentNode = childNode;
          currentLen += matchingLen;
          foundNext = true;
          break;
        }

        return results;
      }
    }

    if(!foundNext) { return results; }
  }

  collectWords(currentNode, std::string(prefix), results);
  return results;
}
