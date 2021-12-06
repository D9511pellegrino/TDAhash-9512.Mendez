#include <string.h>

#include "pa2mm.h"
#include "src/hash.h"

typedef struct consola {
    char* modelo;
    char* descripcion;
} consola_t;


bool mostrar_consola(hash_t* hash, const char* clave, void* aux){
    if(!hash || !clave) return true;
    char* descripcion = hash_obtener(hash, clave);
    printf("Clave: %s = %s \n", clave, descripcion);
    return false;
}

void destruir_string(void* string){
    if(string)
        //char* string_destruir = string;
        free(string);
}

bool iterar_hasta_2(hash_t* hash,const char* clave, void* aux){
    if(!hash || !clave) return true;
    size_t* cantidad = aux;
    if(*cantidad==2) return true;
    return false;
}

void puedoCrearUnHash(){
    hash_t* h = NULL;

    pa2m_afirmar((h=hash_crear(NULL, 5)), "Crear un hash con tamaño 5 devuelve un hash");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    hash_destruir(h);

    pa2m_afirmar((h=hash_crear(NULL, 5)), "Crear un hash con tamaño 100 devuelve un hash");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    hash_destruir(h);

    pa2m_afirmar((h=hash_crear(NULL, 2)), "Crear un hash con tamaño 2 devuelve un hash");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    hash_destruir(h);
}

void puedoInsertarHash(){
    hash_t* h = NULL;
    h=hash_crear(NULL, 5);

    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave 1002 exitosamente");
    pa2m_afirmar(hash_cantidad(h)==1, "Hash tiene 1 elemento");
    pa2m_afirmar(strcmp(hash_obtener(h, "1002"), "Premium 10-Input 2-Bus Mixer")==0, "Busco y encuentro clave 1002");

    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer USB")==0, "Inserto clave 1002 repetida exitosamente");
    pa2m_afirmar(hash_cantidad(h)==1, "Hash tiene 1 elemento");
    pa2m_afirmar(strcmp(hash_obtener(h, "1002"), "Premium 10-Input 2-Bus Mixer USB")==0, "Busco y encuentro clave 1002");

    pa2m_afirmar(hash_insertar(h, "1204USB", "Premium 12-Input 2/2-Bus Mixer")==0, "Inserto clave 1204USB exitosamente");
    pa2m_afirmar(hash_cantidad(h)==2, "Hash tiene 2 elementos");

    pa2m_afirmar(hash_insertar(h, "302USB", "Premium 5-Input Mixer")==0, "Inserto clave 302USB exitosamente");
    pa2m_afirmar(hash_cantidad(h)==3, "Hash tiene 3 elementos");

    pa2m_afirmar(hash_insertar(h, "502", "Premium 5-Input 2-Bus Mixer")==0, "Inserto clave 502 exitosamente");
    pa2m_afirmar(hash_cantidad(h)==4, "Hash tiene 4 elementos");

    pa2m_afirmar(hash_insertar(h, "802", "Premium 8-Input 2-Bus Mixer")==0, "Inserto clave 802 exitosamente");
    pa2m_afirmar(hash_cantidad(h)==5, "Hash tiene 5 elementos");

    pa2m_afirmar(hash_insertar(h, "FLOW 8", "8-Input Digital Mixer")==0, "Inserto clave FLOW 8 exitosamente");
    pa2m_afirmar(hash_cantidad(h)==6, "Hash tiene 6 elementos");

    pa2m_afirmar(strcmp(hash_obtener(h, "502"), "Premium 5-Input 2-Bus Mixer")==0, "Busco y encuentro clave 502");

    pa2m_afirmar(strcmp(hash_obtener(h, "1204USB"), "Premium 12-Input 2/2-Bus Mixer")==0, "Busco y encuentro clave 1204USB");

    pa2m_afirmar(strcmp(hash_obtener(h, "502"), "Premium 5-Input 2-Bus Mixer")==0, "Busco y encuentro clave 502");

    pa2m_afirmar(strcmp(hash_obtener(h, "302USB"), "Premium 5-Input Mixer")==0, "Busco y encuentro clave 302USB");

    pa2m_afirmar(strcmp(hash_obtener(h, "FLOW 8"), "8-Input Digital Mixer")==0, "Busco y encuentro clave FLOW 8");

    pa2m_afirmar(strcmp(hash_obtener(h, "802"), "Premium 8-Input 2-Bus Mixer")==0, "Busco y encuentro clave 802");
    
    pa2m_afirmar(!hash_obtener(h, "2000"), "Busco clave 2000 inexistente y encuentro NULL");

    hash_destruir(h);
}


void puedoBorrarHAsh(){
    hash_t* h = NULL;
    h=hash_crear((hash_destruir_dato_t)destruir_string, 5);

    pa2m_afirmar(hash_quitar(h, "1002")==-1, "Intento quitar clave de hash vacio devuelve -1");

    char** consolas = malloc(4*sizeof(char*));
    if(!consolas){
        hash_destruir(h);
        return;
    }

    consolas[0] = malloc(strlen("Premium 10-Input 2-Bus Mixer")+1);
    if(!consolas[0]){
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[0], "Premium 10-Input 2-Bus Mixer");

    consolas[1] = malloc(strlen("Premium 12-Input 2/2-Bus Mixer")+1);
    if(!consolas[1]){
        free(consolas[0]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[1], "Premium 12-Input 2/2-Bus Mixer");

    consolas[2] = malloc(strlen("Premium 5-Input Mixer")+1);
    if(!consolas[2]){
        free(consolas[0]);
        free(consolas[1]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[2], "Premium 5-Input Mixer");

    consolas[3] = malloc(strlen("Premium 5-Input 2-Bus Mixer")+1);
    if(!consolas[3]){
        free(consolas[0]);
        free(consolas[1]);
        free(consolas[2]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[3], "Premium 5-Input 2-Bus Mixer");

    pa2m_afirmar(hash_insertar(h, "1002", consolas[0])==0, "Inserto clave 1002 exitosamente");
    pa2m_afirmar(hash_insertar(h, "1204USB", consolas[1])==0, "Inserto clave 1204USB exitosamente");

    pa2m_afirmar(hash_cantidad(h)==2, "Hash tiene 2 elementos");

    pa2m_afirmar(hash_quitar(h, "1002")==0, "Intento quitar clave 1002 de hash devuelve 0");
    pa2m_afirmar(hash_quitar(h, "1002")==-1, "Intento quitar clave 1002 nuevamente de hash devuelve -1");

    pa2m_afirmar(hash_cantidad(h)==1, "Hash tiene 1 elemento");

    pa2m_afirmar(hash_insertar(h, "302USB", consolas[2])==0, "Inserto clave 302USB exitosamente");
    pa2m_afirmar(hash_insertar(h, "502", consolas[3])==0, "Inserto clave 502 exitosamente");

    pa2m_afirmar(hash_cantidad(h)==3, "Hash tiene 3 elementos");

    pa2m_afirmar(!hash_obtener(h, "1002"), "Busco clave 1002 borrada y encuentro NULL");

    pa2m_afirmar(hash_quitar(h, "1204USB")==0, "Intento quitar clave 1204USB de hash devuelve 0");
    pa2m_afirmar(hash_quitar(h, "302USB")==0, "Intento quitar clave 302USB de hash devuelve 0");
    pa2m_afirmar(hash_quitar(h, "502")==0, "Intento quitar clave 502 de hash devuelve 0");

    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");
    
    free(consolas);
    hash_destruir(h);
}


void pruebasIteradorHash(){
    hash_t* h = NULL;
    h=hash_crear((hash_destruir_dato_t)destruir_string, 5);

    char** consolas = malloc(4*sizeof(char*));
    if(!consolas){
        hash_destruir(h);
        return;
    }

    consolas[0] = malloc(strlen("Premium 10-Input 2-Bus Mixer")+1);
    if(!consolas[0]){
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[0], "Premium 10-Input 2-Bus Mixer");

    consolas[1] = malloc(strlen("Premium 12-Input 2/2-Bus Mixer")+1);
    if(!consolas[1]){
        free(consolas[0]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[1], "Premium 12-Input 2/2-Bus Mixer");

    consolas[2] = malloc(strlen("Premium 5-Input Mixer")+1);
    if(!consolas[2]){
        free(consolas[0]);
        free(consolas[1]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[2], "Premium 5-Input Mixer");

    consolas[3] = malloc(strlen("Premium 5-Input 2-Bus Mixer")+1);
    if(!consolas[3]){
        free(consolas[0]);
        free(consolas[1]);
        free(consolas[2]);
        free(consolas);
        hash_destruir(h);
        return;
    }
    strcpy(consolas[3], "Premium 5-Input 2-Bus Mixer");

    pa2m_afirmar(hash_insertar(h, "1002", consolas[0])==0, "Inserto clave 1002 exitosamente");
    pa2m_afirmar(hash_insertar(h, "1204USB", consolas[1])==0, "Inserto clave 1204USB exitosamente");
    pa2m_afirmar(hash_insertar(h, "302USB", consolas[2])==0, "Inserto clave 302USB exitosamente");
    pa2m_afirmar(hash_insertar(h, "502", consolas[3])==0, "Inserto clave 502 exitosamente");
    
    pa2m_afirmar(hash_con_cada_clave(h, mostrar_consola, NULL)==4, "Se mostraron 4 consolas, puedo iterar con aux NULL");

    size_t cantidad_iterados = 0;
    pa2m_afirmar(hash_con_cada_clave(h, iterar_hasta_2, &cantidad_iterados)==4, "Se iteraron cantidad_iterados consolas");



    free(consolas);
    hash_destruir(h);

}

void pruebasNULLHash(){
    hash_t* h = NULL;

    pa2m_afirmar((h=hash_crear(NULL, 5)), "Crear un hash con destructor NULL devuelve un hash");
    pa2m_afirmar(hash_insertar(h, NULL, "NULL")==-1, "Insertar con clave NULL devuelve -1");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    pa2m_afirmar(hash_insertar(h, "802", "Premium 8-Input 2-Bus Mixer")==0, "Inserto clave 802 exitosamente");
    pa2m_afirmar(hash_cantidad(h)==1, "Hash tiene 1 elementos");

    pa2m_afirmar(hash_quitar(h, NULL)==-1, "Intento quitar clave NULL de hash devuelve -1");

    pa2m_afirmar(hash_insertar(h, "202", NULL)==0, "Inserto clave 202 con elemento NULL exitosamente");



    hash_destruir(h);
}

int main(){
    pa2m_nuevo_grupo("Pruebas de creacion Hash");
    puedoCrearUnHash();

    pa2m_nuevo_grupo("Pruebas de insercion y busqueda");
    puedoInsertarHash();

    pa2m_nuevo_grupo("Pruebas de borrado");
    puedoBorrarHAsh();

    pa2m_nuevo_grupo("Pruebas de iterador interno");
    pruebasIteradorHash();

    pa2m_nuevo_grupo("Pruebas con NULL");
    pruebasNULLHash();

    return pa2m_mostrar_reporte();
}