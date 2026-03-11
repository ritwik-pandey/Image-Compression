# Huffman Image Compression

A high-performance command-line Image Compression tool built in C++17. It compresses grayscale or RGB images by encoding pixel intensity values using Huffman coding, reducing file sizes without data loss.

## Features
- **Custom Format**: Creates custom binary `.huf` files carrying compressed bits and serialized Huffman Tree metadata.
- **Lightweight I/O**: Interacts with PNG/JPG images natively via single-header `stb_image.h` and `stb_image_write.h`.
- **High Performance**: Encodes millions of pixels via optimized bit-level packing (`std::vector<uint8_t>`) and standard library priority queues.
- **Detailed Metrics**: Outputs original pixel byte-counts versus Huffman-compressed bitstreams, calculating empirical compression ratios natively.

## Implementation Details
The project utilizes the following modular architecture:
- `ImageIO`: Loads/Saves standard image file wrappers.
- `HuffmanTree`: Constructs the core frequency node map and dynamically branches Huffman bit-codes using a Min-Heap.
- `Encoder` / `Decoder`: Interfaces heavily with custom `BitWriter` and `BitReader` routines, precisely masking and flushing sequences of `0`s and `1`s directly to byte arrays.
- `CLI`: Provides a straight-forward frontend experience.

## Building the project
Ensure you have CMake and a C++17 compatible compiler installed.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Compression
```bash
./huffman_image_compressor compress myimage.png output.huf
```

### Decompression
```bash
./huffman_image_compressor decompress output.huf restored_image.png
```
