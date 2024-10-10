#pragma once

#include <bitset>
#include <limits>
#include <cstdint>

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

struct BitStream {
    using Size = size_t;
    using CharType = char;
    static const Size CHAR_SIZE = CHAR_BIT;
    using ResultType = uint32_t;

    bool CanRead() const;
    static ResultType GetSubBits(ResultType element, Size left_offset, Size right_offset, Size max_size,
                                 Size current_size);

    static const Size BUFFER_SIZE = 1024;
    CharType buffer[BUFFER_SIZE] = {};  // Has symbols(bytes) in little-endian
    Size buffer_pointer = 0;
    Size bit_pointer = 0;
    Size buffer_current_size = 0;
};
