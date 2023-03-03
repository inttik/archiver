#pragma once

#include <istream>

#include "bit_stream.h"

class BitReader {
public:
    using Size = BitStream::Size;
    static const Size CHAR_SIZE = BitStream::CHAR_SIZE;
    using ResultType = uint32_t;
    static const Size MAX_GET_REQUEST = 32;

    explicit BitReader(std::istream& input);

    std::pair<BitReader::ResultType, bool> ReadSome(size_t count);
    void Restore();

private:
    bool FreeBuffer();

    BitStream bit_stream_;
    std::istream& input_;
};
