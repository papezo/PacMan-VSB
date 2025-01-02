#include "../menu/loadScore.h"
#include <stdio.h>

int currentScore = 0;
int highScore = 0;    

#define SCORE_FILE "highscore.txt"

int loadHighScore() {
    FILE *file = fopen(SCORE_FILE, "r");
    if (file == NULL) {
        // Pokud soubor neexistuje, nastavíme high score na 0
        return 0;
    }

    fscanf(file, "%d", &highScore);
    fclose(file);
    return highScore;
}

// Funkce pro uložení nejvyššího skóre do souboru
void saveHighScore(int score) {
    FILE *file = fopen(SCORE_FILE, "w");
    if (file != NULL) {
        fprintf(file, "%d", score);
        fclose(file);
    }
}
