// DCSquares-MFCView.cpp : implementation of the CDCSquaresMFCView class
//

#include "stdafx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "wglext.h"
#include "glext.h"

#include "squares.h"
#include "texture.h"
#include "text.h"
#include "theme.h"
#include "hud.h"
#include "game.h"
#include "score.h"
#include "level.h"
#include "DCSquares-MFC.h"
#include "hyperlink.h"
#include "DCSquares-MFCDoc.h"
#include "DCSquares-MFCView.h"
#include "ScoresSOAPService.h"
#include ".\dcsquares-mfcview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDCSquaresMFCView

IMPLEMENT_DYNCREATE(CDCSquaresMFCView, CView)

BEGIN_MESSAGE_MAP(CDCSquaresMFCView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
//	ON_COMMAND(ID_HELP_HOWTOPLAY, OnHelpHowtoplay)
	ON_COMMAND(ID_GAME_FREEPLAY, OnGameFreeplay)
	ON_COMMAND(ID_GAME_CHALLENGEMODE, OnGameChallengemode)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CDCSquaresMFCView construction/destruction
extern int combo;
extern int maxcombo;
extern int score;
extern int highscore;
extern int squares;
extern int scoreval;
extern float tickval;
extern float power;
extern char highcode[20];
extern float speedval;
extern int powerup_mode;
extern int effect_type;
float square_alpha=1.0;
extern int bg_tex;
extern int title_tex;
extern int menu_tex;
float gt = 0;
float fade = 0;
float eoe=0;

CDCSquaresMFCView::CDCSquaresMFCView()
: player(NULL)
, state(0)
, mousex(320)
, mousey(240)
{
	// TODO: add construction code here

}

CDCSquaresMFCView::~CDCSquaresMFCView()
{
}

BOOL CDCSquaresMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	/*cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    return CView::PreCreateWindow(cs);*/

cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC, 
  ::LoadCursor(NULL, IDC_ARROW), NULL, NULL); 
cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 
return CView::PreCreateWindow(cs); 
}

// CDCSquaresMFCView drawing

void CDCSquaresMFCView::OnDraw(CDC* /*pDC*/)
{
	//return;
	CDCSquaresMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
    CPaintDC dc(this); // Needed 

	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hglrc);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	if(state==0) render_bg(title_tex,1);
	else if(state==1) render_bg(bg_tex,1);
	else render_bg(menu_tex,1);
#ifdef DREAMCAST
	glKosFinishList();
#else
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
#endif
	render_squares((gt<1?(gt*square_alpha):fade<=0?square_alpha:fade*square_alpha));
	if(state==0) render_title(gt);
	if(state==1) render_score(gt);
	if(state==2) render_win(gt,eoe);
		if(gt<0.5) {
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glColor4f(0.8,0.8,0.8,1-(gt/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
		}
		if(fade>0) {
#ifndef DREAMCAST
			glEnable(GL_BLEND);
			glDisable(GL_DEPTH_TEST);
#endif
			glDisable(GL_TEXTURE_2D);
			glColor4f(0.8,0.8,0.8,1-(fade/0.5));
			glBegin(GL_QUADS);
			glVertex3f(0,0,0.9);
			glVertex3f(640,0,0.9);
			glVertex3f(640,480,0.9);
			glVertex3f(0,480,0.9);
			glEnd();
			
			if(state==1) {
				player->size+=2;
			}
		}
		SwapBuffers(m_hgldc);
}


// CDCSquaresMFCView diagnostics

#ifdef _DEBUG
void CDCSquaresMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CDCSquaresMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDCSquaresMFCDoc* CDCSquaresMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDCSquaresMFCDoc)));
	return (CDCSquaresMFCDoc*)m_pDocument;
}
#endif //_DEBUG

// Declarations We'll Use
#define WGL_SAMPLE_BUFFERS_ARB	0x2041
#define WGL_SAMPLES_ARB		0x2042

// CDCSquaresMFCView message handlers

int CDCSquaresMFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style &= ~WS_VISIBLE;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
//return 0;
	// TODO:  Add your specialized creation code here
	m_hgldc = ::GetDC(m_hWnd);

    if(!SetPixelformat(m_hgldc))
    {
	::MessageBox(::GetFocus(),"SetPixelformat Failed!","Error",MB_OK);
	return -1;
    }

    m_hglrc = wglCreateContext(m_hgldc);
    int i= wglMakeCurrent(m_hgldc,m_hglrc);

	InitGL();
	return 0;
}

void CDCSquaresMFCView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hglrc != NULL)
	{
		wglDeleteContext(m_hglrc);
		m_hglrc = NULL;
	}
	KillTimer(1000);
}

BOOL CDCSquaresMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;//CView::OnEraseBkgnd(pDC);
}

void CDCSquaresMFCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	int width;
	int height;

	width = cx;
	height = cy;

	ReSizeGLScene((int) width, (int) height);
}

BOOL CDCSquaresMFCView::SetPixelformat(HDC hdc)
{

    PIXELFORMATDESCRIPTOR *ppfd; 
    int pixelformat; 
 
    PIXELFORMATDESCRIPTOR pfd = { 
    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
    1,                     // version number 
    PFD_DRAW_TO_WINDOW |   // support window 
    PFD_SUPPORT_OPENGL |   // support OpenGL 
    PFD_GENERIC_FORMAT |
    PFD_DOUBLEBUFFER,      // double buffered 
    PFD_TYPE_RGBA,         // RGBA type 
    32,                    // 24-bit color depth 
    0, 0, 0, 0, 0, 0,      // color bits ignored 
    8,                     // no alpha buffer 
    0,                     // shift bit ignored 
    8,                     // no accumulation buffer 
    0, 0, 0, 0,            // accum bits ignored 
    64,                    // 32-bit z-buffer	 
    8,                     // no stencil buffer 
    8,                     // no auxiliary buffer 
    PFD_MAIN_PLANE,        // main layer 
    0,                     // reserved 
    0, 0, 0                // layer masks ignored 
    }; 
    
    ppfd = &pfd;

	if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 ) 
	{ 
		::MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
		return FALSE; 
	} 

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE) 
    { 
        ::MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 

}

void CDCSquaresMFCView::ReSizeGLScene(int width, int height)	// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(60.0f,(float)width/(float)height,1.0f,5000.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,640,480,0,-1,1);
  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}


int CDCSquaresMFCView::InitGL(void)										// All Setup For OpenGL Goes Here
{
 	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, /*GL_NICEST*/GL_FASTEST);	// Really Nice Perspective Calculations
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	// Initialization Went OK
	return TRUE;
}


void CDCSquaresMFCView::OnTimer(UINT nIDEvent)
{
	squarelist *c;
	char tmp[100];
	char tmp2[100];
	int x,seed;
	static float oe = 0;
	ScoresService::CScoresService Scores;
	float e = ((float)GetTickCount() / 1000.0f);
	if(state!=2) gt += (e - oe);
	eoe = (e-oe);

	if(state==0) square_alpha=0.4f;
	if(state==1) square_alpha=1.0f;

	if(fade>0) {
		fade -= (e - oe);
		if(fade<=0) {
			if(state==0 || state==2) {
				if(state==0) {
					if(theApp.GetMainWnd()->GetMenu()->GetMenuState(ID_GAME_CHALLENGEMODE,MF_BYCOMMAND) == MF_CHECKED) {
						current_level=level_list_head;
					}
					if(theApp.GetMainWnd()->GetMenu()->GetMenuState(ID_GAME_FREEPLAY,MF_BYCOMMAND) == MF_CHECKED) {
						current_level=free_play;
					}
					state=1;
					while(ShowCursor(FALSE)>=0);
				} else {
					if(check_win(gt) == 1) {
						state=1;
						if(current_level->next!=NULL) {
							current_level=current_level->next;
						} else {
							state=0;
						}
					} else {
						state=0;
					}
					if(state==0) {
						while(ShowCursor(TRUE)<0);
						//[NSCursor unhide];
					}
				}
				if(state!=0) {
					destroy_list();
					player=create_square(mousex,mousey,6,PLAYER1);
					square_alpha=1.0;
				}
			} else if(state==1) {
				if(combo > maxcombo) maxcombo = combo;
				if(check_win(gt) == 1) {
					state=2;
				} else {
					state=2;
					if(theApp.autoSubmit && (theApp.username.GetLength()>0 && theApp.password.GetLength()>0)) {
						theApp.statusDlg.statusTxt.SetWindowText("Submitting score");
						theApp.statusDlg.ShowWindow(SW_SHOW);
						theApp.statusDlg.UpdateWindow();
						Scores.submitScore(theApp.username.AllocSysString(),theApp.password.AllocSysString(),score,maxcombo,gt,L"PC",&x);
						theApp.statusDlg.ShowWindow(SW_HIDE);
					}
				}
				destroy_list();
				player=NULL;
				square_alpha=0.4;
			}
			if(state != 2) {
				tickval=current_level->tickval; //1.0
				speedval=current_level->speedval; //1.2
				scoreval=current_level->scoreval; //100
				score=0;
				combo=0;
				squares=0;
				maxcombo=0;
				gt=0;
				theApp.mp3player.hrStop();
			}
			power=0;
			powerup_mode=-1;
			effect_type=-1;
		}
	}

	if(theApp.bgm && !theApp.mp3player.boIsPlaying()) {
		if(state==0) theApp.mp3player.hrLoad(theme_dir("bgdim_loop.ogg"));
		if(state==1) theApp.mp3player.hrLoad(theme_dir("bg_loop.ogg"));
		theApp.mp3player.hrPlay();
		GetFocus();
	}
	if(theApp.mp3player.boCompleted()) {
		theApp.mp3player.hrRewind();
		//mp3player.hrPlay();
	}
	if(gt>1) {
		add_squares(e-oe);
	}
	if(state==1 && fade<=0) {
		c=check_collide(player);
		if(c!=NULL && ((c->type==ENEMY && powerup_mode!=INVINC) || (c->type==SCORE && powerup_mode==EVIL))) {
			fade=0.5;
			while(ShowCursor(TRUE)<1);
			/*if(score>highscore) {
				x=0;
				highscore=score;
				encode(score,tmp);
				do {
					seed=rand()%26;
					encrypt(seed,(unsigned char *)tmp, (unsigned char *)tmp2);
					x++;
					if(x>20) {
					strcpy(tmp2,"_INVALID_CODE");
					}
				} while(invalid_code(tmp2));
				sprintf(highcode,"%c%s",(unsigned char)seed+'A',tmp2);
				if(theApp.autoSubmit && (theApp.username.GetLength()>0 && theApp.password.GetLength()>0))
					submit_code(highcode);
			}*/
		}
		switch(check_win(gt)) {
			case 1: //win
				fade=1.0;
				break;
			case -1: //lose
				fade=1.0;
				break;
		}
	}
	update_squares(e-oe);
	oe = e;
	RedrawWindow();
	CView::OnTimer(nIDEvent);
}

void CDCSquaresMFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousex=point.x;
	mousey=point.y;
	if(state==1) {
		player->x=point.x;
		player->y=point.y;
	}
	CView::OnMouseMove(nFlags, point);
}

void CDCSquaresMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(state==0 || state==2) {
		fade=0.5;
	}
	CView::OnLButtonDown(nFlags, point);
}

void CDCSquaresMFCView::OnGameFreeplay()
{
	theApp.GetMainWnd()->GetMenu()->CheckMenuItem(ID_GAME_FREEPLAY,MF_CHECKED);
	theApp.GetMainWnd()->GetMenu()->CheckMenuItem(ID_GAME_CHALLENGEMODE,MF_UNCHECKED);
}

void CDCSquaresMFCView::OnGameChallengemode()
{
	theApp.GetMainWnd()->GetMenu()->CheckMenuItem(ID_GAME_FREEPLAY,MF_UNCHECKED);
	theApp.GetMainWnd()->GetMenu()->CheckMenuItem(ID_GAME_CHALLENGEMODE,MF_CHECKED);
}

void CDCSquaresMFCView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	SetTimer(1000,12,NULL);
}
