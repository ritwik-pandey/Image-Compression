#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>
#include <string>

struct HuffmanNode {
    uint8_t data;
    uint64_t freq;
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;

    HuffmanNode(uint8_t data, uint64_t freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
    HuffmanNode(std::shared_ptr<HuffmanNode> left, std::shared_ptr<HuffmanNode> right)
        : data(0), freq(left->freq + right->freq), left(left), right(right) {}
};

struct CompareNode {
    bool operator()(const std::shared_ptr<HuffmanNode>& lhs, const std::shared_ptr<HuffmanNode>& rhs) const {
        return lhs->freq > rhs->freq;
    }
};

class HuffmanTree {
public:
    using CodeMap = std::unordered_map<uint8_t, std::string>;
    using FreqMap = std::unordered_map<uint8_t, uint64_t>;

    HuffmanTree();
    ~HuffmanTree() = default;

    void buildFromFrequencies(const FreqMap& frequencies);
    void buildCodes(std::shared_ptr<HuffmanNode> node, const std::string& currentCode);
    
    std::shared_ptr<HuffmanNode> getRoot() const { return root; }
    const CodeMap& getCodes() const { return codes; }

    // Helper to compute frequencies from raw data
    static FreqMap computeFrequencies(const std::vector<uint8_t>& data);

private:
    std::shared_ptr<HuffmanNode> root;
    CodeMap codes;
};
