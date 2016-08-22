#include <bits/stdc++.h>
using namespace std;

typedef long long Long;

struct BIT {
	vector<int> tree;
	BIT(int n) : tree(n + 1) {}

	void Actualizar(int i, int v) {
		while (i < tree.size()) {
			tree[i] += v;
			i += i & -i;
		}
	}

	Long Query(int i) {
		Long sum = 0;
		while (i > 0) {
			sum += tree[i];
			i -= i & -i;
		}
		return sum;
	}

	Long Rango(int i, int j) {
		return Query(j) -
				Query(i - 1);
	}
};

struct BIT2D {
	vector<BIT> matriz;
	BIT2D(int n) : matriz(n + 1, BIT(n)) {}

	void Actualizar(int y, int x, int a) {
		for (int i = y; i < matriz.size(); i += i & -i)
			matriz[i].Actualizar(x, a);
	}

	Long Area(int t, int r, int b, int l) {
		return Query(t, r) - Query(t, l - 1)
		- Query(b - 1, r) + Query(b - 1, l - 1);
	}

	Long Query(int y, int x) {
		Long ans = 0;
		for (int i = y; i > 0; i -= i & -i)
			ans += matriz[i].Query(x);
		return ans;
	}
};

struct BIT3D {
	vector<BIT2D> cubo;
	BIT3D(int n) : cubo(n + 1, BIT2D(n)) {}

	void Actualizar(int z, int y, int x, int a) {
		for (int i = z; i < cubo.size(); i += i & -i)
			cubo[i].Actualizar(y, x, a);
	}

	Long Query(int z, int y, int x) {
		Long ans = 0;
		for (int i = z; i > 0; i -= i & -i)
			ans += cubo[i].Query(y, x);
		return ans;
	}

	Long Volumen(int xb, int yb, int zb, int xt, int yt, int zt) {
		return Query(zt, yt, xt) + 2*Query(zb, yb - 1, xb - 1) -
		Query(zt, yt, xb - 1) - Query(zb, yt, xt) - Query(zt, yb, xt);
	}
};

int main() {
	/*
	int n, num;
	cin >> n;
	BIT3D ufos(n);
	while (cin >> num && num != 3) {
		if (num == 1)
			ufos.Actualizar(z + 1, y + 1, x + 1, k);
		else {
			int a, b, c, d, e, f;
			cin >> a >> b >> c >> d >> e >> f;
			//cout << ufos.Volumen
		}
	}
	*/
	BIT3D ufos(10);
	ufos.Actualizar(5, 5, 5, 10);
	ufos.Actualizar(10, 10, 10, 10);
	
	cout << ufos.Volumen(0, 0, 0, 5, 5, 5) << endl;
	cout << ufos.Volumen(5, 5, 5, 10, 10, 10) << endl;

	cout << ufos.Volumen(4, 4, 4, 10, 10, 10) << endl;
	cout << ufos.Volumen(1, 1, 1, 10, 10, 10) << endl;
	return 0;
}
