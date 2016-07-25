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
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    Long n,q; 

    while (cin >> n && n != 0) {
        init(n);
        for (int i = 1; i < n; ++i) {
            Long a, t;
            cin >> a >> t;
            g[a].push_back(i);
            g[i].push_back(a);
            pesos[i] += pesos[a] + t;
        }
        
        Construir(n);

        cin >> q;
        for (int i = 0; i < q; ++i) {
            Long u, v;
            cin >> u >> v;
            cout << pesos[u] + pesos[v] - 2 * pesos[LCA(u, v)];
            if (i + 1 != q)
                cout << " ";
        }
        cout << "\n";
    }
    return 0;
}
/*
6 4
3 1
1 2
1 0
0 4
4 5
3 2
3 5
1 0
4 2

6
0 8
1 7
1 9
0 3
4 2
4
2 3
5 2
1 4
0 3
2
0 1
2
1 0
0 1
6
0 1000000000
1 1000000000
2 1000000000
3 1000000000
4 1000000000
1
5 0
0

*/
