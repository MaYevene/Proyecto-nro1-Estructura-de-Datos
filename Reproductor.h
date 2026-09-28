/**
 * @file Reproductor.h
 * @brief Funciones y estructuras para el reproductor
 */

#ifndef REPRODUCT_H
#define REPRODUCT_H
#define Cant_Al 5
#define Cant_So 5

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

/**
 * @brief Estructura para canciones
 */
typedef struct {
    int id;
    int duracion_s;
    int anio;
    int n_reprodcciones;
    char titulo[20];
    char artista[20];
    char genero[20];
    char albums[20];
} Songs;

/**
 * @brief Estructura para Albumes
 */
typedef struct {
    Songs cancion[Cant_Al][Cant_So];
} Album;

void Generar(Album *albumes);
void Lista_albumes(Album *albumes);

#endif
