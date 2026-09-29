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

void Generar(Album *albumes)
{
    int variable;

    srand(time(0));

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            variable = rand() % 100000;
            albumes->cancion[i][j].id = variable;
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            variable = (rand() % 100) + 120;
            albumes->cancion[i][j].duracion_s = variable;
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            variable = (rand() % 9) + 2000;
            albumes->cancion[i][j].anio = variable;
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            variable = rand() % 1000000;
            albumes->cancion[i][j].n_reprodcciones = variable;
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            variable = rand() % 1000000;
            albumes->cancion[i][j].n_reprodcciones = variable;
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                char chac = (rand() % 26) + 97;
                if (k <= 19)
                {
                    albumes->cancion[i][j].albums[k] = chac;
                }
                else
                {
                    albumes->cancion[i][j].albums[k] = '\0';
                }
            }
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                char chac = (rand() % 26) + 97;
                if (k <= 19)
                {
                    albumes->cancion[i][j].titulo[k] = chac;
                }
                else
                {
                    albumes->cancion[i][j].titulo[k] = '\0';
                }
            }
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                char chac = (rand() % 26) + 97;
                if (k <= 19)
                {
                    albumes->cancion[i][j].genero[k] = chac;
                }
                else
                {
                    albumes->cancion[i][j].genero[k] = '\0';
                }
            }
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            for (int k = 0; k < 10; k++)
            {
                char chac = (rand() % 26) + 97;
                if (k <= 19)
                {
                    albumes->cancion[i][j].artista[k] = chac;
                }
                else
                {
                    albumes->cancion[i][j].artista[k] = '\0';
                }
            }
        }
    }

    for (int i = 0; i < CANT_AL; i++)
    {
        for (int j = 0; j < CANT_SO; j++)
        {
            albumes->cancion[i][j].albums[10] = '\0';
            albumes->cancion[i][j].titulo[10] = '\0';
            albumes->cancion[i][j].genero[10] = '\0';
            albumes->cancion[i][j].artista[10] = '\0';
        }
    }
}
