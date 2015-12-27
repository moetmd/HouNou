
#include "DirectX.h"

//创建四个GUI类对象，分别代表四个页面
D3DGUIClass		*g_MainGUI = NULL;//主窗口
D3DGUIClass		*g_StartGUI = NULL; //游戏开始窗口
D3DGUIClass		*g_LoadGUI = NULL; //游戏载入窗口
D3DGUIClass		*g_OptionGUI = NULL; //游戏设置窗口


int						g_MainGUIFontID = -1;						//  GUI中字体对象的ID
int						g_StartGUIFontID = -1;						//  GUI中字体对象的ID
int						g_LoadGUIFontID = -1;						//  GUI中字体对象的ID
int						g_OptionGUIFontID = -1;						//  GUI中字体对象的ID


int						g_currentGUI = GUI_MAIN_SCREEN;				//一个当前的GUI标识



//GUI所需资源初始化
bool GUI_Init()
{
	//--------------------------------------------------【GUI系统相关代码】-------------------------------------------------------

	// 创建一些GUI系统
	g_MainGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //主菜单页面
	g_StartGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //开始新游戏页面
	g_LoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //载入游戏页面
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //设置页面




																			  // 给页面分别添加背景图
	if (!g_MainGUI->AddBackground(L"GameMedia/Menu/maingui.jpg")) return false;
	if (!g_StartGUI->AddBackground(L"GameMedia/Menu/startgui.jpg")) return false;
	if (!g_LoadGUI->AddBackground(L"GameMedia/Menu/loadgui.jpg")) return false;
	if (!g_OptionGUI->AddBackground(L"GameMedia/Menu/optiongui.jpg")) return false;



	// 分别给页面添加字体
	if (!g_MainGUI->CreateTextFont(L"微软雅黑", 28, &g_MainGUIFontID)) return false;
	if (!g_StartGUI->CreateTextFont(L"微软雅黑", 38, &g_StartGUIFontID)) return false;
	if (!g_LoadGUI->CreateTextFont(L"微软雅黑", 38, &g_LoadGUIFontID)) return false;
	if (!g_OptionGUI->CreateTextFont(L"微软雅黑", 38, &g_OptionGUIFontID)) return false;




	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"Version 0.1",
		1170, 735, D3DCOLOR_XRGB(55, 155, 255), g_MainGUIFontID)) return false;

	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"呵呵",
		500, 150, D3DCOLOR_XRGB(255, 255, 255), g_MainGUIFontID)) return false;


	// 添加按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if (!g_MainGUI->AddButton(BUTTON_START_ID, 650, 340, L"GameMedia\\Menu\\startUp.png",
		L"GameMedia\\Menu\\StartOver.png", L"GameMedia\\Menu\\startDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_LOAD_ID, 650, 385, L"GameMedia\\Menu\\loadUp.png",
		L"GameMedia\\Menu\\loadOver.png", L"GameMedia\\Menu\\loadDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_OPTION_ID, 650, 430, L"GameMedia\\Menu\\optionsUp.png",
		L"GameMedia\\Menu\\optionsOver.png", L"GameMedia\\Menu\\optionsDown.png")) return false;


	if (!g_MainGUI->AddButton(BUTTON_QUIT_ID, 650, 510, L"GameMedia\\Menu\\quitUp.png",
		L"GameMedia\\Menu\\quitOver.png", L"GameMedia\\Menu\\quitDown.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID, 550, 380, L"GameMedia/Menu/level1Up.png", L"GameMedia/Menu/level1Over.png",
		L"GameMedia/Menu/level1Down.png")) return false;

	if (!g_StartGUI->AddButton(BUTTON_BACK_ID, 750, 350, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------【载入游戏load页面相关的页面布局代码】------------------------
	//添加静态文本到页面中
	if (!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"这里是load game页面",
		411, 340, D3DCOLOR_XRGB(33, 255, 55), g_LoadGUIFontID)) return false;
	// 添加按钮到页面中
	if (!g_LoadGUI->AddButton(BUTTON_BACK_ID, 750, 400, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_OptionGUI->AddButton(BUTTON_BACK_ID, 750, 450, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//添加静态文本到页面中
	if (!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"这里是Option页面",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_OptionGUIFontID)) return false;

}


//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch (id)
	{
	case BUTTON_START_ID:   //start game开始游戏按钮
		if (state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_START_SCREEN;
		break;

	case BUTTON_LOAD_ID:  //load game载入游戏按钮
		if (state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_LOAD_SCREEN;
		break;

	case BUTTON_OPTION_ID: //option设置按钮
		if (state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_OPTION_SCREEN;
		break;

	case BUTTON_BACK_ID: //back返回按钮
		if (state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MAIN_SCREEN;
		break;


	case BUTTON_QUIT_ID://quit退出按钮
		if (state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:  //start game开始游戏页面中，Level1按钮
		if (state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GAME_RUN;

			if(game_over)
				Game_Init();
		}
		break;
	}
}

//GUI清理函数
void GUI_CleanUp()
{
	SAFE_DELETE(g_MainGUI);
	SAFE_DELETE(g_StartGUI);
	SAFE_DELETE(g_LoadGUI);
	SAFE_DELETE(g_OptionGUI);

}