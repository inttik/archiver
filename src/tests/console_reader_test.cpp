#include <catch.hpp>
#include <sstream>

#include "console_reader.h"

using Arguments = const std::vector<std::string_view>&;

TEST_CASE("no extra arguments") {
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.empty());
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1);

        const size_t size = 1;
        int argc = size;
        char const* argv[size] = {"C:\\"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "12");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 1);

        const size_t size = 2;
        int argc = size;
        char const* argv[size] = {"C:\\", "12"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "1");
            REQUIRE(arguments[1] == "2");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 1);

        const size_t size = 3;
        int argc = size;
        char const* argv[size] = {"C:\\", "1", "2"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            FAIL("expected no launch");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 1, 0);

        const size_t size = 3;
        int argc = size;
        char const* argv[size] = {"C:\\", "1", "2"};

        try {
            console_reader.Read(argc, argv);

            FAIL("expected InvalidArgument error");
        } catch (const InvalidArgument&) {
        }

        REQUIRE(!launched);
        REQUIRE(empty.str().empty());
    }
}

TEST_CASE("no default func") {
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            FAIL("expected: func3: no launch");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1, 0);
        console_reader.AddParam("-key2", func2, "", 1, 0);
        console_reader.AddParam("-key3", func3, "", 1, 0);

        const size_t size = 3;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(!launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "-key1");
            REQUIRE(arguments[1] == "-key2");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            FAIL("expected: func2: no launch");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 2, 0);
        console_reader.AddParam("-key2", func2, "", 1, 0);
        console_reader.AddParam("-key3", func3, "", 1, 0);

        const size_t size = 4;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(!launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1);
        console_reader.AddParam("-key2", func2, "", 1);
        console_reader.AddParam("-key3", func3, "", 1);

        const size_t size = 4;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1=10");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "-key2");
            REQUIRE(arguments[1] == "arg2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1);
        console_reader.AddParam("-key2", func2, "", 1);
        console_reader.AddParam("-key3", func3, "", 1);

        const size_t size = 5;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1=10", "-key2", "arg2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1=10");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "-key2");
            REQUIRE(arguments[1] == "arg2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1);
        console_reader.AddParam("-key2", func2, "", 1);
        console_reader.AddParam("-key3", func3, "", 1, 0);

        const size_t size = 6;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1=10", "-key2", "arg2", "-key3", "unknown arg"};
        try {
            console_reader.Read(argc, argv);

            FAIL("expected FailArgument exception");
        } catch (InvalidArgument&) {
        }
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1=10");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "-key2=");
            REQUIRE(arguments[1] == "arg2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1);
        console_reader.AddParam("-key2", func2, "", 1);
        console_reader.AddParam("-key3", func3, "", 1);

        const size_t size = 6;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1=10", "-key2=", "arg2", "-key3", "-unknownKey"};
        try {
            console_reader.Read(argc, argv);

            FAIL("expected FailArgument exception");
        } catch (InvalidArgument&) {
        }
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
}

TEST_CASE("need exe directory") {
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "C:\\");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 0, 0, true);

        const size_t size = 1;
        int argc = size;
        char const* argv[size] = {"C:\\"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "arg1");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 1, 0, true);

        const size_t size = 2;
        int argc = size;
        char const* argv[size] = {"C:\\", "arg1"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched = false;
        auto func1 = [&launched](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "arg1");
            launched = true;
            return 0;
        };

        console_reader.AddDefault(func1, "", 0, 1, true);

        const size_t size = 2;
        int argc = size;
        char const* argv[size] = {"C:\\", "arg1"};
        console_reader.Read(argc, argv);
        REQUIRE(launched);
        REQUIRE(empty.str().empty());
    }

    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            FAIL("expected: func3: no launch");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1, 0);
        console_reader.AddParam("-key2", func2, "", 1, 0, true);
        console_reader.AddParam("-key3", func3, "", 1, 0, true);

        const size_t size = 3;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(!launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 3);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key1");
            REQUIRE(arguments[2] == "-key2");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            FAIL("expected: func2: no launch");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 2, 0, true);
        console_reader.AddParam("-key2", func2, "", 1, 0);
        console_reader.AddParam("-key3", func3, "", 1, 0);

        const size_t size = 4;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(!launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key1");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 1);
            REQUIRE(arguments[0] == "-key2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1);
        console_reader.AddParam("-key2", func2, "", 1);
        console_reader.AddParam("-key3", func3, "", 1, Function::INFINITY_COUNT, true);

        const size_t size = 4;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1", "-key2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
    {
        std::stringstream empty;
        ConsoleReader console_reader(empty);

        bool launched1 = false;
        bool launched2 = false;
        bool launched3 = false;
        auto func1 = [&launched1](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key1=10");
            launched1 = true;
            return 0;
        };
        auto func2 = [&launched2](Arguments& arguments) {
            REQUIRE(arguments.size() == 3);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key2");
            REQUIRE(arguments[2] == "arg2");
            launched2 = true;
            return 0;
        };
        auto func3 = [&launched3](Arguments& arguments) {
            REQUIRE(arguments.size() == 2);
            REQUIRE(arguments[0] == "C:\\");
            REQUIRE(arguments[1] == "-key3");
            launched3 = true;
            return 0;
        };

        console_reader.AddParam("-key1", func1, "", 1, Function::INFINITY_COUNT, true);
        console_reader.AddParam("-key2", func2, "", 1, Function::INFINITY_COUNT, true);
        console_reader.AddParam("-key3", func3, "", 1, Function::INFINITY_COUNT, true);

        const size_t size = 5;
        int argc = size;
        char const* argv[size] = {"C:\\", "-key1=10", "-key2", "arg2", "-key3"};
        console_reader.Read(argc, argv);
        REQUIRE(launched1);
        REQUIRE(launched2);
        REQUIRE(launched3);
        REQUIRE(empty.str().empty());
    }
}
