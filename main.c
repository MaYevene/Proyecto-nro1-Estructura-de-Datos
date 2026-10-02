#include "Reproductor.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <ctype.h>

#define MAX_CATEGORIAS 9
#define CATEGORY_X 12
#define CATEGORY_W 72
#define CATEGORY_GAP 2
#define TOOLBAR_X 690
#define TOOLBAR_W 95
#define TOOLBAR_GAP 8

enum {
    BUSQUEDA_ID = 0,
    BUSQUEDA_TITULO = 1,
    BUSQUEDA_ARTISTA = 2,
    BUSQUEDA_GENERO = 3
};

enum {
    ESTADISTICA_ARTISTAS = 0,
    ESTADISTICA_GENERO = 1,
    ESTADISTICA_TOP = 2,
    ESTADISTICA_ARTISTA = 3,
    ESTADISTICA_GENERO_MAS_ESCUCHADO = 4
};

typedef struct {
    Album *albumes;
    int pantalla;
    int album_seleccionado;
    int cancion_seleccionada;
    int categoria;
    int busqueda_global;
    int hover_album;
    int hover_cancion;
    int cantidad_canciones;
    int desplazamiento_lista;
    int album_fila[Cant_Al * Cant_So];
    int cancion_fila[Cant_Al * Cant_So];
    char entrada[64];
    char mensaje[120];
    int largo_entrada;
    int modo_busqueda;
    int modo_estadistica;
    int entrada_cola_activa;
    int mouse_x;
    int mouse_y;
    bool reproduciendo;
    double tiempo_actual;
} estado_ventana;

static int punto_en_rectangulo(int x, int y, int rx, int ry, int rw, int rh)
{
    return x >= rx && x <= rx + rw && y >= ry && y <= ry + rh;
}

/* Aporte: Eban Delgado - inicialización de estado y registro de reproducciones. */
static void inicializar_album(Album *albumes)
{
    int i;
    int j;

    albumes->Eleccion = 0;
    albumes->elec_al = 0;
    albumes->ciclo = true;
    albumes->cont = 0;
    albumes->historial_cantidad = 0;
    albumes->fila_cantidad = 0;

    for (i = 0; i < Cola; i++)
    {
        albumes->historial[i] = -1;
        albumes->fila[i] = -1;
    }

    for (i = 0; i < Cant_Al; i++)
    {
        for (j = 0; j < Cant_So; j++)
        {
            albumes->cancion[i][j].past.reproducido = false;
            albumes->cancion[i][j].past.Cant_Re = 0;
            albumes->cancion[i][j].past.orden = 0;
        }
    }
}

/* Aporte: Carlos Cofré - ordenamiento y preparación de consultas del catálogo. */
static void preparar_busqueda_global(estado_ventana *estado)
{
    int cancion;
    ReferenciaCancion referencias[Cant_So];

    for (cancion = 0; cancion < Cant_So; cancion++)
    {
        referencias[cancion].album = estado->album_seleccionado;
        referencias[cancion].cancion = cancion;
    }

    ordenar_referencias_burbuja(estado->albumes, referencias, Cant_So, estado->categoria);
    estado->cantidad_canciones = Cant_So;
    for (cancion = 0; cancion < Cant_So; cancion++)
    {
        estado->album_fila[cancion] = referencias[cancion].album;
        estado->cancion_fila[cancion] = referencias[cancion].cancion;
    }

    estado->desplazamiento_lista = 0;
}

static void iniciar_reproduccion(estado_ventana *estado, int album, int cancion)
{
    estado->album_seleccionado = album;
    estado->cancion_seleccionada = cancion;
    estado->pantalla = PANTALLA_REPRODUCTOR;
    estado->reproduciendo = true;
    estado->tiempo_actual = 0.0;
    Agregar_Historial(estado->albumes, album, cancion);
}

/* Aporte: Carlos Cofré - búsqueda por campos, estadísticas y resultados. */
static void guardar_resultado(estado_ventana *estado, ReferenciaCancion referencia)
{
    if (estado->cantidad_canciones < TOTAL_CANCIONES)
    {
        estado->album_fila[estado->cantidad_canciones] = referencia.album;
        estado->cancion_fila[estado->cantidad_canciones] = referencia.cancion;
        estado->cantidad_canciones++;
    }
}

static int texto_contiene(const char *texto, const char *busqueda)
{
    size_t i;
    size_t j;

    if (busqueda[0] == '\0')
    {
        return 0;
    }

    for (i = 0; texto[i] != '\0'; i++)
    {
        for (j = 0; busqueda[j] != '\0' && texto[i + j] != '\0'; j++)
        {
            if (tolower((unsigned char)texto[i + j]) != tolower((unsigned char)busqueda[j]))
            {
                break;
            }
        }
        if (busqueda[j] == '\0')
        {
            return 1;
        }
    }

    return 0;
}

static void preparar_busqueda(estado_ventana *estado)
{
    int album;
    int cancion;

    estado->cantidad_canciones = 0;
    estado->mensaje[0] = '\0';

    if (estado->entrada[0] == '\0')
    {
        return;
    }

    if (estado->modo_busqueda == BUSQUEDA_ID)
    {
        ReferenciaCancion indice[TOTAL_CANCIONES];
        char *fin;
        long id = strtol(estado->entrada, &fin, 10);

        if (*fin != '\0' || fin == estado->entrada || id < 1 || id > TOTAL_CANCIONES)
        {
            snprintf(estado->mensaje, sizeof(estado->mensaje), "ID invalido. Rango: 1 a %d", TOTAL_CANCIONES);
            return;
        }

        crear_indice_por_id(estado->albumes, indice);
        cancion = buscar_id_binaria_recursiva(estado->albumes, indice, 0, TOTAL_CANCIONES - 1, (int)id);
        if (cancion >= 0)
        {
            guardar_resultado(estado, indice[cancion]);
        }
        return;
    }

    for (album = 0; album < Cant_Al; album++)
    {
        for (cancion = 0; cancion < Cant_So; cancion++)
        {
            Songs *dato = &estado->albumes->cancion[album][cancion];
            const char *campo = estado->modo_busqueda == BUSQUEDA_TITULO ? dato->titulo :
                                estado->modo_busqueda == BUSQUEDA_ARTISTA ? dato->artista : dato->genero;

            if (texto_contiene(campo, estado->entrada))
            {
                ReferenciaCancion referencia = {album, cancion};
                guardar_resultado(estado, referencia);
            }
        }
    }
}

static void preparar_estadisticas(estado_ventana *estado)
{
    ReferenciaCancion canciones[TOTAL_CANCIONES];
    int album;
    int cancion;
    int posicion = 0;
    int i;

    estado->cantidad_canciones = 0;
    estado->mensaje[0] = '\0';

    if (estado->modo_estadistica == ESTADISTICA_TOP)
    {
        char *fin;
        long cantidad = estado->entrada[0] == '\0' ? 10 : strtol(estado->entrada, &fin, 10);

        if (estado->entrada[0] != '\0' && (fin == estado->entrada || *fin != '\0' || cantidad <= 0))
        {
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Ingresa un numero mayor que cero");
            return;
        }
        if (cantidad > TOTAL_CANCIONES)
        {
            cantidad = TOTAL_CANCIONES;
        }

        for (album = 0; album < Cant_Al; album++)
        {
            for (cancion = 0; cancion < Cant_So; cancion++)
            {
                canciones[posicion].album = album;
                canciones[posicion].cancion = cancion;
                posicion++;
            }
        }
        ordenar_referencias_recursivo(estado->albumes, canciones, 0, posicion - 1,
                                     CATEGORIA_REPRODUCCIONES, 1);
        estado->cantidad_canciones = (int)cantidad;
        for (i = 0; i < estado->cantidad_canciones; i++)
        {
            estado->album_fila[i] = canciones[i].album;
            estado->cancion_fila[i] = canciones[i].cancion;
        }
        snprintf(estado->mensaje, sizeof(estado->mensaje), "Top %d canciones", estado->cantidad_canciones);
        return;
    }

    if (estado->modo_estadistica == ESTADISTICA_ARTISTAS)
    {
        for (album = 0; album < Cant_Al; album++)
        {
            for (cancion = 0; cancion < Cant_So; cancion++)
            {
                Songs *dato = &estado->albumes->cancion[album][cancion];
                int existe = 0;
                for (i = 0; i < estado->cantidad_canciones; i++)
                {
                    Songs *anterior = &estado->albumes->cancion[estado->album_fila[i]][estado->cancion_fila[i]];
                    if (strcmp(dato->artista, anterior->artista) == 0)
                    {
                        existe = 1;
                        break;
                    }
                }
                if (!existe)
                {
                    ReferenciaCancion referencia = {album, cancion};
                    guardar_resultado(estado, referencia);
                }
            }
        }
        snprintf(estado->mensaje, sizeof(estado->mensaje), "%d artistas disponibles", estado->cantidad_canciones);
        return;
    }

    if (estado->modo_estadistica == ESTADISTICA_GENERO)
    {
        int coincidencias = 0;
        for (album = 0; album < Cant_Al; album++)
        {
            for (cancion = 0; cancion < Cant_So; cancion++)
            {
                Songs *dato = &estado->albumes->cancion[album][cancion];
                if (texto_contiene(dato->genero, estado->entrada))
                {
                    ReferenciaCancion referencia = {album, cancion};
                    guardar_resultado(estado, referencia);
                    coincidencias++;
                }
            }
        }
        snprintf(estado->mensaje, sizeof(estado->mensaje), "%d canciones coinciden con el genero", coincidencias);
        return;
    }

    {
        int mayor = -1;
        const char *tipo = estado->modo_estadistica == ESTADISTICA_ARTISTA ? "artista" : "genero";
        for (album = 0; album < Cant_Al; album++)
        {
            for (cancion = 0; cancion < Cant_So; cancion++)
            {
                Songs *dato = &estado->albumes->cancion[album][cancion];
                const char *campo = estado->modo_estadistica == ESTADISTICA_ARTISTA ? dato->artista : dato->genero;
                if (texto_contiene(campo, estado->entrada) && dato->n_reprodcciones > mayor)
                {
                    mayor = dato->n_reprodcciones;
                }
            }
        }

        if (mayor >= 0)
        {
            for (album = 0; album < Cant_Al; album++)
            {
                for (cancion = 0; cancion < Cant_So; cancion++)
                {
                    Songs *dato = &estado->albumes->cancion[album][cancion];
                    const char *campo = estado->modo_estadistica == ESTADISTICA_ARTISTA ? dato->artista : dato->genero;
                    if (dato->n_reprodcciones == mayor && texto_contiene(campo, estado->entrada))
                    {
                        ReferenciaCancion referencia = {album, cancion};
                        guardar_resultado(estado, referencia);
                    }
                }
            }
        }
        snprintf(estado->mensaje, sizeof(estado->mensaje), "%s mas escuchada: %d reproducciones", tipo,
                 mayor < 0 ? 0 : mayor);
    }
}

/* Aporte: Matías Yevenes - vistas, controles y presentación visual con Allegro. */
static void dibujar_barra_categorias(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    const char *etiquetas[MAX_CATEGORIAS] = {
        "Albumes", "ID", "Anio", "Duracion", "Reprod.", "Genero", "Artista", "Titulo", "Album"
    };
    const char *vistas[4] = {"Buscar", "Estadisticas", "Cola", "Historial"};
    int y = 20;
    int i;

    for (i = 0; i < MAX_CATEGORIAS; i++)
    {
        int rx = CATEGORY_X + i * (CATEGORY_W + CATEGORY_GAP);
        int ry = y;
        int activo = estado->categoria == i;

        if (activo)
        {
            al_draw_filled_rectangle(rx, ry, rx + CATEGORY_W, ry + 30, al_map_rgb(52, 118, 255));
        }
        else
        {
            al_draw_filled_rectangle(rx, ry, rx + CATEGORY_W, ry + 30, al_map_rgb(32, 35, 46));
        }

        al_draw_rectangle(rx, ry, rx + CATEGORY_W, ry + 30, al_map_rgb(120, 120, 140), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), rx + CATEGORY_W / 2, ry + 6,
                     ALLEGRO_ALIGN_CENTER, etiquetas[i]);
    }

    for (i = 0; i < 4; i++)
    {
        int rx = TOOLBAR_X + i * (TOOLBAR_W + TOOLBAR_GAP);
        int activo = (i == 0 && estado->pantalla == PANTALLA_BUSQUEDA) ||
                     (i == 1 && estado->pantalla == PANTALLA_ESTADISTICAS) ||
                     (i == 2 && estado->pantalla == PANTALLA_COLA) ||
                     (i == 3 && estado->pantalla == PANTALLA_HISTORIAL);
        al_draw_filled_rectangle(rx, y, rx + TOOLBAR_W, y + 30,
                                 activo ? al_map_rgb(52, 118, 255) : al_map_rgb(32, 35, 46));
        al_draw_rectangle(rx, y, rx + TOOLBAR_W, y + 30, al_map_rgb(120, 120, 140), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), rx + TOOLBAR_W / 2, y + 6,
                     ALLEGRO_ALIGN_CENTER, vistas[i]);
    }
}

static void actualizar_hover(estado_ventana *estado)
{
    int i;
    int x;
    int y;

    estado->hover_album = -1;
    estado->hover_cancion = -1;

    if (estado->pantalla == PANTALLA_ALBUMES)
    {
        x = 60;
        for (i = 0; i < Cant_Al; i++)
        {
            y = 120 + i * 38;
            if (punto_en_rectangulo(estado->mouse_x, estado->mouse_y, x, y, ANCHO_VENTANA - 140, 34))
            {
                estado->hover_album = i;
                break;
            }
        }
    }
    else if (estado->pantalla == PANTALLA_CANCIONES ||
             estado->pantalla == PANTALLA_BUSQUEDA ||
             estado->pantalla == PANTALLA_ESTADISTICAS)
    {
        x = 60;
        for (i = 0; i < CANCIONES_VISIBLES; i++)
        {
            y = 120 + i * 42;
            if (punto_en_rectangulo(estado->mouse_x, estado->mouse_y, x, y, ANCHO_VENTANA - 140, 38))
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

static void dibujar_albumes(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    int i;
    int x;
    int y;
    int w;
    int h;
    char nombre[64];

    al_clear_to_color(al_map_rgb(12, 12, 18));
    dibujar_barra_categorias(estado, fuente);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 70, ALLEGRO_ALIGN_CENTER, "Catalogo");
    al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT, "Selecciona un album");

    for (i = 0; i < Cant_Al; i++)
    {
        x = 60;
        y = 120 + i * 38;
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

static void dibujar_canciones(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    int i;
    int x;
    int y;
    int w;
    int h;
    char titulo[120];
    char subtitulo[240];
    char linea[220];
    char valor[120];
    const char *etiquetas[MAX_CATEGORIAS] = {
        "Principal", "ID", "Anio", "Duracion", "Reproducciones", "Genero", "Artista", "Titulo", "Album"
    };

    al_clear_to_color(al_map_rgb(16, 14, 24));
    dibujar_barra_categorias(estado, fuente);

    if (estado->pantalla == PANTALLA_BUSQUEDA)
    {
        const char *tipo[] = {"ID", "Titulo", "Artista", "Genero"};
        snprintf(titulo, sizeof(titulo), "Buscar canciones por %s", tipo[estado->modo_busqueda]);
        snprintf(subtitulo, sizeof(subtitulo), "F1: ID  F2: Titulo  F3: Artista  F4: Genero | Texto: %s | %s",
                 estado->entrada, estado->mensaje[0] == '\0' ? "Escribe para buscar" : estado->mensaje);
    }
    else if (estado->pantalla == PANTALLA_ESTADISTICAS)
    {
        const char *tipo[] = {"Artistas disponibles", "Canciones por genero", "Mas escuchadas",
                              "Mas escuchada por artista", "Mas escuchada por genero"};
        snprintf(titulo, sizeof(titulo), "Estadisticas: %s", tipo[estado->modo_estadistica]);
        snprintf(subtitulo, sizeof(subtitulo), "F1 artistas F2 genero F3 top N F4 artista F5 genero | Dato: %.40s | %.65s",
             estado->entrada, estado->mensaje);
    }
    else if (estado->busqueda_global == 1)
    {
        snprintf(titulo, sizeof(titulo), "Album: %.10s | Orden: %s",
                 estado->albumes->cancion[estado->album_seleccionado][0].albums,
                 etiquetas[estado->categoria]);
        snprintf(subtitulo, sizeof(subtitulo), "Selecciona una cancion");
    }
    else
    {
        snprintf(titulo, sizeof(titulo), "Album: %s", estado->albumes->cancion[estado->album_seleccionado][0].albums);
        snprintf(subtitulo, sizeof(subtitulo), "Selecciona una cancion");
    }

    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 70, ALLEGRO_ALIGN_CENTER, titulo);
    al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT, subtitulo);

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
            if (i >= Cant_So)
            {
                break;
            }
            cancion = &estado->albumes->cancion[estado->album_seleccionado][i];
        }

        x = 60;
        y = 120 + i * 42;
        w = ANCHO_VENTANA - 140;
        h = 38;

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

        snprintf(linea, sizeof(linea), "Titulo: %s | Artista: %s | Album: %s",
                 cancion->titulo, cancion->artista, cancion->albums);
        snprintf(valor, sizeof(valor), "ID: %d | Genero: %s | Duracion: %d s | Anio: %d | Reproducciones: %d",
                 cancion->id, cancion->genero, cancion->duracion_s, cancion->anio,
                 cancion->n_reprodcciones);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), x + 10, y + 4, ALLEGRO_ALIGN_LEFT, linea);
        al_draw_text(fuente, al_map_rgb(190, 190, 205), x + 10, y + 21, ALLEGRO_ALIGN_LEFT, valor);
    }

    if (estado->cantidad_canciones == 0)
    {
        al_draw_text(fuente, al_map_rgb(210, 210, 220), ANCHO_VENTANA / 2, 150,
                     ALLEGRO_ALIGN_CENTER, "No hay resultados");
    }
}

static void dibujar_reproductor(estado_ventana *estado, ALLEGRO_FONT *fuente)
{
    Songs *cancion = &estado->albumes->cancion[estado->album_seleccionado][estado->cancion_seleccionada];
    char linea1[128];
    char linea2[128];
    char linea3[128];
    char linea4[128];
    char linea5[128];
    char linea6[128];
    char linea7[128];
    int y;
    double progreso;
    int w_barra;
    int x_barra = 210;
    int y_barra = 560;
    int ancho_barra = 700;
    int altura_barra = 18;
    char tiempo_txt[64];

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

    if (cancion->duracion_s > 0)
    {
        progreso = estado->tiempo_actual / cancion->duracion_s;
    }
    else
    {
        progreso = 0.0;
    }

    if (progreso > 1.0)
    {
        progreso = 1.0;
    }

    w_barra = (int)(ancho_barra * progreso);
    al_draw_filled_rectangle(x_barra, y_barra, x_barra + ancho_barra, y_barra + altura_barra, al_map_rgb(60, 60, 70));
    al_draw_filled_rectangle(x_barra, y_barra, x_barra + w_barra, y_barra + altura_barra, al_map_rgb(38, 164, 92));
    al_draw_rectangle(x_barra, y_barra, x_barra + ancho_barra, y_barra + altura_barra, al_map_rgb(190, 190, 190), 1);

    snprintf(tiempo_txt, sizeof(tiempo_txt), "%d / %d s", (int)estado->tiempo_actual, cancion->duracion_s);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 540, ALLEGRO_ALIGN_CENTER, tiempo_txt);
    al_draw_text(fuente, al_map_rgb(120, 220, 150), ANCHO_VENTANA / 2, 590, ALLEGRO_ALIGN_CENTER,
                 estado->reproduciendo ? "Reproduciendo" : estado->mensaje);
    al_draw_text(fuente, al_map_rgb(180, 180, 200), ANCHO_VENTANA / 2, 615, ALLEGRO_ALIGN_CENTER,
                 estado->mensaje);
    al_draw_filled_rectangle(60, 640, 270, 672, al_map_rgb(35, 100, 75));
    al_draw_rectangle(60, 640, 270, 672, al_map_rgb(160, 200, 175), 1);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 165, 650, ALLEGRO_ALIGN_CENTER, "Agregar al inicio de cola");
}

static void dibujar_lista_colas(estado_ventana *estado, ALLEGRO_FONT *fuente, int ver_historial)
{
    int cantidad = ver_historial ? estado->albumes->historial_cantidad : estado->albumes->fila_cantidad;
    int i;
    char linea[220];

    al_clear_to_color(al_map_rgb(16, 14, 24));
    dibujar_barra_categorias(estado, fuente);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 70,
                 ALLEGRO_ALIGN_CENTER, ver_historial ? "Historial reciente" : "Fila de reproduccion");

    if (ver_historial)
    {
        al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT,
                     "Ultimas canciones abiertas; la mas reciente aparece primero.");
    }
    else
    {
        snprintf(linea, sizeof(linea), "Seleccion: %d | I: quitar por ID | Delete: quitar | C: vaciar | Enter: reproducir",
                 estado->cancion_seleccionada + 1);
        al_draw_text(fuente, al_map_rgb(180, 180, 200), 60, 90, ALLEGRO_ALIGN_LEFT, linea);
        snprintf(linea, sizeof(linea), "ID para quitar: %s | %s", estado->entrada, estado->mensaje);
        al_draw_text(fuente, al_map_rgb(200, 200, 215), 60, 106, ALLEGRO_ALIGN_LEFT, linea);
    }

    for (i = 0; i < cantidad; i++)
    {
        int valor = ver_historial ? estado->albumes->historial[i] : estado->albumes->fila[i];
        int album = valor / 1000;
        int cancion_indice = valor % 1000;
        Songs *cancion = &estado->albumes->cancion[album][cancion_indice];
        int y = 130 + i * 42;
        int seleccionada = !ver_historial && estado->cancion_seleccionada == i;

        if (seleccionada)
        {
            al_draw_filled_rectangle(60, y, ANCHO_VENTANA - 80, y + 38, al_map_rgb(52, 92, 150));
        }
        else
        {
            al_draw_filled_rectangle(60, y, ANCHO_VENTANA - 80, y + 38, al_map_rgb(27, 27, 34));
        }
        al_draw_rectangle(60, y, ANCHO_VENTANA - 80, y + 38, al_map_rgb(100, 100, 110), 1);
        snprintf(linea, sizeof(linea), "%d. %s - %s | Album: %s | ID: %d",
                 i + 1, cancion->titulo, cancion->artista, cancion->albums, cancion->id);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 70, y + 12, ALLEGRO_ALIGN_LEFT, linea);
    }

    if (cantidad == 0)
    {
        al_draw_text(fuente, al_map_rgb(210, 210, 220), ANCHO_VENTANA / 2, 160,
                     ALLEGRO_ALIGN_CENTER, "No hay canciones en esta lista");
    }

    if (!ver_historial)
    {
        al_draw_filled_rectangle(60, 620, 240, 652, al_map_rgb(80, 55, 55));
        al_draw_rectangle(60, 620, 240, 652, al_map_rgb(160, 140, 140), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 150, 630, ALLEGRO_ALIGN_CENTER, "Quitar seleccion");
        al_draw_filled_rectangle(250, 620, 360, 652, al_map_rgb(80, 55, 55));
        al_draw_rectangle(250, 620, 360, 652, al_map_rgb(160, 140, 140), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 305, 630, ALLEGRO_ALIGN_CENTER, "Vaciar cola");
        al_draw_filled_rectangle(370, 620, 510, 652, al_map_rgb(45, 85, 65));
        al_draw_rectangle(370, 620, 510, 652, al_map_rgb(145, 175, 150), 1);
        al_draw_text(fuente, al_map_rgb(255, 255, 255), 440, 630, ALLEGRO_ALIGN_CENTER, "Reproducir");
    }
}

/* Aporte: Matías Yevenes - navegación y manejo de interacciones de ventana. */
static int clic_categorias(estado_ventana *estado, int mouse_x, int mouse_y)
{
    int i;

    for (i = 0; i < MAX_CATEGORIAS; i++)
    {
        int rx = CATEGORY_X + i * (CATEGORY_W + CATEGORY_GAP);
        if (punto_en_rectangulo(mouse_x, mouse_y, rx, 20, CATEGORY_W, 30))
        {
            estado->categoria = i;
            if (estado->categoria == CATEGORIA_PRINCIPAL)
            {
                estado->pantalla = PANTALLA_ALBUMES;
                estado->busqueda_global = 0;
                estado->desplazamiento_lista = 0;
            }
            else if (estado->pantalla != PANTALLA_ALBUMES)
            {
                estado->pantalla = PANTALLA_CANCIONES;
                estado->cancion_seleccionada = 0;
                estado->busqueda_global = 1;
                preparar_busqueda_global(estado);
            }
            return 1;
        }
    }

    for (i = 0; i < 4; i++)
    {
        int rx = TOOLBAR_X + i * (TOOLBAR_W + TOOLBAR_GAP);
        if (punto_en_rectangulo(mouse_x, mouse_y, rx, 20, TOOLBAR_W, 30))
        {
            estado->entrada[0] = '\0';
            estado->largo_entrada = 0;
            estado->mensaje[0] = '\0';
            estado->desplazamiento_lista = 0;
            if (i == 0)
            {
                estado->pantalla = PANTALLA_BUSQUEDA;
                estado->modo_busqueda = BUSQUEDA_TITULO;
                estado->busqueda_global = 1;
                preparar_busqueda(estado);
            }
            else if (i == 1)
            {
                estado->pantalla = PANTALLA_ESTADISTICAS;
                estado->modo_estadistica = ESTADISTICA_ARTISTAS;
                estado->busqueda_global = 1;
                preparar_estadisticas(estado);
            }
            else if (i == 2)
            {
                estado->pantalla = PANTALLA_COLA;
            }
            else
            {
                estado->pantalla = PANTALLA_HISTORIAL;
            }
            return 1;
        }
    }
    return 0;
}

static void manejar_click(estado_ventana *estado, int mouse_x, int mouse_y)
{
    int i;
    int x;
    int y;

    if (clic_categorias(estado, mouse_x, mouse_y))
    {
        return;
    }

    if (estado->pantalla == PANTALLA_REPRODUCTOR &&
        punto_en_rectangulo(mouse_x, mouse_y, 60, 640, 210, 32))
    {
        int resultado = Agregar_Cola(estado->albumes, estado->album_seleccionado,
                                     estado->cancion_seleccionada);
        snprintf(estado->mensaje, sizeof(estado->mensaje),
                 resultado == 1 ? "Agregada al inicio de la cola" :
                 resultado == 0 ? "La cancion ya esta en la cola" : "La cola esta llena");
        return;
    }

    if (estado->pantalla == PANTALLA_COLA)
    {
        for (i = 0; i < estado->albumes->fila_cantidad; i++)
        {
            y = 130 + i * 42;
            if (punto_en_rectangulo(mouse_x, mouse_y, 60, y, ANCHO_VENTANA - 140, 38))
            {
                estado->cancion_seleccionada = i;
                return;
            }
        }
        if (punto_en_rectangulo(mouse_x, mouse_y, 60, 620, 180, 32))
        {
            Quitar_Cola_Posicion(estado->albumes, estado->cancion_seleccionada);
            if (estado->cancion_seleccionada >= estado->albumes->fila_cantidad)
            {
                estado->cancion_seleccionada = estado->albumes->fila_cantidad - 1;
            }
            if (estado->cancion_seleccionada < 0)
            {
                estado->cancion_seleccionada = 0;
            }
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Elemento quitado");
            return;
        }
        if (punto_en_rectangulo(mouse_x, mouse_y, 250, 620, 110, 32))
        {
            Vaciar_Cola(estado->albumes);
            estado->cancion_seleccionada = 0;
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Cola vaciada");
            return;
        }
        if (punto_en_rectangulo(mouse_x, mouse_y, 370, 620, 140, 32) &&
            estado->albumes->fila_cantidad > 0)
        {
            int valor = estado->albumes->fila[0];
            Quitar_Cola_Posicion(estado->albumes, 0);
            iniciar_reproduccion(estado, valor / 1000, valor % 1000);
            return;
        }
    }

    if (estado->pantalla == PANTALLA_HISTORIAL)
    {
        for (i = 0; i < estado->albumes->historial_cantidad; i++)
        {
            y = 130 + i * 42;
            if (punto_en_rectangulo(mouse_x, mouse_y, 60, y, ANCHO_VENTANA - 140, 38))
            {
                int valor = estado->albumes->historial[i];
                iniciar_reproduccion(estado, valor / 1000, valor % 1000);
                return;
            }
        }
    }

    if (estado->pantalla == PANTALLA_ALBUMES)
    {
        x = 60;
        for (i = 0; i < Cant_Al; i++)
        {
            y = 120 + i * 38;
            if (punto_en_rectangulo(mouse_x, mouse_y, x, y, ANCHO_VENTANA - 140, 34))
            {
                estado->album_seleccionado = i;
                estado->cancion_seleccionada = 0;
                estado->pantalla = PANTALLA_CANCIONES;
                estado->busqueda_global = 1;
                preparar_busqueda_global(estado);
                return;
            }
        }
    }

    if (estado->pantalla == PANTALLA_CANCIONES || estado->pantalla == PANTALLA_BUSQUEDA ||
        estado->pantalla == PANTALLA_ESTADISTICAS)
    {
        x = 60;
        for (i = 0; i < CANCIONES_VISIBLES; i++)
        {
            y = 120 + i * 42;
            if (punto_en_rectangulo(mouse_x, mouse_y, x, y, ANCHO_VENTANA - 140, 38))
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

                iniciar_reproduccion(estado, estado->album_seleccionado, estado->cancion_seleccionada);
                return;
            }
        }
    }
}

static void actualizar_entrada(estado_ventana *estado, int caracter)
{
    if (estado->largo_entrada < (int)sizeof(estado->entrada) - 1 && caracter >= 32 && caracter <= 126)
    {
        estado->entrada[estado->largo_entrada] = (char)caracter;
        estado->largo_entrada++;
        estado->entrada[estado->largo_entrada] = '\0';
    }
}

static void manejar_tecla(estado_ventana *estado, ALLEGRO_EVENT *evento)
{
    int tecla = evento->keyboard.keycode;
    int tecla_funcion = 0;

    if (estado->pantalla == PANTALLA_COLA)
    {
        if (tecla == ALLEGRO_KEY_I)
        {
            estado->entrada_cola_activa = 1;
            estado->entrada[0] = '\0';
            estado->largo_entrada = 0;
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Escribe el ID y presiona Enter");
        }
        else if (tecla == ALLEGRO_KEY_BACKSPACE && estado->entrada_cola_activa && estado->largo_entrada > 0)
        {
            estado->largo_entrada--;
            estado->entrada[estado->largo_entrada] = '\0';
        }
        else if (tecla == ALLEGRO_KEY_DELETE ||
                 (tecla == ALLEGRO_KEY_BACKSPACE && !estado->entrada_cola_activa))
        {
            Quitar_Cola_Posicion(estado->albumes, estado->cancion_seleccionada);
            if (estado->albumes->fila_cantidad == 0)
            {
                estado->cancion_seleccionada = 0;
            }
            else if (estado->cancion_seleccionada >= estado->albumes->fila_cantidad)
            {
                estado->cancion_seleccionada = estado->albumes->fila_cantidad - 1;
            }
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Elemento quitado");
        }
        else if (tecla == ALLEGRO_KEY_C)
        {
            Vaciar_Cola(estado->albumes);
            estado->cancion_seleccionada = 0;
            snprintf(estado->mensaje, sizeof(estado->mensaje), "Cola vaciada");
        }
        else if (tecla == ALLEGRO_KEY_ENTER && estado->entrada_cola_activa)
        {
            char *fin;
            long id = strtol(estado->entrada, &fin, 10);
            int eliminado = fin != estado->entrada && *fin == '\0' && Quitar_Cola_ID(estado->albumes, (int)id);
            snprintf(estado->mensaje, sizeof(estado->mensaje), eliminado ? "ID quitado de la cola" : "ID no encontrado");
            estado->entrada_cola_activa = 0;
            estado->entrada[0] = '\0';
            estado->largo_entrada = 0;
        }
        else if (tecla == ALLEGRO_KEY_ENTER && estado->albumes->fila_cantidad > 0)
        {
            int valor;
            valor = estado->albumes->fila[0];
            Quitar_Cola_Posicion(estado->albumes, 0);
            if (estado->cancion_seleccionada > 0)
            {
                estado->cancion_seleccionada--;
            }
            iniciar_reproduccion(estado, valor / 1000, valor % 1000);
        }
        else if (estado->entrada_cola_activa)
        {
            actualizar_entrada(estado, evento->keyboard.unichar);
        }
        return;
    }

    if (estado->pantalla != PANTALLA_BUSQUEDA && estado->pantalla != PANTALLA_ESTADISTICAS)
    {
        return;
    }

    if (tecla >= ALLEGRO_KEY_F1 && tecla <= ALLEGRO_KEY_F5)
    {
        tecla_funcion = tecla - ALLEGRO_KEY_F1;
        if (estado->pantalla == PANTALLA_BUSQUEDA && tecla_funcion < 4)
        {
            estado->modo_busqueda = tecla_funcion;
            estado->entrada[0] = '\0';
            estado->largo_entrada = 0;
            preparar_busqueda(estado);
        }
        else if (estado->pantalla == PANTALLA_ESTADISTICAS)
        {
            estado->modo_estadistica = tecla_funcion;
            estado->entrada[0] = '\0';
            estado->largo_entrada = 0;
            preparar_estadisticas(estado);
        }
    }
    else if (tecla == ALLEGRO_KEY_BACKSPACE && estado->largo_entrada > 0)
    {
        estado->largo_entrada--;
        estado->entrada[estado->largo_entrada] = '\0';
        if (estado->pantalla == PANTALLA_BUSQUEDA)
        {
            preparar_busqueda(estado);
        }
        else
        {
            preparar_estadisticas(estado);
        }
    }
    else if (tecla == ALLEGRO_KEY_ENTER)
    {
        if (estado->pantalla == PANTALLA_BUSQUEDA)
        {
            preparar_busqueda(estado);
        }
        else
        {
            preparar_estadisticas(estado);
        }
    }
    else
    {
        actualizar_entrada(estado, evento->keyboard.unichar);
        if (estado->pantalla == PANTALLA_BUSQUEDA)
        {
            preparar_busqueda(estado);
        }
        else
        {
            preparar_estadisticas(estado);
        }
    }
}

/* Aporte: Matías Yevenes - ciclo de eventos y ventanas de Allegro. */
static void iniciar_interfaz(Album *albumes)
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

    if (!al_install_keyboard())
    {
        fprintf(stderr, "No se pudo inicializar el teclado.\n");
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
    al_register_event_source(cola_eventos, al_get_keyboard_event_source());
    al_register_event_source(cola_eventos, al_get_timer_event_source(temporizador));

    estado.albumes = albumes;
    estado.pantalla = PANTALLA_ALBUMES;
    estado.album_seleccionado = 0;
    estado.cancion_seleccionada = 0;
    estado.categoria = CATEGORIA_PRINCIPAL;
    estado.busqueda_global = 0;
    estado.hover_album = -1;
    estado.hover_cancion = -1;
    estado.cantidad_canciones = 0;
    estado.desplazamiento_lista = 0;
    estado.mouse_x = 0;
    estado.mouse_y = 0;
    estado.entrada[0] = '\0';
    estado.mensaje[0] = '\0';
    estado.largo_entrada = 0;
    estado.modo_busqueda = BUSQUEDA_TITULO;
    estado.modo_estadistica = ESTADISTICA_ARTISTAS;
    estado.entrada_cola_activa = 0;
    estado.reproduciendo = false;
    estado.tiempo_actual = 0.0;

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

            if (estado.busqueda_global == 1 && evento.mouse.dz != 0 &&
                (estado.pantalla == PANTALLA_CANCIONES || estado.pantalla == PANTALLA_BUSQUEDA ||
                 estado.pantalla == PANTALLA_ESTADISTICAS))
            {
                int maximo = estado.cantidad_canciones - CANCIONES_VISIBLES;
                if (maximo < 0)
                {
                    maximo = 0;
                }
                estado.desplazamiento_lista -= evento.mouse.dz;
                if (estado.desplazamiento_lista < 0)
                {
                    estado.desplazamiento_lista = 0;
                }
                if (estado.desplazamiento_lista > maximo)
                {
                    estado.desplazamiento_lista = maximo;
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
        else if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            manejar_tecla(&estado, &evento);
            redraw = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            /* Aporte: Eban Delgado - avance simulado, contadores y reproduccion de la cola. */
            if (estado.reproduciendo)
            {
                estado.tiempo_actual += 1.0 / 60.0;
                if (estado.tiempo_actual >= estado.albumes->cancion[estado.album_seleccionado][estado.cancion_seleccionada].duracion_s)
                {
                    estado.tiempo_actual = 0.0;
                    estado.reproduciendo = false;
                    estado.albumes->cancion[estado.album_seleccionado][estado.cancion_seleccionada].n_reprodcciones++;
                    estado.albumes->cancion[estado.album_seleccionado][estado.cancion_seleccionada].past.reproducido = true;
                    estado.albumes->cancion[estado.album_seleccionado][estado.cancion_seleccionada].past.Cant_Re++;
                    if (estado.albumes->fila_cantidad > 0)
                    {
                        int album_siguiente;
                        int cancion_siguiente;
                        if (Siguiente_Cola(estado.albumes, &album_siguiente, &cancion_siguiente))
                        {
                            iniciar_reproduccion(&estado, album_siguiente, cancion_siguiente);
                        }
                    }
                }
            }
            redraw = 1;
        }

        if (redraw && al_is_event_queue_empty(cola_eventos))
        {
            if (estado.pantalla == PANTALLA_ALBUMES)
            {
                dibujar_albumes(&estado, fuente);
            }
            else if (estado.pantalla == PANTALLA_CANCIONES)
            {
                dibujar_canciones(&estado, fuente);
            }
            else if (estado.pantalla == PANTALLA_BUSQUEDA || estado.pantalla == PANTALLA_ESTADISTICAS)
            {
                dibujar_canciones(&estado, fuente);
            }
            else if (estado.pantalla == PANTALLA_COLA)
            {
                dibujar_lista_colas(&estado, fuente, 0);
            }
            else if (estado.pantalla == PANTALLA_HISTORIAL)
            {
                dibujar_lista_colas(&estado, fuente, 1);
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
    al_uninstall_keyboard();
    al_uninstall_system();
}

/* Aporte: Eban Delgado - validación, carga y exportación del catálogo CSV. */
bool Cargar_CSV(Album *albumes, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "r");
    char linea[256];
    bool ids_vistos[TOTAL_CANCIONES];
    int i;
    int j;

    if (archivo == NULL)
    {
        return false;
    }

    if (fgets(linea, sizeof(linea), archivo) == NULL)
    {
        fclose(archivo);
        return false;
    }

    for (i = 0; i < TOTAL_CANCIONES; i++)
    {
        ids_vistos[i] = false;
    }

    for (i = 0; i < Cant_Al; i++)
    {
        for (j = 0; j < Cant_So; j++)
        {
            if (fgets(linea, sizeof(linea), archivo) == NULL ||
                sscanf(linea, "%d,%10[^,],%10[^,],%10[^,],%10[^,],%d,%d,%d",
                       &albumes->cancion[i][j].id,
                       albumes->cancion[i][j].titulo,
                       albumes->cancion[i][j].artista,
                       albumes->cancion[i][j].albums,
                       albumes->cancion[i][j].genero,
                       &albumes->cancion[i][j].duracion_s,
                       &albumes->cancion[i][j].anio,
                       &albumes->cancion[i][j].n_reprodcciones) != 8 ||
                albumes->cancion[i][j].duracion_s <= 0 ||
                albumes->cancion[i][j].n_reprodcciones < 0 ||
                albumes->cancion[i][j].anio < 1900 ||
                albumes->cancion[i][j].anio > 2100 ||
                albumes->cancion[i][j].id < 1 ||
                albumes->cancion[i][j].id > TOTAL_CANCIONES ||
                ids_vistos[albumes->cancion[i][j].id - 1])
            {
                fclose(archivo);
                return false;
            }

            ids_vistos[albumes->cancion[i][j].id - 1] = true;

            albumes->cancion[i][j].past.reproducido = false;
            albumes->cancion[i][j].past.Cant_Re = 0;
            albumes->cancion[i][j].past.orden = 0;
        }
    }

    fclose(archivo);
    return true;
}

void Guardar_CSV(Album *albumes, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "w");
    int i;
    int j;

    if (archivo == NULL)
    {
        printf("Error al crear o abrir el archivo CSV.\n");
        return;
    }

    fprintf(archivo, "id,titulo,artista,album,genero,duracion_seg,anio,n_reproducciones\n");

    for (i = 0; i < Cant_Al; i++)
    {
        for (j = 0; j < Cant_So; j++)
        {
            fprintf(archivo, "%d,%s,%s,%s,%s,%d,%d,%d\n",
                    albumes->cancion[i][j].id,
                    albumes->cancion[i][j].titulo,
                    albumes->cancion[i][j].artista,
                    albumes->cancion[i][j].albums,
                    albumes->cancion[i][j].genero,
                    albumes->cancion[i][j].duracion_s,
                    albumes->cancion[i][j].anio,
                    albumes->cancion[i][j].n_reprodcciones);
        }
    }

    fclose(archivo);
}

int main(void)
{
    Album albumes;
    const char *nombre_archivo = "catalogo.csv";

    inicializar_album(&albumes);

    if (!Cargar_CSV(&albumes, nombre_archivo))
    {
        Generar(&albumes);
        Guardar_CSV(&albumes, nombre_archivo);
    }

    iniciar_interfaz(&albumes);
    Guardar_CSV(&albumes, nombre_archivo);
    Guardar_CSV(&albumes, "catalogo_actualizado.csv");
    return 0;
}
