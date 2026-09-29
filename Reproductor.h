/**
 * @file Reproductor.h
 * @brief Funciones y estructuras para el reproductor
 */

#ifndef REPRODUCT_H
#define REPRODUCT_H
#define Cant_Al 15
#define Cant_So 25

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
    char titulo[11];
    char artista[11];
    char genero[11];
    char albums[11];
} Songs;

/**
 * @brief Estructura para Albumes
 */
typedef struct {
    Songs cancion[Cant_Al][Cant_So];
    int Eleccion;
} Album;

void Generar(Album *albumes);
void Lista_albumes(Album *albumes);

#endif
