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

// Lazy propagation
template<class T>
struct SegTree { 
    T dato; int i, d, lazy;
    SegTree* izq, *der;
 
    SegTree(int I, int D)
        : izq(NULL), der(NULL),
          i(I), d(D), dato(), lazy(0){}
 
    ~SegTree() {
        if (izq) delete izq;
        if (der) delete der;
    }
 
    T Construir() {
        if (i == d) return dato = T(1, 0, 0);
        int m = (i + d) >> 1;
        izq = new SegTree(i, m);
        der = new SegTree(m + 1, d);
        return dato = izq->Construir() +
                      der->Construir();
    }
 
    void propagate() {
        if(lazy != 0) {
            if (izq) {
                izq->lazy = (izq->lazy + lazy) % 3;
                der->lazy = (der->lazy + lazy) % 3;
            }
            while (lazy--)
                dato = dato >> 1;
            lazy = 0; // IMPORTANTE
        }
    }
 
    T ActualizarRango(int l, int r) {
        propagate();
        if (r < i || d < l) return dato;        
        if (l <= i && d <= r) {
            dato = dato >> 1;
            if (izq) {
                izq->lazy = (izq->lazy + 1) % 3;
                der->lazy = (der->lazy + 1) % 3;
            }
            return dato;
        }
        return dato = izq->ActualizarRango(l, r) +
                      der->ActualizarRango(l, r);
    }
 
    T Query(int a, int b) {
        if (b < i || d < a) return T();
        propagate();
        if (a <= i && d <= b) return dato;
        return izq? izq->Query(a, b) +
                    der->Query(a, b): T();
    }
};
 
struct TRI {
    int cero, uno, dos;
    TRI() : cero(0), uno(0), dos(0) {}
    TRI(int a, int b, int c) : cero(a), uno(b), dos(c){}
 
    TRI operator+(const TRI &a) {
        return TRI(cero + a.cero, uno + a.uno, dos + a.dos);
    }
    TRI operator>>(int veces) {
        return TRI(dos, cero, uno);
    }
};
 
int main() {
    cin >> N >> Q;
    SegTree<TRI> tree(0, N - 1);
    tree.Construir();
 
    for (int i = 0; i < Q; ++i) {
        cin >> c >> a >> b;
        if (c == 0)
            tree.ActualizarRango(a, b);
        else
            cout << tree.Query(a, b).cero << "\n";
    }
}



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

// (x, y) se manejan como en el plano cartesiano, tal cual en los arreglos
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
    
    // y -> fila   x -> columna
    Long Query(int y, int x) {
        Long ans = 0;
        for (int i = y; i > 0; i -= i & -i)
            for (int j = x; j > 0; j -= j & -j)
                ans += matriz[i].tree[j];
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
    // b -> bottom    t -> top
    Long Volumen(int xb, int yb, int zb, int xt, int yt, int zt) {
        return Query(zt, yt, xt)        
             - Query(zt, yt, xb - 1)
             - Query(zt, yb - 1, xt)
             + Query(zt, yb - 1, xb - 1)        
             - Query(zb - 1, yt, xt)
             + Query(zb - 1, yt, xb - 1)
             + Query(zb - 1, yb - 1, xt)
             - Query(zb - 1, yb - 1, xb - 1);
    }
};

struct union_find {
    vi p; union_find(int n) : p(n, -1) { }
    int find(int x) { return p[x] < 0 ? x : p[x] = find(p[x]); }
    bool unite(int x, int y) {
        int xp = find(x), yp = find(y);
        if (xp == yp) return false;
        if (p[xp] > p[yp]) swap(xp,yp);
        p[xp] += p[yp], p[yp] = xp;
        return true; }
    int size(int x) { return -p[find(x)]; } };

// Trie 
// (t->sig.find(i) != t->sig.end())
vector<char> resp;

struct Trie{
bool es_palabra;
    map<int, Trie*> sig;

    Trie() : es_palabra(false) {}

    void Agregar(const string& str) {
        Trie* cabeza = this;
        for(int i=0; i<str.size(); ++i){
            if( cabeza->sig.find(str[i]-'a') == cabeza->sig.end() )
                cabeza->sig[str[i]-'a'] = new Trie();
            cabeza = cabeza->sig[str[i]-'a'];
        }
        cabeza->es_palabra = true;
    }
};

void Imprimir(Trie* t) {
    if (t->es_palabra)
        resp.push_back('P');
    for (int i = 0; i < 26; ++i)
        if (t->sig.find(i) != t->sig.end()) {
            resp.push_back((char)(i + 'a'));
            Imprimir(t->sig[i]);
            resp.push_back('-');
        }
}

void Solucion(Trie* t, int index) {
    int aux;
    if (t->es_palabra)
        resp.push_back('P');
    for (int i = 0; i < 26; i++) {
        if (t->sig.find(i) != t->sig.end()) {           
            if ((char)(i + 'a') == mayor[index])
                aux = i;
            else {
                resp.push_back((char)(i + 'a'));
                Imprimir(t->sig[i]);
                resp.push_back('-');
            }
        }
    }
    if (index < mayor.size()) {
        resp.push_back((char)(aux + 'a'));
        Solucion(t->sig[aux], index + 1);   
    }
}


int main() {
    return 0;
}
