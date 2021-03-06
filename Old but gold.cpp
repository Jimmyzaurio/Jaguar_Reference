/***************** Miguel *****************/

LIS (nlogn)

typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;

#define STRICTLY_INCRE

vi LIS(vi v) {
    vpii best;
    vi dad(v.size(), - 1);

    for (int i = 0; i < v.size(); ++i) {
    #ifdef STRICTLY_INCRE
        pii item = make_pair(v[i], 0);
        vpii::iterator it = lower_bound(best.begin(), best.end(), item);
        item.second = i;
    #else
        pii item = make_pair(v[i], i);
        vpii::iterator it = upper_bound(best.begin(), best.end(), item);
    #endif
        if (it == best.end()) {
            dad[i] = (best.size() == 0 ? -1 : best.back().second);
            best.push_back(item);
        } else {
            dad[i] = dad[it->second];
            *it = item;
        }
    }

    vi ret;
    for (int i = best.back().second; i >= 0; i = dad[i])
        ret.push_back(v[i]);
    reverse(ret.begin(), ret.end());
    return ret;
}

// Binary apple tree 
const int M = 100000000;
const int A =       105;

int memo[A][A];
int W[A][A];
int size[A];
int cost[A];
vv  graph;
int L[A];    
int R[A];    
/*

Problema:
Dado un árbol binario de tam n con pesos en las ramas,
cual es el maximo arbol que puede quedar reviendo k ramas, 
raíz = 1

input:
n = 5, k = 2
1 3 1
1 4 10
2 3 20
3 5 20

output: 
21
*/

int addEdge(int u, int v) {
  if (L[u] < 0) return L[u] = v;
  if (R[u] < 0) return R[u] = v;
}

// Cuenta el número de ramas
int count(int s, int p = 0) {
  if (size[s] >= 0)
    return size[s];
  int ans = 1;
  for (auto u: graph[s]) if (u != p)
    addEdge(s, u), ans += count(u, s);
  return size[s] = ans;
}

// Calcula la suma de los ramas en el subarbol
int waste(int s, int p = 0) {
  if (cost[s] >= 0)
    return  cost[s];
  int ans = W[s][p];
  for (auto u: graph[s]) if (u != p)
    ans += waste(u, s);
  return cost[s] = ans;
}

// Minimiza el costo de cortar k ramas
int mincst(int idx, int k) {
  if (k <= 0)
    return 0;
  if (k == count(idx))
    return waste(idx);
  if (k >= count(idx))
    return M;
  if (memo[idx][k] >= 0)
    return memo[idx][k];
  int ans = M;
  for (int i = 0; i <= k; i++)
    ans = min(ans, mincst(L[idx], i) + mincst(R[idx], k - i));
  return memo[idx][k] = ans;
}

int main() {
  for (int i = 0; i < A; i++)
    fill(memo[i], memo[i] + A, -1);
  fill(size, size + A, -1);
  fill(cost, cost + A, -1);
  fill(L, L + A, -1);
  fill(R, R + A, -1);
  int w, u, v, n, k;
  cin >>  n  >>   k;
  graph = vv(n + 1, vi());
  for (int i = 1; i < n; i++) {
    cin >> u  >>  v  >> w;
    graph[u].push_back(v);
    graph[v].push_back(u);
    W[u][v] = W[v][u] = w;
  }
  cout << waste(1) - mincst(1, n - k - 1) << '\n';
  return 0;
}

////////////////////////////////////////////////////
Digit Sum

#define digit (N[idx] - '0')

const lld A = 20;
const lld B =  2;

lld test[A][B];
lld memo[A][B];
lld pow [A];

lld D, T = 1;
string N;

// Calcula el número de veces que aparece
// un digito en los numeros del rango 0-N
// El número tienes que manejarlo como 
// string N = "10000" y llamar como count(0, 0)
// Warning: no cuenta el cero
lld count(lld idx, bool lower) {
  if (idx + 1 >= N.size())
    return  (lower ? 1 : D <= digit);
  if (test[idx][lower] == T)
    return memo[idx][lower];
  lld num = (lower ? 9 : digit);
  lld ans = 0;
  for (lld i = 0; i <= num; i++)
    if (!lower && i == digit)
      ans += (i == D) * (stoll(N.substr(idx + 1, N.size())) + 1) + count(idx + 1, 0);
    else
      ans += (i == D) * pow[N.size() - idx - 1] + count(idx + 1, 1);
  test[idx][lower] = T;
  return memo[idx][lower] = ans;
}

int main() {
  pow[0] = 1;
  for (lld i = 1; i < A; i++)
    pow[i] = 10 * pow[i - 1];
  lld n, t;
  cin >> t;
  while (t--) {
    lld ans = 0;
    cin  >>   n;
    N = to_string(n - 1);
    if (N != "0")
      for (D = 1; D <= 9; D++, T++)
        ans -= D * count(0, 0);
    cin >> N;
    for (D = 1; D <= 9; D++, T++)
      ans += D * count(0, 0);
    cout << ans << '\n';
  }
  return 0;
}


//////////////////////////////////////////////
Bickli

#define vb vector<bool>

const int MOD = 1000000000;
const int A   = 10001;

vv graph(A, vi());
vi paths(A,   -1);
vi reach(A,    0);

vb marked(A, 0);
vb linked(A, 0);
bool cycle = 0 ;
bool limit = 0 ;

/*
Problem: 
A bicycle race is being organized in a land far, 
far away. There are N town in the land, numbered 1 through N. 
There are also M one-way roads between the towns. 
The race will start in town 1 and end in town 2.

How many different ways can the route be set?
Two routes are considered different if they do 
not use the exact same roads.
*/

// nos ayuda a identificar cuales caminos son los que podemos tomar,
// si hay un camino que no me lleva al objetivo lo marco como no
// alcanzable.
bool reachable(int s) {
  marked[s] = 1;
  if (s == 2)
    return reach[s] = 1;
  for (auto u: graph[s])
    if (!marked[u])
      reach[s] |= reachable(u);
    else
      reach[s] |= reach[u];
  return reach[s];
}

// En los nodos que son alcanzables checar si hay ciclo
// si hay ciclo entonces hay caminos infinitos.
bool has_cycle(int s) {
  marked[s] = 1;
  linked[s] = 1;
  for (auto u: graph[s]) if (reach[u]) {
    if (!marked[u])
      has_cycle(u);
    else if (linked[u])
      cycle = true;
  }
  linked[s] = 0;
}


// Cuenta los caminos en un DAG.
int count(int s) {
  if (s == 2)
    return 1;
  if (paths[s] >= 0)
    return paths[s];
  int ans = 0;
  for (auto u: graph[s]) if (reach[u])
    if (ans + count(u) >= MOD)
      ans = (ans + count(u)) % MOD, limit = true;
    else
      ans = (ans + count(u));
  return paths[s] = ans;
}

int main() {
  lld u, v;
  lld n, m;
  cin >> n;
  cin >> m;
  while (cin >> u >> v, m--)
    graph[u].push_back(v);
  reachable(1);
  marked = vb(A, 0);
  has_cycle(1);
  if (!cycle) {
    int ans = count(1);
    if (limit)
      cout << setw(9) << setfill('0');
    cout << ans << "\n";
  }
  else
    cout << "inf\n";
}

/////////////////////////////////////
Hyperactive

const int MOD = 100000000;

int memo[101][101];
int test[101][101];
vii data;
int t =0;
int E;
int N;

/*
Problema cuenta el número distinto de subconjuntos de
actividades minimos que cubren el día dado E(duración del día)
y N(el número de actividades)

A subset of activities is a minimal subset that covers 
the day if and only if:
1. every moment of the day is covered by at least one activity of the subset; and 
2. removing any of the activities from the subset would leave at least one moment of the day uncovered. 
*/

int count(int prv, int cur) {
  if (data[cur].second >= E)
    return  1;
  if (test[prv][cur] == t)
    return memo[prv][cur];
  int ans = 0;
  for (int i = cur + 1; i <= N; i++)
    if (data[cur].second >= data[i].first && 
        data[i].second > data[cur].second && (cur == 0 || data[prv].second < data[i].first))
      ans = (ans + count(cur, i)) % MOD;
  test[prv][cur] = t;
  return memo[prv][cur] = ans;
}

int main() {
  while (t++, cin >> E >> N, N + E != 0) {
    data = vii(N + 1,  {0, 0});
    for (int i = 0; i < N; i++)
      cin >> data[i].first >> data[i].second;
    sort(data.begin(), data.end());
    cout << count(0, 0) << "\n";
  }
  return 0;
}


////////////////////////////////////////////////
Ball-stacking

#define N   1001

int memo[N][N];
int test[N][N];
int k = 0;
vvi sum;

/*
Moraleja de la historia: organizar los datos hace más fácil el problema.

The contestant must choose which balls he is going to take and his 
prize is the sum of the values of those balls. However, the contestant
can take any given ball only if he also takes the balls directly on top of it. 

input:
4
3
-5 3
-8 2 -8
3 9 -2 7

output:
7
*/

int cost(int n, int m) {
  if (n < 0 || m < 0)
    return 0;
   int take = 0;
   if (test[n][m] == k)
     return memo[n][m];
   if (n + 1 > m)
     take = cost(n - 1, m - 1) + sum[n][m];
   else
     take = cost(n + 0, m - 1) + sum[n][m];
   test[n][m] = k;
   return memo[n][m] = max(take, cost(n - 1, m)); 
}

int main() {
  int    n;
  while (cin >> n, k++, n != 0) {
    // Para cada pelota acumulamos el costo de quitarla
    // Y de las que se caen con ella.
    sum = vvi(n, vi(n, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j <= i; j++)
        cin >> sum[j][i]; 
    for (int i = 0; i < n; i++)
       reverse(sum[i].begin() + i, sum[i].end());
    for (int i = n - 1; i >= 0; i--)
      for (int j = 1; j <= i; j++)
        sum[j][i] += sum[j - 1][i];
    cout << cost(n - 1, n - 1) << "\n";
  }
  return 0;
}


/////////////////////
Ants

#define pii pair<lld, lld>
#define p3i pair<lld, pii>

const lld A = 251;
const lld off = 1;

lld dist[A][A];
lld data[A][A];
lld N, M, K;

void mindist(lld n, lld m) {
  for (lld i = n - 1; i >= 0; i--) 
    for (lld j = off; j <= m; j++)
      dist[i][j] += min(dist[i + 1][j], 
                    min(dist[i + 1][j + 1], dist[i + 1][j - 1]));
}

vector<lld> getk(lld k, lld n, lld m) {
  priority_queue<p3i, vector<p3i>, greater<p3i>> q;
  for (lld i = 0; i < m; i++)
    q.push(p3i(dist[off][i + off], {off, i + off}));
  vector<lld> ans;
  while (!q.empty() && k > 0) {
    lld i = q.top().snd.fst;
    lld j = q.top().snd.snd;
    lld v = q.top().fst; q.pop();
    if (i == n)
      k--, ans.push_back(v);
    else {
      lld mn = dist[i + 1][j];
      if (j + 1 <= m)
        mn = min(mn, dist[i + 1][j + 1]);
      if (j - 1 >= 1);
        mn = min(mn, dist[i + 1][j - 1]);
      v -= mn;
      q.push(p3i(v + dist[i + 1][j], {i + 1, j}));
      if (j + 1 <= m)
        q.push(p3i(v + dist[i + 1][j + 1], {i + 1, j + 1}));
      if (j - 1 >= 1)
        q.push(p3i(v + dist[i + 1][j - 1], {i + 1, j - 1}));
    }
  }
  return ans;
}

int main() {
  cin >> N;
  cin >> M;
  cin >> K;
  for (lld i = 0; i < A; i++)
    fill(dist[i], dist[i] + A, 1000000000);
  for (lld i = 0; i < N; i++)
    for (lld j = 0; j < M; j++)
      cin >> data[i + off][j + off];
  for (lld i = 0; i < N; i++)
    for (lld j = 0; j < M; j++)
      dist[i + off][j + off] = data[i + off][j + off];
  mindist(N, M);
  for (auto i: getk(K, N, M))
    cout << i << " ";
  cout << "\n";
  return 0;
}


////////////////////////////////////
Dice

const int A = 101;
const int B = 601;
const int C = 6;

double memo[A][B];
bool   test[A][B];

/*
  DP con probabilidad
  
  Calcular la probabilidad de que n tiros 
  de un dado de seis caras sumen k
*/
double probability(int n, int k) {
  if (n == 0 && k == 0)
    return 1;
  if (n == 0 && k != 0)
    return 0;
  if (test[n][k])
    return memo[n][k];
  double ans = 0.0;
  for (int i = 1; i <= C && i <= k; i++)
    ans += probability(n - 1, k - i) / 6.0;
  test[n][k] = true;
  return memo[n][k] = ans;
}


int main() {
  int n, k;
  int    t;
  cin >> t;
  while (t--) {
    cin >> n >> k;
    cout << probability(n, k) << "\n";
  }
  return 0;
}


////////////////////////////////////////////////////////////
Honeycomb

const int offset = 20; // offset para el centro.
const int N = 100;
int memo[N][N][N];

/*
Cuenta caminos en DAG hexagona que empiezan 
y terminan en el mismo nodo con k pasos.
*/
int count(int x, int y, int k) {
  if (x == 0 && y == 0 && k == 0)
    return 1;
  if (k <= 0)
    return 0;
  if (memo[x + offset][y + offset][k + offset] >= 0)
    return memo[x + offset][y + offset][k + offset];
  return memo[x + offset][y + offset][k + offset] = 
    count(x + 1, y + 0, k - 1) +
    count(x + 0, y + 1, k - 1) +
    count(x + 1, y + 1, k - 1) +
    count(x - 1, y - 0, k - 1) +
    count(x - 0, y - 1, k - 1) +
    count(x - 1, y - 1, k - 1);
}

int ans[15] = {0, 0, 6, 12, 90, 360, 2040, 10080, 54810, 290640, 1588356, 8676360, 47977776, 266378112, 1488801600};

int main() {
  int t, n;
  cin >> t;
  while (t--) 
    cin >> n, cout << ans[n] << "\n";
  return 0;
}


/////////////////////////////////////////////////
Nterestinglist

const lld MOD = 1000000007;

/*
Problema: 
Given a set of N (1 <= N <= 10^3) integers containing the numbers from 1 to N. 
We would like to know how many lists of length L (1 <= L <= 10^3) can be formed
using elements from the set such that the difference between two adjacent 
elements in the list is always lower or equal than M (1 <= M <= 10^3).
*/

// Declaracion del BIT

lld count(lld n, lld m, lld k) {
  vector<Bit> bit(k, Bit(n));
  for (lld i = 1; i <= n; i++)
    bit[0].update(i, 1);
  for (lld i = 1; i <  k; i++)
    for (lld j = 1; j <= n; j++)
      bit[i].update(j, bit[i - 1].query(max(1LL, j - m), min(n, j + m)));
  return bit[k - 1].query(1, n);
}

int main() {
  int n, k;
  int m, t;
  cin >> t;
  while (t--)
    cin >> n >> k >> m, cout << count(n, m, k) << '\n';
  return 0;
}

Go-- 3er RPC

#include <iostream>
using namespace std;

/*

Problema: dada una matriz  de NxN con fichas malas y fichas buenas,
          cuenta  cuantas  submatrices  cuadradas hay con al menos
          una ficha buena pero que no contengan ninguna ficha mala.

Observación clave:

  Sea fst[i][j] la menor k tal que el cuadrado de tamaño
  k con esquina (i,j) contenga una ficha buena, o min(i,j) de lo contrario.

  Por otra parte, sea lst[i][j] la menor k tal que el cuadrado de tamaño
  k con esquina (i,j) contenga una ficha mala, o min(i,j) de lo contrario.

  Podemos observar que, entonces, todos los cuadrados de tamaño k con esquina
  (i,j) tales que fst[i][j]≤k<lst[i][j] son válidos.

*/


const int A = 501;

int num[A][A];
int fst[A][A];
int lst[A][A];
int N, T;
int G, E;

int sqrGood(int i, int j) {
  if (i <  0 || j < 0 || num[i][j] == G)
    return 0;
  if (fst[i][j] >= 0)
    return fst[i][j];
  return fst[i][j] = 1 + min(sqrGood(i - 1, j    ),
                         min(sqrGood(i    , j - 1),
                             sqrGood(i - 1, j - 1)));
}

int sqrEvil(int i, int j) {
  if (i <  0 || j < 0 || num[i][j] == E)
    return 0;
  if (lst[i][j] >= 0)
    return lst[i][j];
  return lst[i][j] = 1 + min(sqrEvil(i - 1, j    ),
                         min(sqrEvil(i    , j - 1),
                             sqrEvil(i - 1, j - 1)));
}

int countSqrs(int n) {
  int ans = 0;
  for (int i = 0; i < A; i++)
    fill(fst[i], fst[i] + A, -1),
    fill(lst[i], lst[i] + A, -1);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ans += max(0, sqrEvil(i, j) - sqrGood(i, j));
  return ans;
}

int main() {
  ios::sync_with_stdio(0); cin.tie(0);
  int u, v, p;
  while (cin >> N >> p) {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        num[i][j] = 0;
    for (int i = 0; i < p; i++)
      cin >> u >> v, num[u - 1][v - 1] = 1;
    for (int i = 0; i < p; i++)
      cin >> u >> v, num[u - 1][v - 1] = 2;
    E = 2; G = 1;
    cout << countSqrs(N) <<  ' ';
    E = 1; G = 2;
    cout << countSqrs(N) << '\n';
  }
  return 0;
}


Strickes

#include <iostream>

using namespace std;
/*
  Problema: Encuentra  el  cuadro  más  grande  de unos que
            tenga un cuadrado de ceros rotado 45° inscrito:

          11011
          10001
    101   00000
    000   10001
    101   11011
*/

const int M = 999;
const int A = 200;

int data[A][A];
int accm[A][A];
int memo[A][A];
int N;

// Encuentra el cuadrado de ceros más grande rotado 45° grados
int maxsquare(int i, int j) {
  if (i < 1  || i > N)
    return 0;
  if (j < 1  || j > N)
    return 0;
  if (data[i][j] == 1)
    return 0;
  if (memo[i][j] >= 0)
    return memo[i][j];
  int ans = M;
  ans = min(ans, maxsquare(i - 1, j - 1));
  ans = min(ans, maxsquare(i - 1, j    ));
  ans = min(ans, maxsquare(i - 1, j + 1));
  return memo[i][j] = 1 + ans;
}

void accumulate(int n) {
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      accm[i][j] = data[i][j];
      if (i > 0) accm[i][j] += accm[i - 1][j    ];
      if (j > 0) accm[i][j] += accm[i    ][j - 1];
      if (i > 0 && j > 0) accm[i][j] -= accm[i - 1][j - 1];
    }
  }
}

int query(int i, int j, int u, int v) {
  return accm[u][v] - accm[u][j - 1] - accm[i - 1][v] + accm[i - 1][j - 1];
}


// Encuentra el cuadrado de unos con un cuadrado rotado 45° inscrito
int getans(int n) {
  int mxn  = -1;
  accumulate(n);
  for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++) {
      int s = maxsquare(i, j);
      if (s > 1 && s > mxn) {
        int ones  = query(i - 2 * s + 2, j - s + 1, i, j + s - 1);
        if (ones == 2 * s * (s - 1)) // verifica que la cantidad de unos sea correcta
          mxn = maxsquare(i, j);
      }
    }
  return 2 * mxn - 1;
}

int main() {
  ios::sync_with_stdio(0); cin.tie(0);
  while (cin >> N, N != 0) {
    for (int i = 0; i < A; i++)
      fill(memo[i], memo[i] + A, -1);
    for (int i = 1; i <= N; i++)
      for (int j = 1; j <= N; j++)
        cin >> data[i][j];
    int ans = getans(N);
    if (ans < 0)
      cout << "No solution\n";
    else
      cout << ans << '\n';
  }
  return 0;
}




/////////////////////// Alexis /////////////////////////////////
//Calculo de coeficientes binomiales
//Complejidad: O(n*k)
//Memoria: (n^2)
int bino(int n, int k){
  if (k == n || k == 0) 
  	return 1;
  if (memo[n][k] != -1) 
  	return memo[n][k];
  return memo[n][k] = (bino(n - 1, k) + bino(n - 1, k - 1)) % MOD;
}

//Longest Common Subsequence
//Complejidad: O(|x| * |y|)
//Memoria: (|x| + 1)(|y| + 1)
int LCS(string x, string y){
  int l_x = x.length();
  int l_y = y.length();
  vector<vi> c(l_x + 1, vi(l_y + 1));
  
  for (int i = 1; i <= l_x; i++){
    for (int j = 1; j <= l_y; j++){
  	  if (x[i - 1] == y[j - 1])
  	  	c[i][j] = c[i - 1][j - 1] + 1;
  	  else
  	  	c[i][j] = max(c[i - 1][j], c[i][j - 1]);
  	}
  }
  return c[l_x][l_y];
}

//Longest increasing subsequence
//Complejidad: O(n^2)
//Memoria: (n)
//recibe un vector de números
int LIS(vi num){
  int n = num.size();
  vi lis(n, 1);
  for (int i = 1; i < n; i++){
  	for (int j = 0; j < i; j++){
  	  if (num[i] > num[j] && lis[i] < lis[j] + 1)
  	  	lis[i] = lis[j] + 1;
  	}
  }
  for (int i = 0; i < n; i++)
  	cout << lis[i] << " ";
  cout << endl;
  return lis[n-1];
}
//Coin Change
//Complejidad: O(val * pos)
//Memoria: (|val||N|)
//Se empieza con c(val, 0)
vi cambio;
int N = cambio.size();
int c(int val, int pos){
  if (val == 0)
  	return 1;
  if (pos == N || val < 0)
  	return 0;
  if (memo[val][pos] != -1)
  	return memo[val][pos];
  return memo[val][pos] = c(val - cambio[pos], pos) + c(val, pos + 1);
}

//min coin change
//complejidad: O(valor)
//memoria: (n) lineal
/*
int n = valor;
  for (int j = 0; j <= n; j++)
  	change(j);
  cout << me[n]; <- respuesta
*/
int me[MAX];
const int INF = (1<<30);
int change(int valor){
  if (valor == 0)
  	return 0;
  if (valor < 0)
  	return INF;
  if (me[valor] != -1)
  	return me[valor];
  int ans = INF;
  for (auto cam : cambio)
  	ans = min(ans, change(valor - cam));
  return me[valor] = ans + 1;
}

//Traveling Salesman Problem
//Complejidad: O(2^n * n^2)
//Memoria: (|n||2^n|) -> tamaño de la memorización
//se empieza con tsp(0, 1)
//la matriz dist es la matriz de adyacencia del grafo
int tsp(int pos, int mask){
  if (mask == (1<<n) - 1)
  	return dist[pos][0];
  if (memo[pos][mask] != -1)
  	return memo[pos][mask];
  int ans = INF;
  for (int i = 0; i < N; i++){
    if (mask & (1<<i) || pos == i)
  	  continue;
  	ans = min(ans, dist[pos][i] + tsp(i, mask|(1<<i)));
  }
  return memo[pos][mask] = ans;
}


/*********************************** Chuy **************************/
Writing
Buscar una permutación de una cadena en otra, dicha permutación
debe ser una subcadena. 

bool checar() {
	for (int i = 0; i < 60; i++)
		if (cubeta[i] != actual[i])
			return false;
	return true;
}

int main() {	
	for (int i = 0; i < g; i++) {		
		cubeta[W[i] - 'A']++;
		actual[cad[i] - 'A']++;
	}
	res = cont = 0;
	flag = true;
	for (int i = g; i < s; i++) {		
		if (checar() && flag)
			res++;	
		flag = !(cubeta[ cad[i] - 'A' ] == 0);
		actual[ cad[i] - 'A' ]++;
		actual[ cad[i - g] - 'A' ]--;				
	}
	if (checar())
		res++;
	cout << res << "\n";
}


//////////////
Is cheap enough?
Mapear la entrada

#define MAX 100007
struct coche {
	string nombre;
	int low, hig;
};

#define mapeo(n) distance(V.begin(), lower_bound(V.begin(), V.end(), n))

int main() {
	cin >> N;
	for (int i = 0; i < N; i++) {		
		cin >> cad >> low >> hig;
		C.push_back({cad, low, hig + 1});
		V.push_back(low);
		V.push_back(hig + 1);
	}

	cin >> Q;
	for (int i = 0; i < Q; i++) {
		cin >> aux;
		queries.push_back(aux);
		V.push_back(aux);
	}

	sort(V.begin(), V.end());
	V.erase(unique(V.begin(), V.end()), V.end());

	FenTree varo = FenTree(V.size());	
	FenTree idx = FenTree(V.size());
	int count = 1;
	for (auto var : C) {
		varo.Actualizar(mapeo(var.low) + 1, 1);
		varo.Actualizar(mapeo(var.hig) + 1, -1);
		idx.Actualizar(mapeo(var.low) + 1, count);
		idx.Actualizar(mapeo(var.hig) + 1, -count);
		count++;
	}
	
	for (auto q : queries) {
		aux = varo.Query(mapeo(q) + 1);
		if (aux == 0)
			cout << "NONE\n";
		else if (aux > 1)
			cout << "MULTIPLE\n";
		else {
			cout << C[idx.Query(mapeo(q) + 1) - 1].nombre << "\n";
		}
	}
}

Keypad problem
import java.util.*;
import java.math.*;

public class Main{
    static BigInteger x, y, d;
    public static void egcd(BigInteger a, BigInteger b){
        if (b.compareTo(BigInteger.ZERO) == 0){
            x = BigInteger.ONE;
            y = BigInteger.ZERO;
            d = a;
            return;
        }
        egcd(b, a.mod(b));
        BigInteger x1 = y;
        BigInteger y1 = x.subtract((a.divide(b)).multiply(y));
        x = x1;
        y = y1;
    }
    static int cont = 0;
    public static void main(String [] args){
        Scanner leer = new Scanner(System.in);
        while (leer.hasNext()){
            cont++;
            int n = leer.nextInt();
            BigInteger r = leer.nextBigInteger();
            x = BigInteger.ZERO;
            y = BigInteger.ZERO;
            d = BigInteger.ZERO;
            //BigInteger n = leer.nextBigInteger();
            //BigInteger r = leer.nextBigInteger();
            if (n == 0) 
                if (r.compareTo(BigInteger.ZERO) == 0)
                    break;
            BigInteger a[] = new BigInteger[n + 5];

            for (int i = 0; i < n; i++){
                a[i] = leer.nextBigInteger();
            }

            BigInteger sol[] = new BigInteger[n + 5];
            sol[0] = BigInteger.ONE;
            for (int i = 1; i < n; i++){
                sol[i] = BigInteger.ZERO;
            }

            BigInteger gcd = a[0];
            for (int i = 1; i < n; i++){
                egcd(gcd, a[i]);
                gcd = d;
                sol[i] = y;

                for (int j = 0; j <= i - 1; j++){
                    sol[j] = sol[j].multiply(x);
                }
            }

            if ((r.mod(gcd)).compareTo(BigInteger.ZERO) == 0){
                System.out.print("Case #" + cont + ":");
                for (int i = 0; i < n; i++){
                    String s = " " + (sol[i].multiply(r)).divide(gcd);
                    System.out.print(s);
                }
                System.out.println("");
            }else{
                System.out.println("Case #" + cont + ": Stupid keypad!");
            }
        }
    }
}

// The equation
Long x, y, d;
void extendedEuclid(Long a, Long b) {
    if (b == 0) { x = 1; y = 0; d = a; return; } 
    extendedEuclid(b, a % b); 
    Long x1 = y;
    Long y1 = x - (a / b) * y;
    x = x1;
    y = y1;
}

int main() {
    Long a, b, c, x1, x2, y1, y2;

    cin >> a >> b >> c;
    c = -c;
    cin >> x1 >> x2;
    cin >> y1 >> y2;

    extendedEuclid(a, b);
    Long gcd = d;
    Long ans = 0;

    if (gcd == 0)
        ans = c ? 0 : (x2 - x1 + 1) * (y2 - y1 + 1);
    else if (c % d == 0) {
        double lx, rx, ly, ry, l, r;
        x = c / d * x, y = c / d * y;
        if (a == 0)
            ans = (x2 - x1 + 1) * (y1 <= y && y <= y2);
        else if (b == 0)
            ans = (y2 - y1 + 1) * (x1 <= x && x <= x2);
        else {
            lx = 1.0 * (x1 - x) * gcd / b;
            rx = 1.0 * (x2 - x) * gcd / b;
            if (lx > rx) swap(lx, rx);

            ly = 1.0 * (y - y1) * gcd /a;
            ry = 1.0 * (y - y2) * gcd /a;
            if (ly > ry) swap(ly, ry);
            
            l = ceil(max(lx, ly));
            r = floor(min(rx, ry));
            ans = max(0.0, r - l + 1);
        }
    }
    cout << ans << "\n";
    return 0;
}

Careful with poison
#include <bits/stdc++.h>
using namespace std;

typedef long long Long;
const Long MOD = 1000000007;
const Long MAX = 2000007;

Long Exponenciar(Long a, Long n, Long m) {
    Long res = 1, p = a;
    for (; n; n >>= 1) {
        if (n & 1) res =
            (res * p) % m;
        p = (p * p) % m;
    }
    return res;
}

Long fact[MAX];
void pre(Long tam) {
    fact[0] = 1;
    for (Long i = 1; i <= tam; i++)
        fact[i] = (i * fact[i - 1]) % MOD;
}

Long combina(Long n, Long k) {
    Long num = fact[n];
    Long den = Exponenciar(((fact[n - k]) * (fact[k])) % MOD, MOD - 2, MOD);
    return (num * den) % MOD;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    Long N, x, y;
    pre(MAX - 5);
    while (cin >> N && N != 0) {
        cin >> x >> y;
        Long ans = 0;
        for (int i = x + 2, j = y, k = 0; k < 3; k++, i--, j++)
            ans = (ans + (combina(i + j, i) * combina(2 * N - j - i, N - i)) % MOD) % MOD;
        cout << (combina(2 * N, N) - ans + MOD) % MOD << "\n";
    }
    return 0;
}
