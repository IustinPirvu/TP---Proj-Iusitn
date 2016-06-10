#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

HINSTANCE _hInstance;
int _nCmdShow;
HWND Main_Wnd_Hwnd;
MSG msg;

HDC         hDC;
PAINTSTRUCT Ps;
HBRUSH      NewBrush;

void Paint(HWND hWnd);
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	_nCmdShow = nShowCmd;
	_hInstance = hInst;

	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wClass)){
		int nResult = GetLastError();
		MessageBox(NULL, "Window class creation failed\r\n", "Window Class Failed", MB_ICONERROR);
	}

	HWND hWnd = CreateWindowEx(NULL, "Window Class", "Windows application", WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 1600, 900, NULL, NULL, hInst, NULL);
	Main_Wnd_Hwnd = hWnd;
	if (!hWnd){
		int nResult = GetLastError();
		MessageBox(NULL, "Window creation failed\r\n", "Window Creation Failed", MB_ICONERROR);
	}

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT point;
	switch (msg)
	{
	case WM_PAINT:
		Paint(hWnd);
		break;

	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
void Paint(HWND hWnd){
	hDC = BeginPaint(hWnd, &Ps);

	HPEN hPenOld;
	HPEN hLinePen;
	COLORREF qLineColor;

	qLineColor = RGB(255, 0, 0);
	hLinePen = CreatePen(PS_SOLID, 8, qLineColor);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);
	MoveToEx(hDC, 150, 0, NULL);
	LineTo(hDC, 150, 950);
	MoveToEx(hDC, 150, 775, NULL);
	LineTo(hDC, 1600, 775);

	HBRUSH      brCross = CreateHatchBrush(HS_CROSS, RGB(0, 128, 245));
	SelectObject(hDC, brCross);
	RoundRect(hDC, 150, 775, 1580, 850, 10, 10);

	SetTextColor(hDC, RGB(0, 0, 255));
	TextOut(hDC, 10, 760, "Powered by:", 12);
	TextOut(hDC, 30, 782, "IUSTIN PIRVU", 12);

	qLineColor = RGB(0, 0, 255);
	hLinePen = CreatePen(PS_SOLID, 4, qLineColor);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen);
	HBRUSH      brDiagCross = CreateHatchBrush(HS_DIAGCROSS, RGB(0, 128, 0));
	SelectObject(hDC, brDiagCross);
	RoundRect(hDC, 175, 25, 1565, 750, 10, 10);

	NewBrush = CreateSolidBrush(RGB(255, 2, 5));
	SelectObject(hDC, NewBrush);
	Rectangle(hDC, 5, 440, 145, 515);
}