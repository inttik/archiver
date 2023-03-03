#include "bit_reader.h"

BitReader::BitReader(std::istream& input) : bit_stream_(), input_(input) {
}

std::pair<BitReader::ResultType, bool> BitReader::ReadSome(size_t count) {
    if (count > MAX_GET_REQUEST) {
        throw std::overflow_error("trying to BitReader::ReadSome [" + std::to_string(count) + "] symbols > " +
                                  std::to_string(MAX_GET_REQUEST) +
                                  ". That overflows current result type = " + typeid(ResultType).name() + "!");
    }

    auto& buffer = bit_stream_.buffer;
    auto& buffer_pointer = bit_stream_.buffer_pointer;
    auto& bit_pointer = bit_stream_.bit_pointer;
    auto& buffer_current_size = bit_stream_.buffer_current_size;

    auto current_buffer_pointer = buffer_pointer;
    auto current_bit_pointer = bit_pointer;
    auto current_buffer_current_size = buffer_current_size;

    ResultType answer = 0;
    Size read = 0;

    while (read < count) {
        if (!bit_stream_.CanRead()) {
            if (!FreeBuffer()) {
                buffer_pointer = current_buffer_pointer;
                bit_pointer = current_bit_pointer;
                buffer_current_size = current_buffer_current_size;
                return {0, false};
            }
        }

        Size current_read = 0;
        if (count - read + bit_pointer <= CHAR_SIZE) {
            current_read = count - read;
        } else {
            current_read = CHAR_SIZE - bit_pointer;
        }
        answer = answer << static_cast<ResultType>(current_read);
        answer ^= BitStream::GetSubBits(buffer[buffer_pointer], bit_pointer, CHAR_SIZE - bit_pointer - current_read,
                                        MAX_GET_REQUEST, CHAR_SIZE);

        read += current_read;
        bit_pointer += current_read;
        while (bit_pointer >= CHAR_SIZE) {
            ++buffer_pointer;
            bit_pointer -= CHAR_SIZE;
        }
    }

    return {answer, true};
}
void BitReader::Restore() {
    input_.clear();
    input_.seekg(std::ios::beg);
    bit_stream_.buffer_pointer = 0;
    bit_stream_.buffer_current_size = 0;
    bit_stream_.bit_pointer = 0;
}

bool BitReader::FreeBuffer() {
    input_.read(bit_stream_.buffer, bit_stream_.BUFFER_SIZE);
    bit_stream_.buffer_current_size = input_.gcount();
    bit_stream_.buffer_pointer = 0;
    bit_stream_.bit_pointer = 0;

    if (bit_stream_.buffer_current_size == 0) {
        return false;
    }
    return true;
}
