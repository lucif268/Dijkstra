#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <algorithm>

using namespace std;
using arista = tuple<int,int,int>;

bool ordenarPorPesoDescendente(const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
    return (get<2>(a) > get<2>(b));
}

class Grafo {
public:
    Grafo(int nodos, vector<arista> aristas);
    int nodos;
    vector<arista> aristas;
    vector<vector<tuple<int,int>>> listaAdy;
    int menorPesoDeAristaInterna(vector<bool> compoConexa);
    int mayorPesoDeAristaFrontera(vector<bool> compoConexa);
};

Grafo::Grafo(int nodos, vector<arista> aristas): nodos(nodos), aristas(aristas) {
    listaAdy = vector<vector<tuple<int,int>>>(nodos+1);

    for (int i=0; i<aristas.size(); i++) {
        int nodo1 = get<0>(aristas[i]);
        int nodo2 = get<1>(aristas[i]);
        int peso = get<2>(aristas[i]);
        listaAdy[nodo1].push_back(make_pair(nodo2,peso));
        listaAdy[nodo2].push_back(make_pair(nodo1,peso));
    }
}

int Grafo::menorPesoDeAristaInterna(vector<bool> compoConexa) {
    for (int i=aristas.size()-1; i>=0; i--) {
        if (compoConexa[get<0>(aristas[i])] && compoConexa[get<1>(aristas[i])]) {
            return get<2>(aristas[i]);
        }
    }
    return -1;
}

int Grafo::mayorPesoDeAristaFrontera(vector<bool> compoConexa) {
    for (int i=0; i<aristas.size(); i++) {
        if ((compoConexa[get<0>(aristas[i])] && !compoConexa[get<1>(aristas[i])]) || (!compoConexa[get<0>(aristas[i])] && compoConexa[get<1>(aristas[i])])) {
            return get<2>(aristas[i]);
        }
    }
    return -1;
}

class DisjointSet {
public:
    DisjointSet(Grafo grafo);
    Grafo grafo;
    void makeSet(int x);
    void disjointSetUnion(int x, int y);
    void link(int x, int y);
    int findSet(int x);
    vector<int> parent;
    vector<int> rank;
    vector<tuple<vector<bool>,int>> kruskal();
};

DisjointSet::DisjointSet(Grafo grafo): grafo(grafo) {
    parent = vector<int>(grafo.nodos+1);
    rank = vector<int>(grafo.nodos+1);
}

void DisjointSet::makeSet(int x) {
    parent[x] = x;
    rank[x] = 0;
}

void DisjointSet::disjointSetUnion(int x, int y) {
    link(findSet(x), findSet(y));
}

void DisjointSet::link(int x, int y) {
    if (rank[x] > rank[y]) {
        parent[y] = x;
    } else {
        parent[x] = y;
        if (rank[x] == rank[y]) {
            rank[y] = rank[y] + 1;
        }
    }
}

int DisjointSet::findSet(int x) {
    if (x != parent[x]) {
        parent[x] = findSet(parent[x]);
    }
    return parent[x];
}

vector<tuple<vector<bool>,int>> DisjointSet::kruskal() {
    vector<tuple<vector<bool>,int>> bosques;
    vector<bool> compoConexa;
    int tamCompoConexa;

    for (int v=1; v<=grafo.nodos; v++) {
        makeSet(v);
    }

    for (int i=0; i<grafo.aristas.size(); i++) {
        int u = get<0>(grafo.aristas[i]);
        int v = get<1>(grafo.aristas[i]);

        if (findSet(u) != findSet(v)) {

            disjointSetUnion(u,v);
            compoConexa = vector<bool>(grafo.nodos+1, false);

            tamCompoConexa = 0;

            for (int j=1; j<grafo.nodos+1; j++) {
                if (findSet(j) == findSet(u)) {
                    compoConexa[j] = true;
                    tamCompoConexa++;
                }
            }

            bosques.push_back(make_pair(compoConexa, tamCompoConexa));
        }
    }

    return bosques;
}


int main(int argc, char const *argv[]) {

    /*
    ifstream entrada;
    entrada.open(argv[1]);
    */

    int t;
    cin >> t;

    for (int i=0; i<t; i++) {
        int n, m;
        cin >> n >> m;

        vector<arista> aristas;

        for (int j=0; j<m; j++) {
            int v, w, k;
            cin >> v >> w >> k;
            aristas.push_back(make_tuple(v,w,k));
        }

        sort(aristas.begin(), aristas.end(), ordenarPorPesoDescendente);

        Grafo g = Grafo(n, aristas);

        DisjointSet ds = DisjointSet(g);

        vector<tuple<vector<bool>,int>> kruskal = ds.kruskal();

        int suma = 0;

        for (int i=0; i<kruskal.size(); i++) {
            int peso_i = g.menorPesoDeAristaInterna(get<0>(kruskal[i]));
            int peso_f = g.mayorPesoDeAristaFrontera(get<0>(kruskal[i]));

            if (peso_i > peso_f) {
                suma = suma + get<1>(kruskal[i]);
            }
        }

        cout << suma << endl;

    }

    return 0;
}
