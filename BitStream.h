#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

class BitWriter {
public:
    BitWriter() : currentByte(0), bitPos(0) {}

    void writeBit(bool bit) {
        if (bit) {
            currentByte |= (1 << (7 - bitPos));
        }
        bitPos++;
        if (bitPos == 8) {
            flushByte();
        }
    }

    void writeByte(uint8_t byte) {
        for (int i = 7; i >= 0; --i) {
            writeBit((byte >> i) & 1);
        }
    }

    void flushToBuffer() {
        if (bitPos > 0) {
            buffer.push_back(currentByte);
            currentByte = 0;
            bitPos = 0;
        }
    }

    const std::vector<uint8_t>& getBuffer() const { return buffer; }

private:
    void flushByte() {
        buffer.push_back(currentByte);
        currentByte = 0;
        bitPos = 0;
    }

    std::vector<uint8_t> buffer;
    uint8_t currentByte;
    int bitPos;
};

class BitReader {
public:
    BitReader(const std::vector<uint8_t>& data) : buffer(data), byteIndex(0), bitPos(0) {}

    bool readBit() {
        if (byteIndex >= buffer.size()) {
            throw std::out_of_range("End of bitstream");
        }
        bool bit = (buffer[byteIndex] >> (7 - bitPos)) & 1;
        bitPos++;
        if (bitPos == 8) {
            bitPos = 0;
            byteIndex++;
        }
        return bit;
    }

    uint8_t readByte() {
        uint8_t byte = 0;
        for (int i = 7; i >= 0; --i) {
            if (readBit()) {
                byte |= (1 << i);
            }
        }
        return byte;
    }

    bool isEOF() const {
        return byteIndex >= buffer.size();
    }

private:
    const std::vector<uint8_t>& buffer;
    size_t byteIndex;
    int bitPos;
};
