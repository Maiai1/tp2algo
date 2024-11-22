#ifndef GRAFO_H_
#define GRAFO_H_

#include "GeneradorIdentificador.h"
#include "GeneradorIdentificador.h"
#include <iostream>
#include <string>
#include<sstream>
#include <vector>

using namespace std;



#include "Grafo.h"
#include <algorithm>

    namespace URGGrafo {
        struct NodoAdyacencias {
            int verticeAdyacente;
            NodoAdyacencias* siguiente;
        };

        struct ListaAdyacencias {
            NodoAdyacencias* primero;
        };

        struct NodoVertices {
            string etiqueta;
            ListaAdyacencias* adyacencias;
        };

        struct Grafo {
            NodoVertices* vertices;  // Array de nodos (vértices)
            string nombre;
            int cantidadVertices;
            bool dirigido;
            string identificador;
        };

        // Funciones auxiliares privadas
        void inicializarListaAdyacencias(ListaAdyacencias* lista) {
            lista->primero = nullptr;
        }


        bool existeAdyacencia(const ListaAdyacencias* lista, int vertice) {
            NodoAdyacencias* actual = lista->primero;
            while (actual != nullptr) {
                if (actual->verticeAdyacente == vertice) {
                    return true;
                }
                actual = actual->siguiente;
            }
            return false;
        }

        void agregarAdyacencia(ListaAdyacencias* lista, int vertice) {
            // Evitar duplicados
            if (existeAdyacencia(lista, vertice)) return;

            NodoAdyacencias* nuevo = new NodoAdyacencias;
            nuevo->verticeAdyacente = vertice;
            nuevo->siguiente = lista->primero;
            lista->primero = nuevo;
        }


        void copiarAdyacencias(const Grafo* origen, Grafo* destino) {
            for (int i = 0; i < origen->cantidadVertices; i++) {
                NodoAdyacencias* actual = origen->vertices[i].adyacencias->primero;
                while (actual != nullptr) {
                    agregarAdyacencia(destino->vertices[i].adyacencias, actual->verticeAdyacente);
                    actual = actual->siguiente;
                }
            }
        }

        // Implementación de funciones principales
        Grafo* CrearGrafoDirigido(string nombre, int cantidadVertices) {
            if (cantidadVertices < 0) return nullptr;

            Grafo* grafo = new Grafo;
            grafo->nombre = nombre;
            grafo->cantidadVertices = cantidadVertices;
            grafo->dirigido = true;
            grafo->identificador =URGGeneradorIdentificador ::GenerarIdentificadorUnico();

            grafo->vertices = new NodoVertices[cantidadVertices];
            for (int i = 0; i < cantidadVertices; i++) {
                grafo->vertices[i].adyacencias = new ListaAdyacencias;
                inicializarListaAdyacencias(grafo->vertices[i].adyacencias);
                grafo->vertices[i].etiqueta = "";
            }

            return grafo;
        }

        Grafo* CrearGrafoNoDirigido(string nombre, int cantidadVertices) {
            if (cantidadVertices < 0) return nullptr;

            Grafo* grafo = CrearGrafoDirigido(nombre, cantidadVertices);
            if (grafo != nullptr) {
                grafo->dirigido = false;
            }
            return grafo;
        }

        string ObtenerNombre(const Grafo* grafo) {
            return grafo->nombre;
        }

        string ObtenerIdentificador(const Grafo* grafo) {
            return grafo->identificador;
        }

        void Conectar(Grafo* grafo, int verticeOrigen, int verticeDestino) {
            if (verticeOrigen < 0 || verticeOrigen >= grafo->cantidadVertices ||
                verticeDestino < 0 || verticeDestino >= grafo->cantidadVertices) {
                return;
            }

            agregarAdyacencia(grafo->vertices[verticeOrigen].adyacencias, verticeDestino);

            if (!grafo->dirigido) {
                agregarAdyacencia(grafo->vertices[verticeDestino].adyacencias, verticeOrigen);
            }
        }

        bool SonAdyacentes(const Grafo* grafo, int verticeOrigen, int verticeDestino) {
            if (verticeOrigen < 0 || verticeOrigen >= grafo->cantidadVertices ||
                verticeDestino < 0 || verticeDestino >= grafo->cantidadVertices) {
                return false;
            }

            return existeAdyacencia(grafo->vertices[verticeOrigen].adyacencias, verticeDestino);
        }

        string ObtenerVertices(const Grafo* grafo) {
            std::stringstream ss;
            for (int i = 0; i < grafo->cantidadVertices; i++) {
                if (i > 0) ss << ",";
                if (grafo->vertices[i].etiqueta.empty()) {
                    ss << i;
                }
                else {
                    ss << grafo->vertices[i].etiqueta;
                }
            }
            return ss.str();
        }

        string ObtenerAristas(const Grafo* grafo) {
            std::stringstream ss;
            bool primero = true;

            for (int i = 0; i < grafo->cantidadVertices; i++) {
                NodoAdyacencias* actual = grafo->vertices[i].adyacencias->primero;
                while (actual != nullptr) {
                    // Para grafos no dirigidos, solo imprimimos una vez cada arista
                    if (grafo->dirigido || i < actual->verticeAdyacente) {
                        if (!primero) ss << " ";
                        if (grafo->vertices[i].etiqueta.empty()) {
                            ss << i;
                        }
                        else {
                            ss << grafo->vertices[i].etiqueta;
                        }
                        ss << "-";
                        if (grafo->vertices[actual->verticeAdyacente].etiqueta.empty()) {
                            ss << actual->verticeAdyacente;
                        }
                        else {
                            ss << grafo->vertices[actual->verticeAdyacente].etiqueta;
                        }
                        primero = false;
                    }
                    actual = actual->siguiente;
                }
            }
            return ss.str();
        }

        Grafo* ObtenerUnion(const Grafo* grafo1, const Grafo* grafo2) {
            // Verificar que ambos grafos sean del mismo tipo (dirigidos o no dirigidos)
            if (grafo1->dirigido != grafo2->dirigido) return nullptr;

            // Crear un nuevo grafo con la cantidad máxima de vértices
            int maxVertices = std::max(grafo1->cantidadVertices, grafo2->cantidadVertices);
            Grafo* unionGrafo = grafo1->dirigido ?
                CrearGrafoDirigido("Union", maxVertices) :
                CrearGrafoNoDirigido("Union", maxVertices);

            // Copiar etiquetas y adyacencias del primer grafo
            for (int i = 0; i < grafo1->cantidadVertices; i++) {
                unionGrafo->vertices[i].etiqueta = grafo1->vertices[i].etiqueta;
                NodoAdyacencias* actual = grafo1->vertices[i].adyacencias->primero;
                while (actual != nullptr) {
                    agregarAdyacencia(unionGrafo->vertices[i].adyacencias, actual->verticeAdyacente);
                    actual = actual->siguiente;
                }
            }

            // Agregar adyacencias del segundo grafo
            for (int i = 0; i < grafo2->cantidadVertices; i++) {
                if (grafo2->vertices[i].etiqueta != "" && i < maxVertices) {
                    unionGrafo->vertices[i].etiqueta = grafo2->vertices[i].etiqueta;
                }
                NodoAdyacencias* actual = grafo2->vertices[i].adyacencias->primero;
                while (actual != nullptr && actual->verticeAdyacente < maxVertices) {
                    agregarAdyacencia(unionGrafo->vertices[i].adyacencias, actual->verticeAdyacente);
                    actual = actual->siguiente;
                }
            }

            return unionGrafo;
        }

        Grafo* ObtenerGrafoComplementario(const Grafo* grafo) {
            Grafo* complemento = grafo->dirigido ?
                CrearGrafoDirigido("Complemento", grafo->cantidadVertices) :
                CrearGrafoNoDirigido("Complemento", grafo->cantidadVertices);

            // Copiar etiquetas
            for (int i = 0; i < grafo->cantidadVertices; i++) {
                complemento->vertices[i].etiqueta = grafo->vertices[i].etiqueta;
            }

            // Agregar aristas complementarias
            for (int i = 0; i < grafo->cantidadVertices; i++) {
                for (int j = grafo->dirigido ? 0 : i + 1; j < grafo->cantidadVertices; j++) {
                    if (i != j && !SonAdyacentes(grafo, i, j)) {
                        agregarAdyacencia(complemento->vertices[i].adyacencias, j);
                        if (!complemento->dirigido) {
                            agregarAdyacencia(complemento->vertices[j].adyacencias, i);
                        }
                    }
                }
            }

            return complemento;
        }

        int ObtenerGrado(const Grafo* grafo, int vertice) {
            if (vertice < 0 || vertice >= grafo->cantidadVertices) {
                return 0;
            }

            int grado = 0;
            NodoAdyacencias* actual = grafo->vertices[vertice].adyacencias->primero;
            while (actual != nullptr) {
                grado++;
                actual = actual->siguiente;
            }

            // Para grafos no dirigidos, cada arista cuenta como grado
            if (!grafo->dirigido) {
                return grado;
            }
            // Para grafos dirigidos, debemos contar también las aristas entrantes si queremos el grado total
            else {
                int gradoEntrada = 0;
                for (int i = 0; i < grafo->cantidadVertices; i++) {
                    if (i != vertice && SonAdyacentes(grafo, i, vertice)) {
                        gradoEntrada++;
                    }
                }
                return grado; // Devolvemos solo el grado de salida según la especificación
            }
        }

        void AgregarEtiqueta(Grafo* grafo, int vertice, string etiqueta) {
            if (vertice >= 0 && vertice < grafo->cantidadVertices) {
                grafo->vertices[vertice].etiqueta = etiqueta;
            }
        }

        bool EsCompleto(const Grafo* grafo) {
            int gradoEsperado = grafo->cantidadVertices - 1;

            for (int i = 0; i < grafo->cantidadVertices; i++) {
                for (int j = 0; j < grafo->cantidadVertices; j++) {
                    if (i != j && !SonAdyacentes(grafo, i, j)) {
                        return false;
                    }
                }
            }
            return true;
        }

        string ObtenerSucesionGrafica(const Grafo* grafo) {
            std::stringstream ss;
            vector<int> grados;

            // Obtener todos los grados
            for (int i = 0; i < grafo->cantidadVertices; i++) {
                grados.push_back(ObtenerGrado(grafo, i));
            }

            // Ordenar los grados en orden descendente
            std::sort(grados.begin(), grados.end(), std::greater<int>());

            // Crear la sucesión gráfica
            for (size_t i = 0; i < grados.size(); i++) {
                if (i > 0) ss << ",";
                ss << grados[i];
            }

            return ss.str();
        }

        int ObtenerCantidadVertices(const Grafo* grafo) {
            return grafo->cantidadVertices;
        }

        void CambiarNombre(Grafo* grafo, string nombre) {
            grafo->nombre = nombre;
        }

        void DestruirGrafo(Grafo* grafo) {
            for (int i = 0; i < grafo->cantidadVertices; i++) {
                NodoAdyacencias* actual = grafo->vertices[i].adyacencias->primero;
                while (actual != nullptr) {
                    NodoAdyacencias* siguiente = actual->siguiente;
                    delete actual;
                    actual = siguiente;
                }
                delete grafo->vertices[i].adyacencias;
            }
            delete[] grafo->vertices;
            delete grafo;
        }

    }
#endif
