#include <fstream>

#include "console_reader.h"
#include "decoder.h"
#include "encoder.h"

using Arguments = std::vector<std::string_view>;

class FileNotFound : public std::logic_error {
public:
    explicit FileNotFound(const std::string& exception) : std::logic_error(exception) {
    }
};

int Decode(const Arguments& args) {
    std::ifstream input(std::string(args[1]), std::ios_base::binary);

    if (!input.is_open()) {
        throw FileNotFound("can't open: " + std::string(args[1]));
    }

    Decoder decoder(BitReader(input), "./");
    decoder.Decode();

    input.close();
    return 0;
}
int Encode(const Arguments& args) {
    std::ofstream output(std::string(args[1]), std::ios_base::binary);

    if (!output.is_open()) {
        throw FileNotFound("can't open: " + std::string(args[1]));
    }

    Encoder encoder({.output = BitWriter(output)});
    for (size_t i = 2; i < args.size(); ++i) {
        std::string path = std::string(args[i]);
        std::ifstream file(path, std::ios_base::binary);

        if (!file.is_open()) {
            throw FileNotFound("can't open: " + std::string(args[i]));
        }

        auto pos_name_start = path.rfind('/');
        if (pos_name_start == std::string::npos) {
            pos_name_start = path.rfind('\\');
            if (pos_name_start == std::string::npos) {
                pos_name_start = 0;
            } else {
                ++pos_name_start;
            }
        } else {
            ++pos_name_start;
        }

        bool is_last = (i + 1) == args.size();
        encoder.EncodeFile({.name = path.substr(pos_name_start), .input = BitReader(file)}, is_last);

        file.close();
    }
    output.close();
    return 0;
}

int main(int argc, char const** argv) {
    ConsoleReader console_reader(std::cerr);

    console_reader.SetDescription("Zips and Unzips files");
    try {
        console_reader.AddParam("-c", Encode, "-c archive_name file1 [file2 ...]: zip files into archive_name", 3);
        console_reader.AddParam("-d", Decode, "-d archive_name: unzip archive_name into current directory", 2, 0);
        console_reader.AddParam(
            "-h",
            [&console_reader](const Arguments& args) {
                console_reader.ShowHelp();
                return 0;
            },
            "-h: shows instruction of usage archiver.exe", 1, 0);
    } catch (std::exception& e) {
        std::cerr << "error in code: " << e.what() << "\n";
    }
    try {
        return console_reader.Read(argc, argv);
    } catch (const InvalidArgument& e) {
        std::cerr << "Invalid arguments: " << e.what() << "\n";
        return 111;
    } catch (const FileNotFound& e) {
        std::cerr << e.what() << "\n";
        return 111;
    } catch (const Decoder::IncorrectFile& e) {
        std::cerr << e.what() << "\n";
        return 111;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 111;
    } catch (...) {
        std::cerr << "something went wrong!\n";
        return 111;
    }
}
