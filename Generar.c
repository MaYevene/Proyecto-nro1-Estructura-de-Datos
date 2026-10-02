/**
 * @file
 * @brief
 */

 #include "Reproductor.h"

/* Aporte: Eban Delgado - generación y mezcla Fisher-Yates del catálogo. */

static void generar_texto(char texto[11])
{
    int i;

    for (i = 0; i < 10; i++)
    {
        texto[i] = (rand() % 26) + 'a';
    }
    texto[10] = '\0';
}

 /**
  * @brief
  * 
  * @param albumes
  */

void Generar(Album *albumes)
{
    int i;
    int j;
    int posicion;

    srand(time(0));

    for (i = 0; i < Cant_Al; i++)
    {
        for (j = 0; j < Cant_So; j++)
        {
            albumes->cancion[i][j].id = i * Cant_So + j + 1;
            albumes->cancion[i][j].duracion_s = (rand() % 100) + 120;
            albumes->cancion[i][j].anio = (rand() % 9) + 2000;
            albumes->cancion[i][j].n_reprodcciones = rand() % 1000000;
            generar_texto(albumes->cancion[i][j].titulo);
            generar_texto(albumes->cancion[i][j].artista);
            generar_texto(albumes->cancion[i][j].genero);
        }

        generar_texto(albumes->cancion[i][0].albums);
        for (j = 1; j < Cant_So; j++)
        {
            strcpy(albumes->cancion[i][j].albums, albumes->cancion[i][0].albums);
        }

        for (j = Cant_So - 1; j > 0; j--)
        {
            Songs temporal;
            posicion = rand() % (j + 1);
            temporal = albumes->cancion[i][j];
            albumes->cancion[i][j] = albumes->cancion[i][posicion];
            albumes->cancion[i][posicion] = temporal;
        }
    }
}
