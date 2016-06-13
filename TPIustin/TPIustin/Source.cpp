#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IDC_CALC			2500
#define IDC_RESETARE		2501
#define IDC_HELPB			2502
#define IDC_INSERTOK		2503
#define IDC_LISTMuchie		2504

HINSTANCE _hInstance;
int _nCmdShow;
MSG msg;
HWND Main_Wnd_Hwnd;
HWND _EditSursa, _EditDestinatie, _EditCost, _Sursa, _Destiantia;
HWND _ListaNoduri, _Instructiuni;
bool check_ListaNoduri = false, check_Instructiuni = false;

HDC         hDC;
PAINTSTRUCT Ps;
HBRUSH      NewBrush;

struct  Coordonate
{
	int x;
	int y;
};
struct Muchie
{
	int sursa;
	int destinatie;
	int cost;
};

//Pentru Johnson
void _Johnson(int n, int m, int src, int dest);
int **graf_matrice;
int drum[31], k = 0;
int *dist_cost_minim;
struct Graph{
	int V, E;
	Muchie* edge;
};

Coordonate noduri_tabel[17];
Muchie Muchie_noduri[31];
int nr_noduri = 0, nr_Muchie = 0;
int sursa, destinatia;

//Meniul principal
LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Paint(HWND hWnd);
void Creeaza_Functii(HWND hWnd);
void Creare_Node(int x, int y, HWND hWnd);
bool check_distante(int x, int y);
bool check_digit(char *sir);
bool check_repetitii_Muchie(int x, int y);
void creeaza_muchie(int m1, int m2);
void Creare_Node2(int x, int y, int it);
void Resetare_Meniu(HWND hWnd);
//Meniul cu Muchiele
LRESULT CALLBACK WndProc_ListaMuchie(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void RegisteMENU_WndListaNoduri(void);
//Meniul HELP
LRESULT CALLBACK WndProc_Instructiuni(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Regist_HELP();

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
	case WM_CREATE:
		Creeaza_Functii(hWnd);
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
void Creeaza_Functii(HWND hWnd){
	//Butoanele
	CreateWindow("BUTTON", "Calculeaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 15, 100, 120, 30, hWnd, (HMENU)IDC_CALC, _hInstance, NULL);
	CreateWindow("BUTTON", "Reseteaza", WS_BORDER | WS_CHILD | WS_VISIBLE, 15, 160, 120, 30, hWnd, (HMENU)IDC_RESETARE, _hInstance, NULL);
	CreateWindow("BUTTON", "Lista Muchie", WS_BORDER | WS_CHILD | WS_VISIBLE, 15, 220, 120, 30, hWnd, (HMENU)IDC_LISTMuchie, _hInstance, NULL);
	CreateWindow("BUTTON", "HELP", WS_BORDER | WS_CHILD | WS_VISIBLE, 15, 280, 120, 30, hWnd, (HMENU)IDC_HELPB, _hInstance, NULL);
	CreateWindow("BUTTON", "OK", WS_BORDER | WS_CHILD | WS_VISIBLE, 1440, 800, 120, 30, hWnd, (HMENU)IDC_INSERTOK, _hInstance, NULL);
	CreateWindowW(L"static", L"Sursa:", WS_CHILD | WS_VISIBLE, 10, 450, 55, 25, hWnd, (HMENU)1, NULL, NULL);
	_Sursa = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 70, 450, 50, 25, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);
	CreateWindowW(L"static", L"Dest.:", WS_CHILD | WS_VISIBLE, 10, 480, 55, 25, hWnd, (HMENU)1, NULL, NULL);
	_Destiantia = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 70, 480, 50, 25, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"<< Inserati muchia dintre 2 noduri existente si costul ei dupa care apasati OK >>", WS_CHILD | WS_VISIBLE, 175, 800, 625, 25, hWnd, (HMENU)1, NULL, NULL);
	//Ferestrele pentru inserat noduri
	CreateWindowW(L"static", L"Nodul Sursa: ", WS_CHILD | WS_VISIBLE, 875, 800, 110, 25, hWnd, (HMENU)1, NULL, NULL);
	_EditSursa = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 980, 800, 50, 25, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Nodul Destinatie: ", WS_CHILD | WS_VISIBLE, 1060, 800, 140, 25, hWnd, (HMENU)1, NULL, NULL);
	_EditDestinatie = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 1200, 800, 50, 25, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Costul: ", WS_CHILD | WS_VISIBLE, 1290, 800, 60, 25, hWnd, (HMENU)1, NULL, NULL);
	_EditCost = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 1350, 800, 50, 25, hWnd, (HMENU)1, GetModuleHandle(NULL), NULL);

	CreateWindowW(L"static", L"Drumul Minim:", WS_CHILD | WS_VISIBLE, 10, 670, 120, 25, hWnd, (HMENU)1, NULL, NULL);
}


Graph* createGraph(int V, int E)
{
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	graph->E = E;
	graph->V = V;
	(*graph).edge = (Muchie*)malloc((E + 1) * sizeof(Muchie));
	return graph;
}
void freeGraph(Graph** graph){
	free((**graph).edge);
	free(*graph);
}

void BellmanFord(Graph* graph, int src)
{
	int V = graph->V;
	int E = graph->E;
	dist_cost_minim = (int*)malloc(V*sizeof(int));

	for (int i = 0; i < V; i++)
		dist_cost_minim[i] = INT_MAX;
	dist_cost_minim[src] = 0;

	for (int i = 0; i < V - 1; i++)
	{
		for (int j = 0; j < E; j++)
		{
			int u = graph->edge[j].sursa;
			int v = graph->edge[j].destinatie;
			int weight = graph->edge[j].cost;
			if (dist_cost_minim[u] != INT_MAX && dist_cost_minim[u] + weight < dist_cost_minim[v])
				dist_cost_minim[v] = dist_cost_minim[u] + weight;
		}
	}
	return;
}

void getPath(int *parent, int j)
{
	if (parent[j] == -1)
		return;
	else if (parent[j] == INT_MIN)
	{
		MessageBox(Main_Wnd_Hwnd, "Nu exita nici un drum intre cele 2 puncte!", "Error", MB_ICONERROR);
		return;
	}

	getPath(parent, parent[j]);
	drum[k++] = j;
}
void dijkstra(int **graph, int src, int dest, int nodes)
{
	int *dist = (int*)malloc(sizeof(int)*nodes);
	bool *sptSet = (bool*)malloc(sizeof(bool)*nodes);
	int *parent = (int*)malloc(sizeof(int)*nodes);
	for (int i = 0; i < nodes; i++)
	{
		parent[i] = INT_MIN;
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}
	parent[src] = -1;
	dist[src] = 0;

	for (int count = 0; count < nodes - 1; count++)
	{
		int u;
		int min = INT_MAX, min_index;
		for (int v = 0; v < nodes; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;
		u = min_index;
		sptSet[u] = true;

		for (int v = 0; v < nodes; v++)
		if (!sptSet[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
		{
			parent[v] = u;
			dist[v] = dist[u] + graph[u][v];
		}
	}
	if (dist[dest] != INT_MAX)
	{
		if (dest != src)
		{
			drum[k++] = src;
			getPath(parent, dest);
		}
	}
	else
	{
		MessageBox(Main_Wnd_Hwnd, "Nu exista drum!", "Error", MB_ICONERROR);
	}
}