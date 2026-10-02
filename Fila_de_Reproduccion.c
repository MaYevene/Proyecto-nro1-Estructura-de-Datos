#include "Reproductor.h"

/* Aporte: Eban Delgado - cola, historial y selección del siguiente tema. */

void Agregar_Historial(Album *albumes, int album, int cancion)
{
    int indice;
    int valor = album * 1000 + cancion;

    if (album < 0 || album >= Cant_Al || cancion < 0 || cancion >= Cant_So)
    {
        return;
    }
    if (albumes->historial_cantidad < MAX_HISTORIAL)
    {
        albumes->historial_cantidad++;
    }

    for (indice = albumes->historial_cantidad - 1; indice > 0; indice--)
    {
        albumes->historial[indice] = albumes->historial[indice - 1];
    }
    albumes->historial[0] = valor;
}

int Agregar_Cola(Album *albumes, int album, int cancion)
{
    int indice;
    int valor;

    if (album < 0 || album >= Cant_Al || cancion < 0 || cancion >= Cant_So)
    {
        return -1;
    }

    valor = album * 1000 + cancion;
    for (indice = 0; indice < albumes->fila_cantidad; indice++)
    {
        if (albumes->fila[indice] == valor)
        {
            return 0;
        }
    }
    if (albumes->fila_cantidad >= MAX_COLA)
    {
        return -1;
    }

    for (indice = albumes->fila_cantidad; indice > 0; indice--)
    {
        albumes->fila[indice] = albumes->fila[indice - 1];
    }
    albumes->fila[0] = valor;
    albumes->fila_cantidad++;
    return 1;
}

void Quitar_Cola_Posicion(Album *albumes, int posicion)
{
    int indice;

    if (posicion < 0 || posicion >= albumes->fila_cantidad)
    {
        return;
    }
    for (indice = posicion; indice < albumes->fila_cantidad - 1; indice++)
    {
        albumes->fila[indice] = albumes->fila[indice + 1];
    }
    albumes->fila_cantidad--;
    albumes->fila[albumes->fila_cantidad] = -1;
}

int Quitar_Cola_ID(Album *albumes, int id)
{
    int indice;

    for (indice = 0; indice < albumes->fila_cantidad; indice++)
    {
        int valor = albumes->fila[indice];
        int album = valor / 1000;
        int cancion = valor % 1000;

        if (albumes->cancion[album][cancion].id == id)
        {
            Quitar_Cola_Posicion(albumes, indice);
            return 1;
        }
    }
    return 0;
}

void Vaciar_Cola(Album *albumes)
{
    int indice;

    for (indice = 0; indice < MAX_COLA; indice++)
    {
        albumes->fila[indice] = -1;
    }
    albumes->fila_cantidad = 0;
}

int Siguiente_Cola(Album *albumes, int *album, int *cancion)
{
    int valor;

    if (albumes->fila_cantidad == 0)
    {
        return 0;
    }

    valor = albumes->fila[0];
    *album = valor / 1000;
    *cancion = valor % 1000;
    Quitar_Cola_Posicion(albumes, 0);
    return 1;
}
