#include <memory>
#include <algorithm>
#include "sort_tape.h"
#include "../tape/in_memory_tape.h"


namespace {
    void copy_and_sort_chunks(tape_base &from, tape_base & to, std::size_t M);
    void copy_piece(tape_base* from, tape_base* to, std::size_t count);
    void merge(tape_base *first, tape_base *second, tape_base *to, size_t piece);
}

void sort_tape(tape_base &from, tape_base &to, const std::size_t M) {
    const std::size_t N = from.get_size();

    // TODO
    in_memory_tape first_temp = in_memory_tape(N);
    in_memory_tape second_temp = in_memory_tape(M);
    in_memory_tape third_temp = in_memory_tape(N);

    copy_and_sort_chunks(from, first_temp, M);
    first_temp.move_to_start();

    tape_base* resulted_tape = &first_temp;
    tape_base* small_tape = &second_temp;
    tape_base* big_tape = &third_temp;

    for (std::size_t sorted_piece_size = M; sorted_piece_size < N; sorted_piece_size *= 2) {
        std::swap(resulted_tape, big_tape);
        for (std::size_t cur_chunk = 0; cur_chunk < (N + sorted_piece_size - 1) / sorted_piece_size; cur_chunk += 2) {
            copy_piece(big_tape, small_tape, sorted_piece_size);

            small_tape->move_to_start();
            merge(big_tape, small_tape, resulted_tape, sorted_piece_size);
        }
        resulted_tape->move_to_start();
        big_tape->move_to_start();
    }

    copy_piece(resulted_tape, &to, N);
}

namespace {

void copy_and_sort_chunks(tape_base &from, tape_base & to, const std::size_t M) {
    std::vector<tape_base::int_t> chunk;
    chunk.reserve(M);

    while (from.can_move_right()) {
        chunk.clear();
        for (int i = 0; i < M; i++) {
            chunk.push_back(from.get());
            from.move_right();
            if (!from.can_move_right()) {
                chunk.push_back(from.get());
                break;
            }
        }

        std::make_heap(chunk.begin(), chunk.end());
        std::sort_heap(chunk.begin(), chunk.end());

        for (int i = 0; i < M && to.can_move_right(); i++) {
            to.put(chunk[i]);
            to.move_right();
        }

        if (!to.can_move_right()) {
            to.put(chunk.back());
        }
    }
}

void copy_piece(tape_base* from, tape_base* to, std::size_t count) {
    for (int i = 0; i < count; i++) {
        to->put(from->get());
        if (!from->can_move_right()) {
            break;
        }
        from->move_right();
        if (!to->can_move_right()) {
            break;
        }
        to->move_right();
    }
}

void merge(tape_base *first, tape_base *second, tape_base *to, size_t piece) {
    std::size_t first_index = 0;
    std::size_t second_index = 0;
    while (first_index < piece && second_index < piece) {
        auto first_int = first->get();
        auto second_int = second->get();
        if (first_int <= second_int) {
            to->put(first_int);
            to->move_right();
            first_index++;
            if (!first->can_move_right()) {
                break;
            }
            first->move_right();
        } else {
            to->put(second_int);
            to->move_right();
            second_index++;
            if (!second->can_move_right()) {
                break;
            }
            second->move_right();
        }
    }

    copy_piece(first, to, piece - first_index);
    copy_piece(second, to, piece - second_index);
}

}
