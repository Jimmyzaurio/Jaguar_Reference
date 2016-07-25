#include <bits/stdc++.h>
using namespace std;

typedef long long Long;

// Lowest Common Ancestor
// pb al vector g
// Construir(n)
// LCA(u, v)
#define MAXN 100007
#define LN 17

vector<Long> g[MAXN];
//We use the array padre[i][j] for determining 
//the 2^i parent of the vertex j
Long padre[LN][MAXN];
Long depth[MAXN];
Long pesos[MAXN];

void dfs(Long s, Long parent, Long d) {
    depth[s] = d;
    padre[0][s] = parent;

    for (int v : g[s])  if (depth[v] == -1) 
        dfs(v, s, d + 1);
}

Long LCA(Long u, Long v) {
    if(depth[u] < depth[v]) swap(u, v);
    for (int i = LN; i >= 0; --i)
        if (depth[u] - (1<<i) >= depth[v])
            u = padre[i][u];

    if(u == v) return u;

    for (int i = LN - 1; i >= 0; --i)
        if (padre[i][u] != -1 && 
            padre[i][u] != padre[i][v]) {
            u = padre[i][u];
            v = padre[i][v];
        }    
    return padre[0][u];
}

void init(int N) {
    for (int i = 0; i < N; ++i)
        depth[i] = -1,
        pesos[i] = 0,
        g[i].clear();
    
    for (int i = 0; i < LN; ++i)
        for (int j = 0; j < N; ++j)
            padre[i][j] = -1;
}

void Construir(int N) {
    dfs(0, -1, 0);

    for (int i = 1; i < LN; ++i)
        for (int j = 0; j < N; ++j)
            if (padre[i - 1][j] != -1)
                padre[i][j] = padre[i - 1][ padre[i - 1][j] ];
}

int main() {
    return 0;
}
