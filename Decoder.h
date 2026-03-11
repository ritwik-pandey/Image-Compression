#pragma once

#include "HuffmanTree.h"
#include <vector>
#include <cstdint>
#include <string>

class BitReader;

class Decoder {
public:
    Decoder() = default;

    // Load compressed data from a binary file
    bool loadFromFile(const std::string& filename, std::vector<uint8_t>& compressedData);

    struct DecodedImage {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
        std::vector<uint8_t> pixelData;
    };

    // Decodes the compressed buffer back into raw pixel data
    DecodedImage decode(const std::vector<uint8_t>& compressedData);

private:
    std::shared_ptr<HuffmanNode> deserializeTree(BitReader& reader);
};
