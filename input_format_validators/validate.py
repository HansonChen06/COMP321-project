import sys
import re
import math
import heapq

def dijkstra(graph, start_node):

    distances = {vertex: (10 ** 7 + 7) for vertex in graph}
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
first_line = sys.stdin.readline().rstrip("\r\n")
assert re.match ('^[1-9][0-9]{0,5} [1-9][0-9]{0,5}$', first_line)

n, m = map (int, first_line.split()) # split line

# Check bounds
assert 2 <= n <= 2 * (10 ** 5)
assert 1 <= m <= 2 * (10 ** 5)

# Store guesses to ensure wrong on exactly ceiling (n / 100) of them
guesses = []

for _ in range(n):

    # Read a guess line, which is exactly one int
    guess_line = sys.stdin.readline().rstrip("\r\n")
    assert re.match ('^(0|[1-9][0-9]{0,7})$', guess_line)

    guess = int(guess_line)
    assert 0 <= guess <= (10 ** 7) # Make sure within bounds

    guesses.append(guess)

edges = {i : [] for i in range(1, n+1)} # Since nodes are from 1 to n inclusive

for _ in range(m):

    # Read path line, which is 3 ints: start node, end node and weight/distance
    path_line = sys.stdin.readline().rstrip("\r\n")
    assert re.match ('^[1-9][0-9]{0,5} [1-9][0-9]{0,5} [1-9][0-9]{0,7}$', path_line)

    a, b, k = map (int, path_line.split())

    # Check bounds
    assert 1 <= a <= n
    assert 1 <= b <= n
    assert 1 <= k <= (10 ** 7)

    #Add edges to graph

    edges[a].append((b, k))
    edges[b].append((a, k))

assert sys.stdin.readline() == '' # Make sure there is no more input

# Run Dijkstra algorithm to check the graph is connected and to verify that the guide is wrong on exactly ceil (n / 100) of them

distances = dijkstra (edges, n)

num_wrong = 0 # The number of clearings the guide is wrong on, must be exactly ceil (n / 100)

for i in range(1, n + 1):

    assert distances[i] < (10 ** 7 + 7) # If equal means that clearing can't be reached from clearing n, ie the graph isn't connected

    if distances[i] != guesses[i - 1]:
        num_wrong += 1


assert num_wrong == math.ceil (n / 100)

#If made it here, input is valid, so exit with code 42
sys.exit(42)