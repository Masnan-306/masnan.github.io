// RadixTrie.cpp
#include "RadixTrie.h"
#include <iostream>

template <typename O>
std::shared_ptr<RadixNode<O>> RadixTree<O>::insertRec(std::shared_ptr<RadixNode<O>> node, const std::string& key, const O& value, int depth) {
    if (!node) {
        node = std::make_shared<RadixNode<O>>(key.substr(depth-1,1));
    }
    if (depth == key.length()) {
        node->isTerminal = true;
        node->value = value;
    } else {
        int index = key[depth] - 'a';
        node->children[index] = insertRec(node->children[index], key, value, depth + 1);
    }
    return node;
}

template <typename O>
O RadixTree<O>::put(const std::string& key, const O& value) {
    root = insertRec(root, key, value);
    return value;
}

template <typename O>
std::shared_ptr<RadixNode<O>> RadixTree<O>::findNode(const std::shared_ptr<RadixNode<O>>& node, const std::string& key, int depth) const {
    if (!node || depth == key.length()) {
        return node;
    }
    int index = key[depth] - 'a';
    return findNode(node->children[index], key, depth + 1);
}

template <typename O>
O RadixTree<O>::getValueForExactKey(const std::string& key) const {
    auto node = findNode(root, key);
    if (node && node->isTerminal) {
        return node->value;
    }
    return O(); // return default constructed object if the key is not found
}

template <typename O>
std::vector<std::pair<std::string, O>> RadixTree<O>::collectPairs(const std::string& prefix) const {
    return collectPairsRec(findNode(root, prefix), prefix);
}

template <typename O>
std::vector<std::pair<std::string, O>> RadixTree<O>::collectPairsRec(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix) const {
    std::vector<std::pair<std::string, O>> pairs;
    if (!node) {
        return pairs;
    }
    if (node->isTerminal) {
        pairs.emplace_back(prefix, node->value);
    }
    for (int i = 0; i < 26; i++) {
        if (node->children[i]) {
            char childChar = 'a' + i;
            std::vector<std::pair<std::string, O>> childPairs = collectPairsRec(node->children[i], prefix + childChar);
            pairs.insert(pairs.end(), childPairs.begin(), childPairs.end());
        }
    }
    return pairs;
}

template <typename O>
std::vector<std::string> RadixTree<O>::getKeysStartingWith(const std::string& prefix) const {
    std::vector<std::pair<std::string, O>> pairs = collectPairs(prefix);
    std::vector<std::string> keys;
    for (const auto& pair : pairs) {
        keys.push_back(pair.first);
    }
    return keys;
}

template <typename O>
std::vector<O> RadixTree<O>::getValuesForKeysStartingWith(const std::string& prefix) const {
    std::vector<std::pair<std::string, O>> pairs = collectPairs(prefix);
    std::vector<O> values;
    for (const auto& pair : pairs) {
        values.push_back(pair.second);
    }
    return values;
}

template <typename O>
void RadixTree<O>::printTree(const std::shared_ptr<RadixNode<O>>& node, const std::string& prefix, const std::string& childPrefix) const {
    if (!node) {
        return;
    }

    // Print the current node's key
    std::cout << prefix;
    std::cout << (node->isTerminal ? " (" + node->key + ") -> " + std::to_string(node->value) : node->key) << std::endl;

    // Recursively print each child, passing a new prefix for children
    for (int i = 0; i < node->children.size(); ++i) {
        if (node->children[i] != nullptr) {
            // Last child or not to determine the branch drawing
            bool isLast = (i == node->children.size() - 1);
            std::cout << childPrefix << (isLast ? "└── " : "├── ");
            printTree(node->children[i], "", childPrefix + (isLast ? "    " : "│   "));
        }
    }
}

template <typename O>
void RadixTree<O>::print() const {
    std::cout << "Radix Trie Structure:" << std::endl;
    printTree(root, "", "");
}