#include "Encoder.h"
#include "BitStream.h"
#include <fstream>
#include <iostream>

void Encoder::serializeTree(std::shared_ptr<HuffmanNode> node, BitWriter& writer) {
    if (!node) return;

    if (!node->left && !node->right) {
        writer.writeBit(1); // 1 indicates leaf node
        writer.writeByte(node->data);
    } else {
        writer.writeBit(0); // 0 indicates internal node
        serializeTree(node->left, writer);
        serializeTree(node->right, writer);
    }
}

std::vector<uint8_t> Encoder::encode(const std::vector<uint8_t>& pixelData, uint32_t width, uint32_t height, uint8_t channels) {
    auto frequencies = HuffmanTree::computeFrequencies(pixelData);
    HuffmanTree tree;
    tree.buildFromFrequencies(frequencies);
    
    BitWriter writer;

    // Header structure:
    // Magic "HUF1"
    writer.writeByte('H');
    writer.writeByte('U');
    writer.writeByte('F');
    writer.writeByte('1');

    // Number of original pixels (8 bytes = 64 bits)
    uint64_t totalPixels = pixelData.size();
    for (int i = 7; i >= 0; --i) {
        writer.writeByte((totalPixels >> (i * 8)) & 0xFF);
    }

    // Write image metadata
    for (int i = 3; i >= 0; --i) writer.writeByte((width >> (i * 8)) & 0xFF);
    for (int i = 3; i >= 0; --i) writer.writeByte((height >> (i * 8)) & 0xFF);
    writer.writeByte(channels);

    // Now write the tree
    serializeTree(tree.getRoot(), writer);
    writer.flushToBuffer(); 
    // Flush to byte boundary because the bitstream for encoded tree might end midway in a byte.
    // Wait, the BitWriter flushToBuffer pads the remaining bits with 0s. This is fine,
    // the decoder will know when it's done decoding the tree because it will reconstruct it completely (as building a binary tree is self-terminating).
    // Let's actually not flush byte to avoid wasting space, but doing so keeps tree decoding simple instead of mixing tree bits and data bits.
    // Since bits don't care, we can just mix tree bits and data bits seamlessly!
    // No need to flush byte, wait until the very end.

    const auto& codes = tree.getCodes();

    // Encode actual data
    for (uint8_t byte : pixelData) {
        const std::string& code = codes.at(byte);
        for (char c : code) {
            writer.writeBit(c == '1');
        }
    }

    writer.flushToBuffer();
    return writer.getBuffer();
}

bool Encoder::saveToFile(const std::string& filename, const std::vector<uint8_t>& compressedData) {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    out.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
    out.close();
    return true;
}
