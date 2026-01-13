/*Caratteristiche tecniche principali:
1. Generazione procedurale con algoritmo RANDOM WALKER
2. Uso di puntatori a matrice per il cambio del livello
3. IA dei nemici e pathfinding con algoritmo A*
4. Gestione stati, risorse, condizioni di vittoria/sconfitta e difficoltà variabile
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include"gioco.h"


int main (void)
{
    //setup ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand((unsigned)time(NULL));

    //definizione colori
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);

    Gioco* partita = gioco_init();
    if (!partita)
    {
        endwin();
        return -1;
    }

    gioco_draw(partita);

    int tasto;
    while ((tasto = getch()) != 'q')
    {
        int continua = gioco_update(partita, tasto);

        if (!continua) break;

        gioco_draw(partita);
    }

    gioco_free(partita);
    endwin();
    return 0;
}