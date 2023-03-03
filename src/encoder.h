#pragma once

#include <string>
#include <vector>
#include <map>

#include "bit_reader.h"
#include "bit_writer.h"

class Encoder {
public:
    using FrequencyType = uint64_t;
    using Code = std::pair<size_t, std::vector<bool>>;
    using CodeTable = std::map<size_t, std::vector<bool>>;

    const uint32_t FILENAME_END = 256;
    const uint32_t ONE_MORE_FILE = 257;
    const uint32_t ARCHIVE_END = 258;

    struct InputStream {
        std::string name;
        BitReader input;
    };
    struct OutputStream {
        BitWriter output;
    };

    explicit Encoder(OutputStream&& archive);
    Encoder(const Encoder& other) = delete;
    Encoder(Encoder&& other) = default;

    Encoder& operator=(const Encoder& other) & = delete;
    Encoder& operator=(Encoder&& other) & = delete;
    ~Encoder() = default;

    void EncodeFile(InputStream&& file, bool is_last);

private:
    static void Output(OutputStream& target, const std::vector<bool>& code);

    OutputStream archive_;
};
