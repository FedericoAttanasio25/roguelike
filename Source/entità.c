#include "entità.h"
#include <stdlib.h>

// Definizione struct complete (private)
struct Giocatore {
    int y;
    int x;
    int tasca;  // 0 = vuota, 1 = chiave
    int arma;   // 0 = no arma, 1 = ha arma
    char icon;
};

struct Nemico {
    int y_e;
    int x_e;
    char icon_e;
};

// === IMPLEMENTAZIONE GIOCATORE ===

Giocatore* giocatore_crea(int y, int x) {
    Giocatore* g = (Giocatore*)malloc(sizeof(Giocatore));
    if (g == NULL) {
        return NULL;
    }
    
    g->y = y;
    g->x = x;
    g->tasca = 0;
    g->arma = 0;
    g->icon = '@';
    
    return g;
}

void giocatore_distruggi(Giocatore* g) {
    if (g != NULL) {
        free(g);
    }
}

int giocatore_get_y(Giocatore* g) {
    return (g != NULL) ? g->y : 0;
}

int giocatore_get_x(Giocatore* g) {
    return (g != NULL) ? g->x : 0;
}

int giocatore_get_tasca(Giocatore* g) {
    return (g != NULL) ? g->tasca : 0;
}

int giocatore_get_arma(Giocatore* g) {
    return (g != NULL) ? g->arma : 0;
}

char giocatore_get_icon(Giocatore* g) {
    return (g != NULL) ? g->icon : '@';
}

void giocatore_set_posizione(Giocatore* g, int y, int x) {
    if (g != NULL) {
        g->y = y;
        g->x = x;
    }
}

void giocatore_set_tasca(Giocatore* g, int valore) {
    if (g != NULL) {
        g->tasca = valore;
    }
}

void giocatore_set_arma(Giocatore* g, int valore) {
    if (g != NULL) {
        g->arma = valore;
    }
}

// === IMPLEMENTAZIONE NEMICO ===

Nemico* nemico_crea(int y, int x) {
    Nemico* n = (Nemico*)malloc(sizeof(Nemico));
    if (n == NULL) {
        return NULL;
    }
    
    n->y_e = y;
    n->x_e = x;
    n->icon_e = 'E';
    
    return n;
}

void nemico_distruggi(Nemico* n) {
    if (n != NULL) {
        free(n);
    }
}

int nemico_get_y(Nemico* n) {
    return (n != NULL) ? n->y_e : -1;
}

int nemico_get_x(Nemico* n) {
    return (n != NULL) ? n->x_e : -1;
}

char nemico_get_icon(Nemico* n) {
    return (n != NULL) ? n->icon_e : 'E';
}

int nemico_is_vivo(Nemico* n) {
    return (n != NULL && n->x_e != -1);
}

void nemico_set_posizione(Nemico* n, int y, int x) {
    if (n != NULL) {
        n->y_e = y;
        n->x_e = x;
    }
}

void nemico_uccidi(Nemico* n) {
    if (n != NULL) {
        n->y_e = -1;
        n->x_e = -1;
    }
}

// === FUNZIONI DI GIOCO ===

void muovi_nemici(Mappa* m, Giocatore* p, Nemico* n[], int num_nemici) {
    if (m == NULL || p == NULL || n == NULL) {
        return;
    }
    
    for(int i = 0; i < num_nemici; i++) {
        // Se il nemico è morto salta il ciclo
        if (!nemico_is_vivo(n[i])) {
            continue;
        }

        int current_y = nemico_get_y(n[i]);
        int current_x = nemico_get_x(n[i]);
        int new_y_e = current_y;
        int new_x_e = current_x;

        // Tiro un dado con la possibilità del 25% di far fare al nemico una mossa casuale
        int dado = rand() % 4;
        if (dado == 0) {
            // Mossa casuale del nemico
            int mossa_casuale = rand() % 4;
            switch (mossa_casuale) {
                case 0:
                    new_y_e--;
                    break;
                case 1:
                    new_y_e++;
                    break;
                case 2:
                    new_x_e--;
                    break;
                case 3:
                    new_x_e++;
                    break;
                default:
                    break;
            }
        }
        else {
            int player_y = giocatore_get_y(p);
            int player_x = giocatore_get_x(p);
            int player_arma = giocatore_get_arma(p);
            
            // Se il giocatore ha l'arma i nemici scappano sempre
            if(player_arma == 1) {
                // Calcolo direzione y (scappa)
                if(player_y > current_y) {
                    new_y_e = current_y - 1;
                }
                else if (player_y < current_y) {
                    new_y_e = current_y + 1;
                }

                // Calcolo direzione x (scappa)
                if(player_x > current_x) {
                    new_x_e = current_x - 1;
                }
                else if (player_x < current_x) {
                    new_x_e = current_x + 1;
                }
            }
            else {
                // Calcolo direzione y (insegue)
                if(player_y > current_y) {
                    new_y_e = current_y + 1;
                }
                else if (player_y < current_y) {
                    new_y_e = current_y - 1;
                }

                // Calcolo direzione x (insegue)
                if(player_x > current_x) {
                    new_x_e = current_x + 1;
                }
                else if (player_x < current_x) {
                    new_x_e = current_x - 1;
                }
            }
        }

        // Collisione con il muro
        if (mappa_get_cella(m, new_y_e, new_x_e) != '#') {
            nemico_set_posizione(n[i], new_y_e, new_x_e);
        }
    }
}