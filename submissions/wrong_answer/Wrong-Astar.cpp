// INTENTIONALLY WRONG: A* that stops when clearing n is first popped.
// The guide's incorrect guesses are NOT admissible heuristic estimates.
// On secret08 the shortest route is 1 -> 2 -> 101 (distance 2),
// but the overestimate at clearing 2 makes A* return distance 4.
// The correction lines are computed correctly, so the failure is
// isolated to the false assumption that A* always returns optimal d[1].
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using ll = long long;

int main() {

    ios::sync_with_stdio(false); cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<ll> h(n + 1);
    for (int i = 1; i <= n; ++i) cin >> h[i];

    vector<vector<pair<int,int>>> adj(n + 1);
    for (int i = 0; i < m; ++i) {

        int a,b,w; cin >> a >> b >> w;
        adj[a].push_back({b,w}); adj[b].push_back({a,w});

    }

    const ll INF = (1LL << 61);
    // Incorrect A* computes the first output line.
    vector<ll> fromStart(n + 1, INF);
    using State = pair<ll,int>;
    priority_queue<State,vector<State>,greater<State>> pq;
    fromStart[1] = 0; pq.push({h[1],1});
    ll claimedDistance = INF;
    
    while (!pq.empty()) {

        auto [f,u] = pq.top(); pq.pop();

        if (f != fromStart[u] + h[u]) continue;

        if (u == n) { 
            claimedDistance = fromStart[u];
            break; 
        }

        for (auto [v,w] : adj[u]) {

            if (fromStart[u] + w < fromStart[v]) {

                fromStart[v] = fromStart[u] + w;
                pq.push({fromStart[v] + h[v],v});

            }

        }

    }

    // Reverse Dijkstra for correction lines; this also exposes the bug:
    // in fact correct d[1] is already available but the buggy A* is trusted.
    vector<ll> dist(n + 1, INF);
    priority_queue<State,vector<State>,greater<State>> q;
    dist[n] = 0; q.push({0,n});
    while (!q.empty()) {
        auto [du,u] = q.top(); q.pop();
        if (du != dist[u]) continue;
        for (auto [v,w] : adj[u]) {
            if (du+w < dist[v]) { dist[v]=du+w; q.push({dist[v],v}); }
        }
    }
    
    cout << claimedDistance << '\n';
    for (int i = 1; i <= n; ++i)
        if (h[i] != dist[i]) cout << i << ' ' << dist[i] << '\n';
}
