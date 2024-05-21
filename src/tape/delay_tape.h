#pragma once

#include "tape_base.h"
#include <thread>

struct delays {
public:
    using time_t = uint32_t; // milliseconds

    time_t put_time;
    time_t read_time;
    time_t move_time;
    time_t move_to_start_time;
};

// decorator class that gives delays to all operations
template<class Tape> requires std::is_base_of_v<tape_base, Tape> // underlying tape must be inherited from tape_base
class delay_tape : public tape_base {
private:
    const delays d;
    Tape tape;

    void wait(delays::time_t t) {
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
    }

public:
    template<class... Args>
    explicit delay_tape(delays d, Args &&... args) : d(d), tape(std::forward<Args>(args)...) {}

    ~delay_tape() override = default;

    tape_base::int_t get() override {
        wait(d.read_time);
        return tape.get();
    }

    delay_tape &put(tape_base::int_t val) override {
        wait(d.put_time);
        tape.put(val);
        return *this;
    }

    bool can_move_left() override {
        return tape.can_move_left();
    }

    void move_left() override {
        wait(d.move_time);
        tape.move_left();
    }

    bool can_move_right() override {
        return tape.can_move_right();
    }

    void move_right() override {
        wait(d.move_time);
        tape.move_right();
    }

    void move_to_start() override {
        wait(d.move_to_start_time);
        tape.move_to_start();
    }

    void close() {
        tape.close();
    }

    std::size_t get_size() override {
        return tape.get_size();
    }
};
