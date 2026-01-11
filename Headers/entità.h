#ifndef ILMIOGIOCO_ENTITÀ_H
#define ILMIOGIOCO_ENTITÀ_H

#include "mappa.h"

typedef struct
{
    int y;
    int x;
    int tasca;  //tasca funzionerà se è piena (1) o vuota (0), se il personaggio ha la chiave o no.
    int arma;   //stesso funzionamento di tasca.
    char icon;
}giocatore;

typedef struct
{
    int y_e;
    int x_e;
    char icon_e;
}nemico;

void muovi_nemici(char map [][LARGHEZZA_MAPPA], giocatore p, nemico n[]);

#endif //ILMIOGIOCO_ENTITÀ_H