#include<iostream>
#include<vector>
#include<math.h>
#include<fstream>
#include<algorithm>
#include <bits/stdc++.h>
using namespace std;

#define INDEFINIDO -1
/* 
   1         -> cant de casos de test
   10 2      -> capacidad de la caja (10) , cantidad de feligreses (2)
   2 1 2     -> nºmero q de feligreses a los cuales el portero puede pasar la caja (2) , cada uno de estos feligreses enumerados (1 y 2)
   2 6 0 4 2 -> 1° feligres: n° de reglas (2), cantidad de monedas para la caja de la primer regla (6), feligres al cual pasar la 
                caja (0: si es 0 vuelve al portero), cantidad de monedas para la caja de la segunda regla (4), feligres al cual pasar la 
                caja (2)
   1 5 0     -> 2° feligres: n° de reglas (1), cantidad de monedas para la caja de la primer regla (5), feligres al cual pasar la 
               caja (0: portero)
   0         -> fin del test
*/

void printVector(vector<pair<int, int>> vector){
    for(int i = 0; i < vector.size(); i++){
        cout << vector[i].first << " " << vector[i].second << endl;
    }
}
void print(vector<int> vector){
    for(int i = 0; i < vector.size(); i++){
        cout << vector[i] << " "<< endl;
    }
}

void printGrafo(vector<vector<int>> grafo){
    for(int i = 0; i < grafo.size(); ++i){
        cout << i << ": {";
        for(int j = 0; j < grafo[i].size(); ++j){
            cout << grafo[i][j] << " " ;
        }
        cout << "}" << endl;
    }
}

void dikjstra(vector<vector<int>> grafo, int raiz){
    vector<int> nodosIncorporados = {};
    vector<int> nodosRestantes(grafo.size());
    vector<int> distancia(grafo.size());

    nodosIncorporados[0] = raiz;
    distancia[raiz] = 0;

    for(int u = 0; u < grafo.size(); ++u){
        if(grafo[raiz][u] != INDEFINIDO)
            distancia[u] = grafo[raiz][u];
        else
            distancia[u] = INT64_MAX;

    }
    while(nodosIncorporados.size() != grafo.size()){
        int w = min(distancia);
        nodosIncorporados.push_back(w);
        for(int i = 0; i < nodosRestantes.size()- nodosIncorporados.size(); ++i){
            if(distancia[i] > distancia[w] + grafo[w][i])
                distancia[i] = distancia[w] + grafo[w][i];
        }
    }
    
}

priority_queue <int, vector<int>, greater<int> > min_heap;
//priority_queue <int, vector<int>, greater<int> > pq;
// `The third parameter, ‘Comparison Type’ can either be a function or factor (aka function object) that must have bool as return-type and must have 2 arguments.

int main(int argc, char const *argv[]){
    ios::sync_with_stdio(false);
    cin.tie(0);
    ifstream entrada;
    entrada.open(argv[1]);
    int t, capacidad, f, q;
    entrada >> t >> capacidad >> f >> q;

    vector<vector<int>> grafo (f+1, vector<int>(f+1, INDEFINIDO)); 

    // llenamos las adyacencias del portero
    for(int i = 0; i < q; ++i){
        int feligres_id;
        entrada >> feligres_id;
        grafo[0][feligres_id] = 0;
    }
    //print(feligreses_con_portero);
   
    for(int i = 1 ; i < f+1; ++i){ //cada linea de feligres
        int cantReglas;
        entrada >> cantReglas;
    
        for(int j = 1; j <= cantReglas; ++j){
            int monedas, feligres;
            entrada >> monedas >> feligres;
            grafo[i][feligres] = monedas;
        } 
    }
    printGrafo(grafo);
    return 0;
}


/*

{-1,0,0}
{6,-1,4}
{5,-1,-1}

*/