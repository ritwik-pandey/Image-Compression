#include "HuffmanTree.h"
#include <queue>
#include <stdexcept>

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::FreqMap HuffmanTree::computeFrequencies(const std::vector<uint8_t>& data) {
    FreqMap frequencies;
    for (uint8_t byte : data) {
        frequencies[byte]++;
    }
    return frequencies;
}

void HuffmanTree::buildFromFrequencies(const FreqMap& frequencies) {
    if (frequencies.empty()) {
        root = nullptr;
        return;
    }

    std::priority_queue<std::shared_ptr<HuffmanNode>, std::vector<std::shared_ptr<HuffmanNode>>, CompareNode> pq;

    for (const auto& pair : frequencies) {
        pq.push(std::make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // Handle single unique byte case
    if (pq.size() == 1) {
        auto node = pq.top();
        pq.pop();
        root = std::make_shared<HuffmanNode>(node, std::make_shared<HuffmanNode>((uint8_t)0, (uint64_t)0));
        buildCodes(root, "");
        return;
    }

    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        auto parent = std::make_shared<HuffmanNode>(left, right);
        pq.push(parent);
    }

    root = pq.top();
    buildCodes(root, "");
}

void HuffmanTree::buildCodes(std::shared_ptr<HuffmanNode> node, const std::string& currentCode) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[node->data] = currentCode.empty() ? "0" : currentCode;
    }

    buildCodes(node->left, currentCode + "0");
    buildCodes(node->right, currentCode + "1");
}
