//#include <Windows.h>
#include "Game.h"
#include "constants.h"
#include <time.h>

extern int FIELD_SIZE;

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
            field[i][j].SetPos(i, j);
            field[i][j].SetColor();
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
            field[i][j].SetPos(i, j);
            field[i][j].SetColor();

        }
    RandomizeValueOneTile();
    RandomizeValueOneTile();
   // SaveIntoHistory();
    InvalidateRect(hWnd, NULL, 1);
}

void Game::RandomizeValueOneTile()
{
    static int row, col;

    bool isAnyEmpty = false;
    for(int i = 0; i < FIELD_SIZE; i++)
        for(int j = 0; j < FIELD_SIZE; j++)
            if (field[i][j].value == 0) { isAnyEmpty = true; break; }

    if (!isAnyEmpty) { return; }

    do {
        row = rand() % FIELD_SIZE;
        col = rand() % FIELD_SIZE;
    } while (field[row][col].value != 0);
    
    field[row][col].value = (rand() % 100 < 90) ? 2 : 4;
    field[row][col].SetPos(row, col);
    field[row][col].SetColor();
}

void Game::KeyUpHandler() {
    for (int i = 0; i < FIELD_SIZE; i++)
        for (int j = 0; j < FIELD_SIZE; j++)
            if (field[i][j].value != 0) {
                int x = i;
                for (int l = i - 1; l >= 0; l--) {
                    if (field[l][j].value == 0) {
                        field[l][j].value = field[x][j].value;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        x = l;
                        continue;
                    }
                    if (field[l][j].value == field[x][j].value) {
                        field[l][j].value *= 2;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        Score += field[l][j].value;
                        break;
                    }
                    if (field[l][j].value != 0) {
                        break;
                    }
                }
            }
}

void Game::KeyDownHandler() {
    for (int i = FIELD_SIZE - 1; i >= 0; i--)
        for (int j = FIELD_SIZE - 1; j >= 0; j--)
            if (field[i][j].value != 0) {
                int x = i;
                for (int l = i + 1; l < FIELD_SIZE; l++) {
                    if (field[l][j].value == 0) {
                        field[l][j].value = field[x][j].value;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        x = l;
                        continue;
                    }
                    if (field[l][j].value == field[x][j].value) {
                        field[l][j].value *= 2;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        Score += field[l][j].value;
                        break;
                    }
                    if (field[l][j].value != 0) {
                        break;
                    }
                }
            }
}

void Game::KeyLeftHandler() {
    for (int i = 0; i < FIELD_SIZE; i++)
        for (int j = 0; j < FIELD_SIZE; j++)
            if (field[i][j].value != 0) {
                int x = j;
                for (int l = j - 1; l >= 0; l--) {
                    if (field[i][l].value == 0) {
                        field[i][l].value = field[i][x].value;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        x = l;
                        continue;
                    }
                    if (field[i][l].value == field[i][x].value) {
                        field[i][l].value *= 2;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        Score += field[i][l].value;
                        break;
                    }
                    if (field[i][l].value != 0) {
                        break;
                    }
                }
            }
}

void Game::KeyRightHandler() {
    for (int i = FIELD_SIZE - 1; i >= 0; i--)
        for (int j = FIELD_SIZE - 1; j >= 0; j--)
            if (field[i][j].value != 0) {
                int x = j;
                for (int l = j + 1; l < 4; l++) {
                    if (field[i][l].value == 0) {
                        field[i][l].value = field[i][x].value;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        x = l;
                        continue;
                    }
                    if (field[i][l].value == field[i][x].value) {
                        field[i][l].value *= 2;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        Score += field[i][l].value;
                        break;
                    }
                    if (field[i][l].value != 0) {
                        break;
                    }
                }
            }
}

void Game::SaveIntoHistory()
{

}

bool Game::isGameOver()
{
    bool result = false;
    for (int i = 0; i < FIELD_SIZE; i++)
        for (int j = 0; j < FIELD_SIZE; j++)
            if (field[i][j].value == 2048) { result = true; }

    //Сохраняем наше поле
    Block temp[4][4];
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            temp[i][j].value = field[i][j].value;
            temp[i][j].SetColor();
            temp[i][j].SetPos(i, j);
        }
    }

    KeyUpHandler();
    RandomizeValueOneTile();
    KeyDownHandler();
    RandomizeValueOneTile();
    //KeyLeftHandler();
    //RandomizeValueOneTile();
    //KeyRightHandler();

    bool isSame = true;

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            if (temp[i][j].value != field[i][j].value) {
                isSame = false;
                break;
            }
        }
    }

    if (isSame) { result = true; }
    else {
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                field[i][j].value = temp[i][j].value;
                field[i][j].SetColor();
                field[i][j].SetPos(i, j);
            }

        }
    }

    return result;
}

Game::~Game()
{
}