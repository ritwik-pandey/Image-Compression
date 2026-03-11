#pragma once

#include <string>
#include <vector>
#include <cstdint>

class ImageIO {
public:
    struct ImageInfo {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
        std::vector<uint8_t> pixels;
    };

    static ImageInfo readImage(const std::string& filepath);
    static bool writeImage(const std::string& filepath, const ImageInfo& info);
};
