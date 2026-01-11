/*Caratteristiche tecniche principali:
1. Generazione procedurale con algoritmo RANDOM WALKER
2. Uso di puntatori a matrice per il cambio del livello
3. IA dei nemici
4. Gestione stati con inventario, vite nemici, condizioni di vittoria/sconfitta
*/

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mappa.h"
#include "entità.h"
#include "disegna_mappa.h"


char mappa[ALTEZZA_MAPPA][LARGHEZZA_MAPPA]; //Livello 1 
char mappa_s[ALTEZZA_MAPPA][LARGHEZZA_MAPPA]; //Segrete

//Puntatore a matrice che permette di scambiare il livello 1 con le segrete
char(*p_mappa)[LARGHEZZA_MAPPA];


int main (void)
{
    //setup ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand((unsigned)time(NULL));

    //Inizializzo il puntatore sulla mappa principale
    p_mappa = mappa;

    genera_livello(p_mappa);

    //generazione mappa segreta
    genera_livello(mappa_s);
    int riga_casuale_s2, colonna_casuale_s2;
    do
    {
        riga_casuale_s2 = rand () % ALTEZZA_MAPPA;
        colonna_casuale_s2 = rand () % LARGHEZZA_MAPPA;
    } while (mappa_s[riga_casuale_s2][colonna_casuale_s2] != '.');
    mappa_s[riga_casuale_s2][colonna_casuale_s2] = 's'; 

    int riga_casuale_W, colonna_casuale_W;
    do
    {
        riga_casuale_W = rand () % ALTEZZA_MAPPA;
        colonna_casuale_W = rand () % LARGHEZZA_MAPPA;
    } while (mappa_s[riga_casuale_W][colonna_casuale_W] != '.');
    mappa_s[riga_casuale_W][colonna_casuale_W] = 'W';
    //fine generazione mappa segreta

    //generazione scatola della x
    int r_centro, c_centro;
    do
    {
        r_centro = 3 + rand () % (ALTEZZA_MAPPA - 6); //utilizzo 3 come margine di sicurezza
        c_centro = 3 + rand () % (LARGHEZZA_MAPPA - 6);
    } while (p_mappa[r_centro][c_centro] != '.');

    //costruzione mura 3x3
    for (int y = r_centro - 1; y <= r_centro + 1; y ++)
    {
        for (int x = c_centro - 1; x <= c_centro + 1; x++)
        {
            p_mappa[y][x] = '#';
        }
    }
    p_mappa[r_centro][c_centro] = 'x';

    //porta d su un lato casuale e nella casella davanti metto pavimento
    int lato = rand () % 4;
    switch (lato)
    {
        case 0:
        p_mappa[r_centro - 1][c_centro] = 'd';
        p_mappa[r_centro - 2][c_centro] = '.';
        break;

        case 1:
        p_mappa[r_centro + 1][c_centro] = 'd';
        p_mappa[r_centro + 2][c_centro] = '.';
        break;

        case 2:
        p_mappa[r_centro][c_centro - 1] = 'd';
        p_mappa[r_centro][c_centro - 2] = '.';
        break;

        case 3:
        p_mappa[r_centro][c_centro + 1] = 'd';
        p_mappa[r_centro][c_centro + 2] = '.';
        break;

        default:
        break;
    }
    //fine generazione scatola per la x

    int riga_casuale, colonna_casuale;

    //posizionamento giocatore
    do 
    {
        riga_casuale = rand () % ALTEZZA_MAPPA;
        colonna_casuale = rand () % LARGHEZZA_MAPPA;
    }while(p_mappa[riga_casuale][colonna_casuale] != '.');

    giocatore player;
    player.y = riga_casuale;
    player.x = colonna_casuale;
    player.icon = '@';
    player.tasca = 0;
    player.arma = 0;

    nemico lista_nemici[3];
    int riga_casuale_n, colonna_casuale_n;
    int distanza_sicurezza_y, distanza_sicurezza_x;

    //posizionamento nemici
    for(size_t i = 0; i < 3; i++)
    {
        do
        {
            riga_casuale_n = rand () % ALTEZZA_MAPPA;
            colonna_casuale_n = rand () % LARGHEZZA_MAPPA;

            //calcolo distanza assoluta
            distanza_sicurezza_y = abs(riga_casuale_n - player.y);
            distanza_sicurezza_x = abs(colonna_casuale_n - player.x);
        } while (p_mappa[riga_casuale_n][colonna_casuale_n] != '.' || (distanza_sicurezza_y < 7 && distanza_sicurezza_x < 15));
        
        lista_nemici[i].y_e = riga_casuale_n;
        lista_nemici[i].x_e = colonna_casuale_n;
        lista_nemici[i].icon_e = 'E';
    }

    int riga_casuale_k, colonna_casuale_k;
    do
    {
        riga_casuale_k = rand () % ALTEZZA_MAPPA;
        colonna_casuale_k = rand () % LARGHEZZA_MAPPA;
    } while (p_mappa[riga_casuale_k][colonna_casuale_k] != '.');
    p_mappa[riga_casuale_k][colonna_casuale_k] = 'k';

    int riga_casuale_s, colonna_casuale_s;
    do
    {
        riga_casuale_s = rand () % ALTEZZA_MAPPA;
        colonna_casuale_s = rand () % LARGHEZZA_MAPPA;
    } while (p_mappa[riga_casuale_s][colonna_casuale_s] != '.');
    p_mappa[riga_casuale_s][colonna_casuale_s] = 's';

    //definizione colori
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    int input_tasto = 0;
    char messaggio_status[100] = "Trova la chiave per uscire!";

    disegna_mappa(p_mappa, player, messaggio_status, lista_nemici);

    //game loop
    while((input_tasto = getch()) != 'q')
    {
        int new_y = player.y;
        int new_x = player.x;

        switch(input_tasto)
        {
            case KEY_UP:
            new_y--;
            break;

            case KEY_DOWN:
            new_y++;
            break;

            case KEY_LEFT:
            new_x--;
            break;

            case KEY_RIGHT:
            new_x++;
            break;

            default:
            break;
        }

        //variabile che serve a capire cosa c'è nella casella di destinazione
        char destinazione = p_mappa[new_y][new_x];

        if (destinazione == 's')
        {
            if (p_mappa == mappa)
            {
                p_mappa = mappa_s;
                strcpy(messaggio_status, "Sei sceso nelle segrete");
            }

            else 
            {
                p_mappa = mappa;
                strcpy(messaggio_status, "Sei tornato in superficie");
            }

            //cerco scale nella nuova mappa per metterci sopra il giocatore
            for(int y = 0; y < ALTEZZA_MAPPA; y++)         
            {
                for(int x = 0; x < LARGHEZZA_MAPPA; x++) 
                {
                    if(p_mappa[y][x] == 's') 
                    {
                        player.y = y;
                        player.x = x;
                    }
                }
            }

            new_y = player.y;
            new_x = player.x;

            //evito che i nemici si generino nei muri della nuova mappa
            int riga_casuale_ns, colonna_casuale_ns;
            int distanza_sicurezza_ys, distanza_sicurezza_xs;

            for(size_t i = 0; i < 3; i++)
            {
                do
                {
                    riga_casuale_ns = rand () % ALTEZZA_MAPPA;
                    colonna_casuale_ns = rand () % LARGHEZZA_MAPPA;

                    distanza_sicurezza_ys = abs(riga_casuale_ns - player.y);
                    distanza_sicurezza_xs = abs(colonna_casuale_ns - player.x);
                } while (p_mappa[riga_casuale_ns][colonna_casuale_ns] != '.' || (distanza_sicurezza_ys < 5 && distanza_sicurezza_xs < 5));
        
                lista_nemici[i].y_e = riga_casuale_ns;
                lista_nemici[i].x_e = colonna_casuale_ns;
            }

            clear();
        }

        else if (destinazione == 'k')
        {
            player.tasca = 1;
            p_mappa[new_y][new_x] = '.';
            player.x = new_x;
            player.y = new_y;
            strcpy(messaggio_status, "Hai preso la chiave!");
        }

        else if (destinazione == 'W')
        {
            player.arma = 1;
            p_mappa[new_y][new_x] = '.';
            player.x = new_x;
            player.y = new_y;
            strcpy(messaggio_status, "Hai preso un'arma!");
        }

        else if (destinazione == 'd')
        {
            if (player.tasca == 1)
            {
                p_mappa[new_y][new_x] = '.';
                player.x = new_x;
                player.y = new_y;
                strcpy(messaggio_status, "Porta aperta!");
            }

            else 
            {
                strcpy(messaggio_status, "Serve la chiave!");
            }
        }

        else if(destinazione== 'x'|| destinazione == 'X')
        {
            clear();
            attron(COLOR_PAIR(2));
            mvprintw(ALTEZZA_MAPPA / 2, (LARGHEZZA_MAPPA / 2) - 10, "HAI VINTO! Premi un tasto per uscire...");
            attroff(COLOR_PAIR(2));
            refresh();
            getch();
            break;
        }

        //logica della collisione con i nemici
        int nemico_toccato = -1; //nessuno
        for (int i = 0; i < 3; i++)
        {   
            //controllo se la casella futura è occupata da un nemico VIVO
            if (lista_nemici[i].x_e != -1 && new_x == lista_nemici[i].x_e && new_y == lista_nemici[i].y_e)
            {
                nemico_toccato = i;
            }
        }

        if (nemico_toccato != -1)
        {
            if (player.arma == 1)
            {
                lista_nemici[nemico_toccato].x_e = -1; //-1 indica la morte
                lista_nemici[nemico_toccato].y_e = -1;

                player.x = new_x;
                player.y = new_y;
                strcpy(messaggio_status, "Nemico sconfitto!");
            }

            else 
            {
                clear();
                attron(COLOR_PAIR(1));
                mvprintw(ALTEZZA_MAPPA / 2, (LARGHEZZA_MAPPA / 2) - 10, "SEI STATO PRESO! Premi un tasto per uscire...");
                attroff(COLOR_PAIR(1));
                refresh();
                getch();
                return 0;
            }
        
        }

        //se è pavimento (o era un oggetto preso) e non è muro
        else if (destinazione == '.' && p_mappa[new_y][new_x] != '#')
        {
            player.x = new_x;
            player.y = new_y;
            strcpy(messaggio_status, ""); //reset del messaggio di status 
        }

        //IA dei nemici
        muovi_nemici(p_mappa, player, lista_nemici);

        for (int i = 0; i < 3; i++)
        {
            if (lista_nemici[i].x_e != -1 && player.y == lista_nemici[i].y_e && player.x == lista_nemici[i].x_e)
            {
                if(player.arma == 1)
                {
                    lista_nemici[i].x_e = -1;
                    lista_nemici[i].y_e = -1;
                    strcpy(messaggio_status, "Nemico sconfitto!");
                }

                else
                {
                    clear();
                    attron(COLOR_PAIR(1));
                    mvprintw(ALTEZZA_MAPPA / 2, (LARGHEZZA_MAPPA / 2) - 10, "SEI STATO PRESO! Premi un tasto per uscire...");
                    attroff(COLOR_PAIR(1));
                    refresh();
                    getch();
                    return 0;
                }
            }
        }

        //disegna il frame finale
        disegna_mappa(p_mappa, player, messaggio_status, lista_nemici);
    }

    endwin();
    return 0;
}


//mettere una difficoltà
//durata delle armi e armi diverse 
//area di visione dei nemici