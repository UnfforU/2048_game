#pragma once
#include <Windows.h>

#define WINDOW_MARGIN 180//Размер информационного поля
#define WINDOW_WIDTH 400//Ширина окна
#define WINDOW_HEIGHT 600 //Высота окна(для удобства)
#define PLAYINGFIELD_SIZE 360 //размер игрового поля
#define PLAYINGFIELD_MARGIN 20 //отступ игрового поля
#define TILE_PADDING 7 //"зазор" между плитками(используется при отрисовке)

const TCHAR szClassWindow[] = TEXT("2048");

static int FIELD_SIZE = 4;