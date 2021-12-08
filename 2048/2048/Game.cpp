//#include <Windows.h>
#include "Game.h"
#include "constants.h"
#include <time.h>

Game::Game() :
Score(0), ElementsCount(0), BestScore(0) {

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            saveField[i][j] = 0;
            ElementsCount = 2;
        }
    }

    //Инициализая оставшихся полей массива: начальная позиция соответсвует номеру
    //элемента, значение берем из
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            field[i][j].value = saveField[i][j];
            field[i][j].SetPosAndColor(i, j);
        }
    }
}

void Game::StartNewGame(HWND hWnd)
{
    Score = 0;
    ElementsCount = 0;
    //BestScore = GetBestScore();
    BestScore = 0;
    for (int i = 0; i < FIELD_SIZE; i++)
        for (int j = 0; j < FIELD_SIZE; j++) {
            field[i][j].value = 0;
            field[i][j].SetPosAndColor(i, j);
        }
//    Randomizer();
  //  Randomizer();
   // SaveIntoHistory();
    InvalidateRect(hWnd, NULL, 1);
}

void Game::RandomizeValueOneTile()
{
    Block* randomer[16];
    int x[16], y[16];
    int n = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (field[i][j].value == 0) {
                randomer[n] = &(field[i][j]);
                x[n] = i;
                y[n] = j;
                n++;
            }
    srand((unsigned)time(NULL));
    if (n != 0) {
        int l = rand() % n;
        (randomer[l])->value = 4 - (rand() % 2) * 2;
        (randomer[l])->SetPosAndColor(x[l], y[l]);
    }
}



Game::~Game()
{
}