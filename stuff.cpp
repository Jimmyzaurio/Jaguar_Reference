#include <bits/stdc++.h>
using namespace std;

struct Grafo {
	int n; bool bi;
	vector<vector<int>> ady;
	vector<int> parte_ciclo;
	vector<int> raices;
	vector<bool> vis;
	Grafo(int N, bool B = true)
		: n(N), bi(B), ady(N), parte_ciclo(N, -1), vis(N, false), raices(N) {}

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

		for (int i = 0; i < ciclos.size(); ++i)
			for (int j = 0; j < ciclos[i].size(); ++j)
				parte_ciclo[ ciclos[i][j] ] = i;

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
	int n;
	while (cin >> n) {
		UF arboles(n);
		Grafo g(n, false);
		for (int i = 0; i < n; ++i) {
			int num;
			cin >> num;
			arboles.unir(i, num - 1);
			g.Conecta(i, num - 1);
		}
		auto ciclos = g.DetectarCiclos();		
		g.busquedas();
		
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

		int Q;
		cin >> Q;
		while(Q--) {
			int a, b;
			cin >> a >> b;
			a--;
			b--;
			// Dentro del ciclo
			if (g.parte_ciclo[a] == g.parte_ciclo[b] && g.parte_ciclo[b] != -1) {
				int idx_ciclo = g.parte_ciclo[a];
				int posA = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), a) - ciclos[idx_ciclo].begin();
				int posB = lower_bound(ciclos[idx_ciclo].begin(), ciclos[idx_ciclo].end(), b) - ciclos[idx_ciclo].begin();
				cout << min((int)ciclos[idx_ciclo].size() - abs(posB - posA), abs(posA - posB));
				continue;
			} 
			if (g.parte_ciclo[a] != -1 || g.parte_ciclo[b] != -1) {
				continue;				
			}
		}
	}
}
/*
7
2 1 4 5 3 5 6
5
1 3
4 7
7 4
6 2
2 1



*/
