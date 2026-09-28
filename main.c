#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "Reproductor.h"

int main()
{
    Album albumes;

    Generar(&albumes);
    Lista_albumes(&albumes);

    return 0;
}
