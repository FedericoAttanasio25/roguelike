#ifndef ILMIOGIOCO_ENTITÀ_H
#define ILMIOGIOCO_ENTITÀ_H

#include "mappa.h"

// Struct opache
typedef struct Giocatore Giocatore;
typedef struct Nemico Nemico;

// === GIOCATORE ===
// Costruttore e distruttore
Giocatore* giocatore_crea(int y, int x);
void giocatore_distruggi(Giocatore* g);

// Getter
int giocatore_get_y(Giocatore* g);
int giocatore_get_x(Giocatore* g);
int giocatore_get_tasca(Giocatore* g);
int giocatore_get_arma(Giocatore* g);
char giocatore_get_icon(Giocatore* g);

// Setter
void giocatore_set_posizione(Giocatore* g, int y, int x);
void giocatore_set_tasca(Giocatore* g, int valore);
void giocatore_set_arma(Giocatore* g, int valore);

// === NEMICO ===
// Costruttore e distruttore
Nemico* nemico_crea(int y, int x);
void nemico_distruggi(Nemico* n);

// Getter
int nemico_get_y(Nemico* n);
int nemico_get_x(Nemico* n);
char nemico_get_icon(Nemico* n);
int nemico_is_vivo(Nemico* n);

// Setter
void nemico_set_posizione(Nemico* n, int y, int x);
void nemico_uccidi(Nemico* n);

// === FUNZIONI DI GIOCO ===
void muovi_nemici(Mappa* m, Giocatore* p, Nemico* n[], int num_nemici);

#endif //ILMIOGIOCO_ENTITÀ_H