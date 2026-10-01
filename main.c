#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<string.h>
#include "Reproductor.h"

bool Cargar_CSV(Album *albumes, const char *nombre_archivo);
void Guardar_CSV(Album *albumes, const char *nombre_archivo);

int main()
{
    Album albumes;
    FILE *archivo=fopen("catalogo.csv","r");
    albumes.ciclo=true;
    albumes.cont=0;
    
    const char *nombre_archivo = "catalogo.csv";

    if(archivo==NULL)
    {
        Generar(&albumes);
        Guardar_CSV(&albumes, nombre_archivo);
    }
    else
    {
        fclose(archivo);
        Cargar_CSV(&albumes, "catalogo.csv");
    }
    Lista_albumes(&albumes);
    while(albumes.ciclo==true)
    {
        Fila_de_Reproduccion(&albumes);
    }

    Guardar_CSV(&albumes, nombre_archivo);

    return 0;
}

/**
 * @brief
 * 
 * @param albumes
 * @param nombre_archivo
 */

void Guardar_CSV(Album *albumes, const char *nombre_archivo)
{
    FILE *archivo = fopen(nombre_archivo, "w");
    
    if (archivo == NULL)
    {
        printf("Error al crear o abrir el archivo CSV.\n");
        return;
    }

    fprintf(archivo, "id,titulo,artista,album,genero,duracion_seg,anio,n_reproducciones\n");

    for (int i = 0; i < Cant_Al; i++)
    {
        for (int j = 0; j < Cant_So; j++)
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
    printf("Archivo '%s' creado y guardado con éxito.\n", nombre_archivo);
}

/**
 * @brief
 * 
 * @param albumes
 * @param nombre_archivo
 */

bool Cargar_CSV(Album *albumes, const char *nombre_archivo)
{
    // fopen con "r" intenta abrir el archivo para lectura
    FILE *archivo = fopen(nombre_archivo, "r");
    
    if (archivo == NULL)
    {
        return false; // El archivo no existe todavía
    }

    char linea[256];
    
    // Leemos la primera línea (cabecera) para descartarla
    fgets(linea, sizeof(linea), archivo);

    // Leemos el resto de las canciones y las cargamos a la matriz
    for (int i = 0; i < Cant_Al; i++)
    {
        for (int j = 0; j < Cant_So; j++)
        {
            if (fgets(linea, sizeof(linea), archivo) != NULL)
            {
                sscanf(linea, "%d,%10[^,],%10[^,],%10[^,],%10[^,],%d,%d,%d",
                       &albumes->cancion[i][j].id,
                       albumes->cancion[i][j].titulo,
                       albumes->cancion[i][j].artista,
                       albumes->cancion[i][j].albums,
                       albumes->cancion[i][j].genero,
                       &albumes->cancion[i][j].duracion_s,
                       &albumes->cancion[i][j].anio,
                       &albumes->cancion[i][j].n_reprodcciones);

                albumes->cancion[i][j].past.reproducido = false;
                albumes->cancion[i][j].past.Cant_Re = 0;
            }
        }
    }

    fclose(archivo);
    return true; // Carga exitosa
}
