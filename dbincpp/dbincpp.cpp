// dbincpp.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "dbincpp.h"
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include "../sqlite/sqlite3.h"
#define SETTINGS_MENU_TEST 3
#define MAX_LOADSTRING 100
#define h_button1 4
#define h_button2 5
void AddMenus(HWND hWnd);
void AddControls(HWND);
HMENU hMenu;
const char* dir = "C:\\Database.db";
// Global Variables:
HINSTANCE hInst;                       // current instance
HWND hUsername;
HWND hPassword;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DBINCPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DBINCPP));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DBINCPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DBINCPP);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
static int createTable(const char* s) {
    system("title initializing databases");
    int exception;
    sqlite3* db;
    std::string query = "CREATE TABLE IF NOT EXISTS User ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "cash INTEGER NOT NULL DEFAULT 0);";

    char* Error;
    int exit = 0;
    exit = sqlite3_open(s, &db);
    exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
    if (exit != SQLITE_OK) {
        //	logs("error on user table", 2);
        sqlite3_free(Error);
    }
    else {
        //logs("User table Created Successfully", 1);	
        query = "CREATE TABLE IF NOT EXISTS Marketplace ("
            "userID INTEGER,"
            "itemName TEXT,"
            "price INTEGER);";

        exit = sqlite3_open(s, &db);
        exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
        if (exit != SQLITE_OK) {
            //	logs("error on Marketplace table", 2);
            sqlite3_free(Error);
        }
        else {
            //logs("Marketplace Database table Created Successfully", 1);
            query = "CREATE TABLE IF NOT EXISTS Inventory ("
                "userID INTEGER,"
                "item TEXT,"
                "amount INTEGER);";

            exit = sqlite3_open(s, &db);
            exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
            if (exit != SQLITE_OK) {
                //	logs("error on  table", 2);
                sqlite3_free(Error);
            }
            else {
                //	logs("Inventory table was created successfully!", 1);
                query = "CREATE TABLE IF NOT EXISTS Messages ("
                    "MessageID	INTEGER UNIQUE,"
                    "senderID INTEGER,"
                    "sender_name TEXT,"
                    "receiverID INTEGER,"
                    "message TEXT,"
                    "PRIMARY KEY(MessageID AUTOINCREMENT));";

                exit = sqlite3_open(s, &db);
                exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);

                if (exit != SQLITE_OK) {
                    //	logs("error on  table", 2);
                    sqlite3_free(Error);

                }
                else {

                }

            }
        }
    }

    sqlite3_close(db);

    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_CREATE:
        createTable(dir);
        AddControls(hWnd);
        AddMenus(hWnd);
        break;
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == h_button1) {
            wchar_t uname[100];
            wchar_t pwd[100];
            GetWindowTextW(hUsername, uname, 100);
            GetWindowTextW(hPassword, pwd, 100);
           //TODO LOGIN 
        }
        switch (wParam) {
        case 1:
           //TODO Settings tab
            break;
        case 2:
            //TODO Help Tab
            break;
        case 3:
            int return_ok = MessageBox(hWnd, L"Are you sure?", L"Clear Logs", MB_OKCANCEL);
            if (return_ok == 1) {
                int bcd = remove("dbincpp.txt"); if (bcd != 0) {
                    MessageBeep(MB_ICONERROR);
                    MessageBox(hWnd, L"Something Wen't wrong. Have you tried to run this app with admin perms?", L"Clear Logs error", MB_OK);
                }     

            }
            break;

    }
        break;
    }
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
void AddMenus(HWND hWnd) {
    hMenu = CreateMenu();
    HMENU hSettingsmenu = CreateMenu();
    AppendMenu(hSettingsmenu, MF_STRING, SETTINGS_MENU_TEST, L"Clear Logs (local)");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hSettingsmenu, L"Settings");
    AppendMenu(hMenu, MF_STRING, 2, L"Help");
    SetMenu(hWnd, hMenu);

}
void AddControls(HWND hWnd) {
    CreateWindowW(L"static", L"username:", WS_VISIBLE | WS_CHILD, 200, 100, 100, 15, hWnd, NULL, NULL, NULL);
    hUsername = CreateWindowW(L"edit", L"... ", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 152, 100, 15, hWnd, NULL, NULL, NULL);
    hPassword = CreateWindowW(L"edit", L"... ", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 170, 100, 15, hWnd, NULL, NULL, NULL);
    CreateWindowW(TEXT("BUTTON"), TEXT("Login"), WS_CHILD | WS_VISIBLE, 200, 200, 80, 20, hWnd, (HMENU) h_button1, NULL, NULL);
   // CreateWindowW(L"static", L"Not Registered Yet? Press Register.", WS_VISIBLE | WS_CHILD, 200, 252, 100, 60, hWnd,NULL, NULL, NULL);
    CreateWindowW(TEXT("BUTTON"), TEXT("Register"), WS_CHILD | WS_VISIBLE, 200, 240, 80, 20, hWnd, (HMENU) h_button2, NULL, NULL, NULL);
}