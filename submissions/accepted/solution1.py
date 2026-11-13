import heapq
import sys

input = sys.stdin.buffer.readline

n, m = map(int, input().split())

# guide's guesses, index from 1 to n
guess = [0] * (n + 1)
for i in range(1, n + 1):
    guess[i] = int(input())

graph = [[] for _ in range(n + 1)]

for _ in range(m):
    a, b, w = map(int, input().split())
    graph[a].append((b, w))
    graph[b].append((a, w))

# Dijkstra's algorithm
dist = [float('inf')] * (n + 1)
dist[n] = 0
# heapq sorts by the first factor of the tuple
heap = [(0, n)]

while heap:
    d, c_node = heapq.heappop(heap)

    # skip outdated entries
    if d != dist[c_node]:
        continue

    for next_node, w in graph[c_node]:
        new_dist = d + w

        if new_dist < dist[next_node]:
            dist[next_node] = new_dist
            heapq.heappush(heap, (new_dist, next_node))

# shortest distance from clearing 1 to n
print(dist[1])

# find the incorrect guesses
for i in range(1, n + 1):
    if guess[i] != dist[i]:
        print(i, dist[i])