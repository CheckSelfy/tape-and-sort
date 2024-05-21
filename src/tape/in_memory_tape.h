#pragma once

#include "tape_base.h"
#include <stdexcept>

class in_memory_tape : public tape_base {
private:
    std::vector<tape_base::int_t> numbers;
    std::size_t idx;
public:
    explicit in_memory_tape(std::size_t n) : numbers(n), idx(0) {}
    explicit in_memory_tape(std::vector<tape_base::int_t> const& data) : numbers(data), idx(0) {}

    ~in_memory_tape() override = default;

    in_memory_tape& put(tape_base::int_t x) override {
        numbers[idx] = x;
        return *this;
    }

    tape_base::int_t get() override {
        return numbers[idx];
    }

    bool can_move_left() override {
        return idx != 0;
    }

    void move_left() override {
        if (idx == 0) {
            throw std::out_of_range("Trying to go left while head is on start");
        }
        idx--;
    }

    bool can_move_right() override {
        return idx != numbers.size() - 1;
    }

    void move_right() override {
        if (idx == numbers.size() - 1) {
            throw std::out_of_range("Trying to go right while head is on end");
        }
        idx++;
    }

    void move_to_start() override {
        idx = 0;
    }

};