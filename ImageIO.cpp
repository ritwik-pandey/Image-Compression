#include "ImageIO.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>

ImageIO::ImageInfo ImageIO::readImage(const std::string& filepath) {
    int w, h, c;
    // Load as is (0 for required_comp)
    unsigned char* data = stbi_load(filepath.c_str(), &w, &h, &c, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image: " + filepath);
    }

    ImageInfo info;
    info.width = static_cast<uint32_t>(w);
    info.height = static_cast<uint32_t>(h);
    info.channels = static_cast<uint8_t>(c);
    
    size_t dataSize = w * h * c;
    info.pixels.assign(data, data + dataSize);
    
    stbi_image_free(data);
    return info;
}

bool ImageIO::writeImage(const std::string& filepath, const ImageInfo& info) {
    // Determine format from extension
    size_t extPos = filepath.find_last_of('.');
    if (extPos == std::string::npos) {
        std::cerr << "No file extension provided for output image." << std::endl;
        return false;
    }

    std::string ext = filepath.substr(extPos + 1);
    for (auto& c : ext) c = tolower(c);

    int success = 0;
    if (ext == "png") {
        success = stbi_write_png(filepath.c_str(), info.width, info.height, info.channels, info.pixels.data(), info.width * info.channels);
    } else if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(filepath.c_str(), info.width, info.height, info.channels, info.pixels.data(), 90); // 90 is quality
    } else if (ext == "bmp") {
        success = stbi_write_bmp(filepath.c_str(), info.width, info.height, info.channels, info.pixels.data());
    } else {
        std::cerr << "Unsupported output image format: " << ext << std::endl;
        return false;
    }

    return success != 0;
}
