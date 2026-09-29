#include "Reproductor.h"

#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

int punto_en_rectangulo(int x, int y, int rx, int ry, int rw, int rh)
{
    if (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh)
    {
        return 1;
    }
    return 0;
}

void ordenar_canciones_album(Album *albumes, int album_index, int categoria)
{
    int i;
    int j;

    for (i = 0; i < CANT_SO - 1; i++)
    {
        for (j = 0; j < CANT_SO - i - 1; j++)
        {
            Songs *a = &albumes->cancion[album_index][j];
            Songs *b = &albumes->cancion[album_index][j + 1];
            int cambiar = 0;

            if (categoria == CATEGORIA_PRINCIPAL)
            {
                cambiar = 0;
            }
            else
            {
                if (categoria == CATEGORIA_ID)
                {
                    cambiar = a->id > b->id;
                }
                else if (categoria == CATEGORIA_ANIO)
                {
                    cambiar = a->anio > b->anio;
                }
                else if (categoria == CATEGORIA_DURACION)
                {
                    cambiar = a->duracion_s > b->duracion_s;
                }
                else if (categoria == CATEGORIA_REPRODUCCIONES)
                {
                    cambiar = a->n_reprodcciones > b->n_reprodcciones;
                }
                else if (categoria == CATEGORIA_GENERO)
                {
                    cambiar = strcmp(a->genero, b->genero) > 0;
                }
                else if (categoria == CATEGORIA_ARTISTA)
                {
                    cambiar = strcmp(a->artista, b->artista) > 0;
                }
                else if (categoria == CATEGORIA_TITULO)
                {
                    cambiar = strcmp(a->titulo, b->titulo) > 0;
                }
            }

            if (cambiar)
            {
                Songs aux = *a;
                *a = *b;
                *b = aux;
            }
        }
    }
}

int canciones_fuera_de_orden(estado_ventana *estado, int posicion_a, int posicion_b)
{
    Songs *cancion_a = &estado->albumes->cancion[estado->album_fila[posicion_a]][estado->cancion_fila[posicion_a]];
    Songs *cancion_b = &estado->albumes->cancion[estado->album_fila[posicion_b]][estado->cancion_fila[posicion_b]];

    if (estado->categoria == CATEGORIA_ID)
    {
        return cancion_a->id > cancion_b->id;
    }
    if (estado->categoria == CATEGORIA_ANIO)
    {
        return cancion_a->anio > cancion_b->anio;
    }
    if (estado->categoria == CATEGORIA_DURACION)
    {
        return cancion_a->duracion_s > cancion_b->duracion_s;
    }
    if (estado->categoria == CATEGORIA_REPRODUCCIONES)
    {
        return cancion_a->n_reprodcciones > cancion_b->n_reprodcciones;
    }
    if (estado->categoria == CATEGORIA_GENERO)
    {
        return strcmp(cancion_a->genero, cancion_b->genero) > 0;
    }
    if (estado->categoria == CATEGORIA_ARTISTA)
    {
        return strcmp(cancion_a->artista, cancion_b->artista) > 0;
    }
    if (estado->categoria == CATEGORIA_TITULO)
    {
        return strcmp(cancion_a->titulo, cancion_b->titulo) > 0;
    }
    return 0;
}

void preparar_busqueda(estado_ventana *estado)
{
    int album;
    int cancion;
    int posicion;
    int pasada;

    posicion = 0;
    for (album = 0; album < CANT_AL; album++)
    {
        for (cancion = 0; cancion < CANT_SO; cancion++)
        {
            estado->album_fila[posicion] = album;
            estado->cancion_fila[posicion] = cancion;
            posicion++;
        }
    }
    estado->cantidad_canciones = posicion;

    for (pasada = 0; pasada < estado->cantidad_canciones - 1; pasada++)
    {
        int indice;
        for (indice = 0; indice < estado->cantidad_canciones - pasada - 1; indice++)
        {
            if (canciones_fuera_de_orden(estado, indice, indice + 1))
            {
                int album_aux = estado->album_fila[indice];
                int cancion_aux = estado->cancion_fila[indice];
                estado->album_fila[indice] = estado->album_fila[indice + 1];
                estado->cancion_fila[indice] = estado->cancion_fila[indice + 1];
                estado->album_fila[indice + 1] = album_aux;
                estado->cancion_fila[indice + 1] = cancion_aux;
            }
        }
    }

    estado->desplazamiento_lista = 0;
}

void dibujar_barra_categorias(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    const char *etiquetas[8] = {"Principal", "ID", "Año", "Duracion", "Reprod", "Genero", "Artista", "Titulo"};
    int item_w = 120;
    int separacion = 10;
    int x = 60;
    int y = 24;
    int i;

    for (i = 0; i < 8; i++)
    {
        int rx = x + i * (item_w + separacion);
        int ry = y;
        int activo = 0;

        if (estado->categoria == i)
        {
            activo = 1;
        }

        if (activo)
        {
            al_draw_filled_rectangle(rx, ry, rx + item_w, ry + 28, al_map_rgb(52, 118, 255));
        }
        else
        {
            al_draw_filled_rectangle(rx, ry, rx + item_w, ry + 28, al_map_rgb(32, 35, 46));
        }

        al_draw_rectangle(rx, ry, rx + item_w, ry + 28, al_map_rgb(120, 120, 140), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), rx + item_w / 2, ry + 6, ALLEGRO_ALIGN_CENTER, etiquetas[i]);
    }
}

void actualizar_hover(estado_ventana *estado)
{
    int i;
    int x;
    int y;

    estado->hover_album = -1;
    estado->hover_cancion = -1;

    if (estado->pantalla == PANTALLA_ALBUMES)
    {
        x = 60;
        for (i = 0; i < CANT_AL; i++)
        {
            y = 120 + i * 42;
            if (punto_en_rectangulo(estado->mouse_x, estado->mouse_y, x, y, ANCHO_VENTANA - 140, 34))
            {
                estado->hover_album = i;
                break;
            }
        }
    }
    else if (estado->pantalla == PANTALLA_CANCIONES)
    {
        x = 60;
        for (i = 0; i < CANCIONES_VISIBLES; i++)
        {
            y = 120 + i * 28;
            if (punto_en_rectangulo(estado->mouse_x, estado->mouse_y, x, y, ANCHO_VENTANA - 140, 22))
            {
                if (estado->busqueda_global == 1)
                {
                    estado->hover_cancion = i + estado->desplazamiento_lista;
                }
                else
                {
                    estado->hover_cancion = i;
                }
                break;
            }
        }
    }
}

void dibujar_albumes(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    int i;
    int x;
    int y;
    int w;
    int h;
    char nombre[64];

    al_clear_to_color(al_map_rgb(12, 12, 18));
    dibujar_barra_categorias(estado, fuente);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 70, ALLEGRO_ALIGN_CENTER, "Biblioteca");
    al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT, "Selecciona un album");

    for (i = 0; i < CANT_AL; i++)
    {
        x = 60;
        y = 120 + i * 42;
        w = ANCHO_VENTANA - 140;
        h = 34;

        if (estado->album_seleccionado == i)
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(36, 104, 255));
        }
        else if (estado->hover_album == i)
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(46, 72, 120));
        }
        else
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(30, 30, 38));
        }

        al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(80, 80, 100), 1);
        snprintf(nombre, sizeof(nombre), "%d. %s", i + 1, estado->albumes->cancion[i][0].albums);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), x + 16, y + 8, ALLEGRO_ALIGN_LEFT, nombre);
    }
}

void dibujar_canciones(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    int i;
    int x;
    int y;
    int w;
    int h;
    char titulo[120];
    char linea[220];
    char valor[120];
    const char *etiquetas[8] = {"Principal", "ID", "Anio", "Duracion", "Reproducciones", "Genero", "Artista", "Titulo"};

    al_clear_to_color(al_map_rgb(16, 14, 24));
    dibujar_barra_categorias(estado, fuente);

    if (estado->busqueda_global == 1)
    {
        snprintf(titulo, sizeof(titulo), "Canciones ordenadas por: %s", etiquetas[estado->categoria]);
    }
    else
    {
        snprintf(titulo, sizeof(titulo), "Album: %s", estado->albumes->cancion[estado->album_seleccionado][0].albums);
    }

    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 70, ALLEGRO_ALIGN_CENTER, titulo);
    al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT, "Selecciona una cancion");

    for (i = 0; i < CANCIONES_VISIBLES; i++)
    {
        int indice_lista = i;
        Songs *cancion;

        if (estado->busqueda_global == 1)
        {
            indice_lista = i + estado->desplazamiento_lista;
            if (indice_lista >= estado->cantidad_canciones)
            {
                break;
            }
            cancion = &estado->albumes->cancion[estado->album_fila[indice_lista]][estado->cancion_fila[indice_lista]];
        }
        else
        {
            if (i >= CANT_SO)
            {
                break;
            }
            cancion = &estado->albumes->cancion[estado->album_seleccionado][i];
        }

        x = 60;
        y = 120 + i * 28;
        w = ANCHO_VENTANA - 140;
        h = 22;

        if (estado->hover_cancion == indice_lista)
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(84, 64, 110));
        }
        else if (estado->busqueda_global == 0 && estado->cancion_seleccionada == i)
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(122, 45, 181));
        }
        else
        {
            al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(27, 27, 34));
        }

        al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(100, 100, 110), 1);

        if (estado->busqueda_global == 1)
        {
            if (estado->categoria == CATEGORIA_ID)
            {
                snprintf(valor, sizeof(valor), "ID: %d", cancion->id);
            }
            else if (estado->categoria == CATEGORIA_ANIO)
            {
                snprintf(valor, sizeof(valor), "Anio: %d", cancion->anio);
            }
            else if (estado->categoria == CATEGORIA_DURACION)
            {
                snprintf(valor, sizeof(valor), "Duracion: %d s", cancion->duracion_s);
            }
            else if (estado->categoria == CATEGORIA_REPRODUCCIONES)
            {
                snprintf(valor, sizeof(valor), "Reproducciones: %d", cancion->n_reprodcciones);
            }
            else if (estado->categoria == CATEGORIA_GENERO)
            {
                snprintf(valor, sizeof(valor), "Genero: %s", cancion->genero);
            }
            else if (estado->categoria == CATEGORIA_ARTISTA)
            {
                snprintf(valor, sizeof(valor), "Artista: %s", cancion->artista);
            }
            else
            {
                snprintf(valor, sizeof(valor), "Titulo: %s", cancion->titulo);
            }
            snprintf(linea, sizeof(linea), "%s | %s - %s | Album: %s", valor, cancion->titulo, cancion->artista, cancion->albums);
        }
        else
        {
            snprintf(linea, sizeof(linea), "%d. %s", i + 1, cancion->titulo);
        }

        al_draw_text(fuente, al_map_rgb(255, 255, 255), x + 10, y + 3, ALLEGRO_ALIGN_LEFT, linea);
    }
}

void dibujar_reproductor(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    Songs *cancion = &estado->albumes->cancion[estado->album_seleccionado][estado->cancion_seleccionada];
    char linea1[120];
    char linea2[120];
    char linea3[120];
    char linea4[120];
    char linea5[120];
    char linea6[120];
    char linea7[120];
    int y;

    al_clear_to_color(al_map_rgb(9, 20, 30));
    dibujar_barra_categorias(estado, fuente);
    al_draw_filled_rectangle(70, 100, ANCHO_VENTANA - 70, ALTO_VENTANA - 90, al_map_rgb(28, 34, 44));
    al_draw_filled_rectangle(70, 520, ANCHO_VENTANA - 70, 620, al_map_rgb(31, 48, 59));

    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 130, ALLEGRO_ALIGN_CENTER, "Reproductor");
    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 180, ALLEGRO_ALIGN_CENTER, cancion->titulo);
    al_draw_text(fuente, al_map_rgb(180, 180, 200), ANCHO_VENTANA / 2, 225, ALLEGRO_ALIGN_CENTER, cancion->artista);

    snprintf(linea1, sizeof(linea1), "ID: %d", cancion->id);
    snprintf(linea2, sizeof(linea2), "Titulo: %s", cancion->titulo);
    snprintf(linea3, sizeof(linea3), "Artista: %s", cancion->artista);
    snprintf(linea4, sizeof(linea4), "Genero: %s", cancion->genero);
    snprintf(linea5, sizeof(linea5), "Duracion: %d segundos", cancion->duracion_s);
    snprintf(linea6, sizeof(linea6), "Anio: %d", cancion->anio);
    snprintf(linea7, sizeof(linea7), "Reproducciones: %d", cancion->n_reprodcciones);

    y = 300;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea1); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea2); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea3); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea4); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea5); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea6); y += 35;
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 120, y, ALLEGRO_ALIGN_LEFT, linea7); y += 35;

    al_draw_text(fuente, al_map_rgb(120, 220, 150), ANCHO_VENTANA / 2, 550, ALLEGRO_ALIGN_CENTER, "Reproduciendo");
    al_draw_filled_rectangle(350, 580, 930, 600, al_map_rgb(38, 164, 92));
}

int clic_categorias(estado_ventana *estado, int mouse_x, int mouse_y)
{
    int item_w = 120;
    int separacion = 10;
    int x = 60;
    int y = 24;
    int i;

    for (i = 0; i < 8; i++)
    {
        int rx = x + i * (item_w + separacion);

        if (punto_en_rectangulo(mouse_x, mouse_y, rx, y, item_w, 28))
        {
            estado->categoria = i;

            if (estado->categoria == CATEGORIA_PRINCIPAL)
            {
                estado->pantalla = PANTALLA_ALBUMES;
                estado->busqueda_global = 0;
            }
            else
            {
                estado->pantalla = PANTALLA_CANCIONES;
                estado->cancion_seleccionada = 0;
                estado->busqueda_global = 1;
                preparar_busqueda(estado);
            }

            return 1;
        }
    }

    return 0;
}

void manejar_click(estado_ventana *estado, int mouse_x, int mouse_y)
{
    int i;
    int x;
    int y;

    if (clic_categorias(estado, mouse_x, mouse_y))
    {
        return;
    }

    if (estado->pantalla == PANTALLA_ALBUMES)
    {
        x = 60;
        for (i = 0; i < CANT_AL; i++)
        {
            y = 120 + i * 42;
            if (punto_en_rectangulo(mouse_x, mouse_y, x, y, ANCHO_VENTANA - 140, 34))
            {
                estado->album_seleccionado = i;
                estado->cancion_seleccionada = 0;
                estado->pantalla = PANTALLA_CANCIONES;
                estado->busqueda_global = 0;
                ordenar_canciones_album(estado->albumes, estado->album_seleccionado, estado->categoria);
                return;
            }
        }
    }

    if (estado->pantalla == PANTALLA_CANCIONES)
    {
        x = 60;
        for (i = 0; i < CANCIONES_VISIBLES; i++)
        {
            y = 120 + i * 28;
            if (punto_en_rectangulo(mouse_x, mouse_y, x, y, ANCHO_VENTANA - 140, 22))
            {
                int indice_lista = i + estado->desplazamiento_lista;

                if (estado->busqueda_global == 1)
                {
                    if (indice_lista >= estado->cantidad_canciones)
                    {
                        return;
                    }
                    estado->album_seleccionado = estado->album_fila[indice_lista];
                    estado->cancion_seleccionada = estado->cancion_fila[indice_lista];
                }
                else
                {
                    estado->cancion_seleccionada = i;
                }
                estado->pantalla = PANTALLA_REPRODUCTOR;
                return;
            }
        }
    }
}

void iniciar_interfaz(Album *albumes)
{
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *cola_eventos;
    ALLEGRO_FONT *fuente;
    ALLEGRO_TIMER *temporizador;
    estado_ventana estado;
    int redraw = 1;
    int ejecutando = 1;

    if (!al_init())
    {
        fprintf(stderr, "No se pudo inicializar Allegro.\n");
        return;
    }

    if (!al_install_mouse())
    {
        fprintf(stderr, "No se pudo inicializar el mouse.\n");
        return;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "No se pudo inicializar primitives.\n");
        return;
    }

    if (!al_init_font_addon() || !al_init_ttf_addon())
    {
        fprintf(stderr, "No se pudo inicializar fuentes.\n");
        return;
    }

    display = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);
    if (!display)
    {
        fprintf(stderr, "No se pudo crear la ventana.\n");
        return;
    }

    cola_eventos = al_create_event_queue();
    if (!cola_eventos)
    {
        fprintf(stderr, "No se pudo crear la cola de eventos.\n");
        al_destroy_display(display);
        return;
    }

    fuente = al_create_builtin_font();
    if (!fuente)
    {
        fprintf(stderr, "No se pudo cargar la fuente.\n");
        al_destroy_event_queue(cola_eventos);
        al_destroy_display(display);
        return;
    }

    temporizador = al_create_timer(1.0 / 60.0);
    if (!temporizador)
    {
        fprintf(stderr, "No se pudo crear el timer.\n");
        al_destroy_font(fuente);
        al_destroy_event_queue(cola_eventos);
        al_destroy_display(display);
        return;
    }

    al_register_event_source(cola_eventos, al_get_display_event_source(display));
    al_register_event_source(cola_eventos, al_get_mouse_event_source());
    al_register_event_source(cola_eventos, al_get_timer_event_source(temporizador));

    estado.albumes = albumes;
    estado.pantalla = PANTALLA_ALBUMES;
    estado.album_seleccionado = 0;
    estado.cancion_seleccionada = 0;
    estado.hover_album = -1;
    estado.hover_cancion = -1;
    estado.categoria = CATEGORIA_PRINCIPAL;
    estado.busqueda_global = 0;
    estado.cantidad_canciones = 0;
    estado.desplazamiento_lista = 0;
    estado.mouse_x = 0;
    estado.mouse_y = 0;

    al_start_timer(temporizador);

    while (ejecutando)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(cola_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            ejecutando = 0;
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            estado.mouse_x = evento.mouse.x;
            estado.mouse_y = evento.mouse.y;

            if (estado.busqueda_global == 1 && estado.pantalla == PANTALLA_CANCIONES && evento.mouse.dz != 0)
            {
                estado.desplazamiento_lista = estado.desplazamiento_lista - evento.mouse.dz;
                if (estado.desplazamiento_lista < 0)
                {
                    estado.desplazamiento_lista = 0;
                }
                if (estado.desplazamiento_lista > estado.cantidad_canciones - CANCIONES_VISIBLES)
                {
                    estado.desplazamiento_lista = estado.cantidad_canciones - CANCIONES_VISIBLES;
                }
            }

            actualizar_hover(&estado);
            redraw = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            manejar_click(&estado, evento.mouse.x, evento.mouse.y);
            redraw = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = 1;
        }

        if (redraw == 1 && al_is_event_queue_empty(cola_eventos))
        {
            if (estado.pantalla == PANTALLA_ALBUMES)
            {
                dibujar_albumes(&estado, fuente);
            }
            else if (estado.pantalla == PANTALLA_CANCIONES)
            {
                dibujar_canciones(&estado, fuente);
            }
            else
            {
                dibujar_reproductor(&estado, fuente);
            }

            al_flip_display();
            redraw = 0;
        }
    }

    al_destroy_timer(temporizador);
    al_destroy_font(fuente);
    al_destroy_event_queue(cola_eventos);
    al_destroy_display(display);

    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_shutdown_primitives_addon();
    al_uninstall_mouse();
    al_uninstall_system();
}

