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

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            variable=rand()%100000;
            
            albumes->cancion[i][j].id=variable;
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            variable=(rand()%100)+120;
            
            albumes->cancion[i][j].duracion_s=variable;
        }
    }
    
    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            variable=(rand()%9)+2000;
            
            albumes->cancion[i][j].anio=variable;
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            variable=rand()%1000000;
            
            albumes->cancion[i][j].n_reprodcciones=variable;
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            variable=rand()%1000000;
            
            albumes->cancion[i][j].n_reprodcciones=variable;
        }
    }

    
}
