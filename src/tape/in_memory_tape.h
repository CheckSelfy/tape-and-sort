#pragma once

#include "tape_base.h"
#include <stdexcept>

class in_memory_tape : public tape_base {
private:
    std::vector<tape_base::int_t> numbers;
    std::size_t idx;

    void check_for_size();
public:
    explicit in_memory_tape(std::size_t n);
    explicit in_memory_tape(std::vector<tape_base::int_t> const& data);

    ~in_memory_tape() override = default;

    in_memory_tape& put(tape_base::int_t x) override;

    tape_base::int_t get() override;

    bool can_move_left() override;

    void move_left() override;

    bool can_move_right() override;

    void move_right() override;

    void move_to_start() override;

    std::size_t get_size() override;

    void close() override;

};
