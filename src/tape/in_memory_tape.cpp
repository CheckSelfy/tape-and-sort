#include "in_memory_tape.h"

void in_memory_tape::check_for_size() {
    if (numbers.empty()) {
        throw std::invalid_argument("Data can't be empty");
    }
}

in_memory_tape::in_memory_tape(std::size_t n) : numbers(n), idx(0) {
    check_for_size();
}

in_memory_tape::in_memory_tape(const std::vector<tape_base::int_t> &data) : numbers(data), idx(0) {
    check_for_size();
}

in_memory_tape &in_memory_tape::put(tape_base::int_t x) {
    numbers[idx] = x;
    return *this;
}

tape_base::int_t in_memory_tape::get() {
    return numbers[idx];
}

bool in_memory_tape::can_move_left() {
    return idx != 0;
}

void in_memory_tape::move_left() {
    if (idx == 0) {
        throw std::out_of_range("Trying to go left while head is on start");
    }
    idx--;
}

bool in_memory_tape::can_move_right() {
    return idx < numbers.size() - 1;
}

void in_memory_tape::move_right() {
    if (idx == numbers.size() - 1) {
        throw std::out_of_range("Trying to go right while head is on end");
    }
    idx++;
}

void in_memory_tape::move_to_start() {
    idx = 0;
}

std::size_t in_memory_tape::get_size() {
    return numbers.size();
}

void in_memory_tape::close() { }
