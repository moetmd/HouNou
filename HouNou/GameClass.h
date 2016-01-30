#pragma once

#define MONSTER_TURN 1
#define PLAYER_TURN 2
#define AI_TURN 3

#define  UP 1
#define  DOWN 2
#define  LEFT 3
#define  RIGHT 4


class GameClass
{
public:
	GameClass();
	~GameClass();

	//游戏初始化
	virtual bool Game_Init();
	//游戏画面更新
	virtual void Game_Update(HWND window);
	//游戏画面渲染
	virtual void Game_Render(HWND hwnd);
	//初始化石头
	virtual void Stones_Init();

	//sprites
	Monster* monster;
	map<int, Sprite*> stones;
	Sprite* cursor; //指向当前角色的指针

	int SCREENW;
	int SCREENH;

	int MAX_STONE_NUM;  //最大石头数


	int GAMEPANEL_WIDTH;	//游戏区域长宽
	int GAMEPANEL_HEIGHT;

	int TILEWIDTH;	//块长宽
	int TILEHEIGHT;

	int MAPWIDTH;    //游戏世界长宽
	int MAPHEIGHT;

	int WINDOWWIDTH;	//窗口长宽
	int WINDOWHEIGHT;

	//回合数
	int rounds;
	//当前回合数
	int current_rounds;

	//当前回合
	int current_turn;

	//逃脱数
	int escape_num;

	//游戏状态
	bool	game_pause;         //游戏是否暂停
	bool	game_over;			//游戏是否结束

	int GAMEWORLDWIDTH;
	int GAMEWORLDHEIGHT;

	int MAPDATA[20*13];
	int WALL[11][16];			//游戏面板范围数据


	LPDIRECT3DSURFACE9 gameworld; //游戏世界相关
	long start;
	float SpeedX, SpeedY;
	float ScrollX, ScrollY;

	//font
	LPD3DXFONT g_pFont;

	//sound
	CDirectMusic g_sound_bgm;
};

class MultiGame :public GameClass
{
public:

	MultiGame();
	~MultiGame();

	map<int, map<int, Player*>> AllPlayer;


	bool Game_Init();
	void Game_Update(HWND window);
	void Game_Client_Update(HWND window);

	void Game_Render(HWND hwnd);

	bool client_send(char* text);
	bool client_receive();//客户端接收信息线程函数

	bool server_send(LPVOID lp, char* text);//服务端发送信息线程函数
	bool server_receive(LPVOID lp);//服务端接收

	int GetConnection(char* ip);//尝试建立连接

	bool StartServer(); //启用服务器模式
	bool MultiGame::ProcessLink();//作为服务器端时，处理连接

	SOCKET acceptSocket;	//定义socket 变量，用来建立连接和服务
	char name[50];//准备一个长度为50的字符数组，用来存放自己的名字,主，副线程公用的变量
	int flag;//服务器模式时判断是不是自己发出去的信息0不是，1是,主，副线程公用的变量
	char add[50];//准备一个长度为50的字符数组，用来存放对方的ip地址
	int model;//0为客户机，1为服务器
	char buff[1024]; //用于发送数据的BUFF
	struct sockaddr_in serv;	//定义设置服务器地址和端口变量
	sockaddr_in sin;
	sockaddr_in xx;
	int nxx;

	int ret;

	struct client
	{
		int id;
		SOCKET s;
		sockaddr_in sin;
	};

	int player_num;  //当前玩家标识
	int role_num;    //当前玩家的角色标识

	u_long ul;

	//套接字数组
	client c[8];

	//事件数组
	WSAEVENT eventarray[8];

	//目前套接字个数
	int total;

};