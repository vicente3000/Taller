#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

class Nodo {
    public:
    char valor;
    vector<Nodo*> hijos;
    Nodo(char v) : valor(v) {}
};

//construimos el arbol con los nodos
Nodo* construirArbol(const vector<int>& prev, int destino) {
    vector<Nodo*> nodos(prev.size(), nullptr);
    Nodo* raiz = nullptr;

    int nodo_actual = destino;
    while (nodo_actual != -1) {
        // Convertimos el índice del nodo
        char valor = 'A' + nodo_actual;

        // Si el nodo actual no existe
        if (nodos[nodo_actual] == nullptr) {
            nodos[nodo_actual] = new Nodo(valor);
        }

        // conectamos el nodo actual como hijo
        int nodo_padre = prev[nodo_actual];
        if (nodo_padre != -1) {
            if (nodos[nodo_padre] == nullptr) {
                nodos[nodo_padre] = new Nodo('A' + nodo_padre);
            }
            nodos[nodo_padre]->hijos.push_back(nodos[nodo_actual]);
        } else {
            raiz = nodos[nodo_actual];
        }

        // Avanzamos
        nodo_actual = nodo_padre;
    }

    return raiz;
}

//sacamos el peso del camino mas corto
int dijkstra(vector<vector<int>>& grafo, int elegido, int cantidad, vector<int>& prev) {
    vector<int> distancias(cantidad, INT_MAX);
    vector<bool> visitado(cantidad, false);
    queue<int> cola;

    distancias[0] = 0;
    cola.push(0);

    //emepzamos desde el nodo 0 hasta el elejido comparando lso nodso
    while (!cola.empty()) {
        int nodo_actual = cola.front();
        cola.pop();

        //si esta visitado salta
        if (visitado[nodo_actual]) continue;
        visitado[nodo_actual] = true;

        //verificamos als conexioens y comparamos las aristas
        for (int i = 0; i < cantidad; ++i) {
            if (grafo[nodo_actual][i] > 0) {
                int peso_arista = grafo[nodo_actual][i];
                int nueva_distancia = distancias[nodo_actual] + peso_arista;

                if (nueva_distancia < distancias[i]) {
                    distancias[i] = nueva_distancia;
                    prev[i] = nodo_actual;
                    cola.push(i);
                }
            }
        }
    }
    // Retornar la distancia al nodo elegido
    return distancias[elegido];
}

//recusirividad para imprimri el arbol
void recursividad(Nodo* nodo, int nivel) {
    if (nodo == nullptr) return;

    for (int i = 0; i < nivel; ++i) cout << "  ";
    cout << nodo->valor << endl;

    for (Nodo* hijo : nodo->hijos) {
        recursividad(hijo, nivel + 1);
    }
}
//imprimimos el arbol
void imprimirArbol(Nodo* nodo) {
    recursividad(nodo, 0);
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

    cout << "Nodos disponibles: " ; //imprimimos los nodos disvponibles y pedimos al usuario el Nodo que quiere ir
    for (int i = 0; i < n; i++) {
        char letra = i + 97 - 32;
        cout <<  letra << " ";
    }

    bool verdad;
    char nodo;
    do {
        cout << "Ingrese el nodo que quiere ir:" << endl;
        cin >> nodo;
        if (nodo >= 97 && nodo <= 96 + n) {
            verdad = false;
        } else {
            nodo = nodo + 32;
            if (nodo >= 97 && nodo <= 96 + n) {
                verdad = false;
            } else {
                cout << "Ingrese un nodo de los dispoibles " << endl;
                verdad = true;
            }
        }
    } while (verdad); // verificamos el nodo que ingreso existe en el grafo

    nodo = toupper(nodo);
    int nodo_indice = nodo - 'A';

    vector<int> prev(n, -1); // vector para rencostruir el arbol
    int peso_total = dijkstra(matriz, nodo_indice, n, prev); //sacamos el peso del camino mas corto

    //si no se ecuetra el camino
    if (peso_total == INT_MAX) {
        cout << "No hay un camino al nodo seleccionado." << endl;
    } else {
        cout << "Nodo seleccionado: " << nodo << endl;
        cout << "Peso total del camino mas corto: " << peso_total << endl;

        //imprimir el arbol del camino más corto
        Nodo* raiz = construirArbol(prev, nodo_indice);
        cout << "El arbol del camino mas corto es: " << endl;
        imprimirArbol(raiz);
    }
    archivo.close();

    return 0;
}