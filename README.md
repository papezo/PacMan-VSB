# Pac-Man

Projekt do předmětu UPR

V tomto projektu jsem se snažil vytviř hru **Pac-Man** v jazyce C s využitím knihoven SDL2. Projekt obsahuje několik základních funkcí, které se snaží napodobit originální hru, a také některé vlastní rozšíření.

---

## Funkce

- **Pac-Man** se pohybuje po bludišti a sbírá body.
- **Duchové** se pohybují po mapě a hráč se jim musí vyhnout, aby neztratil život.
- Hráč sbírá malé body (**mince**), které jsou rozmístěny po mapě.
- Na mapě se objevují náhodně **bonusové ovoce**, které přidávají další body.
- Pac-Man a duchové mohou využívat **teleporty** (tunely označené číslem `5` na mapě), které je přenesou na opačnou stranu mapy.
- V režimu **frightened** (když Pac-Man sní **power pellet**, které jsou v rozích mapy) může Pac-Man sníst duchy, což hráči přidá dodatečné body.
- Pokud Pac-Man narazí na ducha, ztratí život.
- **Hra končí**, když Pac-Man ztratí všechny životy.

### Nastavení

- Uživatel si může:
  - Ztišit zvuk.
  - Vypnout zvukové efekty (nejsou zatím implementovány).
  - Snížit nebo zvýšit počet životů.

---

## Známé chyby

- **Pohyb duchů**: Ne moc funkční (špatné zarovnání na políčku?).
- **Hudba**: Po nějakém čase se zasekne a přestane hrát.

---

## Struktura projektu

Projekt je rozdělen do několika souborů:

### Složka `game`:

- **`config`**  
  Základní konfigurace obrazovky.

- **`game.h`**  
  Obsahuje vše, co potřebujeme využívat v dalších souborech.

- **`bfs.c`**  
  Obsahuje BFS (Breadth-first search) algoritmus pro hledání nejkratší cesty k Pac-Manovi pro duchy.

- **`wallChecker.c`**  
  Obsahuje funkce pro Pac-Mana a duchy, které využívají k detekci zdí a volného prostoru.

- **`core.c`**  
  Hlavní soubor hry obsahující směs funkcí potřebných k rozjetí hry.

- **`field.c`**  
  Import mapy z txt souboru a vykreslení na obrazovku.

- **`fruit.c`**  
  Funkce pro vykreslení ovoce, detaily a interakce s Pac-Manem.

- **`ghostsAI.c`**  
  Logika pro duchy využívající BFS algoritmus.

- **`pacmanAI.c`**  
  Logika pro Pac-Mana.

- **`points.c`**  
  Vykresluje power pellets na mapě.

### Složka `entities`:

- **`entities.c`**  
  Hlavní soubor, vykreslující všechny obrázky, texty atp.

### Složka `img`:

- Obsahuje hudbu, obrázky ovoce, duchů, Pac-Mana atd.

### Složka `maps`:

- Obsahuje zatím jen jednu mapu (`level1.txt`).

### Složka `menu`:

- **`loadScore.c`**  
  Soubor sloužící pro import a export skóre, která uživatel ve hře získal.

- **`menu.c`**  
  Soubor obsahující vše potřebné k zobrazení menu hry.

- **`sound.c`**  
  Soubor obsahující funkce potřebné k spuštění zvuků (využívá SDL2 Mixer).

### Další soubory:

- **`main.c`**  
  Slouží k správnému chodu programu.

- **`highscore.txt`**  
  Ukládá nejvyšší skóre, které uživatel ve hře získal.

---

## Jak přeložit program

### Pomocí Makefile

1. Nainstalujte `gcc` a `make` (pokud ještě nemáte).
2. Vytvořte adresář pro projekt a přidejte soubory.
3. Spusťte následující příkaz pro kompilaci projektu:

   ```bash
   make run


### Pomocí CMAKE

**DŮLEŽITÉ NAINSTALOVAT CMAKE!**
 ```bash
$ sudo apt update
$ sudo apt install cmake
```

1. Vytvořte adresář pro projekt a přidejte soubory podle struktury výše.
2. Vytvořte soubor `CMakeLists.txt` (viz níže).

## CMakeLists.txt

Pokud chcete používat CMake, vytvořte soubor `CMakeLists.txt` s následujícím obsahem:

 ```bash
cmake_minimum_required(VERSION 3.12)

project(pacman)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

set(SOURCES
    main.c                         
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
```

3. V cmd se dostaňte do složky s hrou:

	1. `cd build`
	2. `cmake ..`
	3. `make`
	4. `./pacman`


### Pomocí cmd

1. Dostat se do složky s hrou
2. Napsat do cmd
```bash ' gcc ./game main.c menu/menu.c menu/sound.c game/core.c game/wallChecker.c entities/entities.c game/points.c game/AI
/bfs/bfs.c menu/loadScore.c game/ghostsAI.c game/pacmanAI.c game/fruit.c game/field.c game/coins.c -o game_menu -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm'
```

-lm z důvodu toho že by jinak nefungovaly matematické funkce (sqrt...) využívané ve hře

---

### Fotogalerie

![obrazek](https://github.com/user-attachments/assets/7a13fff5-88a7-4e87-8fa3-296e0b346a9c)
![obrazek](https://github.com/user-attachments/assets/1c33fe93-6307-476a-bad3-28694b0eaa8f)
![obrazek](https://github.com/user-attachments/assets/c8ae2c03-b09a-4e55-bba6-b98041361a13)


