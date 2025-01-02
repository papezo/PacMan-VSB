# Pac-Man

Projekt snažící se napodobit originální hru Pac Mana. Obsahuje bugy!

- Pac-Man se pohybuje po bludišti a sbírá body.
- Duchové se pohybují po mapě a hráč se jim musí vyhnout, aby neztratil život.
- Hráč sbírá malé body (mince), které jsou roztroušeny po mapě.
- Na mapě se objevují náhodně bonusové ovoce, které přidávají další body.
- Pac-Man a duchové mohou využívat teleporty (tunelem označeným číslem 5 na mapě), které je přenesou na opačnou stranu mapy.
- V režimu frightened (když pac man sní power pallet, které jsou v rozích mapy) může Pac-Man sníst duchy, které dají hráči dodatečné body
- Pokud Pac-Man narazí na ducha, ztratí život.
- Hra končí, když Pac-Man ztratí všechny životy – Pokud hráč ztratí všechny životy, hra končí a vypne se.

- V nastavení si uživatel může ztišit zvuk, vypnout zvukové efekty (nejsou zatím implementovány) a snížit nebo zvýšit počet životů.


- Hra obsahuje bugy: 
    - Ne moc funkční pohyb duchů (špatné zarovnání na políčku?)
    - Hudba se po nějakém čase zasekává a přestane hrát


## Struktura projektu

Projekt je rozdělen do několika souborů:
Složka game:
    - *'config'*- základní konfigurace obrazovky
    - *'game.h'* - obsahuje vše co potřebujeme využívat v dalších souborech
    - *'bfs.c'* - obsahuje BFS (Breadth-first search) algoritmus pro hledání nejkratší cesty k pac manovi pro duchy. 
    - *'wallChecker.c'* - obsahuje funkce pro Pac Mana a duchy, které využívají k detekci zdí, volného prostoru atd..
    - *'core.c'* - Hlavní soubor hry. Obsahuje směs funkcí potřebné k rozjetí hry
    - *'field.c'* - Obsahuje import mapy z txt souboru a funkci pro následné vykreslení na obrazovku z texťáku
    - *'fruit,c'* - Obsahuje funkce pro vykreslení ovoce, detaily a funkce co dělat když je pac man na pozici ovoce.
    - *'ghostsAI.c'* - Soubor obsahující logiku pro duchy, využívající algoritmus BFS z souboru bfs.c
    - *'pacmanAI.c'* - Soubor obsahující logiku pro pac mana.
    - *'points.c'* - Soubor vykreslující power pallets na mapě.

Složka entities:
    - *'entities.c'* - Hlavní soubor, vykreslující všechny obrázky, texty atp...

- *Složka img* - obsahuje hudbu, obrázky ovoce, duchů, pac mana atd...
- *Složka Maps* - Obsahuje zatím jen jednu mapu

Složka menu:
    - *'loadScore.c'* - Soubor sloužící pro import a export skóre, která uživatel ve hře získal.
    - *'menu.c'* - Soubor obsahující vše potřebné k zobrazení menu hry
    - *'sound.c'* - Soubor obsahující funkce potřebné k spuštění zvuků. (Využívá SDL2 mixer)

*Soubor main.c* - slouží k správnému chodu programu.

*highscore.txt* - ukládá se do něj nejvyšší skóre, které uživatel ve hře získal.

Program taky obsahuje hlavičkové soubory (s příponou .h), které slouží k tomu abychom mohli využívat funkce, proměnné z .c souboru dále

## Jak přeložit projekt

### Pomocí Makefile

1. Nainstalujte `gcc` a `make` (pokud ještě nemáte).
2. Vytvořte adresář pro projekt a přidejte soubory.
3. Spusťte následující příkaz pro kompilaci projektu: `make run`

### Pomocí CMAKE

// DŮLEŽITÉ NAINSTALOVAT CMAKE! //

$ sudo apt update
$ sudo apt install cmake

1. Vytvořte adresář pro projekt a přidejte soubory podle struktury výše.
2. Vytvořte soubor `CMakeLists.txt` (viz níže).

## CMakeLists.txt

Pokud chcete používat CMake, vytvořte soubor `CMakeLists.txt` s následujícím obsahem:

cmake_minimum_required(VERSION 3.12)

project(pacman)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

set(SOURCES
    main.c                          # Hlavní soubor je v kořenovém adresáři pac-man-main
    entities/entities.c
    game/game.h
    game/core.c
    game/wallChecker.c
    game/points.c
    game/ghostsAI.c
    game/pacmanAI.c
    game/fruit.c
    game/field.c
    game/coins.c
    menu/menu.c
    menu/sound.c
    menu/loadScore.c
    game/AI/bfs/bfs.c
)

add_executable(pacman ${SOURCES})

target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

target_link_libraries(pacman SDL2 SDL2_image SDL2_ttf SDL2_mixer)

target_link_libraries(pacman m)

add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/font/Emulogic-zrEw.ttf
    ${CMAKE_BINARY_DIR}/font/Emulogic-zrEw.ttf)

add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/img
    ${CMAKE_BINARY_DIR}/img)

add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/maps/level1.txt
    ${CMAKE_BINARY_DIR}/maps/level1.txt)



3. V cmd se dostaňte do složky s hrou, následně do build složky -> cd build -> cmake .. -> make -> ./pacman

### Pomocí cmd

1. Dostat se do složky s hrou
2. napsat ' gcc ./game main.c menu/menu.c menu/sound.c game/core.c game/wallChecker.c entities/entities.c game/points.c game/AI
/bfs/bfs.c menu/loadScore.c game/ghostsAI.c game/pacmanAI.c game/fruit.c game/field.c game/coins.c -o game_menu -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm'

-lm z důvodu toho že by jinak nefungovaly matematické funkce (sqrt...) využívané ve hře
