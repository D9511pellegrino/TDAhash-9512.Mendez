#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "lista.h"

#define ERROR -1
#define EXITO 0
#define FACTOR_CARGA 0.75

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
}


size_t hash_maker(const char* clave){
    if(clave){
        size_t numero_magico = 0;
        for(int i = 0; clave[i]!='\0'; i++){
            numero_magico += (size_t)clave[i] * (i+1);
        }
        return numero_magico;
    }
    return 0;
}

size_t rehash(hash_t* hash){

    if(hash){
        hash_t* hash_aux = hash_crear(hash->destructor, hash->capacidad * 2 +1); 
        if(!hash_aux) return ERROR;

        elemento_t* elemento_aux;
        lista_iterador_t* iterador = NULL;

        for(int i = 0; i<hash_aux->capacidad; i++){
            if(!lista_vacia(hash_aux->tabla[i])){
                iterador = lista_iterador_crear(hash_aux->tabla[i]);
                
                while(lista_iterador_tiene_siguiente(iterador)){
                    elemento_aux = lista_iterador_elemento_actual(iterador);
                    if(hash_insertar(hash_aux, elemento_aux->clave, elemento_aux->elemento)!=0){
                        hash_destruir(hash_aux);
                        lista_iterador_destruir(iterador);
                        return ERROR;
                    }
                    lista_iterador_avanzar(iterador);  
                }

                lista_iterador_destruir(iterador);
            }
        }

        hash_t hash_pointer = *hash; 
        *hash = *hash_aux;
        *hash_aux = hash_pointer;
        hash_destruir(hash_aux);
        return EXITO;
    }
    return ERROR;
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
    if(!clave || !hash) return ERROR;

    if((hash->cantidad_listas/hash->capacidad)>FACTOR_CARGA) rehash(hash);

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

    if((hash->cantidad_listas/hash->capacidad)>FACTOR_CARGA) rehash(hash);

    return EXITO;
}


int hash_quitar(hash_t* hash, const char* clave){
    if(hash && clave){
        elemento_t* elemento_aux;
        lista_iterador_t* iterador = NULL;
        size_t posicion;
        for(int i = 0; i<hash->capacidad; i++){
            if(!lista_vacia(hash->tabla[i])){
                posicion = 0;
                iterador = lista_iterador_crear(hash->tabla[i]);
                while(lista_iterador_tiene_siguiente(iterador)){
                    elemento_aux = lista_iterador_elemento_actual(iterador);
                    if(strcmp(elemento_aux->clave, clave)==0){
                        elemento_aux = lista_quitar_de_posicion(hash->tabla[i], posicion);
                        free(elemento_aux->clave);
                        if(hash->destructor) hash->destructor(elemento_aux->elemento);
                        free(elemento_aux);
                        lista_iterador_destruir(iterador);
                        hash->cantidad_elementos--;
                        return EXITO;
                    } 
                    lista_iterador_avanzar(iterador);
                    posicion++;
                }
                lista_iterador_destruir(iterador);
            }
        }
    }
    return ERROR;
}


void* hash_obtener(hash_t* hash, const char* clave){
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
                        return elemento_aux->elemento;
                    } 
                    lista_iterador_avanzar(iterador);
                }
                lista_iterador_destruir(iterador);
            }
        }
    }
    return NULL;
}


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

size_t hash_cantidad(hash_t* hash){
    if(hash) return hash->cantidad_elementos;
    else return 0;
}

void hash_destruir(hash_t* hash){
    if(hash){
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
}


size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
    size_t cantidad_iterados = 0;
    if(hash){
        elemento_t* elemento_aux;
        lista_iterador_t* iterador = NULL;

        for(int i = 0; i<hash->capacidad; i++){
            if(!lista_vacia(hash->tabla[i])){
                iterador = lista_iterador_crear(hash->tabla[i]);
                
                while(lista_iterador_tiene_siguiente(iterador)){
                    elemento_aux = lista_iterador_elemento_actual(iterador);

                    if(funcion(hash, elemento_aux->clave, aux)){
                        lista_iterador_destruir(iterador);
                        return cantidad_iterados+1;
                    }
                    lista_iterador_avanzar(iterador);
                    cantidad_iterados++;  
                }
                lista_iterador_destruir(iterador);
            }
        }

    }
    return cantidad_iterados;
}

