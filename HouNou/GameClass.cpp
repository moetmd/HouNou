#include "DirectX.h"




GameClass::GameClass()
{
	SCREENW = WINDOW_WIDTH;
	SCREENH = WINDOW_HEIGHT;

	rounds = 1;
	current_rounds = 1;

	TILEWIDTH = 64;   //块长宽
	TILEHEIGHT = 64;

	MAPWIDTH = 20;    //游戏世界长宽
	MAPHEIGHT = 13;

	GAMEPANEL_WIDTH = 16;  //游戏区域长宽
	GAMEPANEL_HEIGHT = 11;

	WINDOWWIDTH = (SCREENW / TILEWIDTH) * TILEWIDTH;
	WINDOWHEIGHT = (SCREENH / TILEHEIGHT) * TILEHEIGHT;

	GAMEWORLDWIDTH = TILEWIDTH * MAPWIDTH;
	GAMEWORLDHEIGHT = TILEHEIGHT * MAPHEIGHT;

	MAX_STONE_NUM = 11;

	ScrollX = 0;
	ScrollY = 0;
	SpeedX = 0;
	SpeedY = 0;
	start = 0;

	escape_num = 0;

	game_pause = true;         //游戏是否暂停
	game_over = true;			//游戏是否结束
	
	spriteobj = NULL;

	gameworld = NULL;

	int temp[] = {
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
	memcpy(MAPDATA, temp, sizeof(temp));
//	delete[] temp;

	int temp_1[11][16] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1,-1,-1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1,-1,-1 },
		{ 0, 0, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,-1,-1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 0, 0,-1 },
		{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ -1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
		{ -1,-1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0 },
		{ -1,-1,-1, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 },
		{ -1,-1,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	memcpy(WALL, temp_1, sizeof(temp_1));
//	delete[] temp_1;

}


GameClass::~GameClass()
{
	if (gameworld)
		gameworld->Release();

	SAFE_RELEASE(g_pFont);

	SAFE_DELETE(monster);

	g_sound_bgm.Shutdown();

	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}

//	delete[] MAPDATA;
//	delete[] WALL;
}

bool GameClass::Game_Init()
{
	//设置游戏状态为 非结束
	game_over = false;
	game_pause = false;

	//初始化游戏回合数
	rounds = 1;

	//create sprite object
	D3DXCreateSprite(g_pd3dDevice, &spriteobj);

	//创建指针
	cursor = new Sprite();
	cursor->Set_img(L"GameMedia\\cursor.tga");
	cursor->endframe = 1;
	cursor->columns = 2;
	cursor->width = cursor->height = 17;

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

	current_rounds = 1;

	return true;
}

void GameClass::Game_Update(HWND window)
{

}

void GameClass::Game_Render(HWND hwnd)
{

}


void GameClass::Stones_Init()
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