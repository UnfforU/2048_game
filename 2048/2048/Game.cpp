#include <Windows.h>
#include <string>
#include <fstream>
#include "Game.h"
#include "constants.h"
#include <time.h>

using namespace std;

extern int FIELD_SIZE;


Game::Game(HWND hWnd) :
score(0), elementsCount(0), bestScore(0) {

    bool newGame = true;
    string buffStr, path; 

    bestScore = GetBestScore();

    //‘ормируем пути к файлу согласно размерам пол€
    ifstream fin("Memory\\LastSave\\save_" + intToStr(FIELD_SIZE) + ".dat");
    if(fin.is_open())
    {
        fin >> buffStr;
        buffStr = (buffStr == "") ? "0" : buffStr;
        score = stoi(buffStr);
        if (score != 0) {
            newGame = false;
        }
    }
    else 
    { 
        CreateNewLastSaveFile();
        newGame = true; 
    }

    //≈сли игра не нова€ то читаем данные с файла во временный массив и считаем количество не нулевых плиток
    if (!newGame) {
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                fin >> buffStr;
                saveField[i][j] = stoi(buffStr);
                if (saveField[i][j] != 0) {
                    elementsCount++;
                }
            }
        }
    }
    else { //иначе заполн€ем его нул€ми
        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                saveField[i][j] = 0;
                elementsCount = 0;
            }
        }
    }
    fin.close();


    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            field[i][j].value = saveField[i][j];
            field[i][j].SetPos(i, j);
            field[i][j].SetColor();
        }
    }

    if(elementsCount == 0)
    {
        RandomizeValueOneTile();
        RandomizeValueOneTile();
    }

    InvalidateRect(hWnd, NULL, 1);
}

void Game::StartNewGame(HWND hWnd)
{
    score = 0;
    elementsCount = 0;
    bestScore = GetBestScore();
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
    elementsCount++;
}

bool Game::KeyUpHandler(bool isTest) {
    bool isAnyChange = false;
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
                        isAnyChange = true;
                        continue;
                    }
                    if (field[l][j].value == field[x][j].value) {
                        field[l][j].value *= 2;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        if(!isTest)
                            score += field[l][j].value;
                        isAnyChange = true;
                        break;
                    }
                    if (field[l][j].value != 0) {
                        break;
                    }
                }
            }

    return isAnyChange;
}

bool Game::KeyDownHandler(bool isTest) {
    bool isAnyChange = false;
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
                        isAnyChange = true;
                        continue;
                    }
                    if (field[l][j].value == field[x][j].value) {
                        field[l][j].value *= 2;
                        field[l][j].SetColor();
                        field[x][j].value = 0;
                        field[x][j].SetColor();
                        if (!isTest)
                           score += field[l][j].value;
                        isAnyChange = true;
                        break;
                    }
                    if (field[l][j].value != 0) {
                        break;
                    }
                }
            }
    return isAnyChange;
}

bool Game::KeyLeftHandler(bool isTest) {
    bool isAnyChange = false;
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
                        isAnyChange = true;
                        x = l;
                        continue;
                    }
                    if (field[i][l].value == field[i][x].value) {
                        field[i][l].value *= 2;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        if(!isTest)
                            score += field[i][l].value;
                        isAnyChange = true;
                        break;
                    }
                    if (field[i][l].value != 0) {
                        break;
                    }
                }
            }
    return isAnyChange;
}

bool Game::KeyRightHandler(bool isTest) {
    bool isAnyChange = false;
    for (int i = FIELD_SIZE - 1; i >= 0; i--)
        for (int j = FIELD_SIZE - 1; j >= 0; j--)
            if (field[i][j].value != 0) {
                int x = j;
                for (int l = j + 1; l < FIELD_SIZE; l++) {
                    if (field[i][l].value == 0) {
                        field[i][l].value = field[i][x].value;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        x = l;
                        isAnyChange = true;

                        continue;
                    }
                    if (field[i][l].value == field[i][x].value) {
                        field[i][l].value *= 2;
                        field[i][l].SetColor();
                        field[i][x].value = 0;
                        field[i][x].SetColor();
                        if (!isTest)
                            score += field[i][l].value;
                        isAnyChange = true;
                        break;
                    }
                    if (field[i][l].value != 0) {
                        break;
                    }
                }
            }
    return isAnyChange;
}

void Game::SaveIntoHistory()
{
    
}



void Game::SaveResultsInFile()
{
    ofstream fout("Memory\\LastSave\\save_" + intToStr(FIELD_SIZE) + ".dat", ios_base::out | ios_base::trunc);
    if (fout.is_open())
    {
        string buffStr = intToStr(score) + ' ';
        fout << buffStr;



        for (int i = 0; i < FIELD_SIZE; i++) {
            for (int j = 0; j < FIELD_SIZE; j++) {
                buffStr = intToStr(field[i][j].value) + ' ';
                fout << buffStr;
            }
        }
        fout.close();
    } 
    else { CreateNewLastSaveFile(); }

    SaveBestScore(bestScore);
}


bool Game::isGameOver()
{
    bool result = false;
    for (int i = 0; i < FIELD_SIZE; i++)
        for (int j = 0; j < FIELD_SIZE; j++)
            if (field[i][j].value == 2048) { result = true; }

    //—охран€ем наше поле
    Block temp[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            temp[i][j].value = field[i][j].value;
            temp[i][j].SetColor();
            temp[i][j].SetPos(i, j);
        }
    }

    KeyUpHandler(true);
    RandomizeValueOneTile();
    KeyDownHandler(true);
    RandomizeValueOneTile();
    KeyLeftHandler(true);
    RandomizeValueOneTile();
    KeyRightHandler(true);

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