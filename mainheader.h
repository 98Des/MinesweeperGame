#pragma once

#include "resource.h"
#include "framework.h"

#define MAX_LOADSTRING 100
#define ARIAL_BOLD_FONT(size) (CreateFont(size, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Arial")))
#define DEFAULT_UNREVEALED_COLOR (RGB(200, 200, 200))
#define DEFAULT_REVEALED_COLOR (RGB(220, 220, 220))
#define RED_COLOR (RGB(255, 0, 0))
#define GREEN_COLOR (RGB(0, 255, 0))
#define BLUE_COLOR (RGB(0, 0, 255))
#define YELLOW_COLOR (RGB(255, 255, 0))
#define TURQUOISE_COLOR (RGB(0, 255, 255))
#define VIOLET_COLOR (RGB(255, 0, 255))
#define ORANGE_COLOR (RGB(255, 128, 0))
#define BLACK_COLOR (RGB(0, 0, 0))

//#define RANDOM_NEW_GAME

//----------------------------------------------------------------
// Global Variables
HINSTANCE hInst;                                // current instance
HINSTANCE hChildInst;
HINSTANCE hDialogInst;
HWND g_hWnd;
HWND g_hWndDialog;
HWND** g_hWndChild;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT rClientRect{};
UINT uiSeed = time(nullptr);

bool bTimerStarted = false;
bool bDebug = false;


//----------------------------------------------------------------
// Game Variables
INT iMinBombCount = 10;
INT iFlagCount = iMinBombCount;
INT iBombCount = iFlagCount;

INT iCurrentFlagCount = iFlagCount;
INT iActiveBombCount = iBombCount;

bool bGameStarted = false;
bool bGameStopped = false;
bool bGameUpdateInProcess = false;

enum TileStatus { FLAG, NUMBER };
typedef struct _tile
{
	TileStatus status;
	bool is_bomb;
	bool defused;
	bool revealed;
	int value;
} Tile;

Tile** g_ptTiles;


//----------------------------------------------------------------
// Window Settings
INT iBoxesHeight = 10;
INT iBoxesWidth = 10;
INT iBoxesMinHeight = 10;
INT iBoxesMinWidth = 10;
INT iBoxesMaxHeight = 24;
INT iBoxesMaxWidth = 30;
INT iBoxSize = 25;
INT iActualDistBetweenBoxes = 3;

INT iWindowHeight{};
INT iWindowWidth{};
INT iWindowTitleHeight = 30;

INT_PTR ipTimerId = 5;
FLOAT fTimeStep = 0.01f;
FLOAT fCurrentTime = 0.0f;

//----------------------------------------------------------------
// Forward declarations of functions included in this code module
ATOM                RegisterClasses(HINSTANCE hInstance);
ATOM				RegisterMainClass(HINSTANCE hInstance);
ATOM				RegisterChildClass(HINSTANCE hInstance);
ATOM				RegisterDialogClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
BOOL				CreateNewChildren(HWND**&, HWND, INT, INT, INT, INT, INT);
BOOL				CreateNewTileSet(Tile**&, INT, INT);
BOOL				CreateNewDialogWindow();
void				WriteTextOnScreen(HWND);
void				UpdateFlagText(HWND, INT);
void				TimerRoutine(HWND hWnd, WPARAM wParam);
POINT				GetCenterOfScreenPosition(INT, INT, INT);
INT					CalculateWindowHeight(INT, INT, INT, INT);
INT					CalculateWindowWidth(INT, INT, INT);
INT					Random(INT, INT);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProcChild(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK		WndProcDialog(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void PaintBackground(HWND, INT);
void DrawFlag(HWND);
void SeizeFlag(HWND);
void DrawBomb(HWND);
void ProcessTile(HWND, INT, INT);
INT ChooseColor(INT, INT);

void BombAfterProcessCheck();

void NewGame(INT, INT, BOOL);

BOOL CreateNewMainWindow(HINSTANCE hInstance, HWND& hWnd, int nCmdShow);


void GameWon(HWND hWnd, INT_PTR timerId);
void GameWonUnfair(HWND hWnd, INT_PTR timerId);
void GameLost(HWND hWnd, INT_PTR timerId);
void RevealAllBombs();
