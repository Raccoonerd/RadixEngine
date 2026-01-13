#pragma once
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct RadixNode{
  std::string m_prefix;
  bool m_isWord = false;
  std::vector<std::pair<char, std::unique_ptr<RadixNode>>> m_nextNodes;
};



class RadixTree{
  std::unique_ptr<RadixNode> m_root;

  void insertWorker(RadixNode* node, std::string_view word);

  auto containsWorker(const RadixNode* node, std::string_view word) const -> bool;

  void collectWords(const RadixNode* node, 
                    const std::string& currentString, 
                    std::vector<std::string>& results) const;
public:
  RadixTree();

  void insert(std::string_view word);

  [[nodiscard]] auto contains(std::string_view word) const -> bool;

  [[nodiscard]] auto getPredictions(std::string_view prefix) const -> std::vector<std::string>;
};

