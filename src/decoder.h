#pragma once

#include <stdexcept>

#include "bit_reader.h"

class Decoder {
public:
    const uint32_t FILENAME_END = 256;
    const uint32_t ONE_MORE_FILE = 257;
    const uint32_t ARCHIVE_END = 258;

    class IncorrectFile : public std::runtime_error {
    public:
        explicit IncorrectFile(const char* message);
    };

    explicit Decoder(BitReader&& archive, const std::string& output_directory_path);

    void Decode();

private:
    BitReader::ResultType ReadSome(size_t to_read);

    BitReader archive_;
    std::string path_;
};
