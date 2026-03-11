#include "Encoder.h"
#include "Decoder.h"
#include "ImageIO.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>

void printUsage() {
    std::cout << "Usage:\n"
              << "  compress <input_image> <output_file.huf>\n"
              << "  decompress <input_file.huf> <output_image>\n";
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    if (command == "compress") {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            
            auto imgInfo = ImageIO::readImage(inputPath);
            Encoder encoder;
            auto compressed = encoder.encode(imgInfo.pixels, imgInfo.width, imgInfo.height, imgInfo.channels);
            
            if (!encoder.saveToFile(outputPath, compressed)) {
                return 1;
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;

            uintmax_t originalFileSize = std::filesystem::file_size(inputPath);
            uintmax_t uncompressedPixelSize = imgInfo.pixels.size();
            uintmax_t compressedSize = std::filesystem::file_size(outputPath);
            double compressionRatio = (double)uncompressedPixelSize / compressedSize;

            std::cout << "Compression successful!\n";
            std::cout << "Original Image File Size:     " << originalFileSize << " bytes\n";
            std::cout << "Uncompressed Pixel Data Size: " << uncompressedPixelSize << " bytes\n";
            std::cout << "Huffman Compressed Size:      " << compressedSize << " bytes\n";
            std::cout << "Raw to Huffman Ratio:         " << compressionRatio << "x (Higher is better)\n";
            std::cout << "Encoding Time:                " << diff.count() << " seconds\n";
        } catch (const std::exception& e) {
            std::cerr << "Compression failed: " << e.what() << "\n";
            return 1;
        }
    } else if (command == "decompress") {
        try {
            auto start = std::chrono::high_resolution_clock::now();

            Decoder decoder;
            std::vector<uint8_t> compressed;
            if (!decoder.loadFromFile(inputPath, compressed)) {
                return 1;
            }

            auto decoded = decoder.decode(compressed);
            
            ImageIO::ImageInfo imgInfo;
            imgInfo.width = decoded.width;
            imgInfo.height = decoded.height;
            imgInfo.channels = decoded.channels;
            imgInfo.pixels = std::move(decoded.pixelData);

            if (!ImageIO::writeImage(outputPath, imgInfo)) {
                return 1;
            }

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;

            std::cout << "Decompression successful!\n";
            std::cout << "Decoding Time:                " << diff.count() << " seconds\n";
        } catch (const std::exception& e) {
            std::cerr << "Decompression failed: " << e.what() << "\n";
            return 1;
        }
    } else {
        std::cerr << "Unknown command: " << command << "\n";
        printUsage();
        return 1;
    }

    return 0;
}
