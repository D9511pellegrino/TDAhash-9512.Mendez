#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "lista.h"



#define ERROR -1
#define EXITO 0

typedef struct elemento_hash{
    char* clave;
    void* elemento; 
}elemento_t;

struct hash {
    hash_destruir_dato_t destructor;
    size_t cantidad_elementos;
    size_t cantidad_listas;
    size_t capacidad;
    lista_t** tabla;
};

/*
 * Crea el hash reservando la memoria necesaria para el.
 * Destruir_elemento es un destructor que se utilizará para liberar
 * los elementos que se eliminen del hash.  Capacidad indica la
 * capacidad inicial con la que se crea el hash. La capacidad inicial
 * no puede ser menor a 3. Si se solicita una capacidad menor, el hash
 * se creará con una capacidad de 3.
 *
 * Devuelve un puntero al hash creado o NULL en caso de no poder
 * crearlo.
 */
hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad_inicial){
    if(capacidad_inicial<3) capacidad_inicial = 3;

    hash_t* nuevo_hash = malloc(sizeof(hash_t));
    if(!nuevo_hash) return NULL;

    nuevo_hash->tabla = calloc(capacidad_inicial, sizeof(lista_t*));
    if(!nuevo_hash->tabla){
        free(nuevo_hash);
        return NULL;
    }

    nuevo_hash->destructor = destruir_elemento;
    nuevo_hash->cantidad_elementos = 0;
    nuevo_hash->cantidad_listas = 0;

    nuevo_hash->capacidad = capacidad_inicial;

    return nuevo_hash;

    //return NULL;
}


size_t hash_maker(const char* clave){
    if(clave){
        size_t numero_magico = 0;
        for(int i = 0; clave[i]!='\0'; i++){
            numero_magico += (size_t)clave[i] * i;
        }
        return numero_magico;
    }
    return 0;
}

/*
 * Inserta un elemento en el hash asociado a la clave dada.
 *
 * Nota para los alumnos: Recordar que si insertar un elemento provoca
 * que el factor de carga exceda cierto umbral, se debe ajustar el
 * tamaño de la tabla para evitar futuras colisiones.
 *
 * Devuelve 0 si pudo guardarlo o -1 si no pudo.
 */
int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!clave) return ERROR;

    elemento_t* elemento_nuevo_hash = malloc(sizeof(elemento_t));
    if(!elemento_nuevo_hash) return ERROR;

    elemento_nuevo_hash->elemento = elemento;

    elemento_nuevo_hash->clave = malloc(strlen(clave)+1);
    if(!elemento_nuevo_hash->clave){
        free(elemento_nuevo_hash);
        return ERROR;
    }

    elemento_nuevo_hash->clave = strcpy(elemento_nuevo_hash->clave,clave);

    size_t posicion = hash_maker(clave)%hash->capacidad;

    if(hash_contiene(hash, clave)){
        hash_quitar(hash, clave);
    }

    if(!hash->tabla[posicion]){
        hash->tabla[posicion] = lista_crear();
        hash->cantidad_listas++;
    }

    lista_insertar(hash->tabla[posicion], elemento_nuevo_hash);
    hash->cantidad_elementos++;

    /*LUGAR PARA REHASH*/

    return EXITO;
}

/*
 * Quita un elemento del hash e invoca la funcion destructora
 * pasandole dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 si no pudo.
 */
int hash_quitar(hash_t* hash, const char* clave){
    return 0;
}

/*
 * Devuelve un elemento del hash con la clave dada o NULL si dicho
 * elemento no existe (o en caso de error).
 */
void* hash_obtener(hash_t* hash, const char* clave){
    return NULL;
}

/*
 * Devuelve true si el hash contiene un elemento almacenado con la
 * clave dada o false en caso contrario (o en caso de error).
 */
bool hash_contiene(hash_t* hash, const char* clave){
    if(hash && clave){
        elemento_t* elemento_aux;
        lista_iterador_t* iterador = NULL;

        for(int i = 0; i<hash->capacidad; i++){
            if(!lista_vacia(hash->tabla[i])){
                iterador = lista_iterador_crear(hash->tabla[i]);
                while(lista_iterador_tiene_siguiente(iterador)){
                    elemento_aux = lista_iterador_elemento_actual(iterador);
                    if(strcmp(elemento_aux->clave, clave)==0){
                        lista_iterador_destruir(iterador);
                        return true;
                    } 
                    lista_iterador_avanzar(iterador);
                }
                lista_iterador_destruir(iterador);
            }
        }

    }
    return false;
}

/*
 * Devuelve la cantidad de elementos almacenados en el hash o 0 en
 * caso de error.
 */
size_t hash_cantidad(hash_t* hash){
    if(hash) return hash->cantidad_elementos;
    else return 0;
}

/*
 * Destruye el hash liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con cada elemento almacenado en el
 * hash.
 */
void hash_destruir(hash_t* hash){
    /*if(hash_cantidad(hash)!=0){
        for(int i = 0; i < hash->capacidad; i++){
            casillero_t* aux;
            casillero_t* actual = hash->tabla[i];
            while(actual->siguiente){
                aux = actual;
                actual = actual->siguiente;
                free(aux);
            }
            free(actual);
            free(hash->tabla[i]);
        }
    }
    else free(hash->tabla);*/
    elemento_t* dato_aux;
    for(int i = 0; i<hash->capacidad; i++){
        while(!lista_vacia(hash->tabla[i])){
            dato_aux = lista_quitar(hash->tabla[i]);
            free(dato_aux->clave);
            if(hash->destructor) hash->destructor(dato_aux->elemento);
            free(dato_aux);
        }
        lista_destruir(hash->tabla[i]);
    }
    free(hash->tabla);
    free(hash);
}


/*
 * Recorre cada una de las claves almacenadas en la tabla de hash e
 * invoca a la función funcion, pasandole como parámetros el hash, la
 * clave en cuestión y el puntero auxiliar.
 *
 * Mientras que queden mas claves o la funcion retorne false, la
 * iteración continúa. Cuando no quedan mas claves o la función
 * devuelve true, la iteración se corta y la función principal
 * retorna.
 *
 * Devuelve la cantidad de claves totales iteradas (la cantidad de
 * veces que fue invocada la función) o 0 en caso de error.
 *
 */
size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    return 0;
}

