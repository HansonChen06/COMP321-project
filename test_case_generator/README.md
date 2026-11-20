# Test case generator

This directory contains the test case generator, which helped generate all sample and secret test cases.

The generator functions are:

- `tree()`: construct a connected chain, star, comb or random tree.
- `add_edges()`: add weighted edges, optional parallel edges and self-loops.
- `dijkstra()`: shortest distances from destination n, using `long long`.
- `guesses()`: make exactly `ceil(n/100)` guesses wrong, including n.
- `write_case()`: internal constraints check and paired `.in/.ans` writing.
- `astar_trap()` / `overflow_trap()`: targeted incorrect-solution detection.
- `descriptions()`: explain all 23 test cases in `Description.txt`.

Randomized cases use `std::mt19937` with fixed seeds, making files reproducible with the same C++ standard library implementation.

The generator intentionally includes parallel edges and self-loops; the final problem statement and input validator must permit them. `g_i` is always in `[0,10^7]` even if a true shortest distance is larger. The exact incorrect-guess count is enforced.

## Compile and run (Linux/macOS)

In this directory:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic generate.cpp -o generate
./generate .
```

The generator writes `data/sample/`, `data/secret/` and `data/Description.txt`. It produces **3 samples + 20 secrets**, each with matching `.in/.ans`. Graphs are undirected, connected and weighted, with exactly `ceil(n/100)` incorrect guesses. All files combined stay below 50 MB.