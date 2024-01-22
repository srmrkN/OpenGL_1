#include <windows.h>
#include <gl/gl.h>
#include <math.h>



LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void DrawGround() {
	glBegin(GL_TRIANGLE_STRIP);

	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -0.2f);
	glVertex2f(1.0f, -1.0f);
	glEnd();
}


void DrawHouse() {
	glBegin(GL_TRIANGLE_STRIP); // Main body of house
	glColor3f(0.8f, 0.7f, 0.01f);
	glVertex2f(-0.8f, -0.5f);
	glVertex2f(-0.8f, 0.0f);
	glVertex2f(-0.3f, -0.5f);
	glVertex2f(-0.3f, 0.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(-0.8f, -0.5f, 0);
	glScalef(0.5, 0.5, 0);

	glBegin(GL_TRIANGLE_STRIP); // Window
	glColor3f(0.0f, 0.7f, 0.9f);
	glVertex2f(0.15f, 0.4f);
	glVertex2f(0.15f, 0.7f);
	glVertex2f(0.45f, 0.4f);
	glVertex2f(0.45f, 0.7f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP); // Door
	glColor3f(0.4f, 0.2f, 0.2f);
	glVertex2f(0.6f, 0.05f);
	glVertex2f(0.6f, 0.6f);
	glVertex2f(0.85f, 0.05f);
	glVertex2f(0.85f, 0.6f);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glRotatef(-135, 0, 0, 1);
	glTranslatef(0.2f, -0.58f, 0.0f);

	glBegin(GL_TRIANGLES); // Roof
	glColor3f(0.8f,0.5f, 0.3f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.45f);
	glVertex2f(0.45f, 0.0f);

	glEnd();
	glPopMatrix();
}

void DrawSun() {
	
	glEnable(GL_POINT_SMOOTH); // Sun
	glPointSize(90);
	
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0);

	glVertex2f(-0.9f, 0.4f);

	glEnd();
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = -30.0f;
	float color = 0.0f;
	float color2 = 0.0f;

	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		L"GLSample",
		L"OpenGL Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		500,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	/* program main loop */
	while (!bQuit)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */
			if (theta <= 80) {
				glClearColor(0.3f, 0.35f + (color * 0.005f), 0.9f + (color * 0.0009f), 0.0f);
				color += 0.2f;
			}
			else {
				glClearColor(0.0f + (color2*0.001f), 0.953f - (color2 * 0.005f), 1.0f - (color2 * 0.002f), 0.0f);
				color2 += 0.2f;
			}

			glClear(GL_COLOR_BUFFER_BIT);

			if(theta <= 80)
				glColor3f(0.0f, 0.5f+(color*0.001f), 0.4f);
			else
				glColor3f(0.0f, 0.72f - (color2 * 0.002f), 0.4f);

			DrawGround(); //Ground
			DrawHouse(); //House 1
			//House 2
			glPushMatrix();
			glRotatef(-theta, 0, 0, 1);
			DrawSun(); //Sun
			glPopMatrix();


			glEnd();

			theta += 0.1f;
			
			if (theta >= 160.0f ) {
				theta = -30.0f;
				color = 0.0f;
				color2 = 0.0f;
			}
			SwapBuffers(hDC);

			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}

