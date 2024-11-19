#ifndef GRAFO_H_
#define GRAFO_H_

#include "GeneradorIdentificador.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using std::string;
using std::ostringstream;
using std::vector;

namespace URGGrafo {

    struct NodoAdyacencia {
        NodoAdyacencia* siguiente;
        int verticeAdyacente;
    };

    struct listaAdyacencia {
        NodoAdyacencia* primero;
    };

    struct Grafo {
        string id;
        string nombre;
        vector<string> etiquetas;
        int cantidadVertices = 0;
        enum TipoGrafo { DIRIGIDO, NO_DIRIGIDO} tipo = NO_DIRIGIDO;
        listaAdyacencia* adyacencias = nullptr; // es un diccionario donde la clave es el id de vertice y el valor son sus adyacencias
    };

    // Función auxiliar para crear un grafo
    Grafo* CrearGrafo(string nombre, int cantidadVertices, Grafo::TipoGrafo tipo) {
        if (cantidadVertices < 0) {
            return nullptr; // Verificamos la precondición
        }

        Grafo* grafo = new Grafo;
        grafo->nombre = nombre;
        grafo->cantidadVertices = cantidadVertices;
        grafo->tipo = tipo;
        grafo->id = URGGeneradorIdentificador::GenerarIdentificadorUnico();
        grafo->adyacencias = new listaAdyacencia[cantidadVertices]; // Asignamos memoria para la lista de adyacencias

        // Inicializamos las listas de adyacencias
        for (int i = 0; i < cantidadVertices; ++i) {
            grafo->adyacencias[i].primero = nullptr;
        }

        return grafo;
    }

    /*
     * Precondicion: -
     * Postcondicion: Si @cantidad de vertices es un numero mayor o igual que cero
     * devuelve un grafo dirigido de nombre @nombre al cual se le asocia un identificador unico.
     * Si @cantidad de vertices es menor que cero devuelve NULL
     */
    Grafo* CrearGrafoDirigido(string nombre, int cantidadVertices) {
        return CrearGrafo(nombre, cantidadVertices, Grafo::DIRIGIDO);
    }

    /*
     * Precondicion: -
     * Postcondicion: Si @cantidad de vertices es un numero mayor o igual que cero
     * devuelve un grafo no dirigido de nombre @nombre al cual se le asocia un identificador unico.
     * Si @cantidad de vertices es menor que cero devuelve NULL
     */
    Grafo* CrearGrafoNoDirigido(string nombre, int cantidadVertices) {
        return CrearGrafo(nombre, cantidadVertices, Grafo::NO_DIRIGIDO);
    }

    /*
     * Precondicion: @grafo es una instancia valida creada con alguna de las primitivas creacionales
     * Postcondicion: Devuelve el nombre de @grafo
     */
    string ObtenerNombre(const Grafo* grafo) {
        return grafo->nombre; // Retorna el nombre del grafo
    }

    /*
     * Precondicion: @grafo es una instancia valida creada con alguna de las primitivas creacionales
     * Postcondicion: Devuelve el identificador unico de @grafo
     */
    string ObtenerIdentificador(const Grafo* grafo) {
        return grafo->id; // Retorna el identificador del grafo
    }

    // Función para agregar un nodo a la lista de adyacencia
    void AgregarAdyacencia(listaAdyacencia& lista, int verticeAdyacente) {
        NodoAdyacencia* nuevoNodo = new NodoAdyacencia;
        nuevoNodo->verticeAdyacente = verticeAdyacente;
        nuevoNodo->siguiente = lista.primero;
        lista.primero = nuevoNodo;
    }

    /*
     * Precondicion: @grafo es una instancia valida creada con alguna de las primitivas creacionales
     * Postcondicion: Si @grafo es un grafo no dirigido, se agrega una relacion de adyacencia conmutativa entre @verticeOrigen y @verticeDestino.
     * Si @grafo es un grafo dirigido, se agrega una relacion de adyacencia de @verticeOrigen a @verticeDestino.
     * Si @verticeOrigen o @verticeDestino no pertenece al grafo no realiza ninguna accion.
     */
    void Conectar(Grafo* grafo, int verticeOrigen, int verticeDestino) {
        // Verificamos que los vértices estén dentro del rango
        if (verticeOrigen < 0 || verticeOrigen >= grafo->cantidadVertices ||
            verticeDestino < 0 || verticeDestino >= grafo->cantidadVertices) {
            return; // No hacemos nada si los vértices están fuera de rango
        }

        // Agregar la relación de adyacencia para el grafo dirigido
        AgregarAdyacencia(grafo->adyacencias[verticeOrigen], verticeDestino);

        // Si el grafo es no dirigido, agregar la relación de adyacencia en ambas direcciones
        if (grafo->tipo == Grafo::NO_DIRIGIDO) {
            AgregarAdyacencia(grafo->adyacencias[verticeDestino], verticeOrigen);
        }
    }


    // Función para verificar si un vértice está en la lista de adyacencia
    bool EstaEnLista(const listaAdyacencia& lista, int vertice) {
        NodoAdyacencia* actual = lista.primero;
        while (actual != nullptr) {
            if (actual->verticeAdyacente == vertice) {
                return true; // Vértice encontrado
            }
            actual = actual->siguiente; // Avanzamos al siguiente nodo
        }
        return false; // Vértice no encontrado
    }

    /*
     * Precondicion: @grafo es una instancia valida creada con alguna de las primitivas creacionales
     * Postcondicion: Devuelve true si @verticeOrigen es adyacente a @verticeDestino. Caso contrario devuelve false
     */
    bool SonAdyacentes(const Grafo* grafo, int verticeOrigen, int verticeDestino) {
        // Verificamos que los vértices estén dentro del rango
        if (verticeOrigen < 0 || verticeOrigen >= grafo->cantidadVertices ||
            verticeDestino < 0 || verticeDestino >= grafo->cantidadVertices) {
            return false; // Si los vértices están fuera de rango, no son adyacentes
        }

        // Verificamos si verticeDestino está en la lista de adyacencia de verticeOrigen
        return EstaEnLista(grafo->adyacencias[verticeOrigen], verticeDestino);
    }

    /*
    * Precondiciones: @grafo es una instancia valida creada con alguna de las primitivas creacionales
    * Postcondiciones: Devuelve los vertices en un registro en formato CSV donde cada campo es un vertice.
    * Si los vertices tienen etiquetas devuelve las etiquetas en lugar del numero de vertice.
    */
string ObtenerVertices(const Grafo* grafo) {
    string resultado; // Usamos una cadena para construir el resultado

    // Verificamos que no haya índices fuera de rango
    for (int i = 0; i < grafo->cantidadVertices; ++i) {
        if (i >= grafo->etiquetas.size()) {
            resultado += std::to_string(i); // Si no hay etiqueta, usamos el índice
        }
        else if (!grafo->etiquetas[i].empty()) {
            resultado += grafo->etiquetas[i]; // Usamos la etiqueta si está definida
        }
        else {
            resultado += std::to_string(i); // Usamos el índice si la etiqueta está vacía
        }
        if (i < grafo->cantidadVertices - 1) {
            resultado += ",";
        }
    }
    return resultado;
}


    Grafo* ObtenerUnion(const Grafo* grafo1, const Grafo* grafo2) {
        // Verificar que los grafos sean válidos
        if (!grafo1 || !grafo2) {
            return nullptr; // Retornar nullptr si alguno de los grafos es inválido
        }

        // Calcular el número total de vértices
        int totalVertices = grafo1->cantidadVertices + grafo2->cantidadVertices;

        // Inicializar el nuevo grafo
        Grafo* unionGrafo = CrearGrafo("Union",
            totalVertices,
            (grafo1->tipo == Grafo::DIRIGIDO && grafo2->tipo == Grafo::DIRIGIDO) ? Grafo::DIRIGIDO : Grafo::NO_DIRIGIDO);

        if (unionGrafo == nullptr) {
            return nullptr; // Manejo de error si no se pudo inicializar el grafo
        }

        // Copiar las etiquetas de los vértices de grafo1
        for (int i = 0; i < grafo1->cantidadVertices; ++i) {
            unionGrafo->etiquetas[i] = grafo1->etiquetas[i]; // Copiar etiquetas de grafo1
        }

        // Copiar las etiquetas de los vértices de grafo2
        for (int i = 0; i < grafo2->cantidadVertices; ++i) {
            unionGrafo->etiquetas[grafo1->cantidadVertices + i] = grafo2->etiquetas[i]; // Copiar etiquetas de grafo2
        }

        // Conectar las aristas de grafo1
        for (int i = 0; i < grafo1->cantidadVertices; ++i) {
            NodoAdyacencia* adyacente = grafo1->adyacencias[i].primero;
            while (adyacente) {
                // Agregar arista desde el vértice i a su adyacente
                NodoAdyacencia* nuevoNodo = new NodoAdyacencia;
                nuevoNodo->verticeAdyacente = adyacente->verticeAdyacente; // Vertice adyacente original
                nuevoNodo->siguiente = unionGrafo->adyacencias[i].primero; // Apuntar al primer nodo existente
                unionGrafo->adyacencias[i].primero = nuevoNodo; // Actualizar el primer nodo
                adyacente = adyacente->siguiente; // Avanzar al siguiente nodo
            }
        }

        // Conectar las aristas de grafo2
        for (int i = 0; i < grafo2->cantidadVertices; ++i) {
            NodoAdyacencia* adyacente = grafo2->adyacencias[i].primero;
            while (adyacente) {
                // Agregar arista desde el vértice (i + cantidad de grafo1) a su adyacente
                NodoAdyacencia* nuevoNodo = new NodoAdyacencia;
                nuevoNodo->verticeAdyacente = adyacente->verticeAdyacente + grafo1->cantidadVertices; // Ajustar el índice
                nuevoNodo->siguiente = unionGrafo->adyacencias[grafo1->cantidadVertices + i].primero; // Apuntar al primer nodo existente
                unionGrafo->adyacencias[grafo1->cantidadVertices + i].primero = nuevoNodo; // Actualizar el primer nodo
                adyacente = adyacente->siguiente; // Avanzar al siguiente nodo
            }
        }

        return unionGrafo; // Devolver el nuevo grafo que representa la unión
    }

    void CopiarEtiquetas(const Grafo* grafoOrigen, Grafo* grafoDestino) {
        for (int i = 0; i < grafoOrigen->cantidadVertices; ++i) {
            grafoDestino->etiquetas[i] = grafoOrigen->etiquetas[i]; // Copiar la etiqueta de cada vértice
        }
    }

    Grafo* ObtenerGrafoComplementario(const Grafo* grafo) {
        Grafo* complemento = new Grafo;
        complemento->cantidadVertices = grafo->cantidadVertices;
        complemento->tipo = Grafo::NO_DIRIGIDO;  // El complemento será no dirigido
        complemento->adyacencias = new listaAdyacencia[complemento->cantidadVertices];
        complemento->etiquetas.resize(complemento->cantidadVertices);

        // Copiar las etiquetas del grafo original
        CopiarEtiquetas(grafo, complemento);

        // Agregar aristas en el complemento
        for (int i = 0; i < grafo->cantidadVertices; ++i) {
            for (int j = 0; j < grafo->cantidadVertices; ++j) {
                if (i != j && !SonAdyacentes(grafo, i, j)) {
                    Conectar(complemento, i, j);  // Agregar arista si no existe en el grafo original
                }
            }
        }

        return complemento;
    }

    int ObtenerGrado(const Grafo* grafo, int vertice) {
        if (grafo->tipo == Grafo::NO_DIRIGIDO) {
            // En un grafo no dirigido, el grado es el número de adyacentes
            int grado = 0;
            NodoAdyacencia* actual = grafo->adyacencias[vertice].primero;
            while (actual) {
                grado++;
                actual = actual->siguiente;
            }
            return grado;
        }
        else {
            // En un grafo dirigido, el grado de salida es el número de adyacentes
            int gradoSalida = 0;
            NodoAdyacencia* actual = grafo->adyacencias[vertice].primero;
            while (actual) {
                gradoSalida++;
                actual = actual->siguiente;
            }
            return gradoSalida;
        }
    }


    void AgregarEtiqueta(Grafo* grafo, int vertice, string etiqueta) {
        if (vertice >= 0 && vertice < grafo->cantidadVertices) {
            grafo->etiquetas[vertice] = etiqueta;  // Asigna la etiqueta al vértice correspondiente
        }
    }


    bool EsCompleto(const Grafo* grafo) {
        for (int i = 0; i < grafo->cantidadVertices; ++i) {
            for (int j = 0; j < grafo->cantidadVertices; ++j) {
                if (i != j && !SonAdyacentes(grafo, i, j)) {
                    return false;  // Si algún par de vértices no es adyacente, no es completo
                }
            }
        }
        return true;  // Si todos los vértices están conectados, es completo
    }
    string ObtenerAristas(const Grafo* grafo) {
        string resultado;

        // Recorremos todos los vértices
        for (int i = 0; i < grafo->cantidadVertices; ++i) {
            NodoAdyacencia* actual = grafo->adyacencias[i].primero;

            // Para cada vértice, recorremos sus adyacencias
            while (actual != nullptr) {
                // Si es un grafo no dirigido, solo incluimos las aristas donde i < verticeAdyacente
                // para evitar duplicados
                if (grafo->tipo == Grafo::NO_DIRIGIDO && i > actual->verticeAdyacente) {
                    actual = actual->siguiente;
                    continue;
                }

                // Si hay etiquetas, usamos las etiquetas en lugar de los números
                string verticeOrigen = !grafo->etiquetas[i].empty() ?
                    grafo->etiquetas[i] : std::to_string(i);
                string verticeDestino = !grafo->etiquetas[actual->verticeAdyacente].empty() ?
                    grafo->etiquetas[actual->verticeAdyacente] :
                    std::to_string(actual->verticeAdyacente);

                // Agregamos la arista al resultado
                if (!resultado.empty()) {
                    resultado += ",";
                }
                resultado += verticeOrigen + "-" + verticeDestino;

                actual = actual->siguiente;
            }
        }

        return resultado;
    }

    string ObtenerSucesionGrafica(const Grafo* grafo) {
        vector<int> grados;
        for (int i = 0; i < grafo->cantidadVertices; ++i) {
            grados.push_back(ObtenerGrado(grafo, i));  // Obtener el grado de cada vértice
        }

        // Ordenar los grados de mayor a menor
        sort(grados.rbegin(), grados.rend());

        // Convertir los grados a una cadena separada por comas
        string resultado;
        for (int i = 0; i < grados.size(); ++i) {
            resultado += std::to_string(grados[i]);
            if (i < grados.size() - 1) {
                resultado += ",";
            }
        }

        return resultado;
    }

    int ObtenerCantidadVertices(const Grafo* grafo) {
        return grafo->cantidadVertices;
    }

    void CambiarNombre(Grafo* grafo, string nombre) {
        grafo->nombre = nombre;
    }

    void DestruirGrafo(Grafo* grafo) {

        delete[] grafo->adyacencias;

        delete grafo;
    }

    

}




#endif