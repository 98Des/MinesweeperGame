#include "WinAPI.22.S4.L.CPP.PwSG.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI22S4LCPPPWSG, szWindowClass, MAX_LOADSTRING);
    RegisterClasses(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI22S4LCPPPWSG));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}








ATOM RegisterClasses(HINSTANCE hInstance)
{
    return  RegisterMainClass(hInstance) |
			RegisterChildClass(hInstance) |
			RegisterDialogClass(hInstance);
}

ATOM RegisterMainClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_WINAPI22S4LCPPPWSG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPI22S4LCPPPWSG);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM RegisterChildClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = 0;
    wcex.lpfnWndProc = WndProcChild;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(DEFAULT_UNREVEALED_COLOR);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = _T("ChildWindow");
    wcex.hIconSm = nullptr;

    return RegisterClassExW(&wcex);
}

ATOM RegisterDialogClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
    wcex.hInstance = hInstance;
    wcex.lpszClassName = _T("DialogWindow");
    wcex.lpfnWndProc = (WNDPROC)WndProcDialog;

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    srand(uiSeed);

    if (!CreateNewMainWindow(hInstance, g_hWnd, nCmdShow))
        return FALSE;

    NewGame(iBoxesWidth, iBoxesHeight, FALSE);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;

    tagRECT lprRect;
    GetClientRect(hWnd, &lprRect);

    switch (message)
    {
    case WM_TIMER:
    {
        TimerRoutine(hWnd, wParam);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_GAME_CUSTOMSIZE:
	        {
				//CreateNewDialogWindow();

				g_hWndDialog = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)WndProcDialog);

                SetDlgItemText(g_hWndDialog, IDC_STATIC1, _T("Width:"));
                SetDlgItemText(g_hWndDialog, IDC_STATIC2, _T("Height:"));
                SetDlgItemText(g_hWndDialog, IDC_STATIC3, _T("Mines:"));
                SetDlgItemInt(g_hWndDialog, IDC_EDIT3, iBoxesWidth, false);
                SetDlgItemInt(g_hWndDialog, IDC_EDIT4, iBoxesWidth, false);
                SetDlgItemInt(g_hWndDialog, IDC_EDIT5, iBombCount, false);
                ShowWindow(g_hWndDialog, SW_SHOW);
	        }
            break;
        case IDM_GAME_NEW:
#ifdef RANDOM_NEW_GAME
        	NewGame(0, 0, TRUE);
#else
            NewGame(iBoxesWidth, iBoxesHeight, FALSE);
#endif
			break;
        case IDM_HELP_DEBUG:
	        {
				HMENU hMenu = GetMenu(hWnd);

                CheckMenuItem(hMenu, IDM_HELP_DEBUG, bDebug ? MF_UNCHECKED : MF_CHECKED);

                bDebug = !bDebug;

                if(bDebug)
                {
					for(int i = 0; i < iBoxesHeight; i++)
                        for(int j = 0; j < iBoxesWidth; j++)
                        {
                            ProcessTile(g_hWndChild[i][j], i, j);
                        }
                }
                else if(!bDebug)
                {
	                for(int i = 0; i < iBoxesHeight; i++)
                        for(int j = 0; j < iBoxesWidth; j++)
                        {
                            if(!g_ptTiles[i][j].revealed)
                            	PaintBackground(g_hWndChild[i][j], DEFAULT_UNREVEALED_COLOR);

                            if (g_ptTiles[i][j].status == FLAG)
                                DrawFlag(g_hWndChild[i][j]);
                        }
                }
	        }
            break;
        case IDM_HELP_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_GAME_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        HDC hdc = BeginPaint(hWnd, &ps);

		//DrawText(hdc, str, (int)_tcslen(str), &lprRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcChild(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    tagRECT lprRect;
    GetClientRect(hWnd, &lprRect);

    switch (message)
    {
    case WM_LBUTTONDOWN:
	    {
			if (bGameUpdateInProcess) return TRUE;

			if (bDebug && !bGameStarted) return TRUE;

			if (!bGameStarted && !bGameStopped) bGameStarted = !bGameStarted;

	        if (!bTimerStarted && !bGameStopped)
	        {
	            SetTimer(g_hWnd, ipTimerId, (UINT)(fTimeStep * 1000), NULL);
	            bTimerStarted = !bTimerStarted;
	        }

	        int i = (int)GetProp(hWnd, _T("index_i")),
	            j = (int)GetProp(hWnd, _T("index_j"));

            if(!g_ptTiles[i][j].revealed && !bGameStopped)
            {
                if (g_ptTiles[i][j].is_bomb && g_ptTiles[i][j].status != FLAG)
                {
                    g_ptTiles[i][j].revealed = true;
                    DrawBomb(g_hWndChild[i][j]);
                    bTimerStarted = false;
                	KillTimer(g_hWnd, ipTimerId);
                    ::MessageBox(g_hWnd, _T("KABOOM!"), _T("You losed!"), MB_OK | MB_ICONERROR);
                    bGameStopped = true;
                }
                else if(g_ptTiles[i][j].status == NUMBER)
                {
                    ProcessTile(hWnd, i, j);
                    BombAfterProcessCheck();

                    if(iActiveBombCount == 0)
                    {
                        bTimerStarted = false;
                        KillTimer(g_hWnd, ipTimerId);
                        ::MessageBox(g_hWnd, _T("All mines have been defused."), _T("You wined!"), MB_OK);
                        bGameStopped = true;
                    }
                }
            }
	    }
		break;
    case WM_RBUTTONDOWN:
	    {
			if (bGameUpdateInProcess) return TRUE;

			int i = (int)GetProp(hWnd, _T("index_i")),
    			j = (int)GetProp(hWnd, _T("index_j"));

            //WCHAR str[MAX_LOADSTRING];
            //_stprintf_s(str, MAX_LOADSTRING, _T("i: %d,j: %d"), i, j);
            //SetWindowTextW(g_hWnd, str);

            if(g_ptTiles[i][j].status != FLAG && iCurrentFlagCount > 0 && !g_ptTiles[i][j].revealed)
            {
                UpdateFlagText(g_hWnd, --iCurrentFlagCount);

                g_ptTiles[i][j].status = FLAG;

                DrawFlag(hWnd);

                if (g_ptTiles[i][j].is_bomb && !g_ptTiles[i][j].defused)
                {
                    iActiveBombCount--;
                    g_ptTiles[i][j].defused = true;
                }
            }
            else if(g_ptTiles[i][j].status == FLAG && !g_ptTiles[i][j].revealed)
            {
                UpdateFlagText(g_hWnd, ++iCurrentFlagCount);

                g_ptTiles[i][j].status = NUMBER;

                SeizeFlag(hWnd);

                if (bDebug)
                    ProcessTile(hWnd, i, j);

                if (g_ptTiles[i][j].is_bomb)
                {
                    iActiveBombCount++;
                    g_ptTiles[i][j].defused = false;
                }
            }

            if (iActiveBombCount == 0)
            {
                bTimerStarted = false;
                ::MessageBox(g_hWnd, _T("All mines have been defused."), _T("You wined!"), MB_OK);
                bGameStopped = true;
                KillTimer(g_hWnd, ipTimerId);
            }
	    }
	break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        //PostQuitMessage(0);
        break;
    case WM_NCDESTROY:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK WndProcDialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == 2)			//CANCEL
        {
            EndDialog(hWnd, LOWORD(wParam));
            g_hWndDialog = NULL;
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDOK)
        {
            //Kill_Children();
            iBoxesWidth = GetDlgItemInt(hWnd, IDC_EDIT3, nullptr, false);
            iBoxesHeight = GetDlgItemInt(hWnd, IDC_EDIT4, nullptr, false);
            iBombCount = GetDlgItemInt(hWnd, IDC_EDIT5, nullptr, false);

            if (iBombCount < iMinBombCount)
                iBombCount = iMinBombCount;
            if (iBombCount > iBoxesWidth * iBoxesHeight)
                iBombCount = iMinBombCount;

            if (iBoxesWidth < iBoxesMinWidth)
                iBoxesWidth = iBoxesMinWidth;
            if (iBoxesWidth > iBoxesMaxWidth)
                iBoxesWidth = iBoxesMaxWidth;

            if (iBoxesHeight < iBoxesMinHeight)
                iBoxesHeight = iBoxesMinHeight;
            if (iBoxesHeight > iBoxesMaxHeight)
                iBoxesHeight = iBoxesMaxHeight;

            iFlagCount = iBombCount;

            NewGame(iBoxesWidth, iBoxesHeight, FALSE);

            //Make_New_Game(hInst, dialogncmdshow);
            EndDialog(hWnd, LOWORD(wParam));
            g_hWndDialog = NULL;
            return (INT_PTR)TRUE;
        }
        break;

    default:
        return (INT_PTR)FALSE;
    }

    return TRUE;
}

BOOL CreateNewDialogWindow()
{
    auto p = GetCenterOfScreenPosition(iWindowWidth, iWindowHeight, iActualDistBetweenBoxes - 2);

    if(!(g_hWndDialog = CreateWindowW(
        _T("DialogWindow"),
        _T("Dialog"),
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        p.x,
        p.y,
        200,
        200,
        nullptr,
        nullptr,
        hInst,
        nullptr
    ))) 
        return FALSE;

    ShowWindow(g_hWndDialog, 1);
    UpdateWindow(g_hWndDialog);

    return TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}






INT Random(INT min, INT max)
{
    return min + (rand() % (max - min));
}

INT	CalculateWindowHeight(INT boxes_height, INT box_size, INT btwboxes_dist, INT title = iWindowTitleHeight)
{
    return boxes_height * (box_size + btwboxes_dist) + 59 + title + 2 * btwboxes_dist;
}

INT	CalculateWindowWidth(INT boxes_width, INT box_size, INT btwboxes_dist)
{
    return boxes_width * (box_size + btwboxes_dist) + 16 + 2 * btwboxes_dist;
}

BOOL CreateNewChildren(HWND**& childptr, HWND parentptr, INT width, INT height, INT boxsize, INT boxdist, INT title)
{
    childptr = new HWND * [height];
    for (int i = 0; i < height; i++)
    {
        childptr[i] = new HWND[width];
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            childptr[i][j] = CreateWindowW(
                _T("ChildWindow"),
                nullptr,
                WS_BORDER | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP,
                (j + 1) * boxdist + j * (boxsize),
                (i + 1) * boxdist + i * (boxsize)+title,
                boxsize,
                boxsize,
                parentptr,
                nullptr,
                hChildInst,
                nullptr);

            if (!childptr[i][j]) return FALSE;

            SetWindowLong(childptr[i][j], GWL_STYLE, 0);

            SetProp(childptr[i][j], _T("index_i"), (HANDLE)i);
            SetProp(childptr[i][j], _T("index_j"), (HANDLE)j);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ShowWindow(childptr[i][j], 1);
            UpdateWindow(childptr[i][j]);
        }
    }

    return FALSE;
}

BOOL CreateNewTileSet(Tile**& tiles, INT width, INT height)
{
    tiles = new Tile * [height];
    if (!tiles) return FALSE;
    for (int i = 0; i < height; i++)
    {
        tiles[i] = new Tile[width];
        if (!tiles[i]) return FALSE;

        for (int j = 0; j < width; j++)
            tiles[i][j] = { NUMBER, false, false,  false, 0 };
    }


    std::list<POINT> lBombCoordinates;
    int Bombs = iBombCount;
    while (Bombs > 0)
    {
        int i = Random(0, height);
        int j = Random(0, width);

        if(!tiles[i][j].is_bomb)
        {
            // the bomb has been planted
            tiles[i][j].is_bomb = true;
            lBombCoordinates.push_back({ i, j });
            Bombs--;
        }
    }

    while (!lBombCoordinates.empty())
	//for (auto& coor : lBombCoordinates) // x = i, y = j
    {
        POINT coor = lBombCoordinates.back();
        lBombCoordinates.pop_back();

        int i = coor.x;
        int j = coor.y;

        if (i > 0 && j > 0)
            tiles[i - 1][j - 1].value++;

        if (i > 0)
            tiles[i - 1][j].value++;

        if (i > 0 && j < width - 1)
            tiles[i - 1][j + 1].value++;

        if (j > 0)
            tiles[i][j - 1].value++;

        if (j < width - 1)
            tiles[i][j + 1].value++;

        if (i < iBoxesHeight - 1 && j > 0)
            tiles[i + 1][j - 1].value++;

        if (i < height - 1)
            tiles[i + 1][j].value++;

        if (i < height - 1 && j < width - 1)
            tiles[i + 1][j + 1].value++;
        
    }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (tiles[i][j].is_bomb) tiles[i][j].value = -1;

    return TRUE;
}



POINT GetCenterOfScreenPosition(INT width, INT height, INT boxdistance)
{
    return POINT({ (GetSystemMetrics(SM_CXSCREEN) + 2 * boxdistance - width) / 2,
    (GetSystemMetrics(SM_CYSCREEN) + 2 * boxdistance - height) / 2 });
}

void WriteTextOnScreen(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);

    tagRECT lprRect;
    GetClientRect(hWnd, &lprRect);

    auto hFont = ARIAL_BOLD_FONT(25);
    SelectObject(hdc, hFont);

    SetTextColor(hdc, RGB(255, 0, 0));
    


    WCHAR str[MAX_LOADSTRING];
    _stprintf_s(str, MAX_LOADSTRING, _T("%06.1f"), fCurrentTime);

    ExtTextOut(hdc, (lprRect.right - lprRect.left) / 4, 0, ETO_CLIPPED, &lprRect, str, _tcslen(str), nullptr);

    _stprintf_s(str, MAX_LOADSTRING, _T("%d"), iFlagCount);

    ExtTextOut(hdc, (lprRect.right - lprRect.left) * 3 / 4, 0, ETO_CLIPPED, &lprRect, str, _tcslen(str), nullptr);


    DeleteObject(hFont);
    DeleteDC(hdc);
}

void UpdateFlagText(HWND hWnd, INT newFlagCount)
{
    if(newFlagCount < 0) return;

    HDC hdc = GetDC(hWnd);

    tagRECT lprRect;
    GetClientRect(hWnd, &lprRect);

    auto hFont = ARIAL_BOLD_FONT(25);
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 0, 0));

    //iFlagCount = newFlagCount;


    WCHAR str[MAX_LOADSTRING];
    _stprintf_s(str, MAX_LOADSTRING, _T("%02d"), newFlagCount);
    ExtTextOut(hdc, (lprRect.right - lprRect.left) * 3 / 4, 0, ETO_CLIPPED, &lprRect, str, _tcslen(str), nullptr);

    DeleteObject(hFont);
    DeleteDC(hdc);
}

void PaintBackground(HWND hWnd, INT color)
{
    HDC hdc = GetDC(hWnd);
    RECT rc;
    GetClientRect(hWnd, &rc);
    HBRUSH brush = CreateSolidBrush(color);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Rectangle(hdc, -1, -1, rc.right - rc.left + 1, rc.bottom - rc.top + 1);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
    ReleaseDC(hWnd, hdc);
}

void DrawFlag(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    HBITMAP bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
    BITMAP bmInfo;
    GetObject(bitmap, sizeof(bmInfo), &bmInfo);
    BitBlt(hdc, 0, 0, 20, 20, memDC, 0, 0, SRCCOPY);
    StretchBlt(hdc, 0, 0, iBoxSize, iBoxSize, memDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
    SelectObject(memDC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memDC);
}

void SeizeFlag(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    RECT rc;
    GetClientRect(hWnd, &rc);
    HBRUSH brush = CreateSolidBrush(DEFAULT_UNREVEALED_COLOR);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    Rectangle(hdc, -1, -1, rc.right + 1, rc.bottom + 1);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);
    ReleaseDC(hWnd, hdc);
}

void DrawBomb(HWND hWnd)
{
    if (g_ptTiles[(int)GetProp(hWnd, _T("index_i"))][(int)GetProp(hWnd, _T("index_j"))].revealed)
        PaintBackground(hWnd, DEFAULT_REVEALED_COLOR);
    else
        PaintBackground(hWnd, DEFAULT_UNREVEALED_COLOR);

    HDC hdc = GetDC(hWnd);
    HPEN hPenOld, hPen = CreatePen(PS_SOLID, iBoxSize * 4 / 5, BLACK_COLOR);
    hPenOld = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, iBoxSize / 2 - 1, iBoxSize / 2 - 1, nullptr);
    LineTo(hdc, iBoxSize / 2 - 1, iBoxSize / 2 - 1);

    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);
    DeleteDC(hdc);









    //HDC hdc = GetDC(hWnd);
    //HBITMAP bitmap = LoadBitmap(hInst, MAKEINTRESOURCE());
    //HDC memDC = CreateCompatibleDC(hdc);
    //HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
    //BITMAP bmInfo;
    //GetObject(bitmap, sizeof(bmInfo), &bmInfo);
    //BitBlt(hdc, 0, 0, 20, 20, memDC, 0, 0, SRCCOPY);
    //StretchBlt(hdc, 0, 0, iBoxSize, iBoxSize, memDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
    //SelectObject(memDC, oldBitmap);
    //DeleteObject(bitmap);
    //DeleteDC(memDC);
}

void ProcessTile(HWND hWnd, INT i, INT j)
{
    if (g_ptTiles[i][j].status == FLAG)
    {
        DrawFlag(hWnd);
        return;
    }

    if (g_ptTiles[i][j].is_bomb)
    {
        DrawBomb(hWnd);
        return;
    }

	if(!bDebug)
	{
        g_ptTiles[i][j].revealed = true;
	}

    HDC hdc = GetDC(hWnd);
    RECT rc;
    GetClientRect(hWnd, &rc);

    /*TCHAR s[2];
    _itow_s(matrix[i][j], s, 16);
    int r, g, b;
    Choose_Color(matrix[i][j], &r, &g, &b);*/



    if(g_ptTiles[i][j].value == 0 && g_ptTiles[i][j].revealed)
    {
        PaintBackground(hWnd, DEFAULT_REVEALED_COLOR);

        //Sleep(10);

        if (i > 0 &&
            j > 0 &&
            !g_ptTiles[i - 1][j - 1].revealed &&
            !g_ptTiles[i - 1][j - 1].is_bomb)
            ProcessTile(g_hWndChild[i - 1][j - 1], i - 1, j - 1);

        if (i > 0 &&
            !g_ptTiles[i - 1][j].revealed &&
            !g_ptTiles[i - 1][j].is_bomb)
            ProcessTile(g_hWndChild[i - 1][j], i - 1, j);

        if (j > 0 &&
            !g_ptTiles[i][j - 1].revealed &&
            !g_ptTiles[i][j - 1].is_bomb)
            ProcessTile(g_hWndChild[i][j - 1], i, j - 1);

        if (i < iBoxesHeight - 1 &&
            j < iBoxesWidth - 1 &&
            !g_ptTiles[i + 1][j + 1].revealed &&
            !g_ptTiles[i + 1][j + 1].is_bomb)
            ProcessTile(g_hWndChild[i + 1][j + 1], i + 1, j + 1);

        if (i < iBoxesHeight - 1 &&
            !g_ptTiles[i + 1][j].revealed &&
            !g_ptTiles[i + 1][j].is_bomb)
            ProcessTile(g_hWndChild[i + 1][j], i + 1, j);

        if (j < iBoxesWidth - 1 &&
            !g_ptTiles[i][j + 1].revealed &&
            !g_ptTiles[i][j + 1].is_bomb)
            ProcessTile(g_hWndChild[i][j + 1], i, j + 1);

        if (i < iBoxesHeight - 1 &&
            j > 0 &&
            !g_ptTiles[i + 1][j - 1].revealed &&
            !g_ptTiles[i + 1][j - 1].is_bomb)
            ProcessTile(g_hWndChild[i + 1][j - 1], i + 1, j - 1);

        if (i > 0 &&
            j < iBoxesWidth - 1 &&
            !g_ptTiles[i - 1][j + 1].revealed &&
            !g_ptTiles[i - 1][j + 1].is_bomb)
            ProcessTile(g_hWndChild[i - 1][j + 1], i - 1, j + 1);
    }
    else if (g_ptTiles[i][j].value > 0 && g_ptTiles[i][j].revealed)
    {
        SetBkMode(hdc, TRANSPARENT);
        
    	PaintBackground(hWnd, DEFAULT_REVEALED_COLOR);

        SetTextColor(hdc, ChooseColor(i, j));

        WCHAR str[MAX_LOADSTRING];
        _stprintf_s(str, MAX_LOADSTRING, _T("%d"), g_ptTiles[i][j].value);
        DrawText(hdc, str, (int)_tcslen(str), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    if (bDebug && !g_ptTiles[i][j].revealed)
    {
        SetBkMode(hdc, TRANSPARENT);
        
        if (g_ptTiles[i][j].value > 0)
        {
            SetTextColor(hdc, ChooseColor(i, j));

            WCHAR str[MAX_LOADSTRING];
            _stprintf_s(str, MAX_LOADSTRING, _T("%d"), g_ptTiles[i][j].value);
            DrawText(hdc, str, (int)_tcslen(str), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    ReleaseDC(hWnd, hdc);
}

INT ChooseColor(INT i, INT j)
{
    int res{};

    switch (g_ptTiles[i][j].value)
    {
    case 1:
        res = BLUE_COLOR;
        break;
    case 2:
        res = GREEN_COLOR;
        break;
    case 3:
        res = RED_COLOR;
        break;
    case 4:
        res = YELLOW_COLOR;
        break;
    case 5:
        res = TURQUOISE_COLOR;
        break;
    case 6:
        res = VIOLET_COLOR;
        break;
    case 7:
        res = ORANGE_COLOR;
        break;
    case 8:
        res = BLACK_COLOR;
        break;
    }

    return res;
}

void BombAfterProcessCheck()
{
    for (int i = 0; i < iBoxesHeight; i++)
        for (int j = 0; j < iBoxesWidth; j++)
            if (g_ptTiles[i][j].is_bomb && !g_ptTiles[i][j].defused)
                if((i == 0 && j == 0 && 
                    g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j + 1].revealed && g_ptTiles[i + 1][j].revealed) ||

                    (i == 0 && j > 0 && j < iBoxesWidth - 1 && 
                        g_ptTiles[i][j - 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed && g_ptTiles[i + 1][j + 1].revealed) ||

                    (i == 0 && j == iBoxesWidth - 1 && 
                        g_ptTiles[i][j - 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed) ||

                    (i > 0 && i < iBoxesHeight - 1 && j == 0 && 
                        g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j + 1].revealed && g_ptTiles[i + 1][j].revealed) ||

                    (i == iBoxesHeight - 1 && j == 0 && 
                        g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed) ||

                    (i == iBoxesHeight - 1 && j > 0 && j < iBoxesWidth - 1 && 
                        g_ptTiles[i][j - 1].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed) ||

                    (i == iBoxesHeight - 1 && j == iBoxesWidth - 1 && 
                        g_ptTiles[i][j - 1].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed) ||

                    (i > 0 && i < iBoxesHeight - 1 && j == iBoxesWidth - 1 
                        && g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i][j - 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed) ||

                    (i > 0 && i < iBoxesHeight - 1 && j > 0 && j < iBoxesWidth - 1 && 
                        g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j - 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed && g_ptTiles[i + 1][j + 1].revealed))
                {
                    iActiveBombCount--;
                    g_ptTiles[i][j].defused = true;
                }

                //if (i == 0 && j == 0)
                //{
                //    if (g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j + 1].revealed && g_ptTiles[i + 1][j].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i == 0 && j > 0 && j < iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i][j - 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed && g_ptTiles[i + 1][j + 1].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i == 0 && j == iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i][j - 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i > 0 && i < iBoxesHeight - 1 && j == 0)
                //{
                //    if (g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j + 1].revealed && g_ptTiles[i + 1][j].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i == iBoxesHeight - 1 && j == 0)
                //{
                //    if (g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i == iBoxesHeight - 1 && j > 0 && j < iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i][j - 1].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j + 1].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i == iBoxesHeight - 1 && j == iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i][j - 1].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i > 0 && i < iBoxesHeight - 1 && j == iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i][j - 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
                //else if (i > 0 && i < iBoxesHeight - 1 && j > 0 && j < iBoxesWidth - 1)
                //{
                //    if (g_ptTiles[i - 1][j - 1].revealed && g_ptTiles[i - 1][j].revealed && g_ptTiles[i - 1][j + 1].revealed && g_ptTiles[i][j - 1].revealed && g_ptTiles[i][j + 1].revealed && g_ptTiles[i + 1][j - 1].revealed && g_ptTiles[i + 1][j].revealed && g_ptTiles[i + 1][j + 1].revealed)
                //    {
                //        iActiveBombCount--;
                //        g_ptTiles[i][j].defused = true;
                //    }
                //}
}

void NewGame(INT width, INT height, BOOL random)
{
    bGameUpdateInProcess = true;

    if (random) 
    {
        iBoxesHeight = Random(iBoxesMinHeight, iBoxesMaxHeight);
        iBoxesWidth = Random(iBoxesMinWidth, iBoxesMaxWidth);
    }
    else
    {
        iBoxesHeight = height;
        iBoxesWidth = width;
    }

    const int iDistBetweenBoxes = iActualDistBetweenBoxes - 2;

    iWindowHeight = CalculateWindowHeight(iBoxesHeight, iBoxSize, iDistBetweenBoxes, iWindowTitleHeight);
    iWindowWidth = CalculateWindowWidth(iBoxesWidth, iBoxSize, iDistBetweenBoxes);

    POINT newPos = GetCenterOfScreenPosition(iWindowWidth, iWindowHeight, iDistBetweenBoxes);

    MoveWindow(g_hWnd, newPos.x, newPos.y, iWindowWidth, iWindowHeight, TRUE);

    CreateNewChildren(g_hWndChild, g_hWnd, iBoxesWidth, iBoxesHeight, iBoxSize, iDistBetweenBoxes, iWindowTitleHeight);
    CreateNewTileSet(g_ptTiles, iBoxesWidth, iBoxesHeight);

    fCurrentTime = 0.0f;
    bTimerStarted = false;
    bGameStarted = false;
    bGameStopped = false;

    iCurrentFlagCount = iFlagCount;
    iActiveBombCount = iBombCount;

    if (bDebug)
    {
        bDebug = false;

        HMENU hMenu = GetMenu(g_hWnd);
        CheckMenuItem(hMenu, IDM_HELP_DEBUG, !bDebug ? MF_UNCHECKED : MF_CHECKED);
    }

    WriteTextOnScreen(g_hWnd);

    bGameUpdateInProcess = false;
}

BOOL CreateNewMainWindow(HINSTANCE hInstance, HWND& hWnd, int nCmdShow)
{
    const int distbtwboxes = iActualDistBetweenBoxes - 2;

    iWindowHeight = CalculateWindowHeight(iBoxesHeight, iBoxSize, distbtwboxes, iWindowTitleHeight);
    iWindowWidth = CalculateWindowWidth(iBoxesWidth, iBoxSize, distbtwboxes);

    POINT Pos = GetCenterOfScreenPosition(iWindowWidth, iWindowHeight, distbtwboxes);

    g_hWnd = CreateWindowW(
        szWindowClass,
        szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        Pos.x, Pos.y,
        iWindowWidth, iWindowHeight,
        nullptr,
        nullptr,
        hInst,
        nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    WCHAR str[MAX_LOADSTRING];
    _stprintf_s(str, MAX_LOADSTRING, _T("Minesweeper"));
    SetWindowTextW(g_hWnd, str);

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}


void TimerRoutine(HWND hWnd, WPARAM wParam)
{
    if (static_cast<INT_PTR>(wParam) != ipTimerId && bTimerStarted) return;

    fCurrentTime += fTimeStep;

    auto hdc = GetDC(hWnd);
    auto hFont = ARIAL_BOLD_FONT(25);
    RECT lprRect{};

	GetClientRect(hWnd, &lprRect);
    SelectObject(hdc, hFont);
    SetTextColor(hdc, RGB(255, 0, 0));

    WCHAR str[MAX_LOADSTRING];
    _stprintf_s(str, MAX_LOADSTRING, _T("%06.1f"), fCurrentTime);
    ExtTextOut(hdc, (lprRect.right - lprRect.left) / 4, 0, ETO_CLIPPED, &lprRect, str, _tcslen(str), nullptr);

    DeleteObject(hFont);
    DeleteDC(hdc);
}