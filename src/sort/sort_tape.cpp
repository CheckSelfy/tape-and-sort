#include <memory>
#include <algorithm>
#include "sort_tape.h"
#include "../tape/in_memory_tape.h"


namespace {
    void copy_and_sort_chunks(tape_base &from, tape_base & to, std::size_t M);
    std::size_t copy_piece(tape_base* from, tape_base* to, std::size_t count);
    void merge(tape_base *first, tape_base *second, tape_base *to, size_t piece);
}

void sort_tape(tape_base &from, tape_base &to, std::size_t M) {
    const std::size_t N = from.get_size();

    // TODO
    in_memory_tape first_big = in_memory_tape(N);
    in_memory_tape second_big = in_memory_tape(N);
    in_memory_tape intermediate = in_memory_tape(N);

    tape_base* resulted_tape = &first_big;
    tape_base* big_tape = &second_big;
    tape_base* small_tape = &intermediate;

    if (M != 0) {
        copy_and_sort_chunks(from, *resulted_tape, M);
        M = 1; // we sorted chunks (size is 1)
    }
    resulted_tape->move_to_start();

    for (std::size_t sorted_piece_size = M; sorted_piece_size < N; sorted_piece_size *= 2) {
        std::swap(resulted_tape, big_tape);
        for (std::size_t cur_chunk = 0; cur_chunk < (N + sorted_piece_size - 1) / sorted_piece_size; cur_chunk += 2) {
            small_tape->move_to_start();
            std::size_t moved_big = copy_piece(big_tape, small_tape, sorted_piece_size);
            small_tape->move_to_start();
            if (moved_big == sorted_piece_size) { // if we need merging
                merge(big_tape, small_tape, resulted_tape, sorted_piece_size);
            } else { // we copied only small
                copy_piece(small_tape, resulted_tape, sorted_piece_size);
            }
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

std::size_t copy_piece(tape_base* from, tape_base* to, std::size_t count) {
    std::size_t moved_from = 0;
    for (; moved_from < count; moved_from++) {
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
    return moved_from;
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
