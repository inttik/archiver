add_executable(
        archiver
        archiver.cpp
        console_reader.cpp
        encoder.cpp
        decoder.cpp
        bit_reader.cpp
        bit_writer.cpp
        bit_stream.cpp
)

add_catch(test_archiver_trie tests/trie_test.cpp)
add_catch(test_archiver_heap tests/heap_test.cpp)

add_catch(test_archiver_bit_streams tests/bit_streams_test.cpp bit_reader.cpp bit_writer.cpp bit_stream.cpp)

add_catch(test_archiver_encoder tests/encoder_test.cpp encoder.cpp bit_reader.cpp bit_writer.cpp bit_stream.cpp)
add_catch(test_archiver_decoder tests/decoder_test.cpp decoder.cpp bit_reader.cpp bit_writer.cpp bit_stream.cpp)

add_catch(test_archiver_console_reader tests/console_reader_test.cpp console_reader.cpp)
add_catch(
        test_archiver_fast
        tests/trie_test.cpp 
        tests/heap_test.cpp
        tests/bit_streams_test.cpp 
        bit_reader.cpp 
        bit_writer.cpp 
        bit_stream.cpp
        tests/console_reader_test.cpp 
        console_reader.cpp
)
