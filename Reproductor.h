/**
 * @file Reproductor.h
 * @brief esto pa funciones
 */

#ifndef REPRODUCT_H
#define REPRODUCT_H

/**
 * @brief Estructura para canciones
 * 
 */

typedef struct{
    int id;
    int duracion_s;
    int anio;
    int n_reprodcciones;
    char titulo[100];
    char artista[100];
    char genero[50];
}Songs;

/**
 * @brief Estructura para Albumes
 * 
 */

typedef struct{
    char albums[100];
    int Al_cant[5];
    Songs cancion[25];
}Album;

void Lista_albumes(Album *albumes);

#endif 