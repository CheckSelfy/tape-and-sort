#pragma once

#include "tape_base.h"

#include <stdexcept>
#include <fstream>

class file_tape : public tape_base {
    std::fstream file;
    const std::size_t size;

    static constexpr std::ptrdiff_t length_of_unit = sizeof (int_t);

public:
    file_tape(std::fstream &&file, std::size_t n);

    void close() override;

    tape_base &put(int_t value) override;

    int_t get() override;

    bool can_move_left() override;

    void move_left() override;

    bool can_move_right() override;

    void move_right() override;

    void move_to_start() override;

    size_t get_size() override;
};