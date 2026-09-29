#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Reproductor.h"

int main(void)
{
    Album albumes;

    Generar(&albumes);
    Lista_albumes(&albumes);
    iniciar_interfaz(&albumes);

    return 0;
}
