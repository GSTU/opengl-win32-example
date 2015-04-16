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
        drawScene(); 
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
			gameContext.racquet.move(Racquet::LEFT);
            break; 
        case VK_RIGHT: 
			gameContext.racquet.move(Racquet::RIGHT);
            break; 
        case VK_UP: 
			gameContext.racquet.move(Racquet::UP);
            break; 
        case VK_DOWN: 
			gameContext.racquet.move(Racquet::DOWN);
            break; 
        } 
 
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
        gluSphere (quadObj, 0.2, 16, 16); 
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
 
  

 

float sx = 0.01;
float sy = 0.01;
float sz = -0.01;

float posx = 0;
float posy = 0;
float posz = 0;

float widthBorder = 1.5f;

float isGameStarted = false; 

GLvoid drawScene(GLvoid) 
{ 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
 
    glPushMatrix(); 
	
	glPushMatrix();
		glTranslatef(0,0,-3);
		glColor3f(0,1,0);
		glBegin(GL_LINES);
			
		for(float x = -GAME_WIDTH; x<GAME_WIDTH+MESH_X_STEP; x+=MESH_X_STEP) {
			glVertex3f(x,GAME_HEIGHT,0);
			glVertex3f(x,GAME_HEIGHT,GAME_DEPTH);

			glVertex3f(x,-GAME_HEIGHT,0);
			glVertex3f(x,-GAME_HEIGHT,GAME_DEPTH);

			glVertex3f(x,GAME_HEIGHT,GAME_DEPTH);
			glVertex3f(x,-GAME_HEIGHT,GAME_DEPTH);

			glVertex3f(x,GAME_HEIGHT,0);
			glVertex3f(x,GAME_HEIGHT+2,0);

			glVertex3f(x,-GAME_HEIGHT,0);
			glVertex3f(x,-GAME_HEIGHT-2,0);
		}

		for(float y = -GAME_HEIGHT; y<=GAME_HEIGHT; y+=MESH_Y_STEP) {
			glVertex3f(GAME_WIDTH,y,0);
			glVertex3f(GAME_WIDTH,y,GAME_DEPTH);

			glVertex3f(-GAME_WIDTH,y,0);
			glVertex3f(-GAME_WIDTH,y,GAME_DEPTH);

			glVertex3f(GAME_WIDTH, y,GAME_DEPTH);
			glVertex3f(-GAME_WIDTH, y,GAME_DEPTH);

			glVertex3f(GAME_WIDTH,y,0);
			glVertex3f(GAME_WIDTH+2,y,0); 

			glVertex3f(-GAME_WIDTH,y,0);
			glVertex3f(-GAME_WIDTH-2,y,0); 
		}
		
		for(float z = GAME_DEPTH; z<=MESH_Z_STEP; z+=MESH_Z_STEP) {
			glVertex3f(GAME_WIDTH, GAME_HEIGHT, z);
			glVertex3f(-GAME_WIDTH, GAME_HEIGHT, z);

			glVertex3f(-GAME_WIDTH, GAME_HEIGHT, z);
			glVertex3f(-GAME_WIDTH, -GAME_HEIGHT, z);

			glVertex3f(GAME_WIDTH, GAME_HEIGHT, z);
			glVertex3f(GAME_WIDTH, -GAME_HEIGHT, z);

			glVertex3f(GAME_WIDTH, -GAME_HEIGHT, z);
			glVertex3f(-GAME_WIDTH, -GAME_HEIGHT, z);
		}

		glEnd();
			
		glPopMatrix();

	glPushMatrix(); 
	glTranslatef(posx,posy,posz);

	posx+=sx;
	posy+=sy;
	posz+=sz;
	if(posx<-GAME_WIDTH||posx>GAME_WIDTH) {
		sx=-sx;
	}
	if(posy<-GAME_HEIGHT||posy>GAME_HEIGHT) {
		sy=-sy;
	}
	if(posz<GAME_DEPTH||posz>0) {
		sz=-sz;
	}


	glTranslated(0.0, 0.0, -radius); 
	
	glIndexi(BLUE_INDEX); 
    glCallList(GLOBE); 

	glPopMatrix();

	glTranslated(0.0, 0.0, -radius); 
 
	glColor4f(0,1,0,0.8);
    glPushMatrix(); 

	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_BLEND);

	glEnable(GL_BLEND);
	glTranslatef(gameContext.racquet.x, gameContext.racquet.y, 0.0F); 
        glRotatef(90.0F, 1.0F, 0.0F, 0.0F); 
        glCallList(CYLINDER); 
    glPopMatrix(); 
 
    glPopMatrix(); 
 
    SWAPBUFFERS; 
}
