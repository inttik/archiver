#include "console_reader.h"

using namespace std::literals;

InvalidArgument::InvalidArgument(const std::string &exception) : std::invalid_argument(exception) {
}

Function::Function(std::function<int(const std::vector<std::string_view> &)> function) : function(function) {
}

int Function::Apply(const std::vector<std::string_view> &arguments) const {
    if (arguments.size() < min_argument_count + static_cast<size_t>(need_exe_directory)) {
        throw InvalidArgument("Expected at least " + std::to_string(min_argument_count) + " arguments. Got " +
                              std::to_string(arguments.size()));
    }
    if (max_additional_argument_count != INFINITY_COUNT &&
        arguments.size() >
            min_argument_count + max_additional_argument_count + static_cast<size_t>(need_exe_directory)) {
        throw InvalidArgument("Expected at most " +
                              std::to_string(min_argument_count + max_additional_argument_count +
                                             static_cast<size_t>(need_exe_directory)) +
                              " arguments. Got " + std::to_string(arguments.size()));
    }
    return function(arguments);
}

ConsoleReader::ConsoleReader(std::ostream &output_stream) : stream_(output_stream) {
}

void ConsoleReader::AddDefault(std::function<int(const std::vector<std::string_view> &)> function,
                               std::string_view description, size_t min_argument_count,
                               size_t max_additional_argument_count, bool need_exe_directory) {
    AddParam(DEFAULT_KEY, function, description, min_argument_count, max_additional_argument_count, need_exe_directory);
}
void ConsoleReader::SetDescription(std::string_view description) {
    default_description_ = description;
}

void ConsoleReader::AddParam(std::string_view key, std::function<int(const std::vector<std::string_view> &)> function,
                             std::string_view description, size_t min_argument_count,
                             size_t max_additional_argument_count, bool need_exe_directory) {
    if (key[0] != '-') {
        throw std::invalid_argument("Try to add function with key = " + std::string(key) +
                                    ". Every key should start with '-'");
    }
    if (key.find('=') != std::string_view::npos) {
        throw std::invalid_argument("Try to add function with key = " + std::string(key) +
                                    ". Key can't have symbol '='");
    }
    if (commands_.find(key) != commands_.end()) {
        throw std::invalid_argument("Try add second function with key = " + std::string(key));
    }
    Function current(function);
    current.min_argument_count = min_argument_count;
    current.max_additional_argument_count = max_additional_argument_count;
    current.need_exe_directory = need_exe_directory;

    commands_[key] = std::move(current);
    if (key == DEFAULT_KEY) {
        default_description_ = description;
    } else {
        descriptions_[key] = description;
    }
}

int ConsoleReader::Read(size_t argc, char const **argv) const {
    std::vector<std::string_view> commands(argc);
    for (size_t i = 0; i < argc; ++i) {
        commands[i] = argv[i];
    }
    bool has_default = commands_.find(DEFAULT_KEY) != commands_.end();
    std::vector<std::string_view> to_default;
    if (has_default && commands_.at(DEFAULT_KEY).need_exe_directory) {
        to_default.push_back(commands[0]);
    }
    size_t current = 1;
    while (current < argc) {
        if (commands[current][0] != '-') {
            if (!has_default) {
                throw InvalidArgument("Unknown command: " + std::string(commands[current]) +
                                      " (Default command is not set)");
            } else {
                to_default.push_back(commands[current]);
            }
            ++current;
            continue;
        }
        auto current_command = commands[current];
        std::string_view current_key = commands[current].substr(0, commands[current].find('='));
        auto object = commands_.find(current_key);
        if (object == commands_.end()) {
            throw InvalidArgument("Unknown command: " + std::string(current_key) + " (" + std::string(current_command) +
                                  ")");
        }
        const auto &current_function = object->second;
        std::vector<std::string_view> arguments;
        if (current_function.need_exe_directory) {
            arguments.push_back(commands[0]);
        }
        arguments.push_back(commands[current]);
        ++current;
        for (size_t i = 1; i < current_function.min_argument_count; ++i) {
            if (current >= argc) {
                throw InvalidArgument("Invalid input: to few arguments for command " + std::string(current_command));
            }
            arguments.push_back(commands[current]);
            ++current;
        }
        if (current_function.max_additional_argument_count == Function::INFINITY_COUNT) {
            while (current < argc) {
                if (commands[current][0] == '-') {
                    break;
                }
                arguments.push_back(commands[current]);
                ++current;
            }
        } else {
            for (size_t i = 0; i < current_function.max_additional_argument_count; ++i) {
                if (current >= argc) {
                    break;
                }
                if (commands[current][0] == '-') {
                    break;
                }
                arguments.push_back(commands[current]);
                ++current;
            }
        }

        int code = current_function.Apply(arguments);
        if (code != 0) {
            throw std::runtime_error("command " + std::string(current_command) +
                                     " finished with code = " + std::to_string(code));
        }
    }

    if (has_default) {
        int code = commands_.at(DEFAULT_KEY).Apply(to_default);
        if (code != 0) {
            return code;
        }
    }
    return 0;
}

void ConsoleReader::ShowHelp() const {
    stream_ << default_description_ << "\n----------\n";
    stream_ << "Arguments:\n";
    for (const auto &[key, description] : descriptions_) {
        if (key == DEFAULT_KEY) {
            continue;
        }
        stream_ << key << ": " << description;
        stream_ << "\n";
    }
}
