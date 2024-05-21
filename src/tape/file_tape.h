#pragma once

#include "tape_base.h"

#include <stdexcept>
#include <fstream>

class file_tape : public tape_base {
    std::fstream file;
    const std::size_t size;

    static constexpr std::ptrdiff_t length_of_unit = sizeof (int_t);

public:
    file_tape(std::fstream &&file, std::size_t n) : file(std::move(file)),
                                                    size(n) {
        if (n == 0) {
            throw std::invalid_argument("Data can't be empty");
        }
        if (file.is_open()) {
            throw std::invalid_argument("Couldn't read data");
        }
    }

    void close() {
        file.close();
    }

    tape_base &put(int_t value) override {
        auto pos = file.tellg();
        file.write(reinterpret_cast<const char *>(&value), sizeof(value));
        file.seekg(pos);
        return *this;
    }

    int_t get() override {
        auto pos = file.tellg();
        tape_base::int_t result;
        file.read(reinterpret_cast<char *>(&result), sizeof(result));
        file.seekg(pos);
        return result;
    }

    bool can_move_left() override {
        return file.tellp() > 0;
    }

    void move_left() override {
        if (!can_move_left()) {
            throw std::out_of_range("Trying to go left while head is on start");
        }
        file.seekg(-length_of_unit, std::ios::cur);
    }

    bool can_move_right() override {
        auto res = file.tellp();
        return file.tellp() < length_of_unit * size;
    }

    void move_right() override {
        if (!can_move_right()) {
            throw std::out_of_range("Trying to go right while head is on end");
        }
        file.seekg(length_of_unit, std::ios::cur);
    }

    void move_to_start() override {
        file.clear();
        file.seekg(0);
    }

    size_t get_size() override {
        return size;
    }
};