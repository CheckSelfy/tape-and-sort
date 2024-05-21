#include <gtest/gtest.h>
#include <numeric>
#include <utility>

#include "../src/tape/in_memory_tape.h"
#include "../src/tape/delay_tape.h"

TEST(in_memory_tape, read) {
    std::vector<int> data = {1, 5, 2, 6, 3, 7, 4, 8, 5, 9};
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(data);

    for (int idx = 0; idx < data.size() - 1; idx++) {
        ASSERT_EQ(data[idx], tape->get());
        ASSERT_TRUE(tape->can_move_right());
        tape->move_right();
    }

    ASSERT_EQ(data.back(), tape->get());
    ASSERT_FALSE(tape->can_move_right());
}

TEST(in_memory_tape, read_empty) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(0, tape->get());
        ASSERT_TRUE(tape->can_move_right());
        tape->move_right();
    }
    ASSERT_EQ(0, tape->get());
    ASSERT_FALSE(tape->can_move_right());
}

TEST(in_memory_tape, write) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    for (int i = 0; i < 9; i++) {
        ASSERT_TRUE(tape->can_move_right());
        tape->put(i);
        tape->move_right();
    }

    tape->put(9);
    ASSERT_FALSE(tape->can_move_right());
    tape->move_to_start();

    std::vector<int> expected = std::vector<int>(10);
    std::iota(expected.begin(), expected.end(), 0);

    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(expected[i], tape->get());
        ASSERT_TRUE(tape->can_move_right());
        tape->move_right();
    }

    ASSERT_EQ(expected.back(), tape->get());
    ASSERT_FALSE(tape->can_move_right());
}

TEST(in_memory_tape, move_left) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    ASSERT_FALSE(tape->can_move_left());
    ASSERT_TRUE(tape->can_move_right());
    tape->move_right();

    for (int i = 1; i < 9; i++) {
        ASSERT_TRUE(tape->can_move_right());
        ASSERT_TRUE(tape->can_move_left());
        tape->move_right();
    }

    ASSERT_FALSE(tape->can_move_right());
    ASSERT_TRUE(tape->can_move_left());

    tape->move_left();

    for (int i = 1; i < 9; i++) {
        ASSERT_TRUE(tape->can_move_right());
        ASSERT_TRUE(tape->can_move_left());
        tape->move_left();
    }

    ASSERT_FALSE(tape->can_move_left());
}

TEST(in_memory_tape, move_to_start) {
    std::unique_ptr<tape_base> tape = std::make_unique<in_memory_tape>(10);

    ASSERT_FALSE(tape->can_move_left());
    ASSERT_TRUE(tape->can_move_right());

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < i; j++) {
            ASSERT_TRUE(tape->can_move_right());
            tape->move_right();
        }
        tape->move_to_start();
        ASSERT_FALSE(tape->can_move_left());
    }
}

// warning: increasing these numbers brings to longer test execution
static constexpr delays sample_delays = {100, 200, 300, 400};

TEST(delay_tape, move_right) {
    std::unique_ptr<tape_base> tape = std::make_unique<delay_tape<in_memory_tape>>(sample_delays, 10);
    auto start = std::chrono::high_resolution_clock::now();
    while (tape->can_move_right()) {
        tape->move_right();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    constexpr auto expected_min = std::chrono::milliseconds(9 * sample_delays.move_time);
    constexpr auto expected_max = std::chrono::milliseconds(11 * sample_delays.move_time);
    ASSERT_LE(expected_min, actual);
    ASSERT_LE(actual, expected_max);
}

TEST(delay_tape, get) {
    std::unique_ptr<tape_base> tape = std::make_unique<delay_tape<in_memory_tape>>(sample_delays, 1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; i++) {
        tape->get();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    constexpr auto expected_min = std::chrono::milliseconds(9 * sample_delays.read_time);
    constexpr auto expected_max = std::chrono::milliseconds(11 * sample_delays.read_time);
    ASSERT_LE(expected_min, actual);
    ASSERT_LE(actual, expected_max);
}


TEST(delay_tape, put) {
    std::unique_ptr<tape_base> tape = std::make_unique<delay_tape<in_memory_tape>>(sample_delays, 1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; i++) {
        tape->put(42);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    constexpr auto expected_min = std::chrono::milliseconds(9 * sample_delays.put_time);
    constexpr auto expected_max = std::chrono::milliseconds(11 * sample_delays.put_time);
    ASSERT_LE(expected_min, actual);
    ASSERT_LE(actual, expected_max);
}

TEST(delay_tape, move_to_start) {
    std::unique_ptr<tape_base> tape = std::make_unique<delay_tape<in_memory_tape>>(sample_delays, 5);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; i++) {
        tape->move_to_start();
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto actual = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    constexpr auto expected_min = std::chrono::milliseconds(4 * sample_delays.move_to_start_time);
    constexpr auto expected_max = std::chrono::milliseconds(6 * sample_delays.move_to_start_time);
    ASSERT_LE(expected_min, actual);
    ASSERT_LE(actual, expected_max);
}
