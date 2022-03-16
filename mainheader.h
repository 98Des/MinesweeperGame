#pragma once

#include "resource.h"
#include "framework.h"
#include "defines.h"

//----------------------------------------------------------------
// Preprocessor Settings
#define RANDOM_NEW_GAME
//#define THREAD_SAFE_RANDOM

//----------------------------------------------------------------
// Global Variables
HINSTANCE hInst;                                // current instance
HINSTANCE hChildInst;
HINSTANCE hDialogInst;
HWND g_hWnd;
HWND g_hWndDialog;
HWND** g_hWndChild;
WCHAR szTitle[MAX_LOAD_STRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOAD_STRING];            // the main window class name
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
INT iActualDistBetweenBoxes = 1;

INT iWindowHeight{};
INT iWindowWidth{};
INT iWindowTitleHeight = 30;

INT_PTR ipTimerId = 5;
FLOAT fTimeStep = 0.01f;
FLOAT fCurrentTime = 0.0f;

//----------------------------------------------------------------
// Forward declarations of functions included in this code module

// Class registering
ATOM                RegisterClasses(HINSTANCE hInstance);
ATOM				RegisterMainClass(HINSTANCE hInstance);
ATOM				RegisterChildClass(HINSTANCE hInstance);
ATOM				RegisterDialogClass(HINSTANCE hInstance);

// Window procedures
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProcChild(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndProcDialog(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Game initialization
BOOL                InitInstance(HINSTANCE, int);
BOOL                CreateNewMainWindow(HINSTANCE hInstance, HWND& hWnd, int nCmdShow);
BOOL				CreateNewChildrenWindows(HWND**&, HWND, INT, INT, INT, INT, INT);
BOOL                CreateNewDialogWindow(HINSTANCE hInst, HWND hWnd);
BOOL				CreateNewTileSet(Tile**&, INT, INT);

// Text functions
void				WriteTextOnScreen(HWND);
void				UpdateFlagText(HWND, INT);

// Screen numbers
POINT				GetCenterOfScreenPosition(INT, INT, INT);
INT					CalculateWindowHeight(INT, INT, INT, INT);
INT					CalculateWindowWidth(INT, INT, INT);

// Drawing functions
void                ProcessTile(HWND, INT, INT);
void                PaintBackground(HWND, INT);
void                DrawFlag(HWND);
void                SeizeFlag(HWND);
void                DrawBomb(HWND);
void                RevealAllBombs();
INT                 ChooseColor(INT, INT);

// Game conditions
void                NewGame(INT, INT, BOOL);
void                GameWon(HWND hWnd, INT_PTR timerId);
void                GameWonUnfair(HWND hWnd, INT_PTR timerId);
void                GameLost(HWND hWnd, INT_PTR timerId);
void                GameRestarted(HWND hWnd, INT_PTR timerId);
void                LaunchDebugMode();
void                CloseDebugMode();

// Other
void                BombAfterProcessCheck();
void				TimerRoutine(HWND hWnd, WPARAM wParam);
INT					Random(INT, INT);
