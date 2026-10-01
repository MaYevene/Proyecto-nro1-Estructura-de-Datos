/**
 * @file Reproductor.h
 * @brief Funciones y estructuras para el reproductor
 */

#ifndef REPRODUCT_H
#define REPRODUCT_H
#define Cant_Al 15
#define Cant_So 25
#define Cola 10

#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<time.h> 
#include<string.h>

/**
 * @brief Estructura para definir si una cancion fue o no reproducida y cuantas veces
 */
typedef struct {
    bool reproducido;
    int Cant_Re;
    int orden;
} reproc;

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
    reproc past;
} Songs;

/**
 * @brief Estructura para Albumes
 */
typedef struct {
    Songs cancion[Cant_Al][Cant_So];
    int Eleccion;
    int elec_al;
    bool ciclo;
    int cont;
} Album;

void Generar(Album *albumes);
void Lista_albumes(Album *albumes);
void Fila_de_Reproduccion(Album *albumes);

#endif
