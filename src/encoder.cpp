#include "encoder.h"

#include <algorithm>
#include <tuple>

#include "heap.h"
#include "trie.h"

Encoder::Encoder(Encoder::OutputStream&& archive) : archive_(archive) {
}

void Encoder::EncodeFile(Encoder::InputStream&& file, bool is_last) {
    using QueueKey = std::pair<FrequencyType, Trie<size_t>>;

    const size_t alphabet_size = 259;

    // frequencies calculation
    std::vector<FrequencyType> frequencies(alphabet_size);
    frequencies[FILENAME_END] = 1;
    frequencies[ONE_MORE_FILE] = 1;
    frequencies[ARCHIVE_END] = 1;

    for (uint8_t symbol : file.name) {
        ++frequencies[symbol];
    }
    while (true) {
        auto [now, result] = file.input.ReadSome(8);
        if (!result) {
            break;
        }
        ++frequencies[now];
    }

    // trie building
    MinHeap<QueueKey> priority_queue;
    for (size_t i = 0; i < alphabet_size; ++i) {
        if (frequencies[i] == 0) {
            continue;
        }
        QueueKey current(frequencies[i], Trie<size_t>(i));
        priority_queue.Insert(std::move(current));
    }

    while (priority_queue.Size() > 1) {
        auto l = priority_queue.Extract();
        auto r = priority_queue.Extract();
        QueueKey current(l.first + r.first, Trie<size_t>(std::move(l.second), std::move(r.second)));
        priority_queue.Insert(std::move(current));
    }

    auto trie = priority_queue.Extract().second;

    // code generation
    std::vector<Code> codes;
    trie.GenerateCodes(codes);
    std::sort(codes.begin(), codes.end(), [](const Code& a, const Code& b) {
        return std::make_tuple(a.second.size(), a.first) < std::make_tuple(b.second.size(), b.first);
    });

    std::vector<bool> current_code(1, false);
    for (auto& [key, code] : codes) {
        while (current_code.size() < code.size()) {
            current_code.push_back(false);
        }
        code = current_code;
        size_t i = current_code.size() - 1;
        while (true) {
            current_code[i] = !current_code[i];
            if (current_code[i] == 1 || i == 0) {
                break;
            }
            --i;
        }
    }

    // restore information output
    std::vector<size_t> sizes(alphabet_size);
    size_t max_size = 0;
    archive_.output.WriteSome(codes.size(), 9);
    for (auto& [key, code] : codes) {
        archive_.output.WriteSome(key, 9);
        max_size = std::max(max_size, code.size() - 1);
        ++sizes[code.size() - 1];
    }
    for (size_t i = 0; i <= max_size; ++i) {
        archive_.output.WriteSome(sizes[i], 9);
    }

    std::map<size_t, std::vector<bool>> code_map;
    for (auto& [key, code] : codes) {
        code_map[key] = code;
    }

    // encoding
    file.input.Restore();
    for (uint8_t symbol : file.name) {
        Output(archive_, code_map[symbol]);
    }
    Output(archive_, code_map[FILENAME_END]);
    while (true) {
        auto [now, result] = file.input.ReadSome(8);
        if (!result) {
            break;
        }
        Output(archive_, code_map[now]);
    }

    if (is_last) {
        Output(archive_, code_map[ARCHIVE_END]);
        archive_.output.Flush();
    } else {
        Output(archive_, code_map[ONE_MORE_FILE]);
    }
}

void Encoder::Output(Encoder::OutputStream& target, const std::vector<bool>& code) {
    BitWriter::InputType current = 0;
    BitStream::Size current_put = 0;
    BitStream::Size i = 0;
    BitStream::Size per_put = BitWriter::MAX_PUT_REQUEST;

    while (i < code.size()) {
        current <<= 1;
        current ^= code[i];
        ++i;
        ++current_put;

        if (current_put == per_put) {
            target.output.WriteSome(current, current_put);
            current = 0;
            current_put = 0;
        }
    }

    if (current_put != 0) {
        target.output.WriteSome(current, current_put);
    }
}
