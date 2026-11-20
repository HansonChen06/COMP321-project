// The Unreliable Guide: complete deterministic C++17 generator.
// g++ -O2 -std=c++17 generate.cpp -o generate && ./generate .
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;
namespace fs = std::filesystem;
using ll = long long;
constexpr int MAX_N = 200000, MAX_M = 200000, MAX_G = 10000000;
constexpr ll INF = numeric_limits<ll>::max() / 4;
struct Edge { int a, b, w; };
using Edges = vector<Edge>;
using Dist = vector<ll>;
enum class Shape { Chain, Star, Random, Comb };
void ensure(bool ok, const string& why) { if (!ok) throw runtime_error(why); }

// Dijkstra from destination n: distances to n for every vertex.
Dist dijkstra(int n, const Edges& edges) {
    vector<vector<pair<int,int>>> graph(n + 1);
    for (auto e : edges) {
        graph[e.a].push_back({e.b, e.w});
        graph[e.b].push_back({e.a, e.w}); // paths are undirected
    }
    Dist d(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    d[n] = 0; pq.push({0, n});
    while (!pq.empty()) {
        auto [dv, v] = pq.top(); pq.pop();
        if (dv != d[v]) continue;
        for (auto [u, w] : graph[v])
            if (d[u] > dv + w) { d[u] = dv + w; pq.push({d[u], u}); }
    }
    for (int i = 1; i <= n; ++i) ensure(d[i] != INF, "Graph is disconnected");
    return d;
}

// A spanning tree has n-1 edges and guarantees connectivity.
Edges tree(int n, Shape shape, uint32_t seed, int max_weight = 1500) {
    mt19937 rng(seed); uniform_int_distribution<int> weight(1, max_weight);
    Edges edges; edges.reserve(n - 1);
    for (int i = 1; i < n; ++i) {
        int parent = n;
        if (shape == Shape::Chain) parent = i + 1;
        if (shape == Shape::Random) parent = uniform_int_distribution<int>(i + 1, n)(rng);
        if (shape == Shape::Comb) parent = (i % 3 == 0 ? n : i + 1);
        edges.push_back({i, parent, weight(rng)});
    }
    return edges;
}

// Add extra edges; optional parallel edges and self-loops stress implementations.
Edges add_edges(int n, Edges edges, int target, uint32_t seed,
                int max_weight = 2000, bool duplicates = false, bool loops = false) {
    ensure(target >= (int)edges.size() && target <= MAX_M, "bad target m");
    mt19937 rng(seed);
    uniform_int_distribution<int> vertex(1, n), weight(1, max_weight);
    uniform_real_distribution<double> chance(0.0, 1.0);
    edges.reserve(target);
    while ((int)edges.size() < target) {
        int a = vertex(rng), b = vertex(rng);
        if (loops && chance(rng) < 0.04) b = a;
        if (!loops && a == b) b = b % n + 1;
        if (duplicates && !edges.empty() && chance(rng) < 0.08) {
            auto e = edges[uniform_int_distribution<int>(0, (int)edges.size() - 1)(rng)];
            a = e.a; b = e.b;
        }
        edges.push_back({a, b, weight(rng)});
    }
    shuffle(edges.begin(), edges.end(), rng);
    return edges;
}

// Generate exactly q=ceil(n/100) incorrect guide guesses, all in [0,10^7].
// Any true distance >10^7 is necessarily a wrong guess; node n is always wrong.
vector<ll> guesses(int n, const Dist& d, uint32_t seed, vector<int> forced = {}) {
    const int q = (n + 99) / 100;
    vector<char> bad(n + 1, false);
    bad[n] = true;
    for (int i : forced) { ensure(1 <= i && i <= n, "bad forced vertex"); bad[i] = true; }
    for (int i = 1; i < n; ++i) if (d[i] > MAX_G) bad[i] = true;
    int count = 0;
    for (int i = 1; i <= n; ++i) count += bad[i];
    ensure(count <= q, "More than q vertices necessarily need wrong guesses");
    vector<int> options;
    for (int i = 1; i < n; ++i) if (!bad[i]) options.push_back(i);
    mt19937 rng(seed); shuffle(options.begin(), options.end(), rng);
    for (int j = 0; j < q - count; ++j) bad[options[j]] = true;
    vector<ll> g = d;
    const int choices[] = {1, MAX_G, 9999999, 7, 123456};
    int j = 0;
    for (int i = 1; i <= n; ++i) if (bad[i]) {
        ll choice = choices[(j++) % 5];
        if (choice == d[i]) choice = (d[i] == 2 ? 3 : 2);
        g[i] = choice;
    }
    return g;
}

// Check semantic constraints before writing matched .in and .ans files.
void write_case(fs::path folder, const string& name, int n, const Edges& edges,
                uint32_t seed = 7, vector<int> forced = {}, ll expected = -1,
                vector<ll> preset = {}) {
    const int m = (int)edges.size();
    ensure(2 <= n && n <= MAX_N, name + ": n outside range");
    ensure(n - 1 <= m && m <= MAX_M, name + ": m outside range");
    for (auto e : edges)
        ensure(1 <= e.a && e.a <= n && 1 <= e.b && e.b <= n &&
               1 <= e.w && e.w <= MAX_G, name + ": invalid edge");
    Dist d = dijkstra(n, edges);
    vector<ll> g = preset.empty() ? guesses(n, d, seed, forced) : preset;
    ensure((int)g.size() == n + 1, name + ": invalid guess array size");
    vector<pair<int,ll>> wrong;
    for (int i = 1; i <= n; ++i) {
        ensure(0 <= g[i] && g[i] <= MAX_G, name + ": invalid guess value");
        if (g[i] != d[i]) wrong.push_back({i, d[i]});
    }
    ensure((int)wrong.size() == (n + 99) / 100, name + ": wrong number of mistakes");
    if (expected >= 0) ensure(d[1] == expected, name + ": incorrect expected distance");
    fs::create_directories(folder);
    ofstream in(folder / (name + ".in")), ans(folder / (name + ".ans"));
    ensure(in.good() && ans.good(), name + ": file cannot be created");
    in << n << ' ' << m << '\n';
    for (int i = 1; i <= n; ++i) in << g[i] << '\n';
    for (auto e : edges) in << e.a << ' ' << e.b << ' ' << e.w << '\n';
    ans << d[1] << '\n';
    for (auto [i, dist] : wrong) ans << i << ' ' << dist << '\n';
    ensure(in.good() && ans.good(), name + ": write failure");
    cout << name << ": n=" << n << ", m=" << m << ", wrong="
         << wrong.size() << ", answer=" << d[1] << '\n';
}

Edges astar_trap() {
    const int n = 101;
    Edges e{{1,2,1},{2,n,1},{1,3,2},{3,n,2}};
    for (int i = 4; i < n; ++i) e.push_back({i,n,20});
    return e;
}
Edges overflow_trap() {
    const int n = 200000;
    Edges e; e.reserve(n - 1);
    for (int i = 1; i <= 1998; ++i) e.push_back({i,i+1,MAX_G});
    e.push_back({1999,n,MAX_G});
    for (int i = 2000; i < n; ++i) e.push_back({i,n,3});
    ensure((int)e.size() == n - 1, "overflow trap has wrong edge count");
    return e;
}

void descriptions(fs::path root) {
    ofstream f(root / "data" / "Description.txt");
    ensure(f.good(), "cannot write Description.txt");
    f << R"(The Unreliable Guide - Test Case Descriptions
Every .in has a matching .ans. Exactly ceil(n/100) guesses are incorrect.
All paths are undirected; all graphs are connected.
Self-loops and parallel edges are allowed.

sample01: Smallest n=2, one path; terminal guess wrong.
sample02: Competing routes, one incorrect terminal guess.
sample03: Six-node example used in the problem figure.
secret01: n=2, minimum edge weight and minimum guide guess g_i=0.
secret02: n=2, maximum edge weight.
secret03: n=3, maximum valid real distance at vertex 1.
secret04: Parallel edges where the shorter one wins.
secret05: Cycles, self-loops, and repeated edges.
secret06: n=99 rounding boundary, star with high weights.
secret07: n=100 rounding boundary, weighted chain.
secret08: n=101; overestimating heuristic breaks naive A*.
secret09: n=199, high-weight star, two wrong guesses.
secret10: n=200, dense random graph with duplicate edges.
secret11: n=201, three wrong guesses (rounding threshold).
secret12: 500-node sparse chain.
secret13: 1000-node star plus shortcuts.
secret14: 3000-node random connected graph.
secret15: 5000-node comb with self-loops/duplicates.
secret16: 10000-node random graph.
secret17: 20000-node star with maximum weight possibilities.
secret18: 50000 nodes, 100000 edges; performance stress.
secret19: n=m=200000, maximum-size random graph.
secret20: n=200000,m=199999, shortest distance 19990000000;
          tests 64-bit distances and exactly 2000 wrong guesses.
)";
}

void generate(fs::path root) {
    fs::path sample = root / "data" / "sample", secret = root / "data" / "secret";
    fs::create_directories(sample); fs::create_directories(secret);
    // Fixed handcrafted samples match the problem statement.
    write_case(sample,"sample01",2,{{1,2,7}},7,{},7,{0,7,4});
    write_case(sample,"sample02",5,{{1,2,8},{2,5,2},{1,3,2},{3,4,1},{4,5,1},{2,4,2}},
               7,{},4,{0,4,2,2,1,1});
    write_case(sample,"sample03",6,{{1,2,1},{2,6,1},{1,3,2},{3,6,2},
               {2,4,3},{4,5,1},{5,6,3}},7,{},2,{0,2,1,2,4,3,7});
    write_case(secret,"secret01",2,{{1,2,1}},11,{},1,{0,0,0});
    write_case(secret,"secret02",2,{{1,2,MAX_G}},12);
    write_case(secret,"secret03",3,{{1,2,4000000},{2,3,6000000}},13);
    write_case(secret,"secret04",4,{{1,2,10},{1,2,1},{2,4,2},{1,3,2},{3,4,2}},14);
    write_case(secret,"secret05",6,{{1,2,5},{2,6,2},{3,6,3},{1,3,1},
               {4,5,2},{5,6,2},{4,4,1},{2,2,100},{1,3,10}},15);
    write_case(secret,"secret06",99,tree(99,Shape::Star,16,MAX_G),16);
    write_case(secret,"secret07",100,tree(100,Shape::Chain,17,8),17);
    auto trap = astar_trap();
    auto trap_g = dijkstra(101,trap);
    trap_g[2] = MAX_G; trap_g[101] = 1;
    write_case(secret,"secret08",101,trap,18,{},2,trap_g);
    write_case(secret,"secret09",199,tree(199,Shape::Star,19,MAX_G),19);
    write_case(secret,"secret10",200,add_edges(200,tree(200,Shape::Random,20),650,21,
               2000,true),20);
    write_case(secret,"secret11",201,add_edges(201,tree(201,Shape::Random,22),201,23),22);
    write_case(secret,"secret12",500,tree(500,Shape::Chain,24,7),24);
    write_case(secret,"secret13",1000,add_edges(1000,tree(1000,Shape::Star,25,10000),1999,26),25);
    write_case(secret,"secret14",3000,add_edges(3000,tree(3000,Shape::Random,27,1000),7000,28),27);
    write_case(secret,"secret15",5000,add_edges(5000,tree(5000,Shape::Comb,29,1500),
               12000,30,2000,true,true),29);
    write_case(secret,"secret16",10000,add_edges(10000,tree(10000,Shape::Random,31,800),15000,32),31);
    write_case(secret,"secret17",20000,add_edges(20000,tree(20000,Shape::Star,33,MAX_G),
               22000,34,10000),33);
    write_case(secret,"secret18",50000,add_edges(50000,tree(50000,Shape::Random,35,300),100000,36),35);
    write_case(secret,"secret19",200000,add_edges(200000,tree(200000,Shape::Random,37,1000),200000,38),37);
    write_case(secret,"secret20",200000,overflow_trap(),39,{1999,200000},19990000000LL);
    descriptions(root);
    uintmax_t total = 0;
    for (auto dir : {sample,secret})
        for (auto f : fs::directory_iterator(dir))
            if (f.path().extension()==".in") total += fs::file_size(f.path());
    ensure(total < 50000000ULL, "input files exceed 50 MB limit");
    cout << "Generated 3 sample and 20 secret .in/.ans pairs.\n"
         << "Combined input size: " << total << " bytes (" << fixed
         << setprecision(2) << total / 1000000.0 << " MB).\n";
}
int main(int argc,char** argv) {
    try {
        ensure(argc <= 2, "Usage: generate [output_project_root]");
        generate(argc == 2 ? fs::path(argv[1]) : fs::path("."));
    } catch (const exception& ex) { cerr << "Generator error: " << ex.what() << '\n'; return 1; }
}
