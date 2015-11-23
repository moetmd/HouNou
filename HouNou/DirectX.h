#pragma once
//-----------------------------------������˵����----------------------------------------------
//ȫ�ֱ�����������������
//------------------------------------------------------------------------------------------------

//-----------------------------------���궨�岿�֡�--------------------------------------------
// ����������һЩ������
//------------------------------------------------------------------------------------------------
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	1024						//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��

#endif 

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT	576 						//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#endif

#define WINDOW_TITLE _T("����") //Ϊ���ڱ��ⶨ��ĺ�




//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������������������ͷ�ļ�
//------------------------------------------------------------------------------------------------                                                                                      
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 

#include "CDirectMusic.h"
#include "DirectInputClass.h"
#include "D3DUtil.h"
#include "Game_GUI.h"
#include "Game.h"



//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------  
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // ʹ��DirectInput��������Ŀ��ļ���ע��������8
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib") 



// �ذ�Ķ���ṹ
struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _u, _v;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)

//sprite structure
struct SPRITE
{
	float x, y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	D3DCOLOR color;

	SPRITE()
	{
		frame = 0;
		columns = 1;
		width = height = 0;
		scaling = 1.0f;
		startframe = endframe = 0;
		direction = 1;
		starttime = 0;
		delay = 100;
		velx = vely = 0.0f;
		color = D3DCOLOR_XRGB(255, 255, 255);
	}
};


//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------


extern LPDIRECT3DDEVICE9		g_pd3dDevice;		//Direct3D�豸����
extern LPD3DXFONT				g_pTextFPS;			//����COM�ӿ�
extern LPD3DXFONT				g_pTextAdaperName;  // �Կ���Ϣ��2D�ı�
extern LPD3DXFONT				g_pTextHelper;		// ������Ϣ��2D�ı�
extern LPD3DXFONT				g_pTextInfor;		// ������Ϣ��2D�ı�
extern float					g_FPS;				//һ�������͵ı���������֡����
extern wchar_t					g_strFPS[];			//����֡���ʵ��ַ�����
extern wchar_t					g_strAdapterName[]; //�����Կ����Ƶ��ַ�����

extern DInputClass*				g_pDInput;			//DirectInput����
extern LPDIRECT3DSURFACE9		backbuffer;			//����

extern bool						g_LMBDown;			// GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
extern int						g_MouseX, g_MouseY; //�洢����������������
extern bool						game_pause;         //��Ϸ�Ƿ���ͣ
extern bool						game_over;			//��Ϸ�Ƿ����





//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK		WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT					Direct3D_Init(HWND hwnd, HINSTANCE hInstance);
HRESULT					Objects_Init();
void					Direct3D_Render(HWND hwnd, FLOAT fTimeDelta);
void					Direct3D_Update(HWND hwnd, FLOAT fTimeDelta);
void					Direct3D_CleanUp();
float					Get_FPS();
void					HelpText_Render(HWND hwnd);
void					GUICallback(int id, int state);