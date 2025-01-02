#include "../../../game/game.h"  // Pokud je game.h ve složce game

#include "bfs.h"

#include <stdbool.h>
#include <stdio.h>

int bfsNextMove(int startRow, int startCol, int goalRow, int goalCol)
{
    if (startRow == goalRow && startCol == goalCol)
    {
        return -1; // Už jsme v cíli
    }

    // Fronta pro BFS
    Node queue[WIDTH * HEIGHT];
    int front = 0, back = 0;

    // Pamatujeme si předka (odkud jsme přišli) pro každou buňku
    int parentRow[HEIGHT][WIDTH];
    int parentCol[HEIGHT][WIDTH];

    bool visited[HEIGHT][WIDTH];
    for (int r = 0; r < HEIGHT; r++)
    {
        for (int c = 0; c < WIDTH; c++)
        {
            visited[r][c] = false;
            parentRow[r][c] = -1;
            parentCol[r][c] = -1;
        }
    }

    // Začínáme BFS z startRow, startCol
    visited[startRow][startCol] = true;
    queue[back++] = (Node){startRow, startCol};

    bool found = false;

    while (front < back)
    {
        Node current = queue[front++];
        int curRow = current.row;
        int curCol = current.col;

        if (curRow == goalRow && curCol == goalCol)
        {
            found = true;
            break;
        }

        // Projdeme 4 sousedy
        for (int i = 0; i < 4; i++)
        {
            int newRow = curRow + directions[i][0];
            int newCol = curCol + directions[i][1];

            // Ověř, že jsme v mapě a není tam zeď (gameField[newRow][newCol] != 1)
            if (newRow >= 0 && newRow < HEIGHT && newCol >= 0 && newCol < WIDTH)
            {
                // Pokud je pozice tunel (5), teleportujeme ducha na opačnou stranu
                if (gameField[newRow][newCol] == 5)
                {
                    // Předpokládáme, že teleportujeme z levé na pravou nebo z pravé na levou stranu
                    if (newCol < 11) {
                        newCol = 22;  // Teleport na pravý okraj
                    } else {
                        newCol = 1;   // Teleport na levý okraj
                    }
                }

                // Ověření, zda jsme na platné pozici a ne na zdi
                if (!visited[newRow][newCol] && gameField[newRow][newCol] != 1)
                {
                    visited[newRow][newCol] = true;
                    parentRow[newRow][newCol] = curRow;
                    parentCol[newRow][newCol] = curCol;
                    queue[back++] = (Node){newRow, newCol};
                }
            }
        }
    }

    if (!found)
    {
        return -1; // není cesta
    }

    // Pokud jsme našli cestu, projdeme zpátky od goal k start
    int pathRow = goalRow;
    int pathCol = goalCol;

    // Dokud nenarazíme na start
    while (parentRow[pathRow][pathCol] != startRow || parentCol[pathRow][pathCol] != startCol)
    {
        int pr = parentRow[pathRow][pathCol];
        int pc = parentCol[pathRow][pathCol];
        pathRow = pr;
        pathCol = pc;
    }

    // Teď (pathRow, pathCol) je soused startu na cestě ke goal
    // Zjistíme, kterým směrem to je
    int dRow = pathRow - startRow;
    int dCol = pathCol - startCol;

    // Porovnáme s directions (0=dolů, 1=nahoru, 2=vlevo, 3=vpravo)
    if (dRow == 1 && dCol == 0)
        return 0; // dolů
    if (dRow == -1 && dCol == 0)
        return 1; // nahoru
    if (dRow == 0 && dCol == -1)
        return 2; // vlevo
    if (dRow == 0 && dCol == 1)
        return 3; // vpravo

    return -1; // teoreticky by se nemělo stát
}
