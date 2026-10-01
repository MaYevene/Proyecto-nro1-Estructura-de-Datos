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

void Mezclar_Fisher_Yates(Album *albumes);

void Generar(Album *albumes)
{
    int variable;

    srand(time(0));

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            albumes->cancion[i][j].past.reproducido=false;
            albumes->cancion[i][j].past.Cant_Re=0;
        }
    }

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
            
            albumes->cancion[i][j].n_reprodcciones=variable ;
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            for(int k=0;k<11;k++)
            {
                char chac=(rand()%26)+97;
                if(k<=9)
                {
                    albumes->cancion[i][j].albums[k]=chac;
                }
                else
                {
                    albumes->cancion[i][j].albums[k]='\0';
                }
            }
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            for(int k=0;k<11;k++)
            {
                char chac=(rand()%26)+97;
                if(k<=9)
                {
                    albumes->cancion[i][j].titulo[k]=chac;
                }
                else
                {
                    albumes->cancion[i][j].titulo[k]='\0';
                }
            }
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            for(int k=0;k<11;k++)
            {
                char chac=(rand()%26)+97;
                if(k<=9)
                {
                    albumes->cancion[i][j].genero[k]=chac;
                }
                else
                {
                    albumes->cancion[i][j].genero[k]='\0';
                }
            }
        }
    }

    for(int i=0;i<Cant_Al;i++)
    {
        for(int j=0;j<Cant_So;j++)
        {
            for(int k=0;k<11;k++)
            {
                char chac=(rand()%26)+97;
                if(k<=9)
                {
                    albumes->cancion[i][j].artista[k]=chac;
                }
                else
                {
                    albumes->cancion[i][j].artista[k]='\0';
                }
            }
        }
    }

    Mezclar_Fisher_Yates(albumes);
}

 /**
  * @brief
  * 
  * @param albumes
  */

void Mezclar_Fisher_Yates(Album *albumes)
{
    int total=Cant_Al*Cant_So;

    for (int i=total-1;i>0;i--)
    {
        int j=rand()%(i+1);

        int al_i=i/Cant_So;
        int so_i=i%Cant_So;

        int al_j=j/Cant_So;
        int so_j=j%Cant_So;

        Songs aux=albumes->cancion[al_i][so_i];
        albumes->cancion[al_i][so_i]=albumes->cancion[al_j][so_j];
        albumes->cancion[al_j][so_j]=aux;
    }
}
