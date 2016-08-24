#include <bits/stdc++.h>
using namespace std;

typedef long long Long;
const int LN = 17;
struct Grafo {
	int n; bool bi;	
	vector<vector<int>> ady;
	vector<int> parte_ciclo;
	vector<int> raices;
	vector<bool> vis;
	vector<vector<Long>> padre;
	vector<Long> depth;
	Grafo(int N, bool B = true)
		: n(N), bi(B), ady(N), parte_ciclo(N, -1), vis(N, false), raices(N),
		depth(N), padre(LN, vector<Long>(N)) {}

	void Conecta(int u, int v) {
		if (bi) ady[v].push_back(u);
		ady[u].push_back(v);
	}

	vector<int> ciclo;
	vector<char> color;

	void DetectarCiclo(int u, int p) {
		color[u] = ciclo.empty()? 'G' : 'N';
		for (int v : ady[u]) {
			if (bi && v == p) continue;
			if (ciclo.empty() && color[v] == 'G')
				color[v] = 'A', ciclo.push_back(v),
				color[u] = 'R', ciclo.push_back(u);
			if (color[v] != 'B') continue;

			DetectarCiclo(v, u);
			if (color[u] == 'G' && color[v] == 'R')
				color[u] = 'R', ciclo.push_back(u);
		}
		if (color[u] == 'G') color[u] = 'N';
	}

	vector<vector<int>> DetectarCiclos() {
		vector<vector<int>> ciclos;
		color = vector<char>(n, 'B');
		for (int u = 0; u < n; ++u) {
			if (color[u] != 'B') continue;
			ciclo.clear(); DetectarCiclo(u, u);
			reverse(ciclo.begin(), ciclo.end());
			if (ciclo.size() > 0) ciclos.push_back(ciclo);
		}

		init();
		for (int i = 0; i < ciclos.size(); ++i)
			for (int j = 0; j < ciclos[i].size(); ++j) {
				parte_ciclo[ ciclos[i][j] ] = i;
				//Construir(ciclos[i][j]);
			}

		for (auto &c : ciclos)
			sort(c.begin(), c.end());
		/*
		cout << "datos ciclos\n";
		for (auto c : ciclos) {
			for (auto d : c)
				cout << d << " ";
			cout << endl;
		}
		cout << endl << endl;		
		*/
		return ciclos;
	}

	int busca(int d) {
		if (vis[d])
			return raices[d];
		vis[d] = true;

		if (parte_ciclo[d] != -1)
			return d;

		for (auto nodo : ady[d]) {
			raices[d] = busca(nodo);
		}
		return raices[d];
	}

	void busquedas() {
		for (int i = 0; i < n; ++i)
			if (!vis[i])
				raices[i] = busca(i);
		set<int> R;
		for (auto var : raices)
			R.insert(var);
		for (auto var : R)
			Construir(var);
	}
	/*
	int dista(int d) {
		if (vis[d])
			return distPadre[d];
		vis[d] = true;

		if (raices[d] == d)
			return 0;

		for (int nodo : ady[d])
			distPadre[d] = dista(nodo) + 1;

		return distPadre[d];
	}

	void padres() {
		fill(vis.begin(), vis.end(), false);
		for (int i = 0; i < n; ++i) {
			if (!vis[i])
				distPadre[i] = dista(i);
		}
	}
	*/
	void dfs(Long s, Long parent, Long d) {		
		//cout << "  " << s << " -> " << d << endl;
		depth[s] = d;
		padre[0][s] = parent;

		for (int v : ady[s]) if (depth[v] == -1 && parte_ciclo[v] == -1)
			dfs(v, s, d + 1);
	}

	Long LCA(Long u, Long v) {
		if (depth[u] < depth[v]) swap(u, v);
		for (int i = LN; i >= 0; --i)
			if (depth[u] - (1<<i) >= depth[v])
				u = padre[i][u];

		if (u == v) return u;

		for (int i = LN - 1; i >= 0; --i)
			if (padre[i][u] != -1 &&
				padre[i][u] != padre[i][v]) {
				u = padre[i][u];
				u = padre[i][v];
			}
		return padre[0][u];
	}

	void init() {
		for (int i = 0; i < n; i++)
			depth[i] = -1;

		for (int i = 0; i < LN; ++i)
			for (int j = 0; j < n; ++j)
				padre[i][j] = -1;

	}

	void Construir(int s) {
		dfs(s, -1, 0);
		//cout << endl << endl;

		for (int i = 1; i < LN; ++i)
			for (int j = 0; j < n; ++j)
				if (padre[i - 1][j] != -1)
					padre[i][j] = padre[i - 1][ padre[i - 1][j] ];
	}
};

struct UF {
	int n; vector<int> padre, tam;

	UF(int N) : n(N),
		tam(N, 1), padre(N) {
		while (--N) padre[N] = N;
	}

	int raiz(int u) {
		if (padre[u] == u) return u;
		return padre[u] = raiz(padre[u]);
	}

	void unir(int u, int v) {
		int ru = raiz(u);
		int rv = raiz(v);
		if (ru == rv) return;
		--n, padre[ru] = rv;
		tam[rv] += tam[ru];
	}
};

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n;
	while (cin >> n) {
		UF arboles(n);
		Grafo g(n);
		for (int i = 0; i < n; ++i) {
			int num;
			cin >> num;
			arboles.unir(i, num - 1);
			g.Conecta(i, num - 1);
		}
		auto ciclos = g.DetectarCiclos();		
		g.busquedas();
		/*
		//g.padres();
		for (auto var : ciclos) {
			for (auto ele: var) {
				cout << ele << " ";
			}
			cout << endl << endl;
		}		
		int conta = 0;
		for (auto var : g.raices) {
			cout << conta++  << " " << var << endl;
		}
		*/
		int Q;
		cin >> Q;
		while(Q--) {
			int a, b;
			cin >> a >> b;
			a--;
			b--;
			// Diferentes componentes
			if (arboles.raiz(a) != arboles.raiz(b)) {
				cout << "-1\n";
				continue;
			}

			// Dentro del ciclo
			if (g.parte_ciclo[a] == g.parte_ciclo[b] && g.parte_ciclo[b] != -1) {
				int idx_ciclo = g.parte_ciclo[a];
				int posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), a) - ciclos[idx_ciclo].begin();
				int posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), b) - ciclos[idx_ciclo].begin();
				//cout << "ciclo " << min((int)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
				cout << min((int)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB)) << "\n";
				continue;
			}
			// Ciclo y arbol
			if (g.parte_ciclo[a] != -1 || g.parte_ciclo[b] != -1) {
				int idx_ciclo = g.parte_ciclo[b];
				int raizA = g.raices[a];
				int centroA = b;
				int nodo = a;
				if (g.parte_ciclo[a] > g.parte_ciclo[b]) {
					idx_ciclo = g.parte_ciclo[a];
					raizA = g.raices[b];
					centroA = a;
					nodo = b;
				}
				int posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizA) - ciclos[idx_ciclo].begin();
				int posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), centroA) - ciclos[idx_ciclo].begin();
				int distCiclo = min((int)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
				//cout << "ciclo y arbol " << g.depth[nodo] + distCiclo << '\n';
				cout << g.depth[nodo] + distCiclo << '\n';
				continue;				
			}

			if (g.parte_ciclo[a] == -1 && g.parte_ciclo[b] == -1) {
				// Arboles diferentes
				if (g.parte_ciclo[a] != g.parte_ciclo[b]) {
					int raizA = g.raices[a];
					int raizB = g.raices[b];
					int idx_ciclo = g.parte_ciclo[a];
					int posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizA) - ciclos[idx_ciclo].begin();
					int posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), raizB) - ciclos[idx_ciclo].begin();
					int centro = min((int)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
					//cout << "diff arboles " << g.depth[a] + g.depth[b] + centro << "\n";
					cout << g.depth[a] + g.depth[b] + centro << "\n";
				} else { // LCA
					cout << g.depth[a] + g.depth[b] - 2*g.depth[g.LCA(a, b)] << "\n";
					//cout << "LCA\n";
				}
			}
		}
	}
}
/*
3
2 1 2
3
1 2
1 3
1 1

7
2 1 4 5 3 5 6
5
1 3
4 7
7 4
6 2
2 1

6
2 3 1 5 6 4
4
1 2
6 1
5 4
3 2

19
2 3 1 3 4 4 6 6 2 9 9 11 11 9 10 2 16 16 1
8
1 2
1 7
17 19
12 5
15 4
14 18
9 11
19 14

*/
