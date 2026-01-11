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


int main (void)
{
    //setup ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    srand((unsigned)time(NULL));

    //Creazione mappe con ADT
    Mappa* mappa_principale = mappa_crea();
    Mappa* mappa_segreta = mappa_crea();
    Mappa* mappa_corrente = mappa_principale;
    
    if (mappa_principale == NULL || mappa_segreta == NULL) {
        endwin();
        return 1;
    }

    mappa_genera_livello(mappa_principale);

    //generazione mappa segreta
    mappa_genera_livello(mappa_segreta);
    int riga_casuale_s2, colonna_casuale_s2;
    do
    {
        riga_casuale_s2 = rand () % ALTEZZA_MAPPA;
        colonna_casuale_s2 = rand () % LARGHEZZA_MAPPA;
    } while (mappa_get_cella(mappa_segreta, riga_casuale_s2, colonna_casuale_s2) != '.');
    mappa_set_cella(mappa_segreta, riga_casuale_s2, colonna_casuale_s2, 's'); 

    int riga_casuale_W, colonna_casuale_W;
    do
    {
        riga_casuale_W = rand () % ALTEZZA_MAPPA;
        colonna_casuale_W = rand () % LARGHEZZA_MAPPA;
    } while (mappa_get_cella(mappa_segreta, riga_casuale_W, colonna_casuale_W) != '.');
    mappa_set_cella(mappa_segreta, riga_casuale_W, colonna_casuale_W, 'W');
    //fine generazione mappa segreta

    //generazione scatola della x
    int r_centro, c_centro;
    do
    {
        r_centro = 3 + rand () % (ALTEZZA_MAPPA - 6); //utilizzo 3 come margine di sicurezza
        c_centro = 3 + rand () % (LARGHEZZA_MAPPA - 6);
    } while (mappa_get_cella(mappa_corrente, r_centro, c_centro) != '.');

    //costruzione mura 3x3
    for (int y = r_centro - 1; y <= r_centro + 1; y ++)
    {
        for (int x = c_centro - 1; x <= c_centro + 1; x++)
        {
            mappa_set_cella(mappa_corrente, y, x, '#');
        }
    }
    mappa_set_cella(mappa_corrente, r_centro, c_centro, 'x');

    //porta d su un lato casuale e nella casella davanti metto pavimento
    int lato = rand () % 4;
    switch (lato)
    {
        case 0:
        mappa_set_cella(mappa_corrente, r_centro - 1, c_centro, 'd');
        mappa_set_cella(mappa_corrente, r_centro - 2, c_centro, '.');
        break;

        case 1:
        mappa_set_cella(mappa_corrente, r_centro + 1, c_centro, 'd');
        mappa_set_cella(mappa_corrente, r_centro + 2, c_centro, '.');
        break;

        case 2:
        mappa_set_cella(mappa_corrente, r_centro, c_centro - 1, 'd');
        mappa_set_cella(mappa_corrente, r_centro, c_centro - 2, '.');
        break;

        case 3:
        mappa_set_cella(mappa_corrente, r_centro, c_centro + 1, 'd');
        mappa_set_cella(mappa_corrente, r_centro, c_centro + 2, '.');
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
    }while(mappa_get_cella(mappa_corrente, riga_casuale, colonna_casuale) != '.');

    Giocatore* player = giocatore_crea(riga_casuale, colonna_casuale);
    if (player == NULL) {
        mappa_distruggi(mappa_principale);
        mappa_distruggi(mappa_segreta);
        endwin();
        return 1;
    }

    Nemico* lista_nemici[3];
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
            distanza_sicurezza_y = abs(riga_casuale_n - giocatore_get_y(player));
            distanza_sicurezza_x = abs(colonna_casuale_n - giocatore_get_x(player));
        } while (mappa_get_cella(mappa_corrente, riga_casuale_n, colonna_casuale_n) != '.' || (distanza_sicurezza_y < 7 && distanza_sicurezza_x < 15));
        
        lista_nemici[i] = nemico_crea(riga_casuale_n, colonna_casuale_n);
        if (lista_nemici[i] == NULL) {
            // Cleanup in caso di errore
            for (int j = 0; j < i; j++) {
                nemico_distruggi(lista_nemici[j]);
            }
            giocatore_distruggi(player);
            mappa_distruggi(mappa_principale);
            mappa_distruggi(mappa_segreta);
            endwin();
            return 1;
        }
    }

    int riga_casuale_k, colonna_casuale_k;
    do
    {
        riga_casuale_k = rand () % ALTEZZA_MAPPA;
        colonna_casuale_k = rand () % LARGHEZZA_MAPPA;
    } while (mappa_get_cella(mappa_corrente, riga_casuale_k, colonna_casuale_k) != '.');
    mappa_set_cella(mappa_corrente, riga_casuale_k, colonna_casuale_k, 'k');

    int riga_casuale_s, colonna_casuale_s;
    do
    {
        riga_casuale_s = rand () % ALTEZZA_MAPPA;
        colonna_casuale_s = rand () % LARGHEZZA_MAPPA;
    } while (mappa_get_cella(mappa_corrente, riga_casuale_s, colonna_casuale_s) != '.');
    mappa_set_cella(mappa_corrente, riga_casuale_s, colonna_casuale_s, 's');

    //definizione colori
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    int input_tasto = 0;
    char messaggio_status[100] = "Trova la chiave per uscire!";

    disegna_mappa(mappa_corrente, player, messaggio_status, lista_nemici, 3);

    //game loop
    while((input_tasto = getch()) != 'q')
    {
        int new_y = giocatore_get_y(player);
        int new_x = giocatore_get_x(player);

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
        char destinazione = mappa_get_cella(mappa_corrente, new_y, new_x);

        if (destinazione == 's')
        {
            if (mappa_corrente == mappa_principale)
            {
                mappa_corrente = mappa_segreta;
                strcpy(messaggio_status, "Sei sceso nelle segrete");
            }

            else 
            {
                mappa_corrente = mappa_principale;
                strcpy(messaggio_status, "Sei tornato in superficie");
            }

            //cerco scale nella nuova mappa per metterci sopra il giocatore
            for(int y = 0; y < ALTEZZA_MAPPA; y++)         
            {
                for(int x = 0; x < LARGHEZZA_MAPPA; x++) 
                {
                    if(mappa_get_cella(mappa_corrente, y, x) == 's') 
                    {
                        giocatore_set_posizione(player, y, x);
                    }
                }
            }

            new_y = giocatore_get_y(player);
            new_x = giocatore_get_x(player);

            //evito che i nemici si generino nei muri della nuova mappa
            int riga_casuale_ns, colonna_casuale_ns;
            int distanza_sicurezza_ys, distanza_sicurezza_xs;

            for(size_t i = 0; i < 3; i++)
            {
                do
                {
                    riga_casuale_ns = rand () % ALTEZZA_MAPPA;
                    colonna_casuale_ns = rand () % LARGHEZZA_MAPPA;

                    distanza_sicurezza_ys = abs(riga_casuale_ns - giocatore_get_y(player));
                    distanza_sicurezza_xs = abs(colonna_casuale_ns - giocatore_get_x(player));
                } while (mappa_get_cella(mappa_corrente, riga_casuale_ns, colonna_casuale_ns) != '.' || (distanza_sicurezza_ys < 5 && distanza_sicurezza_xs < 5));
        
                nemico_set_posizione(lista_nemici[i], riga_casuale_ns, colonna_casuale_ns);
            }

            clear();
        }

        else if (destinazione == 'k')
        {
            giocatore_set_tasca(player, 1);
            mappa_set_cella(mappa_corrente, new_y, new_x, '.');
            giocatore_set_posizione(player, new_y, new_x);
            strcpy(messaggio_status, "Hai preso la chiave!");
        }

        else if (destinazione == 'W')
        {
            giocatore_set_arma(player, 1);
            mappa_set_cella(mappa_corrente, new_y, new_x, '.');
            giocatore_set_posizione(player, new_y, new_x);
            strcpy(messaggio_status, "Hai preso un'arma!");
        }

        else if (destinazione == 'd')
        {
            if (giocatore_get_tasca(player) == 1)
            {
                mappa_set_cella(mappa_corrente, new_y, new_x, '.');
                giocatore_set_posizione(player, new_y, new_x);
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
            
            // Cleanup prima di uscire
            for (int i = 0; i < 3; i++) {
                nemico_distruggi(lista_nemici[i]);
            }
            giocatore_distruggi(player);
            mappa_distruggi(mappa_principale);
            mappa_distruggi(mappa_segreta);
            
            break;
        }

        //logica della collisione con i nemici
        int nemico_toccato = -1; //nessuno
        for (int i = 0; i < 3; i++)
        {   
            //controllo se la casella futura è occupata da un nemico VIVO
            if (nemico_is_vivo(lista_nemici[i]) && new_x == nemico_get_x(lista_nemici[i]) && new_y == nemico_get_y(lista_nemici[i]))
            {
                nemico_toccato = i;
            }
        }

        if (nemico_toccato != -1)
        {
            if (giocatore_get_arma(player) == 1)
            {
                nemico_uccidi(lista_nemici[nemico_toccato]);

                giocatore_set_posizione(player, new_y, new_x);
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
                
                // Cleanup prima di uscire
                for (int i = 0; i < 3; i++) {
                    nemico_distruggi(lista_nemici[i]);
                }
                giocatore_distruggi(player);
                mappa_distruggi(mappa_principale);
                mappa_distruggi(mappa_segreta);
                endwin();
                return 0;
            }
        
        }

        //se è pavimento (o era un oggetto preso) e non è muro
        else if (destinazione == '.' && mappa_get_cella(mappa_corrente, new_y, new_x) != '#')
        {
            giocatore_set_posizione(player, new_y, new_x);
            strcpy(messaggio_status, ""); //reset del messaggio di status 
        }

        //IA dei nemici
        muovi_nemici(mappa_corrente, player, lista_nemici, 3);

        for (int i = 0; i < 3; i++)
        {
            if (nemico_is_vivo(lista_nemici[i]) && giocatore_get_y(player) == nemico_get_y(lista_nemici[i]) && giocatore_get_x(player) == nemico_get_x(lista_nemici[i]))
            {
                if(giocatore_get_arma(player) == 1)
                {
                    nemico_uccidi(lista_nemici[i]);
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
                    
                    // Cleanup prima di uscire
                    for (int j = 0; j < 3; j++) {
                        nemico_distruggi(lista_nemici[j]);
                    }
                    giocatore_distruggi(player);
                    mappa_distruggi(mappa_principale);
                    mappa_distruggi(mappa_segreta);
                    endwin();
                    return 0;
                }
            }
        }

        //disegna il frame finale
        disegna_mappa(mappa_corrente, player, messaggio_status, lista_nemici, 3);
    }

    // Cleanup alla fine del gioco
    for (int i = 0; i < 3; i++) {
        nemico_distruggi(lista_nemici[i]);
    }
    giocatore_distruggi(player);
    mappa_distruggi(mappa_principale);
    mappa_distruggi(mappa_segreta);
    
    endwin();
    return 0;
}


//mettere una difficoltà
//durata delle armi e armi diverse 
//area di visione dei nemici