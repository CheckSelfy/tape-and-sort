#pragma once

#include <cstdint>
#include <vector>

class tape_base {
public:
    using int_t = std::int32_t;
    using chunk_t = std::vector<int_t>;

    virtual ~tape_base() = default;

    tape_base() = default;
    tape_base(tape_base const&) = delete;

    virtual tape_base& put(int_t) = 0;

    virtual int_t get() = 0;

    virtual bool can_move_left() = 0;
    virtual void move_left() = 0;

    virtual bool can_move_right() = 0;
    virtual void move_right() = 0;

    virtual void move_to_start() = 0;

    virtual std::size_t get_size() = 0;
};