#include "pa2mm.h"
#include "src/hash.h"

void puedoCrearUnHash(){
    hash_t* h = NULL;

    pa2m_afirmar((h=hash_crear(NULL, 5)), "Crear un hash con tamaño valido devuelve un hash");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    hash_destruir(h);

    pa2m_afirmar((h=hash_crear(NULL, 2)), "Crear un hash con tamaño invalido devuelve un hash");
    pa2m_afirmar(hash_cantidad(h)==0, "Hash tiene 0 elementos");

    //pa2m_afirmar(!abb_buscar(a, NULL), "El elemento raiz es NULL");

    hash_destruir(h);
}

void puedoInsertarHash(){
    hash_t* h = NULL;
    h=hash_crear(NULL, 5);

    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
    pa2m_afirmar(hash_insertar(h, "1002", "Premium 10-Input 2-Bus Mixer")==0, "Inserto clave exitosamente");
        



    hash_destruir(h);
}


int main(){
    pa2m_nuevo_grupo("Pruebas de creacion Hash");
    puedoCrearUnHash();

    pa2m_nuevo_grupo("Pruebas de insercion");
    puedoInsertarHash();

    pa2m_nuevo_grupo("Pruebas de busqueda");

    pa2m_nuevo_grupo("Pruebas de borrado");

    pa2m_nuevo_grupo("Pruebas de recorrido");

    return pa2m_mostrar_reporte();
}