// CardScanner.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "commdlg.h"

#include "CardScanner.h"

#include "AboutDialog.h"
#include "ConfigureDialog.h"

#include "defs.h"
#include "globals.h"

#include "ct_error.h"
#include "ct_interface.h"

#define MAX_LOADSTRING 100

// Global Variables:
WCHAR *reader_list = NULL;                  // Readers list (retrieved from win SCard API)
unsigned long dwActProtocol;                // Establish active protocol

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Dlg callback functions
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Error(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Status(HWND, UINT, WPARAM, LPARAM);

// Forward declaration of menu handling 
void set_menus(HWND hWnd);

// main()
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CARDSCANNER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CARDSCANNER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CARDSCANNER));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CARDSCANNER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hMainWindow = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.+
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LONG ret_code;
    BYTE* data;
    RECT rect;
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDialogCallback);
                break;

            case IDM_FILE_CONFIGURE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_CONFIG), hWnd, ConfigureDlgCallback);
                break;

            case IDM_CONNECT:

                // Get hContext for a card scanner
                if (SCARD_S_SUCCESS != (ret_code = _ct_get_context(&hScardContext))) {
                    // Show error dialog
                    show_scard_error(ret_code);
                    _app_state = CONFIGURED;
                    set_menus(hWnd);
                }
                else {
                    // Get card reader list
                    if (SCARD_S_SUCCESS != (ret_code = _ct_get_reader_list(hScardContext, &reader_list))) {
                        // Show error dialog
                        show_scard_error(ret_code);
                        _app_state = CONFIGURED;
                        set_menus(hWnd);
                        break;
                    }
                    else {

                        _app_state = READER_CONNECTED;
                        // Show OK dialog and reset menu items
                        MessageBox(0, reader_list, L"SDCARD reader found and connected", MB_ICONINFORMATION);
                        set_menus(hWnd);
                        break;
                    }
                }

            case IDM_CARD_READ:
                // Start polling thread
                _app_state = READER_POLLING;
                _beginthread(_ct_poll, 0, NULL);
                set_menus(hWnd);
                break;

            case IDM_CARD_STOPREAD:
                // Just stop polling
                _app_state = READER_NOT_POLLING;
                set_menus(hWnd);
                break;

            case IDM_DISCONNECT:
                // Terminate polling thread
                _app_state = CONFIGURED;
                // Set all handles back to 0
                hScardContext = 0;   
                hCard = 0;              
                dwActProtocol = 0;         
                set_menus(hWnd);
                break;

            case IDM_FILE_UPLOAD:
                break;

            case IDM_FILE_CLEAR:

                if (IDOK == MessageBox(hWnd, L"Are you sure? This will clear the current screen and lose all data.", L"Just Checking", MB_ICONINFORMATION)) {
                    // Clear the EditText control and reset the application back to not configure
                    clearEditText();
                    _app_state = NOT_CONFIGURED;
                    set_menus(hWnd);
                }
                break;

            case IDM_FILE_SAVE:
            {
                WCHAR szFileName[MAX_PATH];
                OPENFILENAME ofn;
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Text Files (*.txt)";
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
                ofn.lpstrDefExt = L".txt";

                if (GetSaveFileName(&ofn)) {

                }

                break;
            }
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_BAD_POLL:
        appendEditText(L"DEBUG ... polling");
        appendEditText(L"\n");
        if (_app_state == READER_POLLING) {
            _beginthread(_ct_poll, 0, (void*)NULL);
        }
        break;

    case WM_NEW_SID:

        // lParam is the pointer to the buffer where we stored the SID
        // Get the ID as a string  (WCHAR*) this is a bit hacky but I
        // can't be bothered with the multiToWide API calls
        data = (BYTE*)lParam;
        
        WCHAR sid[16];
        sid[0] = (WCHAR) data[2];
        sid[1] = (WCHAR) data[3];
        sid[2] = (WCHAR) data[4];
        sid[3] = (WCHAR) data[5];
        sid[4] = (WCHAR) data[6];
        sid[5] = (WCHAR) data[7];
        sid[6] = (WCHAR) data[8];
        sid[7] = (WCHAR) data[9];
        sid[8] = (WCHAR) 0x00;
        
        // Check that the ID isn't already in the EditText buffer (strstr)
        WCHAR buffer[10192];
        GetWindowText(hEditWindow, buffer, 10192);
        if (NULL == wcsstr(buffer, sid)) {

            // Get a timestamp as a string
            SYSTEMTIME lt = { 0 };
            GetLocalTime(&lt);
            WCHAR* timeStr = malloc(256 * sizeof(WCHAR));
            wsprintf(timeStr, L"%02d/%02d/%04d %02d:%02d:%02d", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);

            // Append the timestamp and the new ID to the EditText buffer
            appendEditText(timeStr);
            appendEditText(L" Student ID: ");
            appendEditText(sid);
            appendEditText(L"\n");

            free(timeStr);
        }
        free(data);

        if (_app_state == READER_POLLING) {
            _beginthread(_ct_poll, 0, (void*)NULL);
        }
        break;

    case WM_STATE_CHANGE:
        // Check the global app state and modify menu states accordingly
        set_menus(hWnd);
        break;

    case WM_CREATE:
        // Instantiate a child edit window
        GetWindowRect(hWnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        hEditWindow = CreateWindow( L"EDIT",
                                    NULL,
                                    WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT |  ES_MULTILINE | ES_AUTOVSCROLL,
                                    0, 0, width, height,
                                    hWnd,
                                    NULL,
                                    hInst,
                                    NULL);
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


void set_menus(HWND hWnd) {

    HMENU hMenu = GetMenu(hWnd);
    
    switch (_app_state)
    {
    case NOT_CONFIGURED:
        // Cannot connect, poll etc - basically everything gray
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_GRAYED);
        break;

    case CONFIGURED:
        // Now we can connect the card to the reader or
        // we can clear and return to the unconfigured state
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_GRAYED);
        break;

    case READER_CONNECTED:
        // Now we can decide to start reading from the card
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
        break;

    case READER_POLLING:
        // We can stop polling and/or disconnect
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
        break;

    case READER_NOT_POLLING:
        // We can restart polling and/or disconnect
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_ENABLED);
        break;

    case READER_DISCONNECTED:
        // We can reconnect or save/upload data etc
        EnableMenuItem(hMenu, IDM_FILE_CONFIGURE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_CLEAR, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_FILE_UPLOAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CONNECT, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CARD_READ, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_CARD_STOPREAD, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_DISCONNECT, MF_GRAYED);
        break;

    case READER_TERMINATED:
        break;

    default:
        break;
    }
}

