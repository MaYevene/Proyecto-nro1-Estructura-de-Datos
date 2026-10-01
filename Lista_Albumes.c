/**
 * @file
 * @brief
 */

 #include "Reproductor.h"

 /**
  * @brief
  * 
  * @param albumes
  */

void Ordenar(Album *albumes, int elec_org);
void QuickSort_Texto(Album *albumes, int bajo, int alto, int campo);    
void BubbleSort_Numerico(Album *albumes, int campo);
int Busqueda_Binaria(Album *albumes, int inicio, int fin, int opcion, const char *clave_busqueda, int id_buscado);

void Lista_albumes(Album *albumes)
{
    int elec_org=0;
    albumes->elec_al=0;

    printf("Que album desea escuchar:\n");

    for(int i=0;i<Cant_Al;i++)
    {
        printf("%d°%s\n",i+1,albumes->cancion[albumes->elec_al][i].albums);
    }

    printf("\t>>>");
    scanf("%d",&albumes->elec_al);

    while (albumes->elec_al<1 || albumes->elec_al>Cant_Al)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        printf("\t>>>");
        scanf("%d",&albumes->elec_al);
    }

    printf("Indique el filtro de busqueda\n");
    
    printf("1° id\n");
    printf("2° Titulo\n");
    printf("3° artista\n");
    printf("4° Genero\n");
    printf("5° Duracion en segundos\n");
    printf("6° Año de lanzamiento\n");
    printf("7° Numero de reproducciones\n");

    printf("\t>>>");
    scanf("%d",&elec_org);

    while (elec_org<1 || elec_org>7)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        printf("\t>>>");
        scanf("%d",&elec_org);
    }

    Ordenar(albumes,elec_org);

    switch (elec_org)
    {
    case 1:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("id:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 2:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Titulo:%s|\tid:%d|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 3:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Artista:%s|\tid:%d|\tTitulo:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 4:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Genero:%s|\tid:%d|\tTitulo:%s|\tArtista:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 5:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Duracion en segundos:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 6:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Año de lanzamiento:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tNumero de reproducciones:%d\n",
        albumes->cancion[albumes->elec_al][i].anio,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].n_reprodcciones);
    }
        break;
    case 7:for(int i=0;i<Cant_So;i++)
    {
        printf("\n%d°",i+1);
        printf("Numero de reproducciones:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d\n",
        albumes->cancion[albumes->elec_al][i].n_reprodcciones,
        albumes->cancion[albumes->elec_al][i].id,
        albumes->cancion[albumes->elec_al][i].titulo,
        albumes->cancion[albumes->elec_al][i].artista,
        albumes->cancion[albumes->elec_al][i].genero,
        albumes->cancion[albumes->elec_al][i].duracion_s,
        albumes->cancion[albumes->elec_al][i].anio);
    }
        break;
    }

    int total = (Cant_Al * Cant_So) - 1;
    int pos = -1;

    if (elec_org == 1) // ID
    {
        int id_buscado;
        printf("\nIngrese ID a buscar: ");
        scanf("%d", &id_buscado);
        pos = Busqueda_Binaria(albumes, 0, total, 1, NULL, id_buscado);
    }
    else if (elec_org == 2) // Título
    {
        char texto_buscado[30];
        printf("\nIngrese Título a buscar: ");
        scanf("%s", texto_buscado);
        pos = Busqueda_Binaria(albumes, 0, total, 2, texto_buscado, 0);
    }
    else if (elec_org == 3) // Artista
    {
        char texto_buscado[30];
        printf("\nIngrese Artista a buscar: ");
        scanf("%s", texto_buscado);
        pos = Busqueda_Binaria(albumes, 0, total, 3, texto_buscado, 0);
    }

    if (pos != -1)
    {
        int al = pos / Cant_So;
        int so = pos % Cant_So;
        printf("\n¡Canción encontrada en posición [%d][%d]!\n", al, so);
        albumes->Eleccion=so;
        printf("\n");
    }
    else if (elec_org >= 1 && elec_org <= 3)
    {
        printf("\nCanción no encontrada.\n");
    }
}

/**
 * @brief
 * 
 * @param albumes
 */

void Ordenar(Album *albumes, int elec_org)
{
    int total=(Cant_Al*Cant_So)-1;

    for(int j=0;j<Cant_So-1;j++)
    {
        for(int i=0;i<Cant_So-1;i++)
        {
            switch (elec_org)
            {
            case 1:
                if(albumes->cancion[albumes->elec_al][i].id>albumes->cancion[albumes->elec_al][i+1].id)
                {
                    BubbleSort_Numerico(albumes,1);
                }
                break;
            case 2:
                if(albumes->cancion[albumes->elec_al][i].titulo[0]>albumes->cancion[albumes->elec_al][i+1].titulo[0])
                {
                    QuickSort_Texto(albumes,0,total,2);
                }
                break;
            case 3:
                if(albumes->cancion[albumes->elec_al][i].artista[0]>albumes->cancion[albumes->elec_al][i+1].artista[0])
                {
                    QuickSort_Texto(albumes,0,total,3);
                }
                break;
            case 4:
                if(albumes->cancion[albumes->elec_al][i].genero[0]>albumes->cancion[albumes->elec_al][i+1].genero[0])
                {
                    QuickSort_Texto(albumes,0,total,4);
                }
                break;
            case 5:
                if(albumes->cancion[albumes->elec_al][i].duracion_s>albumes->cancion[albumes->elec_al][i+1].duracion_s)
                {
                    BubbleSort_Numerico(albumes,5);
                }
                break;
            case 6:
                if(albumes->cancion[albumes->elec_al][i].anio>albumes->cancion[albumes->elec_al][i+1].anio)
                {
                    BubbleSort_Numerico(albumes,6);
                }
                break;
            case 7:
                if(albumes->cancion[albumes->elec_al][i].n_reprodcciones>albumes->cancion[albumes->elec_al][i+1].n_reprodcciones)
                {
                    BubbleSort_Numerico(albumes,7);
                }
                break;
            }
        }
    }
}

/**
 * @brief
 * 
 * @param albumes
 * @param bajo
 * @param alto
 * @param campo
 */

void QuickSort_Texto(Album *albumes, int bajo, int alto, int campo)
{
    if (bajo >= alto) return;

    // Pivote al final de la sub-lista
    int al_p = alto / Cant_So;
    int so_p = alto % Cant_So;

    int i = bajo - 1;

    for (int j = bajo; j < alto; j++)
    {
        int al_j = j / Cant_So;
        int so_j = j % Cant_So;

        int comparacion = 0;

        // Comparamos cadenas con strcmp según el campo deseado
        if (campo == 2)      // Título
            comparacion = strcmp(albumes->cancion[al_j][so_j].titulo, albumes->cancion[al_p][so_p].titulo);
        else if (campo == 3) // Artista
            comparacion = strcmp(albumes->cancion[al_j][so_j].artista, albumes->cancion[al_p][so_p].artista);
        else if (campo == 4) // Género
            comparacion = strcmp(albumes->cancion[al_j][so_j].genero, albumes->cancion[al_p][so_p].genero);

        // Si la cadena es alfabéticamente menor que el pivote
        if (comparacion < 0)
        {
            i++;
            int al_i = i / Cant_So;
            int so_i = i % Cant_So;

            // Intercambio (Swap) de la estructura Songs
            Songs aux = albumes->cancion[al_i][so_i];
            albumes->cancion[al_i][so_i] = albumes->cancion[al_j][so_j];
            albumes->cancion[al_j][so_j] = aux;
        }
    }

    // Colocar el pivote en su posición final correcta
    int al_i = (i + 1) / Cant_So;
    int so_i = (i + 1) % Cant_So;

    Songs aux = albumes->cancion[al_i][so_i];
    albumes->cancion[al_i][so_i] = albumes->cancion[al_p][so_p];
    albumes->cancion[al_p][so_p] = aux;

    int pi = i + 1;

    // Llamadas recursivas
    QuickSort_Texto(albumes, bajo, pi - 1, campo);
    QuickSort_Texto(albumes, pi + 1, alto, campo);
}

/**
 * @brief
 * 
 * @param albumes
 * @param bajo
 * @param alto
 * @param campo
 */

void BubbleSort_Numerico(Album *albumes, int campo)
{
    int total = Cant_Al * Cant_So; // 375 elementos

    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            // Traducimos los índices j y j+1 a coordenadas (álbum, canción)
            int al_1 = j / Cant_So;
            int so_1 = j % Cant_So;

            int al_2 = (j + 1) / Cant_So;
            int so_2 = (j + 1) % Cant_So;

            bool intercambiar = false;

            // Evaluamos según el campo numérico
            if (campo == 1 && albumes->cancion[al_1][so_1].id > albumes->cancion[al_2][so_2].id)
            {
                intercambiar = true;
            }
            else if (campo == 5 && albumes->cancion[al_1][so_1].duracion_s > albumes->cancion[al_2][so_2].duracion_s)
            {
                intercambiar = true;
            }
            else if (campo == 6 && albumes->cancion[al_1][so_1].anio > albumes->cancion[al_2][so_2].anio)
            {
                intercambiar = true;
            }
            else if (campo == 7 && albumes->cancion[al_1][so_1].n_reprodcciones > albumes->cancion[al_2][so_2].n_reprodcciones)
            {
                intercambiar = true;
            }

            // Realizamos el Swap de la estructura completa
            if (intercambiar)
            {
                Songs aux = albumes->cancion[al_1][so_1];
                albumes->cancion[al_1][so_1] = albumes->cancion[al_2][so_2];
                albumes->cancion[al_2][so_2] = aux;
            }
        }
    }
}

/**
 * @brief
 * 
 * @param albumes
 * @param inicio
 * @param fin
 * @param opcion
 * @param clave_busqueda
 * @param id_buscado
 * @return int
 */

int Busqueda_Binaria(Album *albumes, int inicio, int fin, int opcion, const char *clave_busqueda, int id_buscado)
{
    if (inicio > fin)
    {
        return -1; // No encontrada
    }

    int medio = inicio + (fin - inicio) / 2;

    int al_m = medio / Cant_So;
    int so_m = medio % Cant_So;

    int comparacion = 0;

    switch (opcion)
    {
        case 1: // BÚSQUEDA POR ID (NUMÉRICA)
            if (albumes->cancion[al_m][so_m].id == id_buscado)
                return medio;
            
            if (id_buscado < albumes->cancion[al_m][so_m].id)
                return Busqueda_Binaria(albumes, inicio, medio - 1, opcion, clave_busqueda, id_buscado);
            else
                return Busqueda_Binaria(albumes, medio + 1, fin, opcion, clave_busqueda, id_buscado);

        case 2: // BÚSQUEDA POR TÍTULO (TEXTO)
            comparacion = strcmp(clave_busqueda, albumes->cancion[al_m][so_m].titulo);
            break;

        case 3: // BÚSQUEDA POR ARTISTA (TEXTO)
            comparacion = strcmp(clave_busqueda, albumes->cancion[al_m][so_m].artista);
            break;

        default:
            return -1;
    }

    // Evaluación para las opciones de Texto (Casos 2 y 3)
    if (comparacion == 0)
    {
        return medio;
    }
    else if (comparacion < 0)
    {
        return Busqueda_Binaria(albumes, inicio, medio - 1, opcion, clave_busqueda, id_buscado);
    }
    else
    {
        return Busqueda_Binaria(albumes, medio + 1, fin, opcion, clave_busqueda, id_buscado);
    }
}
