#ifndef REPRODUCT_H
#define REPRODUCT_H

#define CANT_AL 15
#define CANT_SO 25


// Para el Allegro (Matias)
#define ANCHO_VENTANA 1280
#define ALTO_VENTANA 720

#define PANTALLA_ALBUMES 0
#define PANTALLA_CANCIONES 1
#define PANTALLA_REPRODUCTOR 2

#define CATEGORIA_PRINCIPAL 0
#define CATEGORIA_ID 1
#define CATEGORIA_ANIO 2
#define CATEGORIA_DURACION 3
#define CATEGORIA_REPRODUCCIONES 4
#define CATEGORIA_GENERO 5
#define CATEGORIA_ARTISTA 6
#define CATEGORIA_TITULO 7
#define TOTAL_CANCIONES (CANT_AL * CANT_SO)
#define CANCIONES_VISIBLES 20
// Fin Allegro

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

typedef struct {
    Songs cancion[CANT_AL][CANT_SO];
    int Eleccion;
} Album;


// Allegro (Matias)
typedef struct {
    Album *albumes;
    int pantalla;
    int album_seleccionado;
    int cancion_seleccionada;
    int hover_album;
    int hover_cancion;
    int categoria;
    int busqueda_global;
    int cantidad_canciones;
    int desplazamiento_lista;
    int album_fila[TOTAL_CANCIONES];
    int cancion_fila[TOTAL_CANCIONES];
    int mouse_x;
    int mouse_y;
} estado_ventana;
// Fin

void Generar(Album *albumes);
void Lista_albumes(Album *albumes);
void iniciar_interfaz(Album *albumes);

#endif
