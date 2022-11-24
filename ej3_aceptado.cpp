#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Nodo {
    int id;
    int distancia;
    int predecesor;
    int posicion_en_el_heap;
};

struct Arista {
    Nodo* origen;
    Nodo* destino;
    int peso;
};

class Grafo {
public:
    Grafo(vector<Nodo*>* nodos, vector<Arista> aristas);
    vector<Nodo*>* nodos;
    vector<Arista> aristas;
    int cantNodos;
    vector<vector<Arista>> ady;
};

Grafo::Grafo(vector<Nodo*>* nodos, vector<Arista> aristas): nodos(nodos), aristas(aristas) {
    cantNodos = nodos->size();

    ady = vector<vector<Arista>>(cantNodos);

    for (int i=0; i<aristas.size(); i++) {
        int u = aristas[i].origen->id;
        ady[u].push_back(aristas[i]);
    }
}

class Heap {
public:
    Heap(vector<Nodo*> nodos);
    vector<Nodo*> heap;
    void insert(Nodo* n);
    void decreaseKey(int i, Nodo* n);
    Nodo* extractMin();
    void minHeapify(int i);
    int heap_size;

private:
    int parent_(int i);
    int left_(int i);
    int right_(int i);
    Nodo* infinito_;
    Nodo* swap_aux_;
    void swap_(int a, int b);
};

Heap::Heap(vector<Nodo*> nodos) {
    heap_size = 0;
    heap.push_back(nullptr);

    /*
    infinito_ = new Nodo;
    infinito_->id = -1;
    infinito_->distancia = 10000000;
    infinito_->predecesor = -1;
    */

    swap_aux_ = new Nodo;
    swap_aux_->id = -1;
    swap_aux_->distancia = -1;
    swap_aux_->predecesor = -1;
    swap_aux_->posicion_en_el_heap = -1;

    for (int i=0; i<nodos.size(); i++) {
        nodos[i]->posicion_en_el_heap = i+1;
        insert(nodos[i]);
    }
}

void Heap::insert(Nodo* key) {
    heap.push_back(key);
    heap_size = heap_size + 1;
    // heap[heap_size] = infinito_;

    decreaseKey(heap_size, key);
}

void Heap::decreaseKey(int i, Nodo* key) {
    heap[i] = key;
    while (i>1 && (heap[parent_(i)]->distancia > heap[i]->distancia)) {
        swap_(i, parent_(i));
        i = parent_(i);
    }
}

Nodo* Heap::extractMin() {
    Nodo* min = heap[1];
    heap[1]->posicion_en_el_heap = -1;
    heap[1] = heap[heap_size];
    heap[1]->posicion_en_el_heap = 1;
    heap.pop_back();
    heap_size = heap_size - 1;
    minHeapify(1);
    return min;
}

void Heap::minHeapify(int i) {
    int l = left_(i);
    int r = right_(i);
    int menor = -1;

    if ((l <= heap_size) && (heap[l]->distancia < heap[i]->distancia)) {
        menor = l;
    } else {
        menor = i;
    }

    if ((r <= heap_size) && (heap[r]->distancia < heap[menor]->distancia)) {
        menor = r;
    }

    if (menor != i) {
        swap_(i, menor);
        minHeapify(menor);
    }
}

void Heap::swap_(int a, int b) {
    heap[a]->posicion_en_el_heap = b;
    heap[b]->posicion_en_el_heap = a;
    swap_aux_ = heap[a];
    heap[a] = heap[b];
    heap[b] = swap_aux_;
}

int Heap::parent_(int i) {
    return i/2;
}

int Heap::left_(int i) {
    return 2*i;
}

int Heap::right_(int i) {
    return 2*i + 1;
}





bool ordenarPorDistancia(Nodo* const& nodo1, Nodo* const& nodo2) {
    return nodo1->distancia > nodo2->distancia;
}

void relax(Heap* h, Nodo* u, Nodo* v, int peso) {
    if (v->distancia > u->distancia + peso) {
        v->distancia = u->distancia + peso;
        v->predecesor = u->id;
        h->decreaseKey(v->posicion_en_el_heap, v);
    }
}

void dijkstra(Grafo grafo, int s) {
    // vector<Nodo*> nodos_sin_procesar = *grafo.nodos;
    // vector<Nodo*> nodos_procesados;

    Heap h = Heap(*grafo.nodos);

    grafo.nodos->operator[](s)->distancia = 0;

    while (h.heap_size > 0) {
        /*
        sort(nodos_sin_procesar.begin(), nodos_sin_procesar.end(), &ordenarPorDistancia);
        Nodo* u = nodos_sin_procesar[nodos_sin_procesar.size()-1];
        nodos_sin_procesar.pop_back();
        nodos_procesados.push_back(u);
         */

        Nodo* u = h.extractMin();

        for (int i=0; i<grafo.ady[u->id].size(); i++) {
            Arista a = grafo.ady[u->id][i];
            Nodo* v = a.destino;
            relax(&h,u,v,a.peso);
        }
    }

}



int main(int argc, char const *argv[]) {

    /*
    ifstream entrada;
    entrada.open(argv[1]);
    */

    int t;
    cin >> t;

    for (int i=0; i<t; i++) {
        int c, p;
        cin >> c >> p;

        vector<Nodo*> nodos;

        for (int i=0; i<=p+1; i++) {
            Nodo *n = new Nodo;
            n->id = i;
            n->distancia = 10000000;
            n->predecesor = -1;
            n->posicion_en_el_heap = -1;
            nodos.push_back(n);
        }

        vector<Arista> aristas;

        int q, feligres;

        cin >> q;

        for (int j=0; j<q; j++) {
            cin >> feligres;

            Arista a;
            a.origen = nodos[0];
            a.destino = nodos[feligres];
            a.peso = 0;

            aristas.push_back(a);
        }

        int cantReglas, regla_monedas, regla_feligres;

        for (int j=1; j<=p; j++) {
            cin >> cantReglas;

            for (int r=0; r<cantReglas; r++) {
                cin >> regla_monedas >> regla_feligres;

                Arista a;
                a.origen = nodos[j];
                a.peso = regla_monedas;

                if (regla_feligres == 0) {
                    a.destino = nodos[p+1];
                } else {
                    a.destino = nodos[regla_feligres];
                }

                aristas.push_back(a);
            }
        }

        Grafo g = Grafo(&nodos, aristas);

        dijkstra(g, 0);

        int caminoMinimoHastaElPortero = nodos[p+1]->distancia;

        int cantMinimaMonedas = (c - caminoMinimoHastaElPortero) / (caminoMinimoHastaElPortero - 1);
        int resto = (c - caminoMinimoHastaElPortero) % (caminoMinimoHastaElPortero - 1);

        if (resto > 0) {
            cout << cantMinimaMonedas + 1 << endl;
        } else {
            cout << cantMinimaMonedas << endl;
        }

    }

    return 0;
}