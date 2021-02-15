// CrossRoad.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#pragma once
#include <chrono>
#include <random>
#include "framework.h"
#include "CrossRoad.h"
#include "CObject.h"
#include "vector"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HBITMAP hBitMap;
HWND g_hWnd;
HDC g_hDC;
RECT rcWindow;
float g_fDeltaTime;
LARGE_INTEGER g_tTime;
LARGE_INTEGER g_tSecond;
enum PlayerSize { PLAYER_WIDTH = 50, PLAYER_HEIGHT = 50 };
enum ObsSize { OBS_WIDTH = 200, OBS_HEIGHT = 100 };
enum MonsSize { MONS_WIDTH = 25, MONS_HEIGHT = 25 };

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool g_bLoop = true;

void Init();
void Update();
void Render();
void LateUpdate();
void ClearObj();
void RenderPlayer(HDC* const);
void GetPlayerKey(float);
void CheckPlayerBoundary();
void RenderObs(HDC* const);
void RenderMons(HDC* const);
void MoveMonster(float);
void SendPlayerBottom();

Player* g_player;
std::vector<Obj> obs_vec;
std::vector<Monster> mons_vec;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CROSSROAD, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CROSSROAD));

    MSG msg;

    // 기본 메시지 루프입니다:

    Init();
    
    QueryPerformanceFrequency(&g_tSecond);
    QueryPerformanceCounter(&g_tTime);

    while (g_bLoop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            Update();
            Render();
            LateUpdate();
        }
    }

    ReleaseDC(g_hWnd, g_hDC);
    ClearObj(); // 삭제 되는지 꼭 확인해 볼 것

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CROSSROAD));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_CROSSROAD);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   g_hWnd = hWnd;
   g_hDC = GetDC(hWnd);

   if (!hWnd)
   {
      return FALSE;
   }

   RECT rc = { 0,0, 1200, 700 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
   SetWindowPos(hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top,
       SWP_NOMOVE | SWP_NOZORDER);

   GetClientRect(hWnd, &rcWindow);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 0:

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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            
            HDC hMemDC = CreateCompatibleDC(hdc);
            HBITMAP OldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);

            BitBlt(hdc, 0, 0, rcWindow.right, rcWindow.bottom, hMemDC, 0, 0, SRCCOPY);
            SelectObject(hMemDC, OldBitMap);
            DeleteObject(OldBitMap);
            DeleteDC(hMemDC);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        g_bLoop = false;

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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

std::random_device g_rd;
std::mt19937 g_gen(g_rd());

float g_width;
float g_height;

void Init()
{
    g_width = (rcWindow.right + rcWindow.left) / 2.2;
    g_height = rcWindow.bottom;

    std::uniform_int_distribution<int> move_speed(500, 1000);
    std::uniform_int_distribution<int> left_mons_rand_x(0, 200);
    std::uniform_int_distribution<int> right_mons_rand_x(rcWindow.right - 200, rcWindow.right - 50);
    std::uniform_int_distribution<int> g_height_mons_rand_y(150, rcWindow.bottom - 100);

    obs_vec.reserve(2);
    mons_vec.reserve(7);

    g_player = new Player(g_width - PLAYER_WIDTH, g_height - PLAYER_HEIGHT, g_width, g_height, 500);

    obs_vec.push_back(Obj(300, 0, 300 + OBS_WIDTH, 0 + OBS_HEIGHT));
    obs_vec.push_back(Obj(700, 0, 700 + OBS_WIDTH, 0 + OBS_HEIGHT));

    for (size_t i = 0; i < mons_vec.capacity(); i++)
    {
        int startLX = left_mons_rand_x(g_gen);
        int startRX = right_mons_rand_x(g_gen);
        int startY = g_height_mons_rand_y(g_gen);
        int speed = move_speed(g_gen);

        if (i < 3)
            mons_vec.push_back(Monster(startLX, startY, startLX + MONS_WIDTH, startY + MONS_HEIGHT, 1, speed));
        
        else 
            mons_vec.push_back(Monster(startRX, startY, startRX + MONS_WIDTH, startY + MONS_HEIGHT, -1, speed));
    }
}

void Update()
{
    LARGE_INTEGER nowTime;
    QueryPerformanceCounter(&nowTime);

    g_fDeltaTime = (nowTime.QuadPart - g_tTime.QuadPart) /
        (float)g_tSecond.QuadPart;

    g_tTime = nowTime;

    static float fTimeScale = 1.f;

    float fDeltaTime = g_fDeltaTime * fTimeScale;

    GetPlayerKey(fDeltaTime);
    CheckPlayerBoundary();

    MoveMonster(fDeltaTime);
}

void Render()
{
    HDC hMemDC = CreateCompatibleDC(g_hDC);
    HBITMAP OldBitMap;

    if (hBitMap == NULL)
        hBitMap = CreateCompatibleBitmap(g_hDC, rcWindow.right, rcWindow.bottom);

    OldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);
    FillRect(hMemDC, &rcWindow, GetSysColorBrush(COLOR_WINDOW));

    RenderPlayer(&hMemDC);
    RenderObs(&hMemDC);
    RenderMons(&hMemDC);

    SelectObject(hMemDC, OldBitMap);
    DeleteObject(OldBitMap);
    DeleteDC(hMemDC);

    InvalidateRect(g_hWnd, nullptr, false);
}

void LateUpdate()
{
    // 플레이어가 골인했는지 체크 
    if (obs_vec[0].GetR() <= g_player->GetL() &&  
        g_player->GetR() <= obs_vec[1].GetL() &&
        g_player->GetB() <= obs_vec[0].GetB())
    {
        if (MessageBox(g_hWnd, L"Again?", L"CrossRoad", MB_YESNO)
            == IDYES)
        {
            SendMessage(g_hWnd, WM_COMMAND, 0, NULL);
            g_player->SetL(500);
            g_player->SetT(700);
            g_player->SetR(500 + PLAYER_WIDTH);
            g_player->SetB(700 + PLAYER_HEIGHT);
        }

        else
            SendMessage(g_hWnd, WM_DESTROY, 0, 0);
    }

    // 몬스터가 벽에 부딪힐때 마다 반대로 튕기도록
    for (auto& const elem : mons_vec)
    {
        if (elem.GetL() <= rcWindow.left)
            elem.SetDir(1);

        else if (elem.GetR() >= rcWindow.right)
            elem.SetDir(-1);
    }

    // 몬스터와 플레이어와 충돌 여부 확인
    for (auto& const elem : mons_vec)
    {
        if (g_player->GetL() <= elem.GetR() &&
            g_player->GetR() >= elem.GetL() &&
            g_player->GetT() <= elem.GetB() &&
            g_player->GetB() >= elem.GetT())
        {
            SendPlayerBottom();
        }
    }
}

void ClearObj()
{
    if (g_player != nullptr)
    {
        delete g_player;
    }
}

void RenderPlayer(HDC* const hMemDC)
{
    Rectangle(*hMemDC, g_player->GetL(),
        g_player->GetT(),
        g_player->GetR(),
        g_player->GetB());
}

void RenderObs(HDC* const hMemDC)
{
    for (auto const &elem : obs_vec)
        Rectangle(*hMemDC, elem.GetL(), elem.GetT(), elem.GetR(), elem.GetB());
}

void RenderMons(HDC* const hMemDC)
{
    for (auto const& elem : mons_vec)
        Rectangle(*hMemDC, elem.GetL(), elem.GetT(), elem.GetR(), elem.GetB());
}

void GetPlayerKey(float fDeltaTime)
{
    float fspeed = g_player->GetSpeed() * fDeltaTime;

    if (GetAsyncKeyState('A') & 0x8000)
    {
        g_player->MoveX(-fspeed);
    }

    else if (GetAsyncKeyState('D') & 0x8000)
    {
        g_player->MoveX(fspeed);
    }

    else if (GetAsyncKeyState('W') & 0x8000)
    {
        g_player->MoveY(-fspeed);
    }

    else if (GetAsyncKeyState('S') & 0x8000)
    {
        g_player->MoveY(fspeed);
    }
}

void CheckPlayerBoundary() 
{
    // 맵 벽 경계 충돌 체크
    if (g_player->GetL() <= rcWindow.left)
    {
        g_player->SetL(rcWindow.left);
        g_player->SetR(rcWindow.left + PLAYER_WIDTH);
    }
    
    if (g_player->GetT() <= rcWindow.top)
    {
        g_player->SetT(rcWindow.top);
        g_player->SetB(rcWindow.top + PLAYER_HEIGHT);
    }
    
    if (g_player->GetR() >= rcWindow.right)
    {
        g_player->SetR(rcWindow.right);
        g_player->SetL(rcWindow.right - PLAYER_WIDTH);
    }
    
    if (g_player->GetB() >= rcWindow.bottom)
    {
        g_player->SetB(rcWindow.bottom);
        g_player->SetT(rcWindow.bottom - PLAYER_HEIGHT);
    }

    // 장애물 경계 충돌 체크
    for (auto const& elem : obs_vec)
    {
        if (g_player->GetT() <= elem.GetB())
        {
            if (g_player->GetL() <= elem.GetR() && g_player->GetR() >= elem.GetR())
            {
                g_player->SetL(elem.GetR());
                g_player->SetR(elem.GetR() + PLAYER_WIDTH);
            }

            else if (g_player->GetR() >= elem.GetL() && g_player->GetL() <= elem.GetL())
            {
                g_player->SetR(elem.GetL());
                g_player->SetL(elem.GetL() - PLAYER_WIDTH);
            }

            else if (g_player->GetR() <= elem.GetR() && g_player->GetL() >= elem.GetL())
            {
                g_player->SetT(elem.GetB());
                g_player->SetB(elem.GetB() + PLAYER_HEIGHT);
            }
        }
    }
}

void MoveMonster(float fDeltaTime)
{
    for (auto& const elem : mons_vec) // auto const &elem 과 차이점 무엇
        elem.MoveX(elem.GetSpeed() * fDeltaTime * elem.GetDir());
}

void SendPlayerBottom()
{
    g_player->SetL(g_width - PLAYER_WIDTH);
    g_player->SetT(g_height - PLAYER_HEIGHT);
    g_player->SetR(g_width);
    g_player->SetB(g_height);
}