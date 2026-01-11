
#ifndef ILMIOGIOCO_MAPPA_H
#define ILMIOGIOCO_MAPPA_H

#define ALTEZZA_MAPPA 40
#define LARGHEZZA_MAPPA 80

// Struct opaca
typedef struct Mappa Mappa;

// Costruttore e distruttore
Mappa* mappa_crea(void);
void mappa_distruggi(Mappa* m);

// Funzioni di accesso
char mappa_get_cella(Mappa* m, int y, int x);
void mappa_set_cella(Mappa* m, int y, int x, char valore);

// Generazione livello
void mappa_genera_livello(Mappa* m);

#endif //ILMIOGIOCO_MAPPA_H