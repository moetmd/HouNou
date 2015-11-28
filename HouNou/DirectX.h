#pragma once
using namespace std;
//-----------------------------------【程序说明】----------------------------------------------
//全局变量声明，函数声明
//------------------------------------------------------------------------------------------------

//-----------------------------------【宏定义部分】--------------------------------------------
// 描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	1152						//为窗口宽度定义的宏，以方便在此处修改窗口宽度

#endif 

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT	960						//为窗口高度定义的宏，以方便在此处修改窗口高度
#endif

#define WINDOW_TITLE _T("奉纳") //为窗口标题定义的宏




//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含程序所依赖的头文件
//------------------------------------------------------------------------------------------------                                                                                      
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include <map>


#include "CDirectMusic.h"
#include "DirectInputClass.h"
#include "D3DUtil.h"

#include "Game_GUI.h"
#include "Sprite.h"
#include "Game.h"

#include "TileUtil.h"




//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------  
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // 使用DirectInput必须包含的库文件，注意这里有8
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib") 



// 地板的顶点结构
struct CUSTOMVERTEX
{
	FLOAT _x, _y, _z;
	FLOAT _u, _v;
	CUSTOMVERTEX(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)




//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------


extern LPDIRECT3DDEVICE9		g_pd3dDevice;		//Direct3D设备对象
extern LPD3DXFONT				g_pTextFPS;			//字体COM接口
extern LPD3DXFONT				g_pTextAdaperName;  // 显卡信息的2D文本
extern LPD3DXFONT				g_pTextHelper;		// 帮助信息的2D文本
extern LPD3DXFONT				g_pTextInfor;		// 绘制信息的2D文本
extern float					g_FPS;				//一个浮点型的变量，代表帧速率
extern wchar_t					g_strFPS[];			//包含帧速率的字符数组
extern wchar_t					g_strAdapterName[]; //包含显卡名称的字符数组

extern DInputClass*				g_pDInput;			//DirectInput对象
extern LPDIRECT3DSURFACE9		backbuffer;			//缓冲

extern bool						g_LMBDown;			// GUI中的鼠标状态信息，鼠标左键是否按下的标识
extern int						g_MouseX, g_MouseY; //存储鼠标坐标的两个变量
extern bool						game_pause;         //游戏是否暂停
extern bool						game_over;			//游戏是否结束





//-----------------------------------【全局函数声明部分】-------------------------------------
//	描述：全局函数声明，防止“未声明的标识”系列错误
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