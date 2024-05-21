#include <gtest/gtest.h>
#include <numeric>
#include <utility>

#include "../src/tape/in_memory_tape.h"

TEST(tape, in_memory_read) {
    std::vector<int> data = {1, 5, 2, 6, 3, 7, 4, 8, 5, 9};
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(data);

    for (int i : data) {
        ASSERT_EQ(i, tape->get());
        if (tape->can_move_right()) {
            tape->move_right();
        }
    }
}

TEST(tape, in_memory_read_empty) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(0, tape->get());
        if (tape->can_move_right()) {
            tape->move_right();
        }
    }
}

TEST(tape, in_memory_write) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    for (int i = 0; tape->can_move_right(); i++, tape->move_right()) {
        tape->put(i);
    }

    tape->move_to_start();

    std::vector<int> expected = std::vector<int>(10);
    std::iota(expected.begin(), expected.end(), 0);

    for (int i = 0; tape->can_move_right(); i++, tape->move_right()) {
        ASSERT_EQ(expected[i], tape->get());
    }
}
