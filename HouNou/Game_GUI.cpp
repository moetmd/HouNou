
#include "DirectX.h"

//�����ĸ�GUI����󣬷ֱ�����ĸ�ҳ��
D3DGUIClass		*g_MainGUI = NULL;//������
D3DGUIClass		*g_StartGUI = NULL; //��Ϸ��ʼ����
D3DGUIClass		*g_LoadGUI = NULL; //��Ϸ���봰��
D3DGUIClass		*g_OptionGUI = NULL; //��Ϸ���ô���

D3DGUIClass		*g_WinGUI = NULL; //ʤ������
D3DGUIClass		*g_LoseGUI = NULL; //ʧ�ܽ���
D3DGUIClass		*g_HelpGUI = NULL; //��������

int						g_MainGUIFontID = -1;						//  GUI����������ID
int						g_StartGUIFontID = -1;						//  GUI����������ID
int						g_LoadGUIFontID = -1;						//  GUI����������ID
int						g_OptionGUIFontID = -1;						//  GUI����������ID

int						g_WinGUIFontID = -1;
int						g_LoseGUIFontID = -1;
int						g_HelpGUIFontID = -1;

int						g_currentGUI = GUI_MAIN_SCREEN;				//һ����ǰ��GUI��ʶ

bool					gui_input_lock = false;
int tmp_gui = GUI_MAIN_SCREEN;
DWORD lock_timeout;

//GUI������Դ��ʼ��
bool GUI_Init()
{
	//--------------------------------------------------��GUIϵͳ��ش��롿-------------------------------------------------------

	// ����һЩGUIϵͳ
	g_MainGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //���˵�ҳ��
	g_StartGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //��ʼ����Ϸҳ��
	g_LoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //������Ϸҳ��
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //����ҳ��

	g_WinGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //ʤ��ҳ��
	g_LoseGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //ʧ��ҳ��
	g_HelpGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT); //����ҳ��



																			  // ��ҳ��ֱ���ӱ���ͼ
	if (!g_MainGUI->AddBackground(L"GameMedia/Menu/maingui.jpg")) return false;
	if (!g_StartGUI->AddBackground(L"GameMedia/Menu/startgui.jpg")) return false;
	if (!g_LoadGUI->AddBackground(L"GameMedia/Menu/loadgui.jpg")) return false;
	if (!g_OptionGUI->AddBackground(L"GameMedia/Menu/optiongui.jpg")) return false;

	if (!g_WinGUI->AddBackground(L"GameMedia/Menu/wingui.jpg")) return false;
	if (!g_LoseGUI->AddBackground(L"GameMedia/Menu/losegui.jpg")) return false;
	if (!g_HelpGUI->AddBackground(L"GameMedia/Menu/helpgui.jpg")) return false;


	// �ֱ��ҳ���������
	if (!g_MainGUI->CreateTextFont(L"΢���ź�", 28, &g_MainGUIFontID)) return false;
	if (!g_StartGUI->CreateTextFont(L"΢���ź�", 38, &g_StartGUIFontID)) return false;
	if (!g_LoadGUI->CreateTextFont(L"΢���ź�", 38, &g_LoadGUIFontID)) return false;
	if (!g_OptionGUI->CreateTextFont(L"΢���ź�", 38, &g_OptionGUIFontID)) return false;

	if (!g_WinGUI->CreateTextFont(L"΢���ź�", 38, &g_WinGUIFontID)) return false;
	if (!g_LoseGUI->CreateTextFont(L"΢���ź�", 38, &g_LoseGUIFontID)) return false;
	if (!g_HelpGUI->CreateTextFont(L"΢���ź�", 38, &g_HelpGUIFontID)) return false;




	//---------------------------------�����˵�mainҳ����ص�ҳ�沼�ִ��롿---------------------------------
	// ��Ӿ�̬�ı���ҳ����
	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"Version 0.1",
		1170, 735, D3DCOLOR_XRGB(55, 155, 255), g_MainGUIFontID)) return false;

	if (!g_MainGUI->AddStaticText(STATIC_TEXT_ID, L"hehe",
		500, 150, D3DCOLOR_XRGB(255, 255, 255), g_MainGUIFontID)) return false;


	// ��Ӱ�ť���ֱ��ǿ�ʼ��Ϸ��������ȣ�ѡ����˳���Ϸ��ÿ����ť��Ӧ3��ͼ
	if (!g_MainGUI->AddButton(BUTTON_START_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia\\Menu\\startUp.png",
		L"GameMedia\\Menu\\StartOver.png", L"GameMedia\\Menu\\startDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_LOAD_ID, WINDOW_WIDTH / 2 - 50, 595, L"GameMedia\\Menu\\loadUp.png",
		L"GameMedia\\Menu\\loadOver.png", L"GameMedia\\Menu\\loadDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_OPTION_ID, WINDOW_WIDTH / 2 - 50, 650, L"GameMedia\\Menu\\optionsUp.png",
		L"GameMedia\\Menu\\optionsOver.png", L"GameMedia\\Menu\\optionsDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_HELP_ID, WINDOW_WIDTH / 2 - 50, 705, L"GameMedia\\Menu\\helpUp.png",
		L"GameMedia\\Menu\\helpOver.png", L"GameMedia\\Menu\\helpDown.png")) return false;

	if (!g_MainGUI->AddButton(BUTTON_QUIT_ID, WINDOW_WIDTH / 2 - 50, 760, L"GameMedia\\Menu\\quitUp.png",
		L"GameMedia\\Menu\\quitOver.png", L"GameMedia\\Menu\\quitDown.png")) return false;


	//------------------------����ʼ����Ϸstartҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_StartGUI->AddButton(BUTTON_LEVEL_1_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/level1Up.png", L"GameMedia/Menu/level1Over.png",
		L"GameMedia/Menu/level1Down.png")) return false;

	if (!g_StartGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 595, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------��������Ϸloadҳ����ص�ҳ�沼�ִ��롿------------------------
	//��Ӿ�̬�ı���ҳ����
	if (!g_LoadGUI->AddStaticText(STATIC_TEXT_ID, L"������load gameҳ��",
		411, 340, D3DCOLOR_XRGB(33, 255, 55), g_LoadGUIFontID)) return false;
	// ��Ӱ�ť��ҳ����
	if (!g_LoadGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;


	//------------------------����Ϸ����optionҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_OptionGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	if (!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"������Optionҳ��",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_OptionGUIFontID)) return false;

	//------------------------����Ϸ����helpҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_HelpGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	if (!g_HelpGUI->AddStaticText(STATIC_TEXT_ID, L"������helpҳ��",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_HelpGUIFontID)) return false;

	//------------------------����Ϸʤ��winҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_WinGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	if (!g_WinGUI->AddStaticText(STATIC_TEXT_ID, L"������winҳ��",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_WinGUIFontID)) return false;

	//------------------------����Ϸʧ��loseҳ����ص�ҳ�沼�ִ��롿------------------------
	// ��Ӱ�ť��ҳ����
	if (!g_LoseGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH / 2 - 50, 540, L"GameMedia/Menu/backUp.png", L"GameMedia/Menu/backOver.png",
		L"GameMedia/Menu/backDown.png")) return false;
	//��Ӿ�̬�ı���ҳ����
	if (!g_LoseGUI->AddStaticText(STATIC_TEXT_ID, L"������loseҳ��",
		540, 60, D3DCOLOR_XRGB(33, 55, 255), g_LoseGUIFontID)) return false;

	lock_timeout = timeGetTime();

}


//-----------------------------------��GUICallback( )������---------------------------------------
//	������GUIϵͳ�Ļص���������д��ť���º����ش������
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
	case BUTTON_START_ID:   //start game��ʼ��Ϸ��ť
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_START_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_LOAD_ID:  //load game������Ϸ��ť
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_LOAD_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_OPTION_ID: //option���ð�ť
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_OPTION_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_BACK_ID: //back���ذ�ť
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_MAIN_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_HELP_ID:  //help��ť
		if (state == UGP_BUTTON_DOWN)
			if (!gui_input_lock)
			{
				g_currentGUI = GUI_HELP_SCREEN;
				gui_input_lock = true;
			}
		break;

	case BUTTON_QUIT_ID://quit�˳���ť
		if (state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_LEVEL_1_ID:  //start game��ʼ��Ϸҳ���У�Level1��ť
		if (state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GAME_RUN;

			if(game_over)
				Game_Init();
		}
		break;
	}

}

//GUI������
void GUI_CleanUp()
{
	SAFE_DELETE(g_MainGUI);
	SAFE_DELETE(g_StartGUI);
	SAFE_DELETE(g_LoadGUI);
	SAFE_DELETE(g_OptionGUI);
	SAFE_DELETE(g_WinGUI);
	SAFE_DELETE(g_LoseGUI);
	SAFE_DELETE(g_HelpGUI);
}