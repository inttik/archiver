#include "bit_writer.h"

BitWriter::BitWriter(std::ostream& output) : bit_stream_(), output_(output) {
    bit_stream_.buffer_current_size = bit_stream_.BUFFER_SIZE;
}

void BitWriter::WriteSome(BitWriter::InputType target, BitStream::Size size) {
    if (size > MAX_PUT_REQUEST) {
        throw std::overflow_error("trying to BitWriter::WriteSome [" + std::to_string(size) + "] symbols > " +
                                  std::to_string(MAX_PUT_REQUEST) +
                                  ". That overflows current result type = " + typeid(InputType).name() + "!");
    }

    auto& buffer = bit_stream_.buffer;
    auto& buffer_pointer = bit_stream_.buffer_pointer;
    auto& bit_pointer = bit_stream_.bit_pointer;

    Size put = 0;
    while (put < size) {
        if (!bit_stream_.CanRead()) {
            FreeBuffer();
        }

        Size current_put = 0;
        if (size - put + bit_pointer <= CHAR_SIZE) {
            current_put = size - put;
        } else {
            current_put = CHAR_SIZE - bit_pointer;
        }

        auto& copy = buffer[buffer_pointer];
        copy <<= current_put;                                                                         // NOLINT
        copy += BitStream::GetSubBits(target, put, size - put - current_put, MAX_PUT_REQUEST, size);  // NOLINT

        put += current_put;
        bit_pointer += current_put;
        while (bit_pointer >= CHAR_SIZE) {
            ++buffer_pointer;
            bit_pointer -= CHAR_SIZE;
        }
    }
}
void BitWriter::Flush() {
    auto& buffer = bit_stream_.buffer;
    auto& bit_pointer = bit_stream_.bit_pointer;

    FreeBuffer();
    if (bit_pointer != 0) {
        buffer[0] <<= (BitWriter::CHAR_SIZE - bit_pointer);  // NOLINT
        output_ << buffer[0];
        buffer[0] = 0;
        bit_pointer = 0;
    }
}

void BitWriter::FreeBuffer() {
    auto& buffer = bit_stream_.buffer;
    auto& buffer_pointer = bit_stream_.buffer_pointer;

    if (buffer_pointer == 0) {
        return;
    }

    output_.write(buffer, buffer_pointer);

    std::swap(buffer[0], buffer[buffer_pointer]);
    buffer_pointer = 0;
}
