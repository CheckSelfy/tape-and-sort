#pragma once

#include "../tape/tape_base.h"


void sort_tape(tape_base &from, tape_base &to, std::size_t M, std::function<std::unique_ptr<tape_base> ()> factory);

