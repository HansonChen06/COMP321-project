// All distances are 64-bit, as a path can exceed 2^31-1.

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
#include <limits>
using namespace std;

int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Getting guides guesses
    vector<long long> guide(n + 1);
    for (int i = 1; i <= n; ++i) cin >> guide[i];

    // Paths are undirected, so store both directions.
    vector<vector<pair<int, int>>> adj(n + 1);

    // Storing paths
    for (int i = 0; i < m; ++i) {

        int a, b, length;
        cin >> a >> b >> length;
        adj[a].push_back({b, length});
        adj[b].push_back({a, length});

    }

    // Setting up Dijkstra's algorithm
    const long long INF = numeric_limits<long long>::max() / 4;
    vector<long long> dist(n + 1, INF);
    using State = pair<long long, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[n] = 0;
    pq.push({0, n});

    while (!pq.empty()) {

        auto [du, u] = pq.top();
        pq.pop();
        
        if (du != dist[u]) continue;  // Skip outdated entries

        for (auto [v, weight] : adj[u]) {

            if (du + weight < dist[v]) {
                // If found better distance, updating and readding to PQ

                dist[v] = du + weight;
                pq.push({dist[v], v});

            }

        }

    }

    cout << dist[1] << '\n'; // Outputing shortest distance

    for (int i = 1; i <= n; ++i) {

        if (guide[i] != dist[i]) cout << i << ' ' << dist[i] << '\n'; // If guide's guess was wrong, outputing correct distance

    }

    return 0;
}
