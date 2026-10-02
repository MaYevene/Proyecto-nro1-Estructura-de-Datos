#include "Reproductor.h"

/* Aporte: Carlos Cofré - ordenamientos y búsqueda binaria recursiva. */

static int comparar_canciones(Album *albumes, ReferenciaCancion a, ReferenciaCancion b, int categoria, int descendente)
{
    Songs *cancion_a = &albumes->cancion[a.album][a.cancion];
    Songs *cancion_b = &albumes->cancion[b.album][b.cancion];
    int resultado = 0;

    if (categoria == CATEGORIA_ID)
    {
        resultado = (cancion_a->id > cancion_b->id) - (cancion_a->id < cancion_b->id);
    }
    else if (categoria == CATEGORIA_ANIO)
    {
        resultado = (cancion_a->anio > cancion_b->anio) - (cancion_a->anio < cancion_b->anio);
    }
    else if (categoria == CATEGORIA_DURACION)
    {
        resultado = (cancion_a->duracion_s > cancion_b->duracion_s) - (cancion_a->duracion_s < cancion_b->duracion_s);
    }
    else if (categoria == CATEGORIA_REPRODUCCIONES)
    {
        resultado = (cancion_a->n_reprodcciones > cancion_b->n_reprodcciones) -
                    (cancion_a->n_reprodcciones < cancion_b->n_reprodcciones);
    }
    else if (categoria == CATEGORIA_GENERO)
    {
        resultado = strcmp(cancion_a->genero, cancion_b->genero);
    }
    else if (categoria == CATEGORIA_ARTISTA)
    {
        resultado = strcmp(cancion_a->artista, cancion_b->artista);
    }
    else if (categoria == CATEGORIA_TITULO)
    {
        resultado = strcmp(cancion_a->titulo, cancion_b->titulo);
    }
    else if (categoria == CATEGORIA_ALBUM)
    {
        resultado = strcmp(cancion_a->albums, cancion_b->albums);
    }

    if (descendente)
    {
        resultado = -resultado;
    }

    return resultado;
}

static void intercambiar_referencias(ReferenciaCancion *a, ReferenciaCancion *b)
{
    ReferenciaCancion temporal = *a;
    *a = *b;
    *b = temporal;
}

void ordenar_referencias_burbuja(Album *albumes, ReferenciaCancion canciones[], int cantidad, int categoria)
{
    int pasada;
    int indice;

    for (pasada = 0; pasada < cantidad - 1; pasada++)
    {
        for (indice = 0; indice < cantidad - pasada - 1; indice++)
        {
            if (comparar_canciones(albumes, canciones[indice], canciones[indice + 1], categoria, 0) > 0)
            {
                intercambiar_referencias(&canciones[indice], &canciones[indice + 1]);
            }
        }
    }
}

static int particionar_referencias(Album *albumes, ReferenciaCancion canciones[], int inicio, int fin,
                                   int categoria, int descendente)
{
    ReferenciaCancion pivote = canciones[fin];
    int limite = inicio - 1;
    int indice;

    for (indice = inicio; indice < fin; indice++)
    {
        if (comparar_canciones(albumes, canciones[indice], pivote, categoria, descendente) <= 0)
        {
            limite++;
            intercambiar_referencias(&canciones[limite], &canciones[indice]);
        }
    }

    intercambiar_referencias(&canciones[limite + 1], &canciones[fin]);
    return limite + 1;
}

void ordenar_referencias_recursivo(Album *albumes, ReferenciaCancion canciones[], int inicio, int fin,
                                   int categoria, int descendente)
{
    int pivote;

    if (inicio < fin)
    {
        pivote = particionar_referencias(albumes, canciones, inicio, fin, categoria, descendente);
        ordenar_referencias_recursivo(albumes, canciones, inicio, pivote - 1, categoria, descendente);
        ordenar_referencias_recursivo(albumes, canciones, pivote + 1, fin, categoria, descendente);
    }
}

int buscar_id_binaria_recursiva(Album *albumes, ReferenciaCancion canciones[], int inicio, int fin, int id)
{
    int medio;
    int id_medio;

    if (inicio > fin)
    {
        return -1;
    }

    medio = inicio + (fin - inicio) / 2;
    id_medio = albumes->cancion[canciones[medio].album][canciones[medio].cancion].id;

    if (id_medio == id)
    {
        return medio;
    }
    if (id < id_medio)
    {
        return buscar_id_binaria_recursiva(albumes, canciones, inicio, medio - 1, id);
    }

    return buscar_id_binaria_recursiva(albumes, canciones, medio + 1, fin, id);
}

void crear_indice_por_id(Album *albumes, ReferenciaCancion canciones[])
{
    int album;
    int cancion;
    int posicion = 0;

    for (album = 0; album < Cant_Al; album++)
    {
        for (cancion = 0; cancion < Cant_So; cancion++)
        {
            canciones[posicion].album = album;
            canciones[posicion].cancion = cancion;
            posicion++;
        }
    }

    ordenar_referencias_recursivo(albumes, canciones, 0, posicion - 1, CATEGORIA_ID, 0);
}
