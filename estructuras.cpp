#include <bits/stdc++.h>

const int INF = 1 << 30;
typedef long long Long;
// Segment Tree version dinamica. Para generar el
// arbol completo deben llamar a la funcion Construir.
// CUIDADO: Para usarlo deben especificar el tipo de
// dato a utilizar; SegTree<int> por ejemplo.

template<class T>
struct SegTree {

    T dato; int i, d;
    SegTree* izq, *der;

    SegTree(int I, int D)
        : izq(NULL), der(NULL),
          i(I), d(D), dato() {}
    
    ~SegTree() {
        if (izq) delete izq;
        if (der) delete der;
    }
    
    T Construir() {
        if (i == d) return dato = T();
        int m = (i + d) >> 1;
        izq = new SegTree(i, m);
        der = new SegTree(m + 1, d);
        return dato = izq->Construir() +
                      der->Construir();
    }
    
    T Actualizar(int a, T v) {
        if (a < i || d < a) return dato;
        if (a == i && d == a) return dato = v;
        if (!izq) {
            int m = (i + d) >> 1;
            izq = new SegTree(i, m);
            der = new SegTree(m + 1, d);
        }
        return dato = izq->Actualizar(a, v) +
                      der->Actualizar(a, v);
    }
    
    T Query(int a, int b) {
        if (b < i || d < a) return T();
        if (a <= i && d <= b) return dato;
        return izq? izq->Query(a, b) + 
                    der->Query(a, b): T();
    }
};

// A continuación se ejemplifica como sobrecargar
// el operador + dentro de una estructura para poder
// reutilizar el codigo del Segment Tree facilmente.
// El ejemplo sobrecarga el + por la funcion de maximo.
// Es MUY IMPORTANTE tener un constructor por defecto.

struct MaxInt {
    int d; MaxInt(int D) : d(D) {}
    MaxInt() : d(-INF) {} // IMPORTANTE!
    MaxInt operator+(const MaxInt& o) {
        return MaxInt(max(d, o.d));
    }
};


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

int main() {
    return 0;
}
}
