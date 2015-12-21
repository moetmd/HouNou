
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

//最大石头数
const int MAX_STONE_NUM = 11;

//回合数
int rounds;

//当前回合
int current_turn;

//game state
bool	game_pause = true;         //游戏是否暂停
bool	game_over = true;			//游戏是否结束


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

//全局维护的所有player容器，包括玩家和AI
map<int, Sprite*> players;

//在游戏过程中维护的player容器，分为玩家和AI两种
map<int, Player*> Human_Players;
map<int, Player*> AI_Players;

  //游戏过程中用于控制的iter
map<int, Player*>::iterator hp_iter;
map<int, Player*>::iterator ai_iter;

//存储位置的，用于恢复位置


//sound

CDirectMusic g_sound_bgm;

//textures


//font
LPD3DXFONT g_pFont = NULL;

//游戏世界表层
LPDIRECT3DSURFACE9 gameworld = NULL;

//settings for the scoller
const int TILEWIDTH = 64;   //块长宽
const int TILEHEIGHT = 64;
const int MAPWIDTH = 18;    //游戏世界长宽
const int MAPHEIGHT = 15;

const int GAMEPANEL_WIDTH = 16;  //游戏区域长宽
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

//保存墙和血池、以及石头的初始位置
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




//游戏初始化
bool Game_Init()
{
	//设置游戏状态为 非结束
	game_over = false;
	game_pause = false;

	//初始化游戏回合数
	rounds = 1;

	//当前回合设置为 玩家回合
	current_turn = PLAYER_TURN;

	//create sprite object
	D3DXCreateSprite(g_pd3dDevice, &spriteobj);

	//创建玩家角色
	player_1 = new Player(6);
	player_1->Set_img(L"GameMedia\\027_00.png");
	player_2 = new Player(3);
	player_2->Set_img(L"GameMedia\\027_00.png");
	player_3 = new Player(2);
	player_3->Set_img(L"GameMedia\\027_00.png");
	player_4 = new Player(4);
	player_4->Set_img(L"GameMedia\\027_00.png");

	//添加玩家角色到玩家list
	Human_Players[1] = player_1;
	Human_Players[2] = player_2;
	

	//添加玩家角色到全局Map
	players[1] = player_1;
	players[2] = player_2;
	

	//创建AI
	ai_player_1 = new AI_Player(6);
	ai_player_1->Set_img(L"GameMedia\\010_00.png");
	ai_player_2 = new AI_Player(3);
	ai_player_2->Set_img(L"GameMedia\\010_00.png");
	ai_player_3 = new AI_Player(2);
	ai_player_3->Set_img(L"GameMedia\\010_00.png");
	ai_player_4 = new AI_Player(4);
	ai_player_4->Set_img(L"GameMedia\\010_00.png");

	//添加AI到全局MAP
	players[5] = ai_player_1;
	players[6] = ai_player_2;


	//添加AI到AI list
	AI_Players[1] = ai_player_1;
	AI_Players[2] = ai_player_2;
	

	

	//创建怪物
	monster = new Monster();
	monster->Set_img(L"GameMedia\\001_00.png");

	//create font
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("微软雅黑"), &g_pFont)))
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

	

	//玩家的行为
	if (current_turn == PLAYER_TURN && !Human_Players.empty() && Human_Players.size() != 0)
	{
		
		if (g_pDInput->IsKeyDown(DIK_UPARROW))
		{
			//如果步数有剩余
			if(hp_iter->second->current_step > 0)

				//如果按下左shift则可以推动其他玩家
				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Up())
					{
						hp_iter->second->Move_Up(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//改变角色的动画起始和结束帧
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

							//改变角色的动画起始和结束帧
							hp_iter->second->startframe = 12;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
		}

		if (g_pDInput->IsKeyDown(DIK_DOWNARROW))
		{
			//如果步数有剩余
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Down())
					{
						hp_iter->second->Move_Down(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//改变角色的动画起始和结束帧
							hp_iter->second->startframe = 0;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}
				}
				else
					if (hp_iter->second->Move_Down(false))
					{
						if (!hp_iter->second->Is_InBlood()) //如果不在血池里，则减少步数，在血池里不减少步数
						{
							hp_iter->second->current_step -= 1;

							//改变角色的动画起始和结束帧
							hp_iter->second->startframe = 0;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}

		}

		if (g_pDInput->IsKeyDown(DIK_LEFTARROW))
		{
			//如果步数有剩余
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Left())
					{
						hp_iter->second->Move_Left(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//改变角色的动画起始和结束帧
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

							//改变角色的动画起始和结束帧
							hp_iter->second->startframe = 4;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}


					}

		}

		if (g_pDInput->IsKeyDown(DIK_RIGHTARROW))
		{
			//如果步数有剩余
			if (hp_iter->second->current_step > 0)

				if (g_pDInput->IsKeyDown(DIK_LSHIFT))
				{
					if (hp_iter->second->Push_Right())
					{
						hp_iter->second->Move_Right(false);

						if (!hp_iter->second->Is_InBlood())
						{
							hp_iter->second->current_step -= 1;

							//改变角色的动画起始和结束帧
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

							//改变角色的动画起始和结束帧
							hp_iter->second->startframe = 8;
							hp_iter->second->endframe = hp_iter->second->startframe + 3;
						}
					}

		}

		//确认当前角色位置，并继续
		if (g_pDInput->IsKeyDown(DIK_SPACE))
		{
			if(!hp_iter->second->Is_OverPlayer() && !hp_iter->second->Is_InBlood())//如果当前角色没有和其他角色重叠并且不在血池中，则继续
				if (hp_iter == --Human_Players.end())
				{
					current_turn = AI_TURN;
					
				}
				else
				{
					++hp_iter;

				}
		}

		//如果走错了，按C恢复位置
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

	//AI的行为
	if (current_turn == AI_TURN)
	{
		//如果AI都死了直接进入怪物的回合
		if (!AI_Players.empty() && AI_Players.size() != 0)
		{
			ai_iter->second->Move_Up(false);
			ai_iter->second->current_step -= 1;

			if (ai_iter == --AI_Players.end())
			{
				current_turn = MONSTER_TURN;
				//先获取行动步数
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
			//先获取行动步数
			monster->current_step = 0;
			monster->Get_CurrentStep();
		}
	}

	//怪物的行为
	if (current_turn == MONSTER_TURN)
	{
		
	
		switch (monster->step)
		{
		//如果抽到杀人卡
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

	
	//删除出界的石头
	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end();)
	{
		if (iter->second->out_of_map || WALL[iter->second->world_Y][iter->second->world_X] == 1)
			stones.erase(iter++);
		else
			++iter;
	}

	//从全局玩家map中删除已死亡的角色
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end();)
	{
		if (iter->second->killed)
			players.erase(iter++);
		else
			++iter;
	}

	//从人类玩家map中删除已死亡角色
	for (map<int, Player*>::iterator iter = Human_Players.begin(); iter != Human_Players.end();)
	{
		if (iter->second->killed)
			Human_Players.erase(iter++);
		else
			++iter;
	}

	//从AI玩家map中删除已死亡角色
	for (map<int, Player*>::iterator iter = AI_Players.begin(); iter != AI_Players.end();)
	{
		if (iter->second->killed)
			AI_Players.erase(iter++);
		else
			++iter;
	}

	//如果人类玩家数为0，则游戏结束
	if (Human_Players.empty() || Human_Players.size() == 0)
	{
		game_over = true;
		g_currentGUI = GUI_START_SCREEN;
		g_sound_bgm.Stop();
	}

	//如果是第二局，则添加剩余的角色
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


	//重新指向起点，准备下一次游戏循环
	if (current_rounds != rounds)
	{
		//根据回合数确定角色的移动步数
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



