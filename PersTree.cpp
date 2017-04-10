 #include <bits/stdc++.h>

using namespace std;

const int oo = 1e9 + 1;

template<class T>
struct SegTree {
    SegTree *izq,*der;
    int i, d;
    T dato;

    SegTree(int I, int D):
        i(I), d(D), dato(),
        izq(NULL), der(NULL) {}

    SegTree(SegTree* st):
        dato(st->dato),
        der(st->der), d(st->d),
        izq(st->izq), i(st->i) {}

    ~SegTree(){
        if (izq) delete izq;
        if (der) delete der;
    }

    T build(vector<T>& data){
        if(i == d)
            return dato = data[i & d];
        int h = (i + d) >> 1;
        izq = new SegTree(i, h),
        der = new SegTree(h + 1, d);
        return dato = izq->build(data) +
                	     der->build(data);
    }
    
    SegTree* update(int p, T v){
        if(p < i || d < p) return this;

        SegTree *copia;
        if (i == d) {
            copia = new SegTree(this);
            copia->dato = v;
            return copia;
        }
        
        copia = new SegTree(i, d),
        copia->der  = der->update(p, v);
        copia->izq  = izq->update(p, v);
        copia->dato = copia->izq->dato + 
                      copia->der->dato;

        return copia;
    }

    T query(int a, int b){
        if(b < i || d < a)		 return T();  
        if(a <= i && d <= b)	return dato;
        return izq->query(a, b) +
               der->query(a, b);
    }
};

struct MaxInt{
    int v;

    MaxInt(): v(-oo){}
    MaxInt(int V): v(V){}

    MaxInt operator+(const MaxInt& other) const{
        return MaxInt(max(v, other.v));
    }
};

typedef SegTree<MaxInt> MaxTree;

int main(){

    return 0;
}
