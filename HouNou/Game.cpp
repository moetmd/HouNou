
#include "DirectX.h"



#define MONSTER_TURN 1
#define PLAYER_TURN 2
#define AI_TURN 3

#define  UP 1
#define  DOWN 2
#define  LEFT 3
#define  RIGHT 4

using namespace std;

const int SCREENW = WINDOW_WIDTH;
const int SCREENH = WINDOW_HEIGHT;

//���ʯͷ��
const int MAX_STONE_NUM = 11;

//�غ���
int rounds;

//��ǰ�غ�
int current_turn;

//game state
bool	game_pause = true;         //��Ϸ�Ƿ���ͣ
bool	game_over = true;			//��Ϸ�Ƿ����


LPD3DXSPRITE spriteobj = NULL;

//sprites
Monster* monster;
map<int, Sprite*> stones;

Player* player_1;
Player* player_2;
Player* player_3;
Player* player_4;

AI_Player* ai_player_1;
AI_Player* ai_player_2;
AI_Player* ai_player_3;
AI_Player* ai_player_4;

//ȫ��ά��������player������������Һ�AI
map<int, Sprite*> players;

//����Ϸ������ά����player��������Ϊ��Һ�AI����
map<int, Player*> Human_Players;
map<int, Player*> AI_Players;

  //��Ϸ���������ڿ��Ƶ�iter
map<int, Player*>::iterator hp_iter;
map<int, Player*>::iterator ai_iter;

//�洢λ�õģ����ڻָ�λ��


//sound

CDirectMusic g_sound_bgm;

//textures


//font
LPD3DXFONT g_pFont = NULL;

//��Ϸ������
LPDIRECT3DSURFACE9 gameworld = NULL;

//settings for the scoller
const int TILEWIDTH = 64;   //�鳤��
const int TILEHEIGHT = 64;
const int MAPWIDTH = 18;    //��Ϸ���糤��
const int MAPHEIGHT = 15;

const int GAMEPANEL_WIDTH = 16;  //��Ϸ���򳤿�
const int GAMEPANEL_HEIGHT = 11;

//scrolling window size 
const int WINDOWWIDTH = (SCREENW / TILEWIDTH) * TILEWIDTH;
const int WINDOWHEIGHT = (SCREENH / TILEHEIGHT) * TILEHEIGHT;

//entire game world dimensions
const int GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
const int GAMEWORLDHEIGHT = TILEHEIGHT * MAPHEIGHT;

//gameworld position scrolling
float ScrollX = 0;
float ScrollY = 0;
float SpeedX = 0;
float SpeedY = 0;
long start = 0;



//position caculation




int MAPDATA[MAPWIDTH*MAPHEIGHT] = {
	41, 6, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	41, 41,41, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 50, 2, 41, 2, 79,
	80, 41,41, 50, 14, 15, 15, 15, 16, 2, 2, 2, 2, 2, 2, 2, 41, 89,
	90, 41,41, 2, 24, 25, 35, 25, 26, 2, 2, 44, 49, 2, 2, 2, 2, 41,
	2, 41,41, 2, 24, 26, 2, 24, 26, 2, 2, 64, 69, 2, 2, 2, 2, 2,
	41, 41,41, 2, 24, 25, 15, 25, 26, 2, 2, 50, 2, 2, 2, 2, 2, 2,
	2, 41,41, 2, 34, 35, 35, 35, 36, 2, 2, 2, 2, 11, 12, 13, 2, 2,
	2, 41,41, 2, 50, 2, 2, 2, 2, 2, 2, 2, 2, 21, 3, 23, 2, 2,
	2, 41,41, 41, 2, 2, 2, 2, 2, 2, 50, 2, 2, 31, 32, 33, 2, 2,
	2, 41,41, 2, 41, 2, 2, 55, 55, 55, 55, 2, 2, 2, 2, 2, 50, 2,
	2, 41,41, 79, 80, 41, 2, 2, 2, 2, 2, 2, 2, 14, 15, 15, 15, 16,
	2, 41,41, 89, 90, 2, 41, 2, 2, 2, 2, 2, 50, 34, 35, 35, 35, 36,
	2, 41,41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41,
	41, 41,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0
};

//����ǽ��Ѫ�ء��Լ�ʯͷ�ĳ�ʼλ��
int WALL[GAMEPANEL_HEIGHT][GAMEPANEL_WIDTH] = {
	{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1,-1,-1 },
	{  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1,-1 },
	{  0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,-1,-1 },
	{  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 0, 0,-1 },
	{  0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
	{  0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0 },
	{  0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	{ -1,-1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0 },
	{ -1,-1,-1, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 },
	{ -1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};




//��Ϸ��ʼ��
bool Game_Init()
{
	//������Ϸ״̬Ϊ �ǽ���
	game_over = false;
	game_pause = false;

	//��ʼ����Ϸ�غ���
	rounds = 1;

	//��ǰ�غ�����Ϊ ��һغ�
	current_turn = PLAYER_TURN;

	//create sprite object
	D3DXCreateSprite(g_pd3dDevice, &spriteobj);

	//������ҽ�ɫ
	player_1 = new Player(6);
	player_1->Set_img(L"GameMedia\\027_00.png");
	player_2 = new Player(3);
	player_2->Set_img(L"GameMedia\\027_00.png");
	player_3 = new Player(2);
	player_3->Set_img(L"GameMedia\\027_00.png");
	player_4 = new Player(4);
	player_4->Set_img(L"GameMedia\\027_00.png");

	//�����ҽ�ɫ�����list
	Human_Players[1] = player_1;
	Human_Players[2] = player_2;
	

	//�����ҽ�ɫ��ȫ��Map
	players[1] = player_1;
	players[2] = player_2;
	

	//����AI
	ai_player_1 = new AI_Player(6);
	ai_player_1->Set_img(L"GameMedia\\010_00.png");
	ai_player_2 = new AI_Player(3);
	ai_player_2->Set_img(L"GameMedia\\010_00.png");
	ai_player_3 = new AI_Player(2);
	ai_player_3->Set_img(L"GameMedia\\010_00.png");
	ai_player_4 = new AI_Player(4);
	ai_player_4->Set_img(L"GameMedia\\010_00.png");

	//���AI��ȫ��MAP
	players[5] = ai_player_1;
	players[6] = ai_player_2;


	//���AI��AI list
	AI_Players[1] = ai_player_1;
	AI_Players[2] = ai_player_2;
	

	

	//��������
	monster = new Monster();
	monster->Set_img(L"GameMedia\\001_00.png");

	//create font
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("΢���ź�"), &g_pFont)))
		return false;



	if (!g_sound_bgm.CreateSound("GameMedia\\Sound\\bgm.wav", UGP_INFINITE))
		return false;



	BuildGameWorld();

	Stones_Init();

	g_sound_bgm.Play();

	start = GetTickCount();

	hp_iter = Human_Players.begin();
	ai_iter = AI_Players.begin();



	return true;
}


void Game_Update(HWND window) 
{

	//make sure the Direct3D device is valid
	if (!g_pd3dDevice)
		return;


	//update input devices
	g_pDInput->GetInput();
	
	int current_rounds = rounds;

	

	//��ҵ���Ϊ
	if (current_turn == PLAYER_TURN && !Human_Players.empty() && Human_Players.size() != 0)
	{
		
		if (g_pDInput->IsKeyDown(DIK_UPARROW))
		{
			//���������ʣ��
			if(hp_iter->second->current_step > 0)

				//���������shift������ƶ��������
				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Up())
					{
						hp_iter->second->Move_Up(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 12;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
				}
				else
					if (hp_iter->second->Move_Up(false))
					{
						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 12;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
		}

		if (g_pDInput->IsKeyDown(DIK_DOWNARROW))
		{
			//���������ʣ��
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Down())
					{
						hp_iter->second->Move_Down(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 0;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
				}
				else
					if (hp_iter->second->Move_Down(false))
					{
						if (!hp_iter->second->Is_InBlood()) //�������Ѫ�������ٲ�������Ѫ���ﲻ���ٲ���
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 0;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}

		}

		if (g_pDInput->IsKeyDown(DIK_LEFTARROW))
		{
			//���������ʣ��
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Left())
					{
						hp_iter->second->Move_Left(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 4;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
				}
				else
					if (hp_iter->second->Move_Left(false))
					{
						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 4;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}


					}

		}

		if (g_pDInput->IsKeyDown(DIK_RIGHTARROW))
		{
			//���������ʣ��
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Right())
					{
						hp_iter->second->Move_Right(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 8;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
				}
				else
					if (hp_iter->second->Move_Right(false))
					{
						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							hp_iter->second->startframe = 8;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}

		}

		//ȷ�ϵ�ǰ��ɫλ�ã�������
		if (g_pDInput->IsKeyDown(DIK_SPACE))
		{
			if(!hp_iter->second->Is_OverPlayer() && !hp_iter->second->Is_InBlood())//�����ǰ��ɫû�к�������ɫ�ص����Ҳ���Ѫ���У������
				if (hp_iter == --Human_Players.end())
				{
					current_turn = AI_TURN;
					
				}
				else
				{
					++hp_iter;

				}
		}

		//����ߴ��ˣ���C�ָ�λ��
		if (g_pDInput->IsKeyDown(DIK_C))
		{
			
		}
	}

	//escape key exits
	if (g_pDInput->IsKeyDown(DIK_ESCAPE))
	{
		game_over = true;
		g_currentGUI = GUI_START_SCREEN;
		g_sound_bgm.Stop();
	}

	//AI����Ϊ
	if (current_turn == AI_TURN)
	{
		//���AI������ֱ�ӽ������Ļغ�
		if (!AI_Players.empty() && AI_Players.size() != 0)
		{
			ai_iter->second->Move_Up(false);
			ai_iter->second->current_step -= 1;

			if (ai_iter == --AI_Players.end())
			{
				current_turn = MONSTER_TURN;
				//�Ȼ�ȡ�ж�����
				monster->current_step = 0;
				monster->Get_CurrentStep();
			}
			else
			{
				++ai_iter;
			}
		}
		else
		{
			current_turn = MONSTER_TURN;
			//�Ȼ�ȡ�ж�����
			monster->current_step = 0;
			monster->Get_CurrentStep();
		}
	}

	//�������Ϊ
	if (current_turn == MONSTER_TURN)
	{
		
	
		switch (monster->step)
		{
		//����鵽ɱ�˿�
		case 1:
		case 2:
			monster->face_to = monster->Look_Around();
			if (monster->Move())
			{
				monster->current_step++;
			}
			if (monster->kill >= monster->step || monster->current_step >= 20)
			{
				monster->kill = 0;
				rounds += 1;
				current_turn = PLAYER_TURN;
			}
			break;

		default:
			monster->face_to = monster->Look_Around();
			if (monster->Move())
			{
				monster->current_step++;
			}
			if (monster->current_step >= monster->step)
			{
				monster->kill = 0;
				rounds += 1;
				current_turn = PLAYER_TURN;
			}
			break;
		}
	}

	
	//ɾ�������ʯͷ
	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end();)
	{
		if (iter->second->out_of_map || WALL[iter->second->world_Y][iter->second->world_X] == 1)
			stones.erase(iter++);
		else
			++iter;
	}

	//��ȫ�����map��ɾ���������Ľ�ɫ
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end();)
	{
		if (iter->second->killed)
			players.erase(iter++);
		else
			++iter;
	}

	//���������map��ɾ����������ɫ
	for (map<int, Player*>::iterator iter = Human_Players.begin(); iter != Human_Players.end();)
	{
		if (iter->second->killed)
			Human_Players.erase(iter++);
		else
			++iter;
	}

	//��AI���map��ɾ����������ɫ
	for (map<int, Player*>::iterator iter = AI_Players.begin(); iter != AI_Players.end();)
	{
		if (iter->second->killed)
			AI_Players.erase(iter++);
		else
			++iter;
	}

	//������������Ϊ0������Ϸ����
	if (Human_Players.empty() || Human_Players.size() == 0)
	{
		game_over = true;
		g_currentGUI = GUI_START_SCREEN;
		g_sound_bgm.Stop();
	}

	//����ǵڶ��֣������ʣ��Ľ�ɫ
	if (rounds == 2)
	{
		players[3] = player_3;
		players[4] = player_4;

		players[7] = ai_player_3;
		players[8] = ai_player_4;

		Human_Players[3] = player_3;
		Human_Players[4] = player_4;

		AI_Players[3] = ai_player_3;
		AI_Players[4] = ai_player_4;

	}


	//����ָ����㣬׼����һ����Ϸѭ��
	if (current_rounds != rounds)
	{
		//���ݻغ���ȷ����ɫ���ƶ�����
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			rounds % 2 == 0 ?
				((Player*)(iter->second))->current_step = ((Player*)(iter->second))->night_step :
				((Player*)(iter->second))->current_step = ((Player*)(iter->second))->day_step;
		}

		hp_iter = Human_Players.begin();
		ai_iter = AI_Players.begin();
	}
	
}


void Game_Render(HWND hwnd)
{	
	start = GetTickCount();

	ScrollScreen();

	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);

	
	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end(); iter++)
	{
		if (iter->second->visibal)
			iter->second->Draw();
	}

	monster->Draw();

	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); iter++)
	{
		if (iter->second->visibal)
		{
			iter->second->Draw();
			
			formatRect.top = iter->second->Get_RealPosY();
			formatRect.left = iter->second->Get_RealPosX(); 
			wchar_t g_strStep[30] = {0};
			int charCount = swprintf_s(g_strStep, 20, _T("%2d"), ((Player*) (iter->second))->current_step);
			g_pFont->DrawText(NULL, g_strStep, charCount, 
				&formatRect, DT_TOP | DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
		}
	}

	spriteobj->End();


}


void Game_Clean()
{
	//free memory
	if (gameworld)
		gameworld->Release();

	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_DELETE(monster);

	g_sound_bgm.Shutdown();


}

void Stones_Init()
{
	for (int i = 0; i < MAX_STONE_NUM; ++i)
	{
		Stone* stone = new Stone();
		stone->width = 64;
		stone->height = 64;
		stone->foot = 32;
		stone->Set_img(L"GameMedia\\stone.png");
		stone->out_of_map = false;
		stones[i] = stone;
	}

	
	map<int, Sprite*>::iterator iter;
	iter = stones.begin();
	for (int i = 0; i < GAMEPANEL_HEIGHT; i++)
	{
		for (int j = 0; j < GAMEPANEL_WIDTH; j++)
		{
			if (WALL[i][j] == 2)
			{
				iter->second->world_Y = i;
				iter->second->world_X = j;

				if (iter == stones.end())
					return;

				++iter;
			}

		}
	}

}



