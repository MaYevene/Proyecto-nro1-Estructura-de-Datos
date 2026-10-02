/**
 * @file Reproductor.h
 * @brief Funciones y estructuras para el reproductor
 */

#ifndef REPRODUCT_H
#define REPRODUCT_H

/* Aporte: Eban Delgado - estructuras y datos compartidos del reproductor. */

#define Cant_Al 15
#define Cant_So 25
#define Cola 10
#define MAX_HISTORIAL 10
#define MAX_COLA 10
#define ANCHO_VENTANA 1100
#define ALTO_VENTANA 700
#define CANCIONES_VISIBLES 12
#define TOTAL_CANCIONES (Cant_Al * Cant_So)

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <ctype.h>

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
    int historial[Cola];
    int historial_cantidad;
    int fila[Cola];
    int fila_cantidad;
} Album;

typedef struct {
    int album;
    int cancion;
} ReferenciaCancion;

enum {
    PANTALLA_ALBUMES = 0,
    PANTALLA_CANCIONES = 1,
    PANTALLA_REPRODUCTOR = 2,
    PANTALLA_BUSQUEDA = 3,
    PANTALLA_ESTADISTICAS = 4,
    PANTALLA_COLA = 5,
    PANTALLA_HISTORIAL = 6
};

enum {
    CATEGORIA_PRINCIPAL = 0,
    CATEGORIA_ID = 1,
    CATEGORIA_ANIO = 2,
    CATEGORIA_DURACION = 3,
    CATEGORIA_REPRODUCCIONES = 4,
    CATEGORIA_GENERO = 5,
    CATEGORIA_ARTISTA = 6,
    CATEGORIA_TITULO = 7,
    CATEGORIA_ALBUM = 8
};

void Generar(Album *albumes);
bool Cargar_CSV(Album *albumes, const char *nombre_archivo);
void Guardar_CSV(Album *albumes, const char *nombre_archivo);
void Agregar_Historial(Album *albumes, int album, int cancion);
int Agregar_Cola(Album *albumes, int album, int cancion);
void Quitar_Cola_Posicion(Album *albumes, int posicion);
int Quitar_Cola_ID(Album *albumes, int id);
void Vaciar_Cola(Album *albumes);
int Siguiente_Cola(Album *albumes, int *album, int *cancion);
void ordenar_referencias_burbuja(Album *albumes, ReferenciaCancion canciones[], int cantidad, int categoria);
void ordenar_referencias_recursivo(Album *albumes, ReferenciaCancion canciones[], int inicio, int fin, int categoria, int descendente);
int buscar_id_binaria_recursiva(Album *albumes, ReferenciaCancion canciones[], int inicio, int fin, int id);
void crear_indice_por_id(Album *albumes, ReferenciaCancion canciones[]);

#endif
