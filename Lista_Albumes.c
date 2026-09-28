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

void Ordenar(Album *albumes, int elec_al);

void Lista_albumes(Album *albumes)
{
    int elec_al=0, elec_org=0;

    printf("Que album desea escuchar:\n");

    printf("1° Finisterra\n");
    printf("2° Gaia\n");
    printf("3° Wintersaga\n");
    printf("4° Sonic Firestorm\n");
    printf("5° Dawn of Victory\n");

    scanf("%d",&elec_al);

    while (elec_al<1 || elec_al>5)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        scanf("%d",&elec_al);
    }

    printf("Indique el filtro de busqueda");
    
    printf("1° id\n");
    printf("2° Titulo\n");
    printf("3° artista\n");
    printf("4° Genero\n");
    printf("5° Duracion en segundos\n");
    printf("6° Año de lanzamiento\n");
    printf("7° Numero de reproducciones\n");

    scanf("%d",&elec_org);

    while (elec_org<1 || elec_org>7)
    {
        printf("La eleccion no esta entre los parametros posibles\nPor favor elija una opcion existentes");
        scanf("%d",&elec_org);
    }

    Ordenar(albumes,elec_al);

    for(int i=0;i<Cant_Al;i++)
    {
        printf("%d°",i+1);
        printf("id:%d|\tDuracion en segundos:%ds|\tNumero de reproducciones:%d|\tAño de lanzamiento:%d\n",albumes->cancion[elec_al][i].id,albumes->cancion[elec_al][i].duracion_s,albumes->cancion[elec_al][i].n_reprodcciones,albumes->cancion[elec_al][i].anio);
    }
}

/**
 * @brief
 * 
 * @param albumes
 */

void Ordenar(Album *albumes, int elec_al)
{
    for(int j=0;j<Cant_Al-1;j++)
    {
        for(int i=0;i<Cant_Al-1;i++)
        {
            if(albumes->cancion[elec_al][i].id>albumes->cancion[elec_al][i+1].id)
            {
                int aux=albumes->cancion[elec_al][i].id;
                albumes->cancion[elec_al][i].id=albumes->cancion[elec_al][i+1].id;
                albumes->cancion[elec_al][i+1].id=aux;
            }
        }
    }
}
