#include "bit_stream.h"

using Size = typename BitStream::Size;
using ResultType = typename BitStream::ResultType;

bool BitStream::CanRead() const {
    return (buffer_pointer < buffer_current_size);
}
ResultType BitStream::GetSubBits(ResultType element, Size left_offset, Size right_offset, Size max_size,
                                 Size current_size) {
    auto without_left = element << (max_size - current_size + left_offset);
    auto answer = without_left >> (max_size - current_size + left_offset + right_offset);
    return answer;
}
