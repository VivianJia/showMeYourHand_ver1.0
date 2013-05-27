#include <windows.h>												// Header File For Windows
#include <stdio.h>													// Header File For Standard Input/Output
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library

#include <leap.h>													// Header File For The Leap Functions
#include "glListener.h"
#include "MilkshapeModel.h"											// Header File For Milkshape File
#include "cvSlidebar.h"

#pragma comment( lib, "opengl32.lib" )								// Search For OpenGL32.lib While Linking ( NEW )
#pragma comment( lib, "glu32.lib" )									// Search For GLu32.lib While Linking    ( NEW )
#pragma comment( lib, "glaux.lib" )									// Search For GLaux.lib While Linking    ( NEW )
#pragma comment( lib, "leap.lib")
#pragma comment( lib, "leapd.lib")
using namespace std;
using namespace Leap;

HDC			hDC=NULL;												// Private GDI Device Context
HGLRC		hRC=NULL;												// Permanent Rendering Context
HWND		hWnd=NULL;												// Holds Our Window Handle
HINSTANCE	hInstance;												// Holds The Instance Of The Application

Model *pModel = NULL;												// Holds The Model Data

bool	keys[256];													// Array Used For The Keyboard Routine
bool	active=TRUE;												// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;											// Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat	xrot = 0.0f,yrot = 0.0f,zrot = 0.0f;													// Y Rotation
GLfloat	xpos = 0.0f, ypos = 0.0f,zpos = 0.0f;

struct colorArray{
	float r;
	float g;
	float b;
}colorArray[20];

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				// Declaration For WndProc

AUX_RGBImageRec *LoadBMP(const char *Filename)						// Loads A Bitmap Image
{
	FILE *File=NULL;												// File Handle

	if (!Filename)													// Make Sure A Filename Was Given
	{
		return NULL;												// If Not Return NULL
	}

	File=fopen(Filename,"r");										// Check To See If The File Exists

	if (File)														// Does The File Exist?
	{
		fclose(File);												// Close The Handle
		return auxDIBImageLoad(Filename);							// Load The Bitmap And Return A Pointer
	}

	return NULL;													// If Load Failed Return NULL
}

GLuint LoadGLTexture( const char *filename )						// Load Bitmaps And Convert To Textures
{
	AUX_RGBImageRec *pImage;										// Create Storage Space For The Texture
	GLuint texture = 0;												// Texture ID

	pImage = LoadBMP( filename );									// Loads The Bitmap Specified By filename

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if ( pImage != NULL && pImage->data != NULL )					// If Texture Image Exists
	{
		glGenTextures(1, &texture);									// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		free(pImage->data);											// Free The Texture Image Memory
		free(pImage);												// Free The Image Structure
	}

	return texture;													// Return The Status
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)					// Resize And Initialize The GL Window
{
	if (height==0)													// Prevent A Divide By Zero By
	{
		height=1;													// Making Height Equal One
	}

	glViewport(0,0,width,height);									// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);									// Select The Projection Matrix
	glLoadIdentity();												// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,1.0f,1000.0f);	// View Depth of 1000

	glMatrixMode(GL_MODELVIEW);										// Select The Modelview Matrix
	glLoadIdentity();												// Reset The Modelview Matrix
}

int InitGL(GLvoid)															// All Setup For OpenGL Goes Here
{
	pModel->reloadTextures();										// Loads Model Textures

	glEnable(GL_TEXTURE_2D);										// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);									// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth(1.0f);													// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);										// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);										// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Really Nice Perspective Calculations

	//init the color array
	float red = 0, green = 0, blue = 0; 
	for (int i = 0; i < 20; i++) {
		colorArray[i].r = red;
		colorArray[i].g = green;
		colorArray[i].b = blue;
		red += 2;
		green += 2;
		blue += 2;
	} 
	return TRUE;																// Initialization Went OK
}

int DrawGLScene(GLvoid)												// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear The Screen And The Depth Buffer
	glLoadIdentity();												// Reset The Modelview Matrix
	//gluLookAt( 75, 75, 10, 0, 0, 0, 0, 1, 0 );					// (3) Eye Postion (3) Center Point (3) Y-Axis Up Vector
	glTranslatef(0.0f,10.0f,-70.0f);

	//模型移动
	glTranslatef(xpos,0.0f,0.0f);
	glTranslatef(0.0f,ypos,0.0f);
	glTranslatef(0.0f,0.0f,zpos);
	//模型旋转
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);									 
	glRotatef(zrot,0.0f,0.0f,1.0f);									 
													
	/*if(pModel->flag == 1)
	pModel->reDraw();*/
	pModel->draw();	
	return TRUE;													// Keep Going
}

GLvoid KillGLWindow(GLvoid)											// Properly Kill The Window
{
	if (fullscreen)													// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);								// If So Switch Back To The Desktop
		ShowCursor(TRUE);											// Show Mouse Pointer
	}

	if (hRC)														// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))								// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))									// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;													// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))								// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;													// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))								// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;													// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))						// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;												// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;										// Holds The Results After Searching For A Match
	WNDCLASS	wc;													// Windows Class Structure
	DWORD		dwExStyle;											// Window Extended Style
	DWORD		dwStyle;											// Window Style
	RECT		WindowRect;											// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;										// Set Left Value To 0
	WindowRect.right=(long)width;									// Set Right Value To Requested Width
	WindowRect.top=(long)0;											// Set Top Value To 0
	WindowRect.bottom=(long)height;									// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;										// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);					// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;						// WndProc Handles Messages
	wc.cbClsExtra		= 0;										// No Extra Window Data
	wc.cbWndExtra		= 0;										// No Extra Window Data
	wc.hInstance		= hInstance;								// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground	= NULL;										// No Background Required For GL
	wc.lpszMenuName		= NULL;										// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";									// Set The Class Name

	if (!RegisterClass(&wc))										// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}
	
	if (fullscreen)													// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;									// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;					// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;					// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;						// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;									// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;										// Return FALSE
			}
		}
	}

	if (fullscreen)													// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;									// Window Extended Style
		dwStyle=WS_POPUP;											// Windows Style
		ShowCursor(FALSE);											// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;				// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;								// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=								// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),								// Size Of This Pixel Format Descriptor
		1,															// Version Number
		PFD_DRAW_TO_WINDOW |										// Format Must Support Window
		PFD_SUPPORT_OPENGL |										// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,											// Must Support Double Buffering
		PFD_TYPE_RGBA,												// Request An RGBA Format
		bits,														// Select Our Color Depth
		0, 0, 0, 0, 0, 0,											// Color Bits Ignored
		0,															// No Alpha Buffer
		0,															// Shift Bit Ignored
		0,															// No Accumulation Buffer
		0, 0, 0, 0,													// Accumulation Bits Ignored
		16,															// 16Bit Z-Buffer (Depth Buffer)  
		0,															// No Stencil Buffer
		0,															// No Auxiliary Buffer
		PFD_MAIN_PLANE,												// Main Drawing Layer
		0,															// Reserved
		0, 0, 0														// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))											// Did We Get A Device Context?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))					// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))						// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))								// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))									// Try To Activate The Rendering Context
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);										// Show The Window
	SetForegroundWindow(hWnd);										// Slightly Higher Priority
	SetFocus(hWnd);													// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);									// Set Up Our Perspective GL Screen

	if (!InitGL())													// Initialize Our Newly Created GL Window
	{
		KillGLWindow();												// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;												// Return FALSE
	}

	return TRUE;													// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,							// Handle For This Window
							UINT	uMsg,							// Message For This Window
							WPARAM	wParam,							// Additional Message Information
							LPARAM	lParam)							// Additional Message Information
{
	switch (uMsg)													// Check For Windows Messages
	{
		case WM_ACTIVATE:											// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))									// Check Minimization State
			{
				active=TRUE;										// Program Is Active
			}
			else
			{
				active=FALSE;										// Program Is No Longer Active
			}

			return 0;												// Return To The Message Loop
		}

		case WM_SYSCOMMAND:											// Intercept System Commands
		{
			switch (wParam)											// Check System Calls
			{
				case SC_SCREENSAVE:									// Screensaver Trying To Start?
				case SC_MONITORPOWER:								// Monitor Trying To Enter Powersave?
				return 0;											// Prevent From Happening
			}
			break;													// Exit
		}

		case WM_CLOSE:												// Did We Receive A Close Message?
		{
			PostQuitMessage(0);										// Send A Quit Message
			return 0;												// Jump Back
		}

		case WM_KEYDOWN:											// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;									// If So, Mark It As TRUE
			return 0;												// Jump Back
		}

		case WM_KEYUP:												// Has A Key Been Released?
		{
			keys[wParam] = FALSE;									// If So, Mark It As FALSE
			return 0;												// Jump Back
		}

		case WM_SIZE:												// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));			// LoWord=Width, HiWord=Height
			return 0;												// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
//callback function
void onSlided(int, void*){
	int pitch[15],roll[15],yaw[15];
	//获取每个trackbar的位置
	for(int i = 1; i <= 15; i++) {
		/*
			将 int型: i 转换为 char型:ch 
		*/
		//先定义一个字符串流对象
		stringstream ss;
		//输入流对象
		ss << i;
		//转换为string
		string str = ss.str();
		str = "joint" + str;
		//转换为const char
		const char *ch = str.c_str();

		//获取trackbar的位置
		pitch[i - 1] = cvGetTrackbarPos(ch, "PitchAngle");
		roll[i - 1] = cvGetTrackbarPos(ch, "RollAngle");
		yaw[i - 1] = cvGetTrackbarPos(ch, "YawAngle");
	}
	pModel->animation(pitch,roll,yaw);
}

//callback function
void glListener::onFrame(const Controller& controller){
	float pitches[15],rolls[15],yaws[15];
	//memset(pitches, 0,sizeof(pitches));
	//memset(rolls, 0,sizeof(rolls));
	//memset(yaws, 0,sizeof(yaws));
	//Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	if(!frame.hands().empty()) {
		//get the first hand
		const Hand hand = frame.hands()[0];

		//check if the hand has any fingers
		const FingerList fingers = hand.fingers();
		if (!fingers.empty()) {
			//Thumb
			yaws[2] = fingers[3].direction().pitch();
			//Index
			yaws[5] = fingers[1].direction().pitch();
			//Middle
			yaws[8]= fingers[0].direction().pitch();
			//Ring
			yaws[11]= fingers[2].direction().pitch();
			//Little
			yaws[14]= fingers[4].direction().pitch();
		}
	}
	pModel->animation(pitches,rolls,yaws);
}

int WINAPI WinMain(	HINSTANCE	hInstance,							// Instance
					HINSTANCE	hPrevInstance,						// Previous Instance
					LPSTR		lpCmdLine,							// Command Line Parameters
					int			nCmdShow)							// Window Show State
{
	MSG		msg;													// Windows Message Structure
	BOOL	done=FALSE;												// Bool Variable To Exit Loop

	pModel = new MilkshapeModel();									// Memory To Hold The Model
	if ( pModel->loadModelData( "data/Hand_1.0.ms3d" ) == false )		// Loads The Model And Checks For Errors
	{
		MessageBox( NULL, "Couldn't load the model data\\Hand_bones_2.0.ms3d", "Error", MB_OK | MB_ICONERROR );
		return 0;													// If Model Didn't Load Quit
	}

	/*在这里调用setupJoint，初始化骨骼和顶点位置*/
	pModel->SetupJointMatrices();

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;											// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("ShowMeYourHand",640,480,16,fullscreen))
	{
		return 0;													// Quit If Window Was Not Created
	}
#pragma region opencv
	int angleSlider = 0; 
	int sliderMax = 2 * PI * 100;
	//在这里创建一个调节pitch角的opencv窗口
	cvNamedWindow("PitchAngle", CV_WINDOW_AUTOSIZE);
	cvResizeWindow("PitchAngle", 300,850);
	cvMoveWindow("PitchAngle", 650,0);
	
	/*
		创建20个Joint的局部pitch角调节,回调函数相同
	*/
	createTrackbar("joint1", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint2", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint3", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint4", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint5", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint6", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint7", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint8", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint9", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint10", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint11", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint12", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint13", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint14", "PitchAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint15", "PitchAngle",&angleSlider,sliderMax, onSlided);

	cvNamedWindow("RollAngle", CV_WINDOW_AUTOSIZE);
	cvResizeWindow("RollAngle", 300,850);
	cvMoveWindow("RollAngle", 850,0);
	
	/*
		创建20个Joint的局部Row角调节,回调函数相同
	*/
	createTrackbar("joint1", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint2", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint3", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint4", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint5", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint6", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint7", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint8", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint9", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint10", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint11", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint12", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint13", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint14", "RollAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint15", "RollAngle",&angleSlider,sliderMax, onSlided);

	cvNamedWindow("YawAngle", CV_WINDOW_AUTOSIZE);
	cvResizeWindow("YawAngle", 300,850);
	cvMoveWindow("YawAngle", 1050,0);
	
	/*
		创建20个Joint的局部yaw角调节,回调函数相同
	*/
	createTrackbar("joint1", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint2", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint3", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint4", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint5", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint6", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint7", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint8", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint9", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint10", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint11", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint12", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint13", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint14", "YawAngle",&angleSlider,sliderMax, onSlided);
	createTrackbar("joint15", "YawAngle",&angleSlider,sliderMax, onSlided);
#pragma endregion opencv 

	
	while(!done)													// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))					// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)								// Have We Received A Quit Message?
			{
				done=TRUE;											// If So done=TRUE
			}
			else													// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);								// Translate The Message
				DispatchMessage(&msg);								// Dispatch The Message
			}
		}
		else														// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])		// Active?  Was There A Quit Received?
			{
				done=TRUE;											// ESC or DrawGLScene Signalled A Quit
			}
			else													// Not Time To Quit, Update Screen
			{
				SwapBuffers(hDC);									// Swap Buffers (Double Buffering)
			}

			if (keys[VK_F1])										// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;									// If So Make Key FALSE
				KillGLWindow();										// Kill Our Current Window
				fullscreen=!fullscreen;								// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("VivianJia's OpenGL Studio",640,480,16,fullscreen))
				{
					return 0;										// Quit If Window Was Not Created
				}
			}
		}

		//控制模型移动
		if(keys[VK_UP]){
			ypos +=  0.2f;
		}

		if(keys[VK_DOWN]){
			ypos -=  0.2f;
		}

		if(keys[VK_LEFT]){
			xpos -=  0.2f;
		}

		if(keys[VK_RIGHT]){
			xpos +=  0.2f;
		}

		if(keys['W']) {
			zpos += 0.2f;
		}
		if(keys['S']) {
			zpos -= 0.2f;
		}

		//控制模型旋转
		if(keys[VK_PRIOR]) {
			yrot += 0.2f;
		}
		if(keys[VK_NEXT]){
			yrot -= 0.2f;
		}
		if(keys[VK_HOME]){
			xrot += 0.2f;
		}
		if(keys[VK_END]){
			xrot -= 0.2f;
		}

		if(keys[VK_INSERT]){
			zrot += 0.2f;
		}
		if(keys[VK_DELETE]){
			zrot -= 0.2f;
		}

		if(keys['p']){
			
		}

//#pragma region LeapMotion
//		glListener listener;
//		Controller controller;
//		controller.addListener(listener);
//		listener.onFrame(controller);
//#pragma endregion LeapMotion
	}

	// Shutdown
	KillGLWindow();													// Kill The Window
	return (msg.wParam);											// Exit The Program
}
