#include "decoder.h"

#include <fstream>
#include <functional>
#include <vector>

#include "trie.h"

#include <iostream>

using Int = BitReader::ResultType;
using TriePtr = RawTrie<Int>::TriePtr;

namespace {

void AddCode(Trie<Int>& target, Int key, const std::vector<bool>& code) {
    auto current = target.GetRaw();
    if (current == nullptr) {
        current.reset(new RawTrie<Int>());
    }
    for (auto e : code) {
        auto next = (e == 0 ? current->GetLeft() : current->GetRight());
        if (next == nullptr) {
            if (e == 0) {
                current->SetLeft(std::make_shared<RawTrie<Int>>());
                next = current->GetLeft();
            }
            else {
                current->SetRight(std::make_shared<RawTrie<Int>>());
                next = current->GetRight();
            }
        }
        current = next;
    }
    current->SetValue(key);
}

uint32_t GetChar(Trie<Int>& target, Decoder& decoder, std::function<uint32_t(Decoder&, size_t)> read) {
    auto current = target.GetRaw();

    while (!current->IsTerminal()) {
        uint32_t now = read(decoder, 1);
        assert(current != nullptr);
        auto next = (now == 0 ? current->GetLeft() : current->GetRight());
        current = next;
    }
    return current->GetValue();
}

}  // namespace

Decoder::IncorrectFile::IncorrectFile(const char* message) : std::runtime_error(message) {
}

Decoder::Decoder(BitReader&& archive, const std::string& output_directory_path)
    : archive_(archive), path_(output_directory_path) {
}

void Decoder::Decode() {
    using Code = std::pair<uint32_t, std::vector<bool>>;

    while (true) {
        size_t character_count = ReadSome(9);
        std::vector<Int> characters(character_count);

        for (auto& ch : characters) {
            ch = ReadSome(9);
        }

        std::vector<Int> lengths;
        Int total_length = 0;
        while (total_length < character_count) {
            Int current = ReadSome(9);
            lengths.push_back(current);
            total_length += current;
        }

        std::vector<Code> codes;
        codes.reserve(character_count);
        std::vector<bool> current_code(1, false);

        size_t length_pointer = 0;
        for (size_t i = 0; i < character_count; ++i) {
            while (lengths[length_pointer] == 0) {
                current_code.push_back(false);
                ++length_pointer;
            }
            --lengths[length_pointer];
            codes.push_back({characters[i], current_code});
            size_t k = current_code.size() - 1;
            while (true) {
                current_code[k] = !current_code[k];
                if (current_code[k] == 1 || k == 0) {
                    break;
                }
                --k;
            }
        }

        Trie<Int> trie_codes;
        for (const auto& [key, code] : codes) {
            AddCode(trie_codes, key, code);
        }

        bool is_last = false;
        bool file_name_ended = false;
        std::string file_name;

        std::ofstream current_file;

        while (true) {
            auto char_code = GetChar(trie_codes, *this, std::mem_fn(&Decoder::ReadSome));
            if (char_code == ARCHIVE_END) {
                is_last = true;
                break;
            }
            if (char_code == ONE_MORE_FILE) {
                break;
            }
            if (char_code == FILENAME_END) {
                file_name_ended = true;
                current_file.open(path_ + file_name, std::ios_base::binary);
                continue;
            }
            if (!file_name_ended) {
                file_name += static_cast<char>(char_code);
            } else {
                current_file << static_cast<char>(char_code);
            }
        }

        current_file.close();

        if (is_last) {
            break;
        }
    }
}

BitReader::ResultType Decoder::ReadSome(size_t to_read = 1) {
    auto [value, result] = archive_.ReadSome(to_read);
    if (!result) {
        throw IncorrectFile("Invalid file. Expected archive-format file");
    }
    return value;
}
