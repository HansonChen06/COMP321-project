// The Unreliable Guide -- COMP 321 input format and semantic validator.
// Valid input: exit status 42. Invalid input: exit status 1.
// Compile: g++ -O2 -std=c++17 -Wall -Wextra validate.cpp -o validate
// Run: ./validate < ../data/sample/sample03.in; echo $?  # prints 42
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;

[[noreturn]] static void invalid(const string& message) {
    cerr << "Invalid input: " << message << '\n';
    exit(1);
}

static vector<ll> read_integers(int count, const string& where) {
    string line;
    if (!getline(cin, line)) invalid("missing line: " + where);
    if (!line.empty() && line.back() == '\r') line.pop_back();
    istringstream input(line);
    vector<ll> numbers;
    numbers.reserve(count);
    for (int i = 0; i < count; ++i) {
        ll val;
        if (!(input >> val)) invalid("expected " + to_string(count) + " integers on " + where);
        numbers.push_back(val);
    }
    string trailing;
    if (input >> trailing) invalid("too many entries on " + where);
    return numbers;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto first = read_integers(2, "first line");
    ll nn = first[0], mm = first[1];
    if (nn < 2 || nn > 200000 || mm < 1 || mm > 200000)
        invalid("n or m outside stated constraints");
    const int n = static_cast<int>(nn), m = static_cast<int>(mm);
    if (m < n - 1) invalid("an undirected connected graph needs at least n-1 edges");

    vector<ll> guesses(n + 1);
    for (int i = 1; i <= n; ++i) {
        auto vals = read_integers(1, "guess " + to_string(i));
        ll g = vals[0];
        if (g < 1 || g > 10000000LL) invalid("guide guess outside [1, 10^7]");
        guesses[i] = g;
    }

    vector<vector<pair<int,int>>> graph(n + 1);
    for (int j = 0; j < m; ++j) {
        auto vals = read_integers(3, "edge " + to_string(j + 1));
        ll a = vals[0], b = vals[1], w = vals[2];
        if (a < 1 || a > n || b < 1 || b > n || w < 1 || w > 10000000LL)
            invalid("edge endpoint or weight outside stated constraints");
        // Undirected paths. Parallel edges and self-loops are permitted.
        graph[static_cast<int>(a)].emplace_back(static_cast<int>(b), static_cast<int>(w));
        graph[static_cast<int>(b)].emplace_back(static_cast<int>(a), static_cast<int>(w));
    }
    string extra;
    if (getline(cin, extra)) invalid("extra trailing data (including blank lines)");

    // Validate both semantic promises: connectivity and exactly ceil(n / 100)
    // incorrect guesses. Distances can be as high as (n-1)*10^7, so use int64.
    const ll INF = LLONG_MAX / 4;
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    dist[n] = 0;
    pq.emplace(0, n);
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != dist[u]) continue;
        for (auto [v, w] : graph[u]) {
            ll nd = du + w;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.emplace(nd, v);
            }
        }
    }
    int wrong = 0;
    for (int i = 1; i <= n; ++i) {
        if (dist[i] == INF) invalid("graph is not connected");
        wrong += (guesses[i] != dist[i]);
    }
    const int need = (n + 99) / 100;
    if (wrong != need) {
        invalid("exactly ceil(n/100) guesses must be incorrect; found "
                + to_string(wrong) + " instead of " + to_string(need));
    }
    return 42;
}
