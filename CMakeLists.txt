cmake_minimum_required(VERSION 3.10)
project(HuffmanImageCompressor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffman_image_compressor 
    main.cpp 
    HuffmanTree.cpp 
    Encoder.cpp 
    Decoder.cpp 
    ImageIO.cpp
)
