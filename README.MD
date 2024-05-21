# Tape and sort

In this task, I had two goals:
* implement interface `Tape`
* learn, how to sort `Tape`

## Build
```shell
$ git clone git@github.com:CheckSelfy/tape-and-sort.git
$ cd tape-and-sort
$ cmake -S . -B build
$ cd build
$ make 
# compiled binary is tape_and_sort
```

## Run
```shell
# build before
$ ./tape_and_sort <input-text> <output-text> <N> <M>
```

### Configurate 
Configuration file must be in same folder as executable and have next format:
```
write_time=100
read_time=2000
move_time=30
move_to_start_time=0
```
All units are milliseconds.

## Test
```shell
# build before
$ make test
```


## Tape

The Tape storage device is designed to write and read data sequentially. 
Actually, it is array of `int32_t` and head, pointed to one of number. 
Tape is not random-access - moving head is time-wasting operation.

Tape provides next methods to manipulate:
* `can_move_left` checks that head is not on leftest cell
* `move_left` moves head left
* `can_move_right` checks that head is not on rightest cell
* `move_right` moves head right 
* `move_to_start` moves to leftest cell
* `put` write number in cell
* `get` get number from cell

## Sorting idea

Chunk is part of array with size `M`.

Algorithm:
1. Sort chunks using STL library (use [heapsort](https://en.wikipedia.org/wiki/Heapsort), [`std::make_heap`](https://en.cppreference.com/w/cpp/algorithm/make_heap), [`std::heap_sort`](https://en.cppreference.com/w/cpp/algorithm/sort_heap) for `O(1)` memory)
2. Merge them, using 2 pointers method

Number of merging is `O(X log X)`, where `X = N / M`, sorting chunks is `O(M log M)`, so, sorting complexity is `O(N log n)`
