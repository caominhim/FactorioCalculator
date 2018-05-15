// FactorioCalculator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FactorioCalculator.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szTitle[] = _T("Factorio Calculator");
WCHAR szWindowClass[] = _T("fcWinApp");
WCHAR szButtonClass[] = _T("Button");
WCHAR szDropdownClass[] = _T("Combobox");
WCHAR szEditClass[] = _T("Edit");

int listTestSize = 10;
WCHAR listTest[10][16] = {
	_T("Test0"), _T("Test1"), _T("Test2"), _T("Test3"), _T("Test4"),
	_T("Test5"), _T("Test6"), _T("Test7"), _T("Test8"), _T("Test9")
};

int listPerSize = 4;
WCHAR listPer[4][5] = {
	_T("sec"), _T("min"), _T("hour"), _T("day")
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadStringW(hInstance, IDC_FACTORIOCALCULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FACTORIOCALCULATOR));

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

    return (int) msg.wParam;
	
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FACTORIOCALCULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FACTORIOCALCULATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //return RegisterClassExW(&wcex);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), szTitle, NULL);
	}

	return 1;
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
   hInst = hInstance; // Store instance handle in our global variable

   // base window
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 794, 673, nullptr, nullptr, hInstance, nullptr);

   // createWindowW( lpClassName, lpWindow, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam )
   // createWindowEx( dwExStyle, lpClassWindow, lpWindowName, dwStyle, x, y, nWidth, nHeight, hwndParent, hMenu, hInstance, lpParam )

   HWND hGroupCalcInputs =		CreateWindowW(szButtonClass, _T("Calculated Inputs Group"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 387, 11, 380, 621, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hEditCalcInputs =		CreateWindowW(szEditClass, _T("Calculated Inputs Edit"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL, 396, 29, 363, 595, hWnd, (HMENU)0, hInstance, nullptr);
   
   HWND hGroupOutputItem =		CreateWindowW(szButtonClass, _T("Output Items Group"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 12, 63, 353, 50, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hComboOutputItem =		CreateWindowW(szDropdownClass, _T("Output Items Combobox"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 21, 81, 336, 800, hWnd, (HMENU)0, hInstance, nullptr);
   WCHAR a[16];
   int i = 0;
   memset(&a, 0, sizeof(a));
   for (i = 0; i < listTestSize; i += 1)
   {
	   wcscpy_s(a, sizeof(a) / sizeof(WCHAR), (WCHAR*)listTest[i]);
	   SendMessage(hComboOutputItem, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)a);
   }
   SendMessage(hComboOutputItem, (UINT)CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

   HWND hGroupItemCategory =	CreateWindowW(szButtonClass, _T("Item Category Group"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 12, 5, 353, 50, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hButtonLogistic =		CreateWindowW(szButtonClass, _T("Logistic Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 41, 29, 65, 13, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hButtonProduction =		CreateWindowW(szButtonClass, _T("Production Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 113, 29, 75, 13, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hButtonIntermediate =	CreateWindowW(szButtonClass, _T("Intermediate Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 204, 29, 83, 13, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hButtonCombat =			CreateWindowW(szButtonClass, _T("Combat Button"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 294, 29, 60, 13, hWnd, (HMENU)0, hInstance, nullptr);
   
   HWND hButtonCalculate =		CreateWindowW(szButtonClass, _T("Calculate Button"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_CENTER, 65, 609, 123, 23, hWnd, (HMENU)IDOK, hInstance, nullptr);
   HWND hButtonReset =			CreateWindowW(szButtonClass, _T("Reset Button"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_CENTER, 191, 609, 123, 23, hWnd, (HMENU)IDOK, hInstance, nullptr);
   
   HWND hGroupOutputRate =		CreateWindowW(szButtonClass, _T("Output Rate Group"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 11, 122, 353, 50, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hEditOutputRate =		CreateWindowW(szEditClass, _T("Output Rate Edit"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_CENTER, 26, 141, 162, 23, hWnd, (HMENU)0, hInstance, nullptr);
   
   HWND hGroupPer =				CreateWindowW(szButtonClass, _T("per"), WS_VISIBLE | WS_CHILD | SS_LEFT | BS_GROUPBOX, 200, 143, 17, 15, hWnd, (HMENU)0, hInstance, nullptr);
   HWND hComboPer =				CreateWindowW(szDropdownClass, _T("Per Combobox"), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 224, 143, 122, 20, hWnd, (HMENU)0, hInstance, nullptr);
   WCHAR b[5];
   memset(&b, 0, sizeof(b));
   for (i = 0; i < listPerSize; i += 1)
   {
	   wcscpy_s(b, sizeof(b) / sizeof(WCHAR), (WCHAR*)listPer[i]);
	   SendMessage(hComboPer, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)b);
   }
   SendMessage(hComboPer, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
   

   if (!hWnd)
   {
		MessageBox(NULL, _T("Window generation failed!"), szTitle, NULL);
		return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
