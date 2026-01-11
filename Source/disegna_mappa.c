#include "disegna_mappa.h"
#include <ncurses.h>

void disegna_mappa(Mappa* m, Giocatore* p, char* msg, Nemico* n[], int num_nemici)
{
    if (m == NULL || p == NULL) {
        return;
    }
    
    clear();

    // Disegno mappa statica
    for(int y = 0; y < ALTEZZA_MAPPA; y++)
    {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++)
        {
            char carattere = mappa_get_cella(m, y, x);
            switch(carattere)
            {
                case '#':
                    attron(COLOR_PAIR(3));
                    break;

                case 'x':
                    attron(COLOR_PAIR(1));
                    break;

                case '.':
                    attron(COLOR_PAIR(2));
                    break;

                case 'd':
                    attron(COLOR_PAIR(5));
                    break;

                case 'W':
                    attron(COLOR_PAIR(1));
                    break;

                case 's':
                    attron(COLOR_PAIR(1));
                    break;

                case 'k':
                    attron(COLOR_PAIR(5));
                    break;

                default:
                    break;
            }
            mvaddch(y, x, carattere);
            attrset(A_NORMAL);
        }
    }

    // Ciclo per stampare i nemici
    attron(COLOR_PAIR(1));
    for(int i = 0; i < num_nemici; i++)
    {
        if(nemico_is_vivo(n[i]))
        {
            mvaddch(nemico_get_y(n[i]), nemico_get_x(n[i]), nemico_get_icon(n[i]));
        }
    }
    attroff(COLOR_PAIR(1));

    // Disegno giocatore
    attron(COLOR_PAIR(4));
    mvaddch(giocatore_get_y(p), giocatore_get_x(p), giocatore_get_icon(p));
    attroff(COLOR_PAIR(4));

    mvprintw(ALTEZZA_MAPPA + 1, 0, "Status: %s", msg);
    mvprintw(ALTEZZA_MAPPA + 2, 0, "Tasca: %s", giocatore_get_tasca(p) ? "Piena (Chiave)" : "Vuota");
    mvprintw(ALTEZZA_MAPPA + 3, 0, "Arma: %s", giocatore_get_arma(p) ? "SI" : "No");
    refresh();
}