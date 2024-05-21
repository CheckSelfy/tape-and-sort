#include "file_tape.h"

file_tape::file_tape(std::fstream &&file, std::size_t n) : file(std::move(file)),
                                                           size(n) {
    if (n == 0) {
        throw std::invalid_argument("Data can't be empty");
    }
    if (file.is_open()) {
        throw std::invalid_argument("Couldn't read data");
    }
}

void file_tape::close() {
    file.close();
}

tape_base &file_tape::put(tape_base::int_t value) {
    auto pos = file.tellg();
    file.write(reinterpret_cast<const char *>(&value), sizeof(value));
    file.seekg(pos);
    return *this;
}

tape_base::int_t file_tape::get() {
    auto pos = file.tellg();
    tape_base::int_t result;
    file.read(reinterpret_cast<char *>(&result), sizeof(result));
    file.seekg(pos);
    return result;
}

bool file_tape::can_move_left() {
    return file.tellp() > 0;
}

void file_tape::move_left() {
    if (!can_move_left()) {
        throw std::out_of_range("Trying to go left while head is on start");
    }
    file.seekg(-length_of_unit, std::ios::cur);
}

bool file_tape::can_move_right() {
    return file.tellp() < length_of_unit * (size - 1);
}

void file_tape::move_right() {
    if (!can_move_right()) {
        throw std::out_of_range("Trying to go right while head is on end");
    }
    file.seekg(length_of_unit, std::ios::cur);
}

void file_tape::move_to_start() {
    file.clear();
    file.seekg(0);
}

size_t file_tape::get_size() {
    return size;
}
