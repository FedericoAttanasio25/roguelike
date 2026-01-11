#include "entità.h"
#include <stdlib.h>

void muovi_nemici(char map [][LARGHEZZA_MAPPA], giocatore p, nemico n[])
{
    for(int i = 0; i < 3; i++)
    {
        //se il nemico è morto salta il ciclo
        if (n[i].x_e == -1)
        {
            continue;
        }

        int new_y_e = n[i].y_e;
        int new_x_e = n[i].x_e;

        //tiro un dado con la possibilità del 25% di far fare al nemico una mossa casuale
        int dado = rand () % 4;
        if (dado == 0)
        {
            //mossa casuale del nemico
            int mossa_casuale = rand () % 4;
            switch (mossa_casuale)
            {
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

        else
        {
            //se il giocatore ha l'arma i nemici scappano sempre con margine d'errore
            if(p.arma == 1)
            {
                //calcolo direzione y
                if(p.y > n[i].y_e)
                {
                    new_y_e = n[i].y_e - 1;
                }

                else if (p.y < n[i].y_e)
                {
                    new_y_e = n[i].y_e + 1;
                }

                //calcolo direzione x
                if(p.x > n[i].x_e)
                {
                    new_x_e = n[i].x_e - 1;
                }

                else if (p.x < n[i].x_e)
                {
                    new_x_e = n[i].x_e + 1;
                }
            }

            else
            {
                //calcolo direzione y
                if(p.y > n[i].y_e)
                {
                    new_y_e = n[i].y_e + 1;
                }

                else if (p.y < n[i].y_e)
                {
                    new_y_e = n[i].y_e - 1;
                }

                //calcolo direzione x
                if(p.x > n[i].x_e)
                {
                    new_x_e = n[i].x_e + 1;
                }

                else if (p.x < n[i].x_e)
                {
                    new_x_e = n[i].x_e - 1;
                }
            }

        }

        //collisione con il muro
        if (map[new_y_e][new_x_e] != '#')
        {
            n[i].y_e = new_y_e;
            n[i].x_e = new_x_e;
        }
    }

}