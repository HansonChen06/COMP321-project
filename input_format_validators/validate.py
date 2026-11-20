import sys
import re
import math
import heapq

def invalid(message):
    print(f"Invalid input: {message}", file=sys.stderr)
    sys.exit(1)

def read_line(pattern, where):
    line = sys.stdin.readline()
    if not line:
        invalid(f"missing line: {where}")
    if not line.endswith("\n"):
        invalid(f"missing final newline on {where}")
    line = line[:-1]
    if not re.fullmatch(pattern, line):
        invalid(f"invalid syntax on {where}")
    return line

def dijkstra(graph, start_node):
    distances = {vertex: math.inf for vertex in graph}
    distances[start_node] = 0
    pq = [(0, start_node)]

    while pq:

        current_distance, u = heapq.heappop(pq)

        if current_distance > distances[u]:
            continue
        
        for neighbor, weight in graph[u]:

            distance = current_distance + weight

            if distance < distances[neighbor]:

                distances[neighbor] = distance
                heapq.heappush(pq, (distance, neighbor))

    return distances

# Check first line, must have 2 ints
first_line = read_line(r'[1-9][0-9]{0,5} [1-9][0-9]{0,5}', 'first line')

n, m = map (int, first_line.split()) # split line

# Check bounds
if not 2 <= n <= 2 * (10 ** 5):
    invalid('n outside [2, 200000]')
if not 1 <= m <= 2 * (10 ** 5):
    invalid('m outside [1, 200000]')
if m < n - 1:
    invalid('an undirected connected graph needs at least n-1 edges')

# Store guesses to ensure wrong on exactly ceiling (n / 100) of them
guesses = []

for _ in range(n):

    # Read a guess line, which is exactly one int
    guess_line = read_line(r'(0|[1-9][0-9]{0,7})', f'guess {_ + 1}')

    guess = int(guess_line)
    if not 0 <= guess <= (10 ** 7):
        invalid(f'guide guess {_ + 1} outside [0, 10^7]')

    guesses.append(guess)

edges = {i : [] for i in range(1, n+1)} # Since nodes are from 1 to n inclusive

for _ in range(m):

    # Read path line, which is 3 ints: start node, end node and weight/distance
    path_line = read_line(
        r'[1-9][0-9]{0,5} [1-9][0-9]{0,5} [1-9][0-9]{0,7}',
        f'edge {_ + 1}',
    )

    a, b, k = map (int, path_line.split())

    # Check bounds
    if not 1 <= a <= n or not 1 <= b <= n:
        invalid(f'edge {_ + 1} endpoint outside [1, n]')
    if not 1 <= k <= (10 ** 7):
        invalid(f'edge {_ + 1} weight outside [1, 10^7]')

    #Add edges to graph

    edges[a].append((b, k))
    edges[b].append((a, k))

if sys.stdin.readline() != '':
    invalid('extra trailing input')

# Run Dijkstra algorithm to check the graph is connected and to verify that the guide is wrong on exactly ceil (n / 100) of them

distances = dijkstra (edges, n)

num_wrong = 0 # The number of clearings the guide is wrong on, must be exactly ceil (n / 100)

for i in range(1, n + 1):

    if math.isinf(distances[i]):
        invalid('graph is not connected')

    if distances[i] != guesses[i - 1]:
        num_wrong += 1


expected_wrong = math.ceil(n / 100)
if num_wrong != expected_wrong:
    invalid(f'expected {expected_wrong} incorrect guesses, found {num_wrong}')

#If made it here, input is valid, so exit with code 42
sys.exit(42)
