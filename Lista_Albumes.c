/**
 * @file
 * @brief
 */

#include "Reproductor.h"

void Ordenar(Album *albumes, int elec_al, int elec_org);

void Lista_albumes(Album *albumes)
{
    int elec_al = 0;
    int elec_org = 0;

    printf("Que album desea escuchar:\n");

    for (int i = 0; i < CANT_AL; i++)
    {
        printf("%d°%s\n", i + 1, albumes->cancion[elec_al][i].albums);
    }

    printf("\t>>>");
    scanf("%d", &elec_al);

    while (elec_al < 1 || elec_al > 5)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        printf("\t>>>");
        scanf("%d", &elec_al);
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
    scanf("%d", &elec_org);

    while (elec_org < 1 || elec_org > 7)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        printf("\t>>>");
        scanf("%d", &elec_org);
    }

    Ordenar(albumes, elec_al, elec_org);

    switch (elec_org)
    {
        case 1:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("id:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 2:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Titulo:%s|\tid:%d|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 3:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Artista:%s|\tid:%d|\tTitulo:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 4:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Genero:%s|\tid:%d|\tTitulo:%s|\tArtista:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 5:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Duracion en segundos:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tAño de lanzamiento:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 6:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Año de lanzamiento:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tNumero de reproducciones:%d\n",
                       albumes->cancion[elec_al][i].anio,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].n_reprodcciones);
            }
            break;
        case 7:
            for (int i = 0; i < CANT_SO; i++)
            {
                printf("\n%d°", i + 1);
                printf("Numero de reproducciones:%d|\tid:%d|\tTitulo:%s|\tArtista:%s|\tGenero:%s|\tDuracion en segundos:%d|\tAño de lanzamiento:%d\n",
                       albumes->cancion[elec_al][i].n_reprodcciones,
                       albumes->cancion[elec_al][i].id,
                       albumes->cancion[elec_al][i].titulo,
                       albumes->cancion[elec_al][i].artista,
                       albumes->cancion[elec_al][i].genero,
                       albumes->cancion[elec_al][i].duracion_s,
                       albumes->cancion[elec_al][i].anio);
            }
            break;
    }

    printf("\t>>>");
    scanf("%d", &albumes->Eleccion);
}

void Ordenar(Album *albumes, int elec_al, int elec_org)
{
    for (int j = 0; j < CANT_SO - 1; j++)
    {
        for (int i = 0; i < CANT_SO - 1; i++)
        {
            switch (elec_org)
            {
                case 1:
                    if (albumes->cancion[elec_al][i].id > albumes->cancion[elec_al][i + 1].id)
                    {
                        int aux = albumes->cancion[elec_al][i].id;
                        albumes->cancion[elec_al][i].id = albumes->cancion[elec_al][i + 1].id;
                        albumes->cancion[elec_al][i + 1].id = aux;
                    }
                    break;
                case 2:
                    if (albumes->cancion[elec_al][i].titulo[0] > albumes->cancion[elec_al][i + 1].titulo[0])
                    {
                        char aux = albumes->cancion[elec_al][i].titulo[0];
                        albumes->cancion[elec_al][i].titulo[0] = albumes->cancion[elec_al][i + 1].titulo[0];
                        albumes->cancion[elec_al][i + 1].titulo[0] = aux;
                    }
                    break;
                case 3:
                    if (albumes->cancion[elec_al][i].artista[0] > albumes->cancion[elec_al][i + 1].artista[0])
                    {
                        char aux = albumes->cancion[elec_al][i].artista[0];
                        albumes->cancion[elec_al][i].artista[0] = albumes->cancion[elec_al][i + 1].artista[0];
                        albumes->cancion[elec_al][i + 1].artista[0] = aux;
                    }
                    break;
                case 4:
                    if (albumes->cancion[elec_al][i].genero[0] > albumes->cancion[elec_al][i + 1].genero[0])
                    {
                        char aux = albumes->cancion[elec_al][i].genero[0];
                        albumes->cancion[elec_al][i].genero[0] = albumes->cancion[elec_al][i + 1].genero[0];
                        albumes->cancion[elec_al][i + 1].genero[0] = aux;
                    }
                    break;
                case 5:
                    if (albumes->cancion[elec_al][i].duracion_s > albumes->cancion[elec_al][i + 1].duracion_s)
                    {
                        int aux = albumes->cancion[elec_al][i].duracion_s;
                        albumes->cancion[elec_al][i].duracion_s = albumes->cancion[elec_al][i + 1].duracion_s;
                        albumes->cancion[elec_al][i + 1].duracion_s = aux;
                    }
                    break;
                case 6:
                    if (albumes->cancion[elec_al][i].anio > albumes->cancion[elec_al][i + 1].anio)
                    {
                        int aux = albumes->cancion[elec_al][i].anio;
                        albumes->cancion[elec_al][i].anio = albumes->cancion[elec_al][i + 1].anio;
                        albumes->cancion[elec_al][i + 1].anio = aux;
                    }
                    break;
                case 7:
                    if (albumes->cancion[elec_al][i].n_reprodcciones > albumes->cancion[elec_al][i + 1].n_reprodcciones)
                    {
                        int aux = albumes->cancion[elec_al][i].n_reprodcciones;
                        albumes->cancion[elec_al][i].n_reprodcciones = albumes->cancion[elec_al][i + 1].n_reprodcciones;
                        albumes->cancion[elec_al][i + 1].n_reprodcciones = aux;
                    }
                    break;
            }
        }
    }
}
