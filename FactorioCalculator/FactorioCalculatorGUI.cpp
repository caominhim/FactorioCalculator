// FactorioCalculator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst; // current instance           
HWND hwnd;
//TCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
TCHAR szTitle[] = _T("Factorio Calculator");
TCHAR szWindowClass[] = _T("fcWinApp");
TCHAR szButtonClass[] = _T("Button");
TCHAR szDropdownClass[] = _T("Combobox");
TCHAR szEditClass[] = _T("Edit");

// Combo globals
HWND hGroupPer, hComboPer, hGroupOutputItem, hComboOutputItem;
const unsigned int ID_CBN_PER = 9;
const unsigned int ID_CBN_OUT = 10;
int listTestSize = 10;
TCHAR listTest[10][16] = {
	_T("Test0"), _T("Test1"), _T("Test2"), _T("Test3"), _T("Test4"),
	_T("Test5"), _T("Test6"), _T("Test7"), _T("Test8"), _T("Test9")
};

int listPerSize = 4;
TCHAR listPer[4][5] = {
	_T("sec"), _T("min"), _T("hour"), _T("day")
};

// Radio Button globals
HWND hGroupItemCategory, hButtonLogistic, hButtonProduction, hButtonIntermediate, hButtonCombat;
const unsigned int ID_BTN_LOG = 1;
const unsigned int ID_BTN_PRD = 2;
const unsigned int ID_BTN_INT = 3;
const unsigned int ID_BTN_CBT = 4;
//Button globals
HWND hButtonCalculate, hButtonReset;
const unsigned int ID_BTN_CAL = 5;
const unsigned int ID_BTN_RST = 6;

//  Edit globals
HWND hGroupOutputRate, hEditOutputRate, hGroupCalcInputs, hEditCalcInputs;
const unsigned int ID_EDT_OUT = 7;
const unsigned int ID_EDT_INP = 8;
const int editCalcInputsBufferSize = 10000;
wchar_t editCalcInputsBuffer[editCalcInputsBufferSize];
int editCalcInputsBufferPos = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

GuiEventHandler handler;



int getComboIndexOutput()
{
	return ComboBox_GetCurSel(hComboOutputItem);
}

int getComboIndexPer()
{
	return ComboBox_GetCurSel(hComboPer);
}

void writeToCalcInputsBuffer(std::wstring str, bool linebreak)
{
	int i;
	for (i = 0; i < str.size() && i + editCalcInputsBufferPos < editCalcInputsBufferSize; i += 1)
	{
		editCalcInputsBuffer[editCalcInputsBufferPos++] = str[i];
	}
	if (linebreak)
	{
		editCalcInputsBuffer[editCalcInputsBufferPos++] = '\r';
		editCalcInputsBuffer[editCalcInputsBufferPos++] = '\n';
	}
}

char * getEditOutputText()
{
	LRESULT textSize = SendMessage(hEditOutputRate, EM_GETLIMITTEXT, 0, 0);
	char * retval = new char[textSize];
	SendMessage(hEditOutputRate, WM_GETTEXT, textSize, (LPARAM)retval);
	return retval;
}

void loadComboItemsPer()
{
	vector<wstring> * vector_ptr = handler.get_per_items();
	int i;
	SendMessage(hComboPer, (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for (i = 0; i < vector_ptr->size(); i += 1)
	{
		const WCHAR * item = (*vector_ptr)[i].c_str();
		SendMessage(hComboPer, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(item));
	}
	SendMessage(hComboPer, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

void loadComboItemsOutput()
{
	vector<wstring> * vector_ptr = handler.get_output_items();
	int i;
	SendMessage(hComboOutputItem, (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for (i = 0; i < vector_ptr->size(); i += 1)
	{
		const WCHAR * item = (*vector_ptr)[i].c_str();
		SendMessage(hComboOutputItem, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(item));
	}
	SendMessage(hComboOutputItem, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
}

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

    wcex.style          = CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    //wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(nullptr, IDI_APPLICATION);

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
   std::wstring retval = handler.init();
   
   // base window
   hwnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 794, 673, nullptr, nullptr, hInstance, nullptr);

   hGroupCalcInputs =		CreateWindowEx(0, WC_BUTTON, _T("Calculated Inputs"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 387, 11, 380, 621, hwnd, (HMENU)0, hInstance, nullptr);
   hEditCalcInputs =		CreateWindowEx(0, WC_EDIT, _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 396, 29, 363, 595, hwnd, (HMENU)ID_EDT_INP, hInstance, nullptr);
   
   hGroupOutputItem =		CreateWindowEx(0, WC_BUTTON, _T("Output Items"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 12, 63, 353, 50, hwnd, (HMENU)0, hInstance, nullptr);
   hComboOutputItem =		CreateWindowEx(0, WC_COMBOBOX, _T("Output Items Combobox"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 21, 81, 336, 800, hwnd, (HMENU)ID_CBN_OUT, hInstance, nullptr);

   hGroupItemCategory =		CreateWindowEx(0, WC_BUTTON, _T("Item Category"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 12, 5, 353, 50, hwnd, (HMENU)0, hInstance, nullptr);
   hButtonLogistic =		CreateWindowEx(0, WC_BUTTON, _T("Logistic"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 41, 29, 65, 13, hwnd, (HMENU)ID_BTN_LOG, hInstance, nullptr);
   hButtonProduction =		CreateWindowEx(0, WC_BUTTON, _T("Production"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 113, 29, 75, 13, hwnd, (HMENU)ID_BTN_PRD, hInstance, nullptr);
   hButtonIntermediate =	CreateWindowEx(0, WC_BUTTON, _T("Intermediate"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 204, 29, 83, 13, hwnd, (HMENU)ID_BTN_INT, hInstance, nullptr);
   hButtonCombat =			CreateWindowEx(0, WC_BUTTON, _T("Combat"), WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 294, 29, 60, 13, hwnd, (HMENU)ID_BTN_CBT, hInstance, nullptr);
   
   hButtonCalculate =		CreateWindowEx(0, WC_BUTTON, _T("Calculate"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_CENTER | BS_NOTIFY | BS_PUSHBUTTON, 65, 609, 123, 23, hwnd, (HMENU)ID_BTN_CAL, hInstance, nullptr);
   hButtonReset =			CreateWindowEx(0, WC_BUTTON, _T("Reset"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_CENTER | BS_NOTIFY | BS_PUSHBUTTON, 191, 609, 123, 23, hwnd, (HMENU)ID_BTN_RST, hInstance, nullptr);
   
   hGroupOutputRate =		CreateWindowEx(0, WC_BUTTON, _T("Output Rate"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 11, 122, 353, 50, hwnd, (HMENU)0, hInstance, nullptr);
   hEditOutputRate =		CreateWindowEx(0, WC_EDIT, _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_CENTER, 26, 141, 162, 23, hwnd, (HMENU)ID_EDT_OUT, hInstance, nullptr);
   
   hGroupPer =				CreateWindowEx(0, WC_BUTTON, _T("per"), WS_VISIBLE | WS_CHILD | SS_LEFT | BS_GROUPBOX, 200, 143, 17, 15, hwnd, (HMENU)0, hInstance, nullptr);
   hComboPer =				CreateWindowEx(0, WC_COMBOBOX, _T("Per Combobox"), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 224, 143, 122, 100, hwnd, (HMENU)ID_CBN_PER, hInstance, nullptr);
   
   loadComboItemsPer();
   loadComboItemsOutput();

   if (!hwnd)
   {
		MessageBox(NULL, _T("Window generation failed!"), szTitle, NULL);
		return FALSE;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

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
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	std::wstring retval;
    switch (message)
    {
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			if (wmEvent == BN_CLICKED)
			{
				switch (wmId)
				{
					case ID_BTN_CAL:
						retval = handler.btn_click_calculate(getComboIndexOutput(), getEditOutputText());
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						break;
					case ID_BTN_RST:
						retval = handler.btn_click_reset();
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						break;
					case ID_BTN_CBT:
						retval = handler.rbtn_click_cbt();
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						loadComboItemsOutput();
						break;
					case ID_BTN_INT:
						retval = handler.rbtn_click_int();
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						loadComboItemsOutput();
						break;
					case ID_BTN_LOG:
						retval = handler.rbtn_click_log();
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						loadComboItemsOutput();
						break;
					case ID_BTN_PRD:
						retval = handler.rbtn_click_prd();
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						loadComboItemsOutput();
						break;
				}
			}
			else if (wmEvent == CBN_SELCHANGE)
			{
				switch (wmId)
				{
					case ID_CBN_PER:
						retval = handler.cbn_ichange_per(getComboIndexPer());
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						break;
					case ID_CBN_OUT:
						retval = handler.cbn_ichange_output(getComboIndexOutput());
						writeToCalcInputsBuffer(retval, true);
						SendMessage(hEditCalcInputs, WM_SETTEXT, 0, (LPARAM)editCalcInputsBuffer);
						break;
				}
			}
			UpdateWindow(hwnd);
			break;
		case WM_PAINT:
			//{
				//PAINTSTRUCT ps;
				//HDC hdc = BeginPaint(hwnd, &ps);
				// TODO: Add any drawing code that uses hdc here...
				//EndPaint(hwnd, &ps);
			//}
			break;
		case WM_DESTROY:
			handler.destroy();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(window, message, wParam, lParam);
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
