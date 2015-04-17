/* 
 * Example of a Windows OpenGL program.  
 * The OpenGL code is the same as that used in  
 * the X Window System sample 
 */ 
#include <windows.h> 
#include <GL/gl.h> 
#include <GL/glu.h> 
#include "Game.h"

/*
 * Link opengl libraries 
 */
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
 
/* Windows globals, defines, and prototypes */ 
CHAR szAppName[]="Win OpenGL"; 
HWND  ghWnd; 
HDC   ghDC; 
HGLRC ghRC; 

#define SWAPBUFFERS SwapBuffers(ghDC) 
#define BLACK_INDEX     0 
#define RED_INDEX       13 
#define GREEN_INDEX     14 
#define BLUE_INDEX      16 
#define WIDTH           640 
#define HEIGHT          480 
 
LONG WINAPI MainWndProc (HWND, UINT, WPARAM, LPARAM); 
BOOL bSetupPixelFormat(HDC); 
 
/* OpenGL globals, defines, and prototypes */ 
GLfloat latitude, longitude, latinc, longinc; 
GLdouble radius; 
 

#define GLOBE    1 
#define CYLINDER 2 
#define CONE     3 
#define QUAD     4
 
GLvoid resize(GLsizei, GLsizei); 
GLvoid initializeGL(GLsizei, GLsizei); 
GLvoid drawScene(GLvoid); 

Game gameContext;


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 
    MSG        msg; 
    WNDCLASS   wndclass; 
 
    /* Register the frame class */ 
    wndclass.style         = 0; 
    wndclass.lpfnWndProc   = (WNDPROC)MainWndProc; 
    wndclass.cbClsExtra    = 0; 
    wndclass.cbWndExtra    = 0; 
    wndclass.hInstance     = hInstance; 
    wndclass.hIcon         = LoadIcon (hInstance, szAppName); 
    wndclass.hCursor       = LoadCursor (NULL,IDC_ARROW); 
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wndclass.lpszMenuName  = szAppName; 
    wndclass.lpszClassName = szAppName; 
 
    if (!RegisterClass (&wndclass) ) 
        return FALSE; 
 
    /* Create the frame */ 
    ghWnd = CreateWindow (szAppName, 
             "Ping Pong", 
         WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
             CW_USEDEFAULT, 
             CW_USEDEFAULT, 
             WIDTH, 
             HEIGHT, 
             NULL, 
             NULL, 
             hInstance, 
             NULL); 
 
    /* make sure window was created */ 
    if (!ghWnd) 
        return FALSE; 
 
    /* show and update main window */ 
    ShowWindow (ghWnd, nCmdShow); 
 
    UpdateWindow (ghWnd); 
 
    /* animation loop */ 
    while (1) { 
        /* 
         *  Process all pending messages 
         */ 
 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } else { 
                return TRUE; 
            } 
        }

		if(!gameContext.isPause) {
			if(gameContext.isGameStarted) {
				drawScene(); 
			} else {
				 MessageBox(NULL, "You Lose, press SPACE to start new game", "Info", MB_OK); 
				 Game g;
				 gameContext = g;
				 gameContext.isPause = true;
			}
		}
    } 
} 
 
/* main window procedure */ 
LONG WINAPI MainWndProc ( 
    HWND    hWnd, 
    UINT    uMsg, 
    WPARAM  wParam, 
    LPARAM  lParam) 
{ 
    LONG    lRet = 1; 
    PAINTSTRUCT    ps; 
    RECT rect; 
 
    switch (uMsg) { 
 
    case WM_CREATE: 
        ghDC = GetDC(hWnd); 
        if (!bSetupPixelFormat(ghDC)) 
            PostQuitMessage (0); 
 
        ghRC = wglCreateContext(ghDC); 
        wglMakeCurrent(ghDC, ghRC); 
        GetClientRect(hWnd, &rect); 
        initializeGL(rect.right, rect.bottom); 
        break; 
 
    case WM_PAINT: 
        BeginPaint(hWnd, &ps); 
        EndPaint(hWnd, &ps); 
        break; 
 
    case WM_SIZE: 
        GetClientRect(hWnd, &rect); 
        resize(rect.right, rect.bottom); 
        break; 
 
    case WM_CLOSE: 
        if (ghRC) 
            wglDeleteContext(ghRC); 
        if (ghDC) 
            ReleaseDC(hWnd, ghDC); 
        ghRC = 0; 
        ghDC = 0; 
 
        DestroyWindow (hWnd); 
        break; 
 
    case WM_DESTROY: 
        if (ghRC) 
            wglDeleteContext(ghRC); 
        if (ghDC) 
            ReleaseDC(hWnd, ghDC); 
 
        PostQuitMessage (0); 
        break; 
     
    case WM_KEYDOWN: 
        switch (wParam) { 
        case VK_LEFT: 
			gameContext.racquet.keyDown(Racquet::LEFT);
            break; 
        case VK_RIGHT: 
			gameContext.racquet.keyDown(Racquet::RIGHT);
            break; 
        case VK_UP: 
			gameContext.racquet.keyDown(Racquet::UP);
            break; 
        case VK_DOWN: 
			gameContext.racquet.keyDown(Racquet::DOWN);
            break; 
		case VK_SPACE:
			gameContext.isPause = !gameContext.isPause;
			break;
        }
		break;
	case WM_KEYUP:
		switch (wParam) { 
        case VK_LEFT: 
			gameContext.racquet.keyUp(Racquet::LEFT);
            break; 
        case VK_RIGHT: 
			gameContext.racquet.keyUp(Racquet::RIGHT);
            break; 
        case VK_UP: 
			gameContext.racquet.keyUp(Racquet::UP);
            break; 
        case VK_DOWN: 
			gameContext.racquet.keyUp(Racquet::DOWN);
            break; 
        }
		break;
		
 
    default: 
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 
 
    return lRet; 
} 
 
BOOL bSetupPixelFormat(HDC hdc) 
{ 
    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 
    ppfd = &pfd; 
 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  
                        PFD_DOUBLEBUFFER; 
    ppfd->dwLayerMask = PFD_MAIN_PLANE; 
    ppfd->iPixelType = PFD_TYPE_COLORINDEX; 
    ppfd->cColorBits = 8; 
    ppfd->cDepthBits = 16; 
    ppfd->cAccumBits = 0; 
    ppfd->cStencilBits = 0; 
 
    pixelformat = ChoosePixelFormat(hdc, ppfd); 
 
    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    }
 
    return TRUE; 
} 
 
/* OpenGL code */ 
 
GLvoid resize( GLsizei width, GLsizei height ) 
{ 
    GLfloat aspect; 
    glViewport( 0, 0, width, height ); 
    aspect = (GLfloat) width / height; 
    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity(); 
    gluPerspective( 45.0, aspect, 3.0, 17.0 ); 
    glMatrixMode( GL_MODELVIEW ); 
}     
 
GLvoid createObjects() 
{ 
    GLUquadricObj *quadObj; 

    glNewList(GLOBE, GL_COMPILE); 
        quadObj = gluNewQuadric (); 
        gluQuadricDrawStyle (quadObj, GLU_LINE); 
		gluSphere (quadObj, gameContext.ball.radius, 16, 16); 
    glEndList(); 
 	
    glNewList(CYLINDER, GL_COMPILE); 
        glPushMatrix (); 
        glRotatef ((GLfloat)90.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0); 
        glTranslatef ((GLfloat)0.0, (GLfloat)0.0, (GLfloat)-1.0); 
        quadObj = gluNewQuadric (); 
		gluQuadricDrawStyle (quadObj, GLU_FILL); 
        gluQuadricNormals (quadObj, GLU_SMOOTH); 
		gluDisk(quadObj, 0, gameContext.racquet.radius, 32,16);
		glPopMatrix (); 
    glEndList(); 


} 
 
GLvoid initializeGL(GLsizei width, GLsizei height) 
{ 
    GLfloat     maxObjectSize, aspect; 
    GLdouble    near_plane, far_plane; 
 
    glClearIndex( (GLfloat)BLACK_INDEX); 
    glClearDepth( 1.0 ); 
 
    glEnable(GL_DEPTH_TEST); 
 
    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity();
	
	aspect = (GLfloat) width / height; 
    gluPerspective( 45.0, aspect, 6.0, 14.0 ); 
    glMatrixMode( GL_MODELVIEW ); 
	


    near_plane = 3.0; 
    far_plane = 7.0; 
    maxObjectSize = 3.0F; 
    radius = near_plane + maxObjectSize/2.0; 
 
    latitude = 0.0F; 
    longitude = 0.0F; 
    latinc = 0.06F; 
    longinc = 0.025F; 
 
    createObjects(); 
} 

GLvoid drawPlayField() {
	glBegin(GL_LINES);
	
	float w = gameContext.playField.width;
	float h = gameContext.playField.height;
	float d = gameContext.playField.depth;
	float stepX = gameContext.playField.meshXStep;
	float stepY = gameContext.playField.meshYStep;
	float stepZ = gameContext.playField.meshZStep;

	for(float x = -w; x<w+stepX; x+=stepX) {
			glVertex3f(x,h,0);
			glVertex3f(x,h,d);

			glVertex3f(x,-h,0);
			glVertex3f(x,-h,d);

			glVertex3f(x,h,d);
			glVertex3f(x,-h,d);

			glVertex3f(x,h,0);
			glVertex3f(x,h+2,0);

			glVertex3f(x,-h,0);
			glVertex3f(x,-h-2,0);
		}

		for(float y = -h; y<=h; y+=stepY) {
			glVertex3f(w,y,0);
			glVertex3f(w,y,d);

			glVertex3f(-w,y,0);
			glVertex3f(-w,y,d);

			glVertex3f(w, y,d);
			glVertex3f(-w, y,d);

			glVertex3f(w,y,0);
			glVertex3f(w+2,y,0); 

			glVertex3f(-w,y,0);
			glVertex3f(-w-2,y,0); 
		}
		
		for(float z = d; z<-stepZ; z+=stepZ) {
			glVertex3f(w, h, z);
			glVertex3f(-w, h, z);

			glVertex3f(-w, h, z);
			glVertex3f(-w, -h, z);

			glVertex3f(w, h, z);
			glVertex3f(w, -h, z);

			glVertex3f(w, -h, z);
			glVertex3f(-w, -h, z);
		}

		float over= 2;
		for(float xx = 0; xx<=over; xx+=0.2) {
			glVertex3f(w+over, h+xx, 0);
			glVertex3f(-w-over, h+xx, 0);

			glVertex3f(w+over, -h-xx, 0);
			glVertex3f(-w-over, -h-xx, 0);
			
			glVertex3f(-w-xx, h+over, 0);
			glVertex3f(-w-xx, -h-over, 0);

			glVertex3f(w+xx, h+over, 0);
			glVertex3f(w+xx, -h-over, 0);
		}

		glEnd();
}

GLvoid drawScene(GLvoid) 
{ 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
 
    glPushMatrix(); 
	
	glPushMatrix();
		glTranslatef(0,0,-radius);
		glColor3f(0,1,0);
		drawPlayField();
			
	glPopMatrix();

	glPushMatrix(); 
		glTranslated(0.0, 0.0, -radius); 
		glTranslatef(gameContext.ball.x,gameContext.ball.y,gameContext.ball.z);
		gameContext.moveBall();

		glIndexi(BLUE_INDEX); 
		glCallList(GLOBE); 
	glPopMatrix();

	gameContext.racquet.move();
    glPushMatrix(); 
		glTranslated(0.0, 0.0, -radius); 
		glColor4f(0,1,0,0.8);
		glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable (GL_BLEND);

		glEnable(GL_BLEND);
		glTranslatef(gameContext.racquet.x, gameContext.racquet.y, -gameContext.racquet.radius); 
		glRotatef(90.0F, 1.0F, 0.0F, 0.0F); 
		glCallList(CYLINDER); 
    glPopMatrix(); 
 
    glPopMatrix(); 
 
    SWAPBUFFERS; 
}
