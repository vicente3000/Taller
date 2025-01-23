#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

class Nodo {
public:
    char valor;
    int peso;
    vector<Nodo*> hijos;

    Nodo(char v, int p) : valor(v), peso(p) {}
};

Nodo* dijkstra(vector<vector<int>>& grafo, int destino) {
    int n = grafo.size(); // creamos los visidatods y  una lsita para crear el arbol de nodos
    vector<bool> visitado(n, false);
    vector<Nodo*> nodos(n, nullptr);
    vector<int> distancias(n, INT_MAX);
    int origen = 0;

    //nodo raiz
    nodos[origen] = new Nodo('A' + origen, 0);
    distancias[origen] = 0;

    queue<int> q;
    q.push(origen);

    //emepzamos  a buscar los nodos
    while (!q.empty()) {
        int nodo_actual = q.front();
        q.pop();

        // si esta visitado lo saltamos
        if (visitado[nodo_actual]) continue;
        visitado[nodo_actual] = true;

        //buscamos cada nodo
        for (int i = 0; i < n; ++i) {
            if (grafo[nodo_actual][i] > 0) {
                int nueva_distancia = distancias[nodo_actual] + grafo[nodo_actual][i];

                if (nueva_distancia < distancias[i]) {
                    distancias[i] = nueva_distancia;

                    //si el nodo no existe se crea
                    if (nodos[i] == nullptr) {
                        nodos[i] = new Nodo('A' + i, nueva_distancia);
                    } else {
                        //si ya existe se guarda distancia
                        nodos[i]->peso = nueva_distancia;
                    }

                    //conectamos
                    nodos[nodo_actual]->hijos.push_back(nodos[i]);

                    q.push(i);
                }
            }
        }
    }

    //si no encontramos el camno al nodo retornamos null
    if (distancias[destino] == INT_MAX) {
        cout << "No hay un camino disponible hacia el nodo "<< endl;
        return nullptr;
    }


    return nodos[origen];
}


void dfsImprimirCamino(Nodo* nodo, char destino, vector<char>& camino_actual, bool& encontrado) {
    if (nodo == nullptr || encontrado) return;

    camino_actual.push_back(nodo->valor);

    //si encontramos el nodo que queremos imprimimos el camino recorrido para llegar
    if (nodo->valor == destino) {
        encontrado = true;
        cout << "camino mas corto es: ";
        for (char n : camino_actual) {
            cout << n << " ";
        }
        cout << endl;
        cout << "peso total:  " << nodo->peso << endl;
    } else {
        // recorremos el arbol
        for (Nodo* hijo : nodo->hijos) {
            //recursividad para llamar los hijos                               4,7,12,8
            dfsImprimirCamino(hijo, destino, camino_actual, encontrado);
        }
    }

    for (int i = camino_actual.size() - 1; i >= 0; --i) {
        camino_actual[i] = 0;
        break;
    }
}

int main() {
    ifstream archivo("matriz.txt"); // lectura de archivo
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }
    int n;
    archivo >> n;
    archivo.ignore();

    vector<vector<int>> matriz(n, vector<int>(n)); // matriz
    string linea;

    for (int i = 0; i < n; i++) {
        getline(archivo, linea);
        int numero = 0;
        int col = 0;

        for (char c : linea) {
            if (c == ' ') {
                matriz[i][col++] = numero;
                numero = 0;
            } else {
                numero = numero * 10 + (c - '0');
            }
        }
        matriz[i][col] = numero;
    } //saca cada caracter para convertirlo en int y luego lo guarda en la posicion de la matriz

    cout << "nodos disponibles: " ; //imprimimos los nodos disvponibles y pedimos al usuario el Nodo que quiere ir
    for (int i = 0; i < n; i++) {
        char letra = i + 97 - 32;
        cout <<  letra << " ";
    }
    //arreglar
    bool verdad;
    char nodo;
    do {
        cout << "ingrese el nodo que quiere ir:" << endl;
        cin >> nodo;
        if (nodo >= 97 && nodo <= 96 + n) {
            verdad = false;
        } else {
            nodo = nodo + 32;
            if (nodo >= 97 && nodo <= 96 + n) {
                verdad = false;
            } else {
                cout << "ingrese un nodo de los dispoibles " << endl;
                verdad = true;
            }
        }
    } while (verdad); // verificamos el nodo que ingreso existe en el grafo

    nodo = toupper(nodo);
    int nodo_indice = nodo - 'A';
    Nodo* raiz = dijkstra(matriz,nodo_indice); //sacamos el peso del camino mas corto y mientras en paralelo se arma el arbol con los
    //pesos acumulados

    //si no tenemos raiz no hacemos nada
    if (raiz != nullptr) {
        vector<char> camino_actual;
        bool encontrado = false;

        dfsImprimirCamino(raiz, nodo, camino_actual, encontrado);
    }
    archivo.close();

    return 0;
}
