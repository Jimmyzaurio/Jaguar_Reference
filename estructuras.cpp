
struct BIT {
    vector<int> tree;
    BIT(int n) : tree(n + 1) {}

    void Actualizar(int i, int v) {
        while(i < tree.size()) {
            tree[i] += v;
            i += i & -i;
        }
    }

    Long Query(int i) {
        long long sum = 0;
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

    // matriz.Actualizar(x + 1, y + 1, a)
    void Actualizar(int x, int y, int a) {
        for (int i = y; i < matriz.size(); i += i & -i)
            matriz[i].Actualizar(x, a);
    }
    // cout << matriz.Area(t + 1, r + 1, b + 1, l + 1)
    // top, right, bottom, left
    Long Area(int t, int r, int b, int l) {
        return Query(t, r) - Query(t, l - 1)
        - Query(b - 1, r) + Query(b - 1, l - 1);
    }

    Long Query(int y, int x) {
        Long ans = 0;
        for (int i = y; i > 0; i -= i & -i)
            for (int j = x; j > 0; j -= j & -j)
                ans += matriz[i].tree[j];
        return ans; 
    }
};
