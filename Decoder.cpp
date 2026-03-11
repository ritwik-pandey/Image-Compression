#include "Decoder.h"
#include "BitStream.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

std::shared_ptr<HuffmanNode> Decoder::deserializeTree(BitReader& reader) {
    if (reader.isEOF()) throw std::runtime_error("Unexpected end of file while reading tree");

    bool isLeaf = reader.readBit();
    if (isLeaf) {
        uint8_t data = reader.readByte();
        return std::make_shared<HuffmanNode>(data, 0); // Freq is 0 as we don't need it for decoding
    } else {
        auto left = deserializeTree(reader);
        auto right = deserializeTree(reader);
        return std::make_shared<HuffmanNode>(left, right);
    }
}

Decoder::DecodedImage Decoder::decode(const std::vector<uint8_t>& compressedData) {
    BitReader reader(compressedData);

    // Verify magic bytes
    if (reader.readByte() != 'H' || reader.readByte() != 'U' ||
        reader.readByte() != 'F' || reader.readByte() != '1') {
        throw std::runtime_error("Invalid file format: Magic bytes missing");
    }

    // Read total pixels
    uint64_t totalPixels = 0;
    for (int i = 7; i >= 0; --i) {
        totalPixels |= (static_cast<uint64_t>(reader.readByte()) << (i * 8));
    }

    // Read metadata
    uint32_t width = 0, height = 0;
    for (int i = 3; i >= 0; --i) width |= (static_cast<uint32_t>(reader.readByte()) << (i * 8));
    for (int i = 3; i >= 0; --i) height |= (static_cast<uint32_t>(reader.readByte()) << (i * 8));
    uint8_t channels = reader.readByte();

    // Read tree
    auto root = deserializeTree(reader);
    if (!root) {
        throw std::runtime_error("Failed to parse Huffman tree");
    }

    std::vector<uint8_t> pixelData;
    pixelData.reserve(totalPixels);

    // Decode actual data
    for (uint64_t i = 0; i < totalPixels; ++i) {
        auto node = root;
        while (node->left || node->right) {
            bool bit = reader.readBit();
            node = bit ? node->right : node->left;
            if (!node) {
                throw std::runtime_error("Invalid bitstream: Reached a null node");
            }
        }
        pixelData.push_back(node->data);
    }

    return {width, height, channels, std::move(pixelData)};
}

bool Decoder::loadFromFile(const std::string& filename, std::vector<uint8_t>& compressedData) {
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (!in) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return false;
    }
    
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);
    
    compressedData.resize(size);
    if (in.read(reinterpret_cast<char*>(compressedData.data()), size)) {
        return true;
    }
    return false;
}
