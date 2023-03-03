#pragma once

#include <ostream>

#include "bit_stream.h"

class BitWriter {
public:
    using Size = BitStream::Size;
    static const Size CHAR_SIZE = BitStream::CHAR_SIZE;
    using InputType = uint32_t;
    static const Size MAX_PUT_REQUEST = 32;

    explicit BitWriter(std::ostream& output);

    void WriteSome(InputType target, Size size);
    void Flush();

private:
    void FreeBuffer();

    BitStream bit_stream_;
    std::ostream& output_;
};
