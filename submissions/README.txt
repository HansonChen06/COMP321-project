Submissions are categorized as follows:

- accepted/: correct reference solutions.
- wrong_answer/: intentionally incorrect solutions caught by the test data.
- run_time_error/: submissions expected to terminate abnormally.
- time_limit_exceeded/: submissions expected to exceed the time limit.

The accepted submissions all work by running Dijkstra's algorithm starting from clearing n.
Once the algorithm returns a list with all of the shortest distances from clearing n,
the programs output the shortest distance (as it is from clearing n to clearing 1),
then go through all of the guide's guesses and check which ones are wrong. If they
find one that is wrong, it is output according to the problem statement.

The intentionally incorrect submissions target these common mistakes:

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

- wrong_answer/Wrong-Reverse.py runs Dijkstra's algorithm starting from clearing 1
  instead of clearing n, which is where it should be starting it from. This means
  it'll still get a correct solution for the shortest path, but the distance calculations
  to correct the guide will be wrong as they need to be from clearing n and not clearing
  1.

- time_limit_exceeded/Wrong-TLE.py is too slow as it scans all nodes for the shortest distance
  instead of using a priority queue. Otherwise it is identical to running the accepted submission.
