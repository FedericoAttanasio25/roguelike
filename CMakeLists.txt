cmake_minimum_required(VERSION 3.20)
project(IlMioGioco)

set(CMAKE_C_STANDARD 99)

# 1. ISTRUZIONE FONDAMENTALE PER GLI HEADERS
# Dice a CLion di cercare i file .h dentro la cartella Headers.
# Così nel codice potrai scrivere #include "mappa.h" invece di percorsi lunghi.
include_directories(Headers)

# 2. DEFINIZIONE ESEGUIBILE
# Aggiungiamo tutti i file .c che compongono il progetto.
# Nota: Gli header (.h) non sono strettamente necessari qui per la compilazione,
# ma metterli aiuta CLion a indicizzarli meglio.
add_executable(IlMioGioco
        main.c
        Source/genera_livello.c
        Headers/entità.h
        Source/entità.c
        Headers/disegna_mappa.h
        Source/disegna_mappa.c
        Headers/gioco.h
        Source/gioco.c
        Headers/pathfinding.h
        Source/pathfinding.c
)

# 3. LINK LIBRERIE DI SISTEMA
target_link_libraries(IlMioGioco curses form)