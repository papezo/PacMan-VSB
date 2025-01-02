cmake_minimum_required(VERSION 3.12)

project(pacman)

# Přidání přepínačů překladače
set(CMAKE_C_STANDARD 11)

# Vyhledání knihoven SDL2 a dalších SDL knihoven
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Seznam všech zdrojových souborů
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

# Vytvoření programu s názvem `pacman`
add_executable(pacman ${SOURCES})

# Přidání adresářů s hlavičkovými soubory
target_include_directories(pacman PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

# Přilinkování knihoven k programu
target_link_libraries(pacman SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# Pokud používáte matematickou knihovnu (pro sqrt() atd.), přidejte -lm
target_link_libraries(pacman m)

# Kopírování fontu do složky build, aby byl přístupný při spuštění hry
add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/font/Emulogic-zrEw.ttf
    ${CMAKE_BINARY_DIR}/font/Emulogic-zrEw.ttf)

# Kopírování všech obrázků z adresáře img/ do složky build/img/
add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/img
    ${CMAKE_BINARY_DIR}/img)

# Kopírování souboru mapy do složky build, aby byl přístupný při spuštění hry
add_custom_command(TARGET pacman POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/maps/level1.txt
    ${CMAKE_BINARY_DIR}/maps/level1.txt)

