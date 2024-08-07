# Huffman Compression Algorithm - CLI Program

## Overview
The Huffman Compression Algorithm is a lossless data compression algorithm used to reduce the size of files without losing any information. This Huffman Compression CLI program written in C++ that allows you to compress any file type to a .hufbin file format. The compressed file can be decompressed back to its original form using the same program.

## Prerequisites
To use the Huffman Compression CLI program, you need to have the following prerequisites installed on your system:
- C++ compiler (supporting C++11 or higher)

## Installation
Follow the steps below to install and set up the Huffman Compression CLI program on your system:

1. Clone the repository:
   ```
   git clone https://github.com/unnureddy/File-Compression-Master
   ```

2. Change into the project directory:
   ```
   cd huffman_encoding_compression
   ```

3. Compile the source code using CMake:
   ```
   gcc huffman.cpp -o huffman
   ```

## Usage
Once you have successfully installed the Huffman Compression CLI program, follow the steps below to compress and decompress files:

### Compressing a File
To compress a file, run the following command:
```
./huffman c <input_file_path_with_extension> <output_file_name_without_extension> 
```
Replace `<input_file_path_with_extension>` with the path to the file you want to compress and `<output_file_name_without_extension>` with the output file name without any extension.

You can also run without arguments and interact with the menu.

### Decompressing a File
To decompress a compressed file, run the following command:
```
./huffman d <compressed_file_path_with_.hufbin> <output_file_name_with_extension>
```
Replace `<compressed_file_path_with_.hufbin>` with the path to the compressed file you want to decompress and `<output_file_name_without_extension>` with the output file name with file extension that you require.

## License
This Huffman Compression CLI program is licensed under the [MIT License](LICENSE).
