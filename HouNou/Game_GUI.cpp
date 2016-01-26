
#include "DirectX.h"

//创建四个GUI类对象，分别代表四个页面
D3DGUIClass		*g_MainGUI = NULL;//主窗口
D3DGUIClass		*g_StartGUI = NULL; //游戏开始窗口
D3DGUIClass		*g_MultiGUI = NULL; //游戏载入窗口
D3DGUIClass		*g_OptionGUI = NULL; //游戏设置窗口

D3DGUIClass		*g_WinGUI = NULL; //胜利界面
D3DGUIClass		*g_LoseGUI = NULL; //失败界面
D3DGUIClass		*g_HelpGUI = NULL; //帮助界面

int						g_MainGUIFontID = -1;						//  GUI中字体对象的ID
int						g_StartGUIFontID = -1;						//  GUI中字体对象的ID
int						g_LoadGUIFontID = -1;						//  GUI中字体对象的ID
int						g_OptionGUIFontID = -1;						//  GUI中字体对象的ID

int						g_WinGUIFontID = -1;
int						g_LoseGUIFontID = -1;
int						g_HelpGUIFontID = -1;

int						g_currentGUI = GUI_MAIN_SCREEN;				//一个当前的GUI标识

bool					gui_input_lock = false;//防止连点的锁
int						tmp_gui = GUI_MAIN_SCREEN;
DWORD					lock_timeout;

//GUI所需资源初始化
bool GUI_Init()
{
	//--------------------------------------------------【GUI系统相关代码】-------------------------------------------------------

	// 创建一些GUI系统
	g_MainGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //主菜单页面
	g_StartGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //开始新游戏页面
	g_MultiGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //载入游戏页面
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //设置页面

	g_WinGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //胜利页面
	g_LoseGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //失败页面
	g_HelpGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //帮助页面



																			  // 给页面分别添加背景图
	if (!g_MainGUI->AddBackground(L"GameMedia/Menu/maingui.jpg")) return false;
	if (!g_StartGUI->AddBackground(L"GameMedia/Menu/startgui.jpg")) return false;
	if (!g_MultiGUI->AddBackground(L"GameMedia/Menu/loadgui.jpg")) return false;
	if (!g_OptionGUI->AddBackground(L"GameMedia/Menu/optiongui.jpg")) return false;

	if (!g_WinGUI->AddBackground(L"GameMedia/Menu/wingui.jpg")) return false;
	if (!g_LoseGUI->AddBackground(L"GameMedia/Menu/losegui.jpg")) return false;
	if (!g_HelpGUI->AddBackground(L"GameMedia/Menu/helpgui.jpg")) return false;


	// 分别给页面添加字体
	if (!g_MainGUI->CreateTextFont(L"微软雅黑", 28, &g_MainGUIFontID)) return false;
	if (!g_StartGUI->CreateTextFont(L"微软雅黑", 38, &g_StartGUIFontID)) return false;
	if (!g_MultiGUI->CreateTextFont(L"微软雅黑", 38, &g_LoadGUIFontID)) return false;
	if (!g_OptionGUI->CreateTextFont(L"微软雅黑", 38, &g_OptionGUIFontID)) return false;

	if (!g_WinGUI->CreateTextFont(L"微软雅黑", 38, &g_WinGUIFontID)) return false;
	if (!g_LoseGUI->CreateTextFont(L"微软雅黑", 38, &g_LoseGUIFontID)) return false;
	if (!g_HelpGUI->CreateTextFont(L"微软雅黑", 38, &g_HelpGUIFontID)) return false;




	//---------------------------------【主菜单main页面相关的页面布局代码】---------------------------------
	// 添加静态文本到页面中
	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"Version 0.1",
		1170, 735, D3DCOLOR_XRGB(55, 155, 255), g_MainGUIFontID)) return false;

	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"hehe",
		500, 150, D3DCOLOR_XRGB(255, 255, 255), g_MainGUIFontID)) return false;


	// 添加按钮，分别是开始游戏，载入进度，选项和退出游戏，每个按钮对应3幅图
	if (!g_MainGUI->AddButton(BUTTON_START_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia\\Menu\\startUp.png",
		L"GameMedia\\Menu\\StartOver.png", L"GameMedia\\Menu\\startDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_MULTI_ID, WINDOW_WIDTH / 2 - 50, 595, L"GameMedia\\Menu\\loadUp.png",
		L"GameMedia\\Menu\\loadOver.png", L"GameMedia\\Menu\\loadDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_OPTION_ID, WINDOW_WIDTH / 2 - 50, 650, L"GameMedia\\Menu\\optionsUp.png",
		L"GameMedia\\Menu\\optionsOver.png", L"GameMedia\\Menu\\optionsDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_HELP_ID, WINDOW_WIDTH / 2 - 50, 705, L"GameMedia\\Menu\\helpUp.png",
		L"GameMedia\\Menu\\helpOver.png", L"GameMedia\\Menu\\helpDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_QUIT_ID, WINDOW_WIDTH / 2 - 50, 760, L"GameMedia\\Menu\\quitUp.png",
		L"GameMedia\\Menu\\quitOver.png", L"GameMedia\\Menu\\quitDown.png")) return false;


	//------------------------【开始新游戏start页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/level1Up.png", L"GameMedia/Menu/level1Over.png",
		L"GameMedia/Menu/level1Down.png")) return false;

	if (!g_StartGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 595, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------【载入游戏load页面相关的页面布局代码】------------------------
	//添加静态文本到页面中
	if (!g_MultiGUI->AddStaticText(STATIC_TEXT_ID, L"这里是load game页面",
		411, 340, D3DCOLOR_XRGB(33, 255, 55), g_LoadGUIFontID)) return false;
	
	//添加动态文本到页面中
	if (-1 == g_MultiGUI->AddDynamicText(DYNAMIC_TEXT_ID, L"请输入IP地址：",
		200, 200, D3DCOLOR_XRGB(80, 80, 80), g_LoadGUIFontID)) return false;

	// 添加按钮到页面中
	if (!g_MultiGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------【游戏设置option页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_OptionGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//添加静态文本到页面中
	if (!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"这里是Option页面",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_OptionGUIFontID)) return false;

	//------------------------【游戏帮助help页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_HelpGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//添加静态文本到页面中
	if (!g_HelpGUI->AddStaticText(STATIC_TEXT_ID, L"这里是help页面",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_HelpGUIFontID)) return false;

	//------------------------【游戏胜利win页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_WinGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//添加静态文本到页面中
	if (!g_WinGUI->AddStaticText(STATIC_TEXT_ID, L"这里是win页面",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_WinGUIFontID)) return false;

	//------------------------【游戏失败lose页面相关的页面布局代码】------------------------
	// 添加按钮到页面中
	if (!g_LoseGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//添加静态文本到页面中
	if (!g_LoseGUI->AddStaticText(STATIC_TEXT_ID, L"这里是lose页面",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_LoseGUIFontID)) return false;

	lock_timeout = timeGetTime();

}


//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{

	if (timeGetTime() - lock_timeout > 500)
	{
		gui_input_lock = false;
	}
	else
	{
		state = -1;
	}

	if (tmp_gui != g_currentGUI)
	{
		gui_input_lock = true;
		lock_timeout = timeGetTime();
		tmp_gui = g_currentGUI;
	}

	switch (id)
	{
	case BUTTON_START_ID:   //start game开始游戏按钮
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_START_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_MULTI_ID:  //多人游戏 按钮
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_MULTI_SCREEN;
				gui_input_lock = true;
				
				multi_game = new MultiGame();
			}
		break;

	case BUTTON_OPTION_ID: //option设置按钮
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_OPTION_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_BACK_ID: //back返回按钮
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_MAIN_SCREEN;
				gui_input_lock = true;

				if (multi_game)
					SAFE_DELETE(multi_game);
			}
		break;

	case BUTTON_HELP_ID:  //help按钮
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_HELP_SCREEN;
				gui_input_lock = true;
			}
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

//GUI输入交互
void GUI_InputUpdate(D3DGUIClass* gui)
{
	if (g_pDInput->IsKeyDown(DIK_1) || g_pDInput->IsKeyDown(DIK_NUMPAD1))
	{
		GUI_DTextUpdate(gui, L'1', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}

	if (g_pDInput->IsKeyDown(DIK_2) || g_pDInput->IsKeyDown(DIK_NUMPAD2))
	{
		GUI_DTextUpdate(gui, L'2', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_3) || g_pDInput->IsKeyDown(DIK_NUMPAD3))
	{
		GUI_DTextUpdate(gui, L'3', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_4) || g_pDInput->IsKeyDown(DIK_NUMPAD4))
	{
		GUI_DTextUpdate(gui, L'4', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_5) || g_pDInput->IsKeyDown(DIK_NUMPAD5))
	{
		GUI_DTextUpdate(gui, L'5', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_6) || g_pDInput->IsKeyDown(DIK_NUMPAD6))
	{
		GUI_DTextUpdate(gui, L'6', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_7) || g_pDInput->IsKeyDown(DIK_NUMPAD7))
	{
		GUI_DTextUpdate(gui, L'7', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_8) || g_pDInput->IsKeyDown(DIK_NUMPAD8))
	{
		GUI_DTextUpdate(gui, L'8', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_9) || g_pDInput->IsKeyDown(DIK_NUMPAD9))
	{
		GUI_DTextUpdate(gui, L'9', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_0) || g_pDInput->IsKeyDown(DIK_NUMPAD0))
	{
		GUI_DTextUpdate(gui, L'0', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_PERIOD) || g_pDInput->IsKeyDown(DIK_DECIMAL))
	{
		GUI_DTextUpdate(gui, L'.', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_BACKSPACE))
	{
		GUI_DTextUpdate(gui, L'-', 200, 200, D3DCOLOR_XRGB(80, 80, 80));
	}
	if (g_pDInput->IsKeyDown(DIK_RETURN))
	{
		g_currentGUI = GUI_MULTI_READY_SCREEN;
	}
	g_pDInput->ClearKey();
}


//GUI清理函数
void GUI_CleanUp()
{
	SAFE_DELETE(g_MainGUI);
	SAFE_DELETE(g_StartGUI);
	SAFE_DELETE(g_MultiGUI);
	SAFE_DELETE(g_OptionGUI);
	SAFE_DELETE(g_WinGUI);
	SAFE_DELETE(g_LoseGUI);
	SAFE_DELETE(g_HelpGUI);
}