#pragma once



#include "D3DGUIClass.h"


//�����ĸ�GUI����󣬷ֱ�����ĸ�ҳ��
extern D3DGUIClass		*g_MainGUI;//������
extern D3DGUIClass		*g_StartGUI; //��Ϸ��ʼ����
extern D3DGUIClass		*g_LoadGUI; //��Ϸ���봰��
extern D3DGUIClass		*g_OptionGUI; //��Ϸ���ô���

extern int						g_MainGUIFontID;						//  GUI����������ID
extern int						g_StartGUIFontID;						//  GUI����������ID
extern int						g_LoadGUIFontID;						//  GUI����������ID
extern int						g_OptionGUIFontID;					//  GUI����������ID
extern int						g_currentGUI;    //һ����ǰ��GUI��ʶ

bool GUI_Init();
void GUICallback(int id, int state);
void GUI_CleanUp();