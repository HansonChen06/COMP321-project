# wrong solution runs Dijkstra from clearing 1 instead of clearing n
# It correctly finds the shortest distance from clearing 1 to clearing n
# but the distances used to check the guide's guesses are incorrect. 
# The guide's guesses represent distances from each clearing to clearing n
# while this solution calculates distances from clearing 1 to each clearing

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

# WRONG: start from clearing 1 instead of n
dist[1] = 0
heap = [(0, 1)]

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
print(dist[n])

# WRONG: these distances are from clearing 1, not n
for i in range(1, n + 1):
    if guess[i] != dist[i]:
        print(i, dist[i])