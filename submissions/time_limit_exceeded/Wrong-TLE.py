# TLE since we are not using a priority queue for Dijkstra's algorithm

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

# Dijkstra's algorithm without priority queue
dist = [float('inf')] * (n + 1)
dist[n] = 0

visited = [False] * (n + 1)

for _ in range(n):

    c_node = -1

    # find the unvisited node with the smallest distance
    for i in range(1, n + 1):

        if not visited[i]:

            if c_node == -1 or dist[i] < dist[c_node]:
                c_node = i

    if c_node == -1 or dist[c_node] == float('inf'):
        break

    visited[c_node] = True

    # update distances to neighbors
    for next_node, w in graph[c_node]:

        new_dist = dist[c_node] + w

        if new_dist < dist[next_node]:
            dist[next_node] = new_dist

# shortest distance from clearing 1 to n
print(dist[1])

# find the incorrect guesses
for i in range(1, n + 1):

    if guess[i] != dist[i]:
        
        print(i, dist[i])
