#include "GeneradorIdentificador.h"
#include <cstdlib>
#include <ctime>

namespace URGGeneradorIdentificador {
    // Variable global para verificar si la semilla ha sido inicializada
    static bool estaInicializado = false;

    // Función para inicializar la semilla
    void InicializarSemilla() {
        if (!estaInicializado) {
            srand(static_cast<unsigned int>(time(0))); // Inicializa la semilla con el tiempo actual
            estaInicializado = true; // Marca que la semilla ha sido inicializada
        }
    }

    /*
    Precondicion: -
    Postcondicion: Devuelve un string unico que tiene el siguiente formato
    ########-####-####-########
    Donde # puede ser
    cualquier caracter del alfabeto español en minuscula
    cualquier numero del 0 al 9
    donde cada valor del identificador debe tener una probabilidad uniforme de aparecer.*/
    string GenerarIdentificadorUnico() {
        InicializarSemilla(); // Llama a la función para inicializar la semilla

        const char caracteres[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        int longitud = sizeof(caracteres) - 1;
        std::string id_formato = "########-####-####-########";
        std::string identificador;

        for (unsigned int i = 0; i < id_formato.length(); i++) {
            if (id_formato[i] == '#') {
                identificador += caracteres[rand() % longitud];
            }
            else {
                identificador += id_formato[i];
            }
        }

        return identificador;
    }
}