#include"mappa.h"
#include <stdlib.h>

// Definizione struct completa (privata)
struct Mappa {
    char celle[ALTEZZA_MAPPA][LARGHEZZA_MAPPA];
};

// Costruttore
Mappa* mappa_crea(void) {
    Mappa* m = (Mappa*)malloc(sizeof(Mappa));
    if (m == NULL) {
        return NULL;
    }
    
    // Inizializza tutte le celle a muro
    for(int y = 0; y < ALTEZZA_MAPPA; y++) {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++) {
            m->celle[y][x] = '#';
        }
    }
    
    return m;
}

// Distruttore
void mappa_distruggi(Mappa* m) {
    if (m != NULL) {
        free(m);
    }
}

// Getter cella
char mappa_get_cella(Mappa* m, int y, int x) {
    if (m == NULL || y < 0 || y >= ALTEZZA_MAPPA || x < 0 || x >= LARGHEZZA_MAPPA) {
        return '#'; // Ritorna muro se fuori bounds
    }
    return m->celle[y][x];
}

// Setter cella
void mappa_set_cella(Mappa* m, int y, int x, char valore) {
    if (m != NULL && y >= 0 && y < ALTEZZA_MAPPA && x >= 0 && x < LARGHEZZA_MAPPA) {
        m->celle[y][x] = valore;
    }
}

// Generazione livello con algoritmo RANDOM WALKER
void mappa_genera_livello(Mappa* m) {
    if (m == NULL) {
        return;
    }
    
    // Riempie tutto con muri
    for(int y = 0; y < ALTEZZA_MAPPA; y++) {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++) {
            m->celle[y][x] = '#';
        }
    }

    // Coordinate del minatore casuali
    int miner_x = 2 + rand () % 78;
    int miner_y = 2 + rand () % 39;
    m->celle[miner_y][miner_x] = '.';

    // Scavo casuale per 8000 passi
    for (int i = 0; i < 8000; i++) {
        int direzione = rand () % 4;
        int temp_miner_x = miner_x;
        int temp_miner_y = miner_y;

        switch(direzione) {
            case 0:
                temp_miner_y--;
                break;

            case 1:
                temp_miner_y++;
                break;

            case 2:
                temp_miner_x--;
                break;

            case 3:
                temp_miner_x++;
                break;

            default:
                break;
        }

        // Resto nei bordi lasciando una cornice di 2
        if(temp_miner_x > 1 && temp_miner_y > 1 && temp_miner_x < LARGHEZZA_MAPPA - 2 && temp_miner_y < ALTEZZA_MAPPA - 2) {
            miner_x = temp_miner_x;
            miner_y = temp_miner_y;
            m->celle[miner_y][miner_x] = '.';
        }
    }
}