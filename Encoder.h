#pragma once

#include "HuffmanTree.h"
#include <vector>
#include <cstdint>
#include <string>

class BitWriter;

class Encoder {
public:
    Encoder() = default;
    
    // Encodes raw pixel data into a compressed buffer and calculates the tree.
    // Includes file header, serialized tree, and encoded bitstream.
    std::vector<uint8_t> encode(const std::vector<uint8_t>& pixelData, uint32_t width, uint32_t height, uint8_t channels);

    // Save the compressed data to a binary file
    bool saveToFile(const std::string& filename, const std::vector<uint8_t>& compressedData);

private:
    void serializeTree(std::shared_ptr<HuffmanNode> node, BitWriter& writer);
};
