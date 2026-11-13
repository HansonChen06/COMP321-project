// INTENTIONALLY WRONG: uses signed 32-bit int for distances.
// On secret20, d[1] = 19,990,000,000 > INT_MAX.
// The clamp below prevents undefined signed-overflow behavior, but still
// demonstrates incorrect reasoning about the range of distances.
#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,m; if (!(cin >> n >> m)) return 0;
    vector<int> g(n+1); for(int i=1;i<=n;++i) cin>>g[i];
    vector<vector<pair<int,int>>> a(n+1);
    for(int i=0;i<m;++i){int u,v,w;cin>>u>>v>>w;a[u].push_back({v,w});a[v].push_back({u,w});}
    // Incorrect "infinity" and distance type are capped at INT_MAX.
    vector<int> d(n+1,INT_MAX);
    using State=pair<int,int>;
    priority_queue<State,vector<State>,greater<State>> pq;
    d[n]=0;pq.push({0,n});
    while(!pq.empty()) {
      auto [du,u]=pq.top();pq.pop();if(du!=d[u])continue;
      for(auto [v,w]:a[u]) {
        long long cand=(long long)du+w;
        if(cand < d[v]){d[v]=(int)cand;pq.push({d[v],v});}
      }
    }
    cout<<d[1]<<'\n';
    for(int i=1;i<=n;++i)if(g[i]!=d[i])cout<<i<<' '<<d[i]<<'\n';
}
