Submissions are categorized as follows:

- accepted/: correct reference solutions.
- wrong_answer/: intentionally incorrect solutions caught by the test data.
- run_time_error/: submissions expected to terminate abnormally.
- time_limit_exceeded/: submissions expected to exceed the time limit.

The intentionally incorrect C++ submissions target these common mistakes:

- wrong_answer/Wrong-Astar.cpp treats the guide's guesses as an admissible A*
  heuristic. Because some guesses are incorrect and may overestimate the
  remaining distance, stopping when the destination is first removed from the
  priority queue does not guarantee an optimal path. secret08 rejects this
  submission: the correct path 1 -> 2 -> 101 has length 2, but the incorrect
  A* search returns 4.

- wrong_answer/Wrong-Overflow.cpp stores complete shortest-path distances in
  signed 32-bit integers. Although one edge weighs at most 10,000,000, a path
  may be much longer than INT_MAX. secret20 rejects this submission because
  the correct distance from clearing 1 to clearing n is 19,990,000,000, so
  shortest-path distances must use 64-bit integers.
