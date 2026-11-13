# The Unreliable Guide — C++17 Test Case Generator

This package uses C++17 for the entire Deliverable 2.4 generator and includes C++ accepted and wrong solutions and a C++ input validator for testing.

## Compile and run (Linux/macOS)

In this directory:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic test_case_generator/generate.cpp -o generate
./generate .
```

The optional argument `.` is the output project root. The generator writes `data/sample/`, `data/secret/` and `data/Description.txt`. It produces **3 samples + 20 secrets**, each with matching `.in/.ans`. Graphs are undirected, connected and weighted, with exactly `ceil(n/100)` incorrect guesses. All files combined stay below 50 MB.

For independent checks:

```bash
g++ -std=c++17 -O2 input_format_validators/validate.cpp -o validate
g++ -std=c++17 -O2 submissions/accepted/dijkstra_reverse.cpp -o accepted
./validate < data/sample/sample01.in
echo $?        # expected 42
./accepted < data/sample/sample01.in
cat data/sample/sample01.ans
```

The generator functions are:

- `tree()`: construct a connected chain, star, comb or random tree.
- `add_edges()`: add weighted edges, optional parallel edges and self-loops.
- `dijkstra()`: shortest distances from destination n, using `long long`.
- `guesses()`: make exactly `ceil(n/100)` guesses wrong, including n.
- `write_case()`: internal constraints check and paired `.in/.ans` writing.
- `astar_trap()` / `overflow_trap()`: targeted incorrect-solution detection.
- `descriptions()`: explain all 23 test cases in `Description.txt`.

Randomized cases use `std::mt19937` with fixed seeds, making files reproducible with the same C++ standard library implementation.

The generator intentionally includes parallel edges and self-loops; the final problem statement and input validator must permit them. `g_i` is always in `[1,10^7]` even if a true shortest distance is larger. The exact incorrect-guess count is enforced.

**Important course rule:** Delivery 2.2 requires *two correct solutions in two different languages*. This standalone generator package includes a C++ solution, but the final Delivery 2 submission must also have Java or Python accepted code; the previous full project includes a Java implementation.

This is not the official MyCourses `problemskeleton.zip`. Copy the files into its appropriate directories before submitting.
