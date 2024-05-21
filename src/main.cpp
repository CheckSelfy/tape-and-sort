#include <iostream>
#include <algorithm>
#include <iterator>
#include <filesystem>
#include "tape/delay_tape.h"
#include "tape/file_tape.h"
#include "sort/sort_tape.h"
#include "config_reader/config_read.h"

namespace {
    void convert_to_binary(const char *in, const char *out);

    void convert_to_text(const char *in, const char *out);

    void create_temp();
}


using tape = delay_tape<file_tape>;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::cout << "Wrong arguments, expected 3, found: " << argc << std::endl;
        std::cout << "Usage: " << argv[0] << " <input-file> <output-file> <N> <M>" << std::endl;
        return EXIT_FAILURE;
    }

    std::size_t M;
    std::size_t N;
    try {
        N = std::stoul(argv[3]);
        M = std::stoul(argv[4]);
    } catch (std::exception const& e) {
        std::cerr << "Exception during parsing N or M: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    create_temp();

    try {
        convert_to_binary(argv[1], "tmp/read_binary");
    } catch (std::exception const& e) {
        std::cerr << "Exception during converting into binary: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delays d;
    try {
        d = config_read("config.txt");
    } catch (std::runtime_error const & e) {
        std::cerr << "Exception during read config: " << e.what();
        return EXIT_FAILURE;
    }

    try {
        tape input = tape(d, std::fstream("tmp/read_binary", std::ios::in | std::ios::out | std::ios::binary), N);
        tape output = tape(d, std::fstream("tmp/write_binary", std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary), N);

        sort_tape(input, output, M, [&d, &N]() {
            static uint32_t idx = 0;
            std::string file_name = "tmp/temp" + std::to_string(idx++);
            return std::make_unique<tape>(d, std::fstream(file_name, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc), N);
        });

        input.close();
        output.close();
    } catch (std::exception const& e) {
        std::cerr << "Exception during sorting: " << e.what() << std::endl;
    }

    try {
        convert_to_text("tmp/write_binary", argv[2]);
    } catch (std::exception const& e) {
        std::cerr << "Exception during converting into text: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace {
    void convert_to_binary(const char *in, const char *out) {
        std::ifstream fin = std::ifstream(in, std::ios::in);
        std::ofstream fout = std::ofstream(out, std::ios::trunc | std::ios::binary | std::ios::out);

        if (!fin.is_open()) {
            throw std::runtime_error("I/O exception: could not open input");
        }

        if (!fout.is_open()) {
            throw std::runtime_error("I/O exception: could not open output");
        }

        std::for_each(std::istream_iterator<int>(fin), std::istream_iterator<int>(), [&fout](int r) {
            fout.write(reinterpret_cast<const char *>(&r), sizeof(r));
        });
        fin.close();
        fout.close();
    }

    void convert_to_text(const char *in, const char *out) {
        std::ifstream fin = std::ifstream(in, std::ios::in | std::ios::binary);
        std::ofstream fout = std::ofstream(out, std::ios::out | std::ios::trunc);

        if (!fin.is_open()) {
            throw std::runtime_error("I/O exception: could not open input");
        }

        if (!fout.is_open()) {
            throw std::runtime_error("I/O exception: could not open output");
        }

        std::int32_t readed;
        while (fin.read(reinterpret_cast<char *>(&readed), sizeof(readed))) {
            fout << readed << " ";
        }

        fin.close();
        fout.close();
    }

    void create_temp() {
        std::filesystem::create_directory(std::filesystem::path("tmp"));
    }
}
