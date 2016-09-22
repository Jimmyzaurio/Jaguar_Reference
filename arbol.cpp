#include <bits/stdc++.h>
using namespace std;

typedef long long Long;

// Lowest Common Ancestor
// Tratar como si fuera grafo con
// lista de adyacencia
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



typedef long long lld;
const lld A  = 100100;
const lld L  =     20;

typedef pair<lld, lld> pii;
typedef pair<lld, pii> p3i;
typedef vector<p3i> vi;
typedef vector< vi> vv;
#define snd second
#define fst first

lld P[A][L]; // padres binarios
lld M[A][L]; // maxims binarios
lld S[A][L]; // sumas  binarias
lld D[A];

vv graph;

void dfs(lld s, lld p, lld d) {
  D[s] = d;
  for (auto u: graph[s]) if (u.fst != p) {
    dfs(u.fst, s, d + 1);
    M[u.fst][0] =  u.snd.fst;
    S[u.fst][0] =  u.snd.snd;
    P[u.fst][0] =  s;
  }
}

void precalculate(lld n) {
  for (lld l = 1; l < L; l++)
    for (lld i = 0; i < n; i++) {
      M[i][l] = max(M[P[i][l - 1]][l - 1],  M[i][l - 1]);
      S[i][l] =     S[P[i][l - 1]][l - 1] + S[i][l - 1] ;
      P[i][l] =     P[P[i][l - 1]][l - 1];
    }
}

lld lca(lld u, lld v) {
  lld s = 0;
  lld m = 0;
  if (D[u] < D[v]) swap(u, v);

  for (lld i = L - 1; i >= 0; i--)
    if (D[P[u][i]] >= D[v]) {
      m = max(M[u][i], m);
      s =     S[u][i] + s;
      u =     P[u][i];
  }
  if (u == v) return m * s;

  for (lld i = L - 1; i >= 0; i--)
    if (P[u][i] != P[v][i]) {
      m = max(M[u][i], m);
      m = max(M[v][i], m);
      s =     S[u][i] + s;
      s =     S[v][i] + s;
      u =     P[u][i];
      v =     P[v][i];
    }

  m = max(M[u][0], m);
  m = max(M[v][0], m);
  s =     S[u][0] + s;
  s =     S[v][0] + s;

  return m * s;
}

int main() {
  ios::sync_with_stdio(0); cin.tie(0);
  lld n, q;
  lld u, v;
  lld l, h;
  cin >> n;
  graph = vv(n + 1, vi());
  for (lld i = 1; i < n; i++) {
    cin >> u >> v >> h >> l;
    graph[--u].push_back(p3i(--v, pii(h, l)));
    graph[  v].push_back(p3i(  u, pii(h, l)));
  }
  dfs(0,  0,   0);
  precalculate(n);
  cin >> q;
  while (q--)
    cin >> u >> v, cout << lca(--u, --v) << '\n';
  return 0;
}

int main() {
    return 0;
}
