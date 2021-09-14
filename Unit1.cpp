//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

#include "Com_text.h"
#include <GL/MyWinGL.h>
#include <math.h>
#include <float.h>
#include "HeartMod.h"


#define NEAR_PLANE 1.0f

#define START_PLANE 10.0f

#define FAR_PLANE 40.0f


//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

HGLRC hrc;
HDC hdc;

unsigned int _W= 1024,_H = 768;

// Heart Material
GLfloat mat_specular[] = {1,1,1,1};
GLfloat mat_amb_dif_red[] = {1,0,0,1};
GLfloat mat_amb_dif_white[] = {1,1,1,1};
GLfloat mat_shininess = 100;
// Light
GLfloat light_specular[] = {1,1,1,1};
GLfloat light_diffuse[] = {1,1,1,1};
GLfloat light_ambient[] = {0,0,0,1};
GLfloat glob_amb[] = {0.2,0.2,0.2,1};
//Light Position
GLfloat light_rad = 20;
GLfloat light_pos[] = {0,20,0,1};

PFNGLACTIVETEXTUREPROC   glActiveTexture = NULL;

static GLfloat theta[] = {0,0};
static GLfloat theta2 = 0.0f;
static GLfloat zRot = START_PLANE;

int x0,y0;

GLfloat   MaxZRot;
GLfloat   MinZRot;

bool Is_Red = false;
bool Is_Transparent = false;
bool Is_Background = false;
bool Is_Light = false;
bool Is_MipMap = false;
bool Is_Motion = false;

float MotionBias = 0.5f;
float MotionKoef = 0.0f;
float GammaCorrection = 1.0f;

float MotionShag = 1.0f;

bool fPass=false;


bool _pressed = false;

_Heart_Model _first;
// Textures
_Common_texture c_tex[2];

void Draw_Background();
void Get_Textures();
void ChangeSize(int w, int h);
void SetupRC();
void RenderScene();
//bool _Scale_Bits(BYTE *bits,BYTE *scale_bits,unsigned short Width,unsigned short Height);

void ChangeSize(int w, int h)
    {
    GLfloat fAspect;
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0f, fAspect, NEAR_PLANE, FAR_PLANE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }


void SetupRC()
    {
    _control87(MCW_EM, MCW_EM);

        //installing ActiveTexture pointer
        glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

        //starting lighting
                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

        //common lighting
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,glob_amb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
        glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
        //material
        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);

        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

        _first.MadeGLList(2,35,160,100,3,3,1.5);
        MaxZRot = _first.Get_Parameter(HEART_RADIUS) + NEAR_PLANE;
        MinZRot = FAR_PLANE - _first.Get_Parameter(HEART_RADIUS);

    }
// Called to draw scene
void RenderScene()
    {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(Is_Background)
        Draw_Background();

        glActiveTexture(GL_TEXTURE0);
        c_tex[0].BindIt();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Is_MipMap?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);

        glActiveTexture(GL_TEXTURE1);
        c_tex[1].BindIt();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Is_MipMap?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);

        if(!Is_Transparent)
                glDisable(GL_TEXTURE_2D);
        else
                glEnable(GL_TEXTURE_2D);
        if(Is_Red)
                glColor4fv(mat_amb_dif_red);
        else
                glColor4fv(mat_amb_dif_white);
        if(Is_Light)
                glEnable(GL_LIGHTING);
        else
                glDisable(GL_LIGHTING);


        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -zRot);
        glRotatef(theta[0],1,0,0);
        glRotatef(theta[1],0,1,0);
        _first.Draw_Heart();

    glPopMatrix();
}



void Draw_Background(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    
    glDisable(GL_LIGHTING);
    glActiveTexture(GL_TEXTURE0);
    c_tex[1].BindIt();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);

    // No depth buffer writes for background
    glDepthMask(GL_FALSE);
    glColor4fv(mat_amb_dif_white);
    // Background image
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();
    glDepthMask(GL_TRUE);

    // Back to 3D land
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


void __fastcall TForm1::FormCreate(TObject *Sender)
{
        HWND hWnd;
        hWnd = GetDesktopWindow();
        hdc = GetDC(hWnd);
        _W = GetDeviceCaps(hdc,HORZRES);
        _H = GetDeviceCaps(hdc,VERTRES);
        ReleaseDC(hWnd,hdc);

        hdc = GetDC(Handle);
        SetupPixelFormat();
        hrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc,hrc);
        SetupRC();
        Get_Textures();
        Application->OnIdle = &WhenIdle;
        ClientHeight=_H;
        ClientWidth=_W;
        Left=0;
        Top =0;
        ChangeSize(_W,_H);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::WhenIdle(TObject * Sender, bool & Done)
{
        Done = false;

        RenderScene();
        if(Is_Motion){
                if(!fPass){
                        glAccum(GL_LOAD, 1.0f);
                        fPass = true;
                }
                else{
                        glAccum(GL_MULT,MotionBias);
                        glAccum(GL_ACCUM, 1.0f - MotionBias);
                }
                glAccum(GL_RETURN, GammaCorrection);
        }
        glFlush();
        SwapBuffers(hdc);
}
void __fastcall TForm1::FormResize(TObject *Sender)
{
    /*    _H = ClientHeight;
        _W = ClientWidth;
        ChangeSize(_W,_H); */
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        wglMakeCurrent(hdc,NULL);
        wglDeleteContext(hrc);
        ReleaseDC(Handle,hdc);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetupPixelFormat()
{
        PIXELFORMATDESCRIPTOR pfd =
        {sizeof(PIXELFORMATDESCRIPTOR),1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,24,
        0,0,0,0,0,0,0,0,
		24,0,0,0,0,16,0,0,PFD_MAIN_PLANE,0,0,0};

        int PixelFormat = ChoosePixelFormat(hdc,&pfd);

        SetPixelFormat(hdc,PixelFormat,&pfd);
}
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        if(Button == mbLeft){
                x0 = X;
                y0=Y;
                _pressed = true;
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
        if(!_pressed)
                return;
         theta[1] += 360*(double)(X-x0)/(double)_W;
        theta[0] += 360*(double)(Y-y0)/(double)_H;

        theta[0] = (GLfloat)((const int)theta[0] % 360);
        theta[1] = (GLfloat)((const int)theta[1] % 360);

        x0 = X;
        y0 = Y;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        if(Button == mbLeft)
                _pressed = false;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	if(Key == VK_F1)
                N1Click(Sender);
	else if(Key == VK_F2)
		N2Click(Sender);
	else if(Key == VK_F3)
		N3Click(Sender);
	else if(Key == VK_F4)
		N4Click(Sender);
	else if(Key == VK_F5)
		F51Click(Sender);
	else if(Key == VK_F6)
		F61Click(Sender);
	else if(Key == VK_LEFT)
                Left1Click(Sender);
	else if(Key == VK_RIGHT)
                Right1Click(Sender);
	else if(Key == VK_UP)
                N9Click(Sender);
	else if(Key == VK_DOWN)
                SubAccum01051Click(Sender);
        else if(Key == VK_ADD)
		N6Click(Sender);
	else if(Key == VK_SUBTRACT)
		N7Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Exit1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
        Is_Red = !Is_Red;
        if(Is_Red)
                MainMenu1->Items->Items[0]->Items[0]->Caption = "Выключить красный цвет - F1";
        else
                MainMenu1->Items->Items[0]->Items[0]->Caption = "Включить красный цвет - F1";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N2Click(TObject *Sender)
{
        Is_Background = !Is_Background;
        if(Is_Background)
                MainMenu1->Items->Items[0]->Items[1]->Caption = "Выключить фоновый рисунок - F2";
        else
                MainMenu1->Items->Items[0]->Items[1]->Caption = "Включить фоновый рисунок - F2";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N3Click(TObject *Sender)
{
        Is_Light = !Is_Light;
        if(Is_Light){
                MainMenu1->Items->Items[0]->Items[2]->Caption = "Выключить источник света - F3";
                MainMenu1->Items->Items[1]->Items[3]->Enabled = true;
                MainMenu1->Items->Items[1]->Items[4]->Enabled = true;
        }
        else{
                MainMenu1->Items->Items[0]->Items[2]->Caption = "Включить источник света - F3";
                MainMenu1->Items->Items[1]->Items[3]->Enabled = false;
                MainMenu1->Items->Items[1]->Items[4]->Enabled = false;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N4Click(TObject *Sender)
{
        Is_Transparent = !Is_Transparent;
        if(Is_Transparent)
                MainMenu1->Items->Items[0]->Items[3]->Caption = "Выключить зеркальное отражение - F4";
        else
                MainMenu1->Items->Items[0]->Items[3]->Caption = "Включить зеркальное отражение - F4";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N6Click(TObject *Sender)
{
        MainMenu1->Items->Items[1]->Items[1]->Enabled = true;
        zRot -= 0.1f;
        if((zRot-MaxZRot)<_PEQUAL){
                zRot = MaxZRot;
                MainMenu1->Items->Items[1]->Items[0]->Enabled = false;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::N7Click(TObject *Sender)
{
        MainMenu1->Items->Items[1]->Items[0]->Enabled = true;
        zRot += 0.1f;
        if(MinZRot-zRot<_PEQUAL){
                zRot = MinZRot;
                MainMenu1->Items->Items[1]->Items[1]->Enabled = false;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Left1Click(TObject *Sender)
{
        if(Is_Light){
                theta2 -= 5.0f;
                theta2 = (GLfloat)((const int)theta2 % 360);
                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Right1Click(TObject *Sender)
{
        if(Is_Light){
                theta2 += 5.0f;
                theta2 = (GLfloat)((const int)theta2 % 360);
                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::F51Click(TObject *Sender)
{
        Is_MipMap = !Is_MipMap;
        if(Is_MipMap)
                MainMenu1->Items->Items[0]->Items[4]->Caption = "Выключить фильтрацию текстуры - F5";
        else
                MainMenu1->Items->Items[0]->Items[4]->Caption = "Включить фильтрацию текстуры - F5";



       /* if(Is_MipMap){
                //glActiveTexture(GL_TEXTURE0);
                c_tex[0].BindIt();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
               // glActiveTexture(GL_TEXTURE1);
                c_tex[1].BindIt();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                MainMenu1->Items->Items[0]->Items[4]->Caption = "Выключить фильтрацию текстуры - F5";
        }
        else {
               // glActiveTexture(GL_TEXTURE0);
                c_tex[0].BindIt();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
             //   glActiveTexture(GL_TEXTURE1);
                c_tex[1].BindIt();
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                MainMenu1->Items->Items[0]->Items[4]->Caption = "Включить фильтрацию текстуры - F5";
        }  */
}
//---------------------------------------------------------------------------
void Get_Textures(){

//first image
//glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);


       // glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        c_tex[0].Tex_Init("heart.jpg",1);
      //  c_tex[0].BindAndUpdate();
//second image
//glActiveTexture(GL_TEXTURE1);


      //  glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        c_tex[1].Tex_Init("mine.jpg",1);
     //   c_tex[1].BindAndUpdate();

glActiveTexture(GL_TEXTURE1);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

}

void __fastcall TForm1::N9Click(TObject *Sender)
{
        MotionKoef += MotionShag;
        MotionBias = 0.5f + atan(MotionKoef)/M_PI;
        if(MotionBias>=1.0f){
                MotionKoef -= MotionShag;
                MotionBias = 0.5f + atan(MotionKoef)/M_PI;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SubAccum01051Click(TObject *Sender)
{
        MotionKoef -= MotionShag;
        MotionBias = 0.5f + atan(MotionKoef)/M_PI;
        if(MotionBias<=0.0f){
                MotionKoef += MotionShag;
                MotionBias = 0.5f + atan(MotionKoef)/M_PI;
        }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::F61Click(TObject *Sender)
{

        Is_Motion = !Is_Motion;
        if(Is_Motion){
                MainMenu1->Items->Items[2]->Enabled = true;
                MainMenu1->Items->Items[0]->Items[5]->Caption = "Выключить эффект \"Опьяннённое сердце\" - F6";
        }
        else{
                fPass = false;
                MainMenu1->Items->Items[2]->Enabled = false;
                MainMenu1->Items->Items[0]->Items[5]->Caption = "Включить эффект \"Опьяннённое сердце\" - F6";
        }
}
//---------------------------------------------------------------------------

