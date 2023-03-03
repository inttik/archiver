#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <string_view>
#include <stdexcept>
#include <vector>

class InvalidArgument : public std::invalid_argument {
public:
    explicit InvalidArgument(const std::string& exception);
};

struct Function {
    static inline const size_t INFINITY_COUNT = -1;

    Function() = default;
    explicit Function(std::function<int(const std::vector<std::string_view>&)> function);
    Function(const Function& other) = default;

    int Apply(const std::vector<std::string_view>& arguments) const;

    std::function<int(const std::vector<std::string_view>&)> function;
    size_t min_argument_count = 0;
    size_t max_additional_argument_count = 0;
    bool need_exe_directory = false;
};

class ConsoleReader {
public:
    inline static const std::string_view DEFAULT_KEY = "--default";

    explicit ConsoleReader(std::ostream& output_stream);

    void SetDescription(std::string_view description);

    void AddDefault(std::function<int(const std::vector<std::string_view>&)> function,
                    std::string_view description = "", size_t min_argument_count = 0,
                    size_t max_additional_argument_count = Function::INFINITY_COUNT, bool need_exe_directory = false);
    void AddParam(std::string_view key, std::function<int(const std::vector<std::string_view>&)> function,
                  std::string_view description = "", size_t min_argument_count = 0,
                  size_t max_additional_argument_count = Function::INFINITY_COUNT, bool need_exe_directory = false);

    int Read(size_t argc, char const** argv) const;

    void ShowHelp() const;

private:
    std::ostream& stream_;

    std::map<std::string_view, Function> commands_;
    std::map<std::string_view, std::string_view> descriptions_;

    std::string_view default_description_;
};
