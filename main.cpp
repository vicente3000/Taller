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
    int n = grafo.size(); // Número de nodos en el grafo

    // Vectores auxiliares
    vector<bool> visitado(n, false); // Marca si un nodo fue visitado
    vector<Nodo*> nodos(n, nullptr); // Nodos del árbol
    vector<int> distancias(n, INT_MAX); // Distancias desde el origen

    int origen = 0; // Nodo inicial

    // Nodo raíz
    nodos[origen] = new Nodo('A' + origen, 0);
    distancias[origen] = 0;

    // Empezamos a buscar los nodos (Dijkstra sin cola de prioridad)
    for (int iter = 0; iter < n; ++iter) {
        // Buscar el nodo no visitado con la menor distancia actual
        int nodo_actual = -1;
        int min_distancia = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (!visitado[i] && distancias[i] < min_distancia) {
                min_distancia = distancias[i];
                nodo_actual = i;
            }
        }

        // Si no hay más nodos alcanzables, terminamos
        if (nodo_actual == -1) {
            break;
        }

        visitado[nodo_actual] = true; // Marcamos el nodo como visitado

        // Relajamos las aristas del nodo actual
        for (int i = 0; i < n; ++i) {
            if (grafo[nodo_actual][i] > 0) { // Si hay una arista
                int nueva_distancia = distancias[nodo_actual] + grafo[nodo_actual][i];

                if (nueva_distancia < distancias[i]) {
                    distancias[i] = nueva_distancia;

                    // Si el nodo no existe, lo creamos
                    if (nodos[i] == nullptr) {
                        nodos[i] = new Nodo('A' + i, nueva_distancia);
                    } else {
                        // Si ya existe, actualizamos su peso
                        nodos[i]->peso = nueva_distancia;
                    }

                    // Conectamos el nodo actual con el hijo
                    nodos[nodo_actual]->hijos.push_back(nodos[i]);
                }
            }
        }
    }

    // Si no encontramos el camino al destino, devolvemos nullptr
    if (distancias[destino] == INT_MAX) {
        cout << "No hay un camino disponible hacia el nodo" << endl;
        return nullptr;
    }

    return nodos[origen]; // Devolvemos el nodo raíz
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
