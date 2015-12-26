
#include "DirectX.h"



#define MONSTER_TURN 1
#define PLAYER_TURN 2
#define AI_TURN 3

#define  UP 1
#define  DOWN 2
#define  LEFT 3
#define  RIGHT 4

double GaussRand();

using namespace std;

const int SCREENW = WINDOW_WIDTH;
const int SCREENH = WINDOW_HEIGHT;

//settings for the scoller
const int TILEWIDTH = 64;   //�鳤��
const int TILEHEIGHT = 64;

const int MAPWIDTH = 20;    //��Ϸ���糤��
const int MAPHEIGHT = 13;

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

//���ʯͷ��
const int MAX_STONE_NUM = 11;

//�غ���
int rounds;
int current_rounds;

//��ǰ�غ�
int current_turn;

//game state
bool	game_pause = true;         //��Ϸ�Ƿ���ͣ
bool	game_over = true;			//��Ϸ�Ƿ����

//���������
int h_escape_num = 0;
//AI������
int ai_escape_num = 0;

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

Sprite* cursor; //ָ��ǰ��ɫ��ָ��

//ȫ��ά��������player������������Һ�AI
map<int, Sprite*> players;

//����Ϸ������ά����player��������Ϊ��Һ�AI����
map<int, Player*> Human_Players;
map<int, Player*> AI_Players;

  //��Ϸ���������ڿ��Ƶ�iter
map<int, Player*>::iterator hp_iter;
map<int, Player*>::iterator ai_iter;

//�洢λ�õģ����ڻָ�λ��


//����AI�͹����ж�Ƶ��
DWORD timer;

//AI��ʱ�趨
DWORD ai_timeout;

//sound

CDirectMusic g_sound_bgm;

//textures


//font
LPD3DXFONT g_pFont = NULL;

//��Ϸ������
LPDIRECT3DSURFACE9 gameworld = NULL;


//position caculation



int MAPDATA[MAPWIDTH*MAPHEIGHT] = {
	1, 2, 3, 4, 4, 4, 5, 6, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 7, 8, 9,
	10, 11, 11, 11, 12, 13, 14, 11, 11, 11, 11, 11, 11, 15, 16, 17,
	18, 19, 20, 21,
	22, 11, 11, 11, 23, 24, 25, 11, 26, 27, 27, 28, 29, 11, 30, 31,
	32, 19, 11, 33,
	34, 11, 11, 11, 35, 36, 37, 11, 38, 39, 40, 41, 42, 11, 11, 43,
	44, 19, 11, 45,
	34, 11, 11, 11, 11, 11, 11, 11, 38, 46, 47, 48, 11, 11, 11, 11,
	49, 19, 11, 11,
	34, 11, 11, 11, 11, 11, 11, 11, 50, 51, 51, 52, 11, 11, 11, 11,
	11, 19, 11, 11,
	34, 11, 53, 54, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 19, 11, 11,
	34, 11, 55, 56, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
	11, 19, 11, 11,
	57, 58, 11, 11, 26, 27, 27, 27, 27, 59, 11, 11, 11, 11, 11, 11,
	11, 19, 60, 61,
	62, 63, 64, 11, 38, 65, 66, 66, 67, 48, 11, 11, 11, 11, 11, 11,
	11, 19, 68, 69,
	70, 71, 72, 73, 50, 51, 51, 51, 51, 52, 11, 11, 11, 11, 11, 74,
	75, 76, 77, 78,
	79, 80, 81, 82, 83, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 84,
	85, 86, 87, 88,
	89, 90, 91, 92, 93, 94, 95, 95, 95, 95, 95, 95, 95, 95, 95, 96,
	97, 98, 99, 100
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
	player_1->current_step -= 1;

	player_2 = new Player(3);
	player_2->Set_img(L"GameMedia\\027_00.png");
	player_2->current_step -= 1;

	player_3 = new Player(2);
	player_3->Set_img(L"GameMedia\\027_00.png");
	player_3->current_step -= 1;

	player_4 = new Player(4);
	player_4->Set_img(L"GameMedia\\027_00.png");
	player_4->current_step -= 1;

	//�����ҽ�ɫ�����list
	Human_Players[1] = player_1;
	Human_Players[2] = player_2;
	

	//�����ҽ�ɫ��ȫ��Map
	players[1] = player_1;
	players[2] = player_2;
	

	//����AI
	ai_player_1 = new AI_Player(6);
	ai_player_1->Set_img(L"GameMedia\\010_00.png");
	ai_player_1->current_step -= 1;

	ai_player_2 = new AI_Player(3);
	ai_player_2->Set_img(L"GameMedia\\010_00.png");
	ai_player_2->current_step -= 1;

	ai_player_3 = new AI_Player(2);
	ai_player_3->Set_img(L"GameMedia\\010_00.png");
	ai_player_3->current_step -= 1;

	ai_player_4 = new AI_Player(4);
	ai_player_4->Set_img(L"GameMedia\\010_00.png");
	ai_player_4->current_step -= 1;

	//���AI��ȫ��MAP
	players[5] = ai_player_1;
	players[6] = ai_player_2;


	//���AI��AI list
	AI_Players[1] = ai_player_1;
	AI_Players[2] = ai_player_2;
	

	//����ָ��
	cursor = new Sprite();
	cursor->world_X = player_1->world_X;
	cursor->world_Y = player_1->world_Y - 1;
	cursor->Set_img(L"GameMedia\\cursor.tga");
	cursor->endframe = 1;
	cursor->columns = 2;
	cursor->width = cursor->height = 17;

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

	current_rounds = 1;

	timer = timeGetTime();
	srand(timeGetTime());

	return true;
}


void Game_Update(HWND window) 
{

	//make sure the Direct3D device is valid
	if (!g_pd3dDevice)
		return;


	//update input devices
	g_pDInput->GetInput();
	

	/*���㿪ʼ*/
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

	//������������Ϊ3�������ϣ�����Ϸ����
	if (h_escape_num >= 3)
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

	

	current_rounds = rounds;
	/*�������*/
	

	//��ҵ���Ϊ
	if (current_turn == PLAYER_TURN && !Human_Players.empty() && Human_Players.size() != 0)
	{
		
		if (g_pDInput->IsKeyDown(DIK_UPARROW))
		{
			//���������ʣ��
			if (hp_iter->second->current_step > 0)
			{
				//����������Ͻǣ��������������1���ý�ɫ��������
				if (hp_iter->second->world_X == 0 && hp_iter->second->world_Y == 0)
				{
					h_escape_num += 1;
					hp_iter->second->killed = true;
					hp_iter->second->visibal = false;
				}

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
			{
				//����������Ͻǣ��������������1���ý�ɫ��������
				if (hp_iter->second->world_X == 0 && hp_iter->second->world_Y == 0)
				{
					h_escape_num += 1;
					hp_iter->second->killed = true;
					hp_iter->second->visibal = false; 
				}

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

		//ȷ�ϵ�ǰ��ɫλ�ã�������   ���� �Ѿ��ӳ�������Ϊ������
		if (g_pDInput->IsKeyDown(DIK_SPACE) || hp_iter->second->killed)
		{
			if(!hp_iter->second->Is_OverPlayer() && !hp_iter->second->Is_InBlood())//�����ǰ��ɫû�к�������ɫ�ص����Ҳ���Ѫ���У������
				if (hp_iter == --Human_Players.end())
				{
					current_turn = AI_TURN;
					ai_timeout = timeGetTime(); //ai��ʱ��ʼ��
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

		//��ָ��ָ��ǰ��ɫ
		cursor->world_X = hp_iter->second->world_X;
		cursor->world_Y = hp_iter->second->world_Y - 1;

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

		//�ж�AI�Ƿ�������
		if (!AI_Players.empty() && AI_Players.size() != 0)
		{
			//����AI���ƶ�Ƶ��Ϊһ��һ��
			if (timeGetTime() - timer > 1000)

			//�����������0��������  �����ʱ 12s����ֱ������
			if (ai_iter->second->current_step > 0 && timeGetTime() - ai_timeout < 12000 )
				switch ( (int)GaussRand() % 4)
				{
				case 0:
					if (ai_iter->second->Move_Down(false))
					{
						if (!ai_iter->second->Is_InBlood())
						{
							ai_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							ai_iter->second->startframe = 0;
							ai_iter->second->endframe = ai_iter->second->startframe + 3;
						}
					}
					timer = timeGetTime();
					break;
				case 1:
					if (ai_iter->second->Move_Up(false))
					{
						if (!ai_iter->second->Is_InBlood())
						{
							ai_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							ai_iter->second->startframe = 12;
							ai_iter->second->endframe = ai_iter->second->startframe + 3;
						}
					}
					timer = timeGetTime();
					break;
				case 2:
					if (ai_iter->second->Move_Left(false))
					{
						if (!ai_iter->second->Is_InBlood())
						{
							ai_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							ai_iter->second->startframe = 4;
							ai_iter->second->endframe = ai_iter->second->startframe + 3;
						}
					}
					timer = timeGetTime();
					break;
				case 3:
					if (ai_iter->second->Move_Right(false))
					{
						if (!ai_iter->second->Is_InBlood())
						{
							ai_iter->second->current_step -= 1;

							//�ı��ɫ�Ķ�����ʼ�ͽ���֡
							ai_iter->second->startframe = 8;
							ai_iter->second->endframe = ai_iter->second->startframe + 3;
						}
					}
					timer = timeGetTime();
					break;

				default:
					break;
				}
			else //��������þ�
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
					ai_timeout = timeGetTime();
				}


			cursor->world_X = ai_iter->second->world_X;
			cursor->world_Y = ai_iter->second->world_Y - 1;
		}
		else	//���AI������ֱ�ӽ������Ļغ�
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
		//���ƹ����ƶ�Ƶ��Ϊ1��һ��
		if(timeGetTime() - timer > 1000)
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
				timer = timeGetTime();
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
				timer = timeGetTime();
				break;
			}

		cursor->world_X = monster->world_X;
		cursor->world_Y = monster->world_Y - 1;
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

	cursor->Draw();

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

	SAFE_RELEASE(g_pFont);

	SAFE_DELETE(monster);

	SAFE_DELETE(cursor);

	SAFE_DELETE(player_1);
	SAFE_DELETE(player_2);
	SAFE_DELETE(player_3);
	SAFE_DELETE(player_4);

	SAFE_DELETE(ai_player_1);
	SAFE_DELETE(ai_player_2);
	SAFE_DELETE(ai_player_3);
	SAFE_DELETE(ai_player_4);

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

//��˹�ֲ������
double GaussRand()
{
	static double v1, v2, s;
	static int phase = 0;
	double x;
	double V = 0.4; //����
	double E = 2; //����ֵ

	if (0 == phase)
	{
		do
		{
			double u1 = (double)rand() / RAND_MAX;
			double u2 = (double)rand() / RAND_MAX;

			v1 = 2 * u1 - 1;
			v2 = 2 * u2 - 1;
			s = v1 * v1 + v2 * v2;
		} while (1 <= s || 0 == s);
		x = v1 * sqrt(-2 * log(s) / s);
	}
	else
	{
		x = v2 * sqrt(-2 * log(s) / s);
	}
	phase = 1 - phase;

	return x * V + E;
}

