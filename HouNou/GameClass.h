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

	//��Ϸ��ʼ��
	virtual bool Game_Init();
	//��Ϸ�������
	virtual void Game_Update(HWND window);
	//��Ϸ������Ⱦ
	virtual void Game_Render(HWND hwnd);
	//��ʼ��ʯͷ
	virtual void Stones_Init();

	//sprites
	Monster* monster;
	map<int, Sprite*> stones;
	Sprite* cursor; //ָ��ǰ��ɫ��ָ��

	int SCREENW;
	int SCREENH;

	int MAX_STONE_NUM;  //���ʯͷ��


	int GAMEPANEL_WIDTH;	//��Ϸ���򳤿�
	int GAMEPANEL_HEIGHT;

	int TILEWIDTH;	//�鳤��
	int TILEHEIGHT;

	int MAPWIDTH;    //��Ϸ���糤��
	int MAPHEIGHT;

	int WINDOWWIDTH;	//���ڳ���
	int WINDOWHEIGHT;

	//�غ���
	int rounds;
	//��ǰ�غ���
	int current_rounds;

	//��ǰ�غ�
	int current_turn;

	//������
	int escape_num;

	//��Ϸ״̬
	bool	game_pause;         //��Ϸ�Ƿ���ͣ
	bool	game_over;			//��Ϸ�Ƿ����

	int GAMEWORLDWIDTH;
	int GAMEWORLDHEIGHT;

	int MAPDATA[20*13];
	int WALL[11][16];			//��Ϸ��巶Χ����


	LPDIRECT3DSURFACE9 gameworld; //��Ϸ�������
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
	bool client_receive();//�ͻ��˽�����Ϣ�̺߳���

	bool server_send(LPVOID lp, char* text);//����˷�����Ϣ�̺߳���
	bool server_receive(LPVOID lp);//����˽���

	int GetConnection(char* ip);//���Խ�������

	bool StartServer(); //���÷�����ģʽ
	bool MultiGame::ProcessLink();//��Ϊ��������ʱ����������

	SOCKET acceptSocket;	//����socket �����������������Ӻͷ���
	char name[50];//׼��һ������Ϊ50���ַ����飬��������Լ�������,�������̹߳��õı���
	int flag;//������ģʽʱ�ж��ǲ����Լ�����ȥ����Ϣ0���ǣ�1��,�������̹߳��õı���
	char add[50];//׼��һ������Ϊ50���ַ����飬������ŶԷ���ip��ַ
	int model;//0Ϊ�ͻ�����1Ϊ������
	char buff[1024]; //���ڷ������ݵ�BUFF
	struct sockaddr_in serv;	//�������÷�������ַ�Ͷ˿ڱ���
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

	int player_num;  //��ǰ��ұ�ʶ
	int role_num;    //��ǰ��ҵĽ�ɫ��ʶ

	u_long ul;

	//�׽�������
	client c[8];

	//�¼�����
	WSAEVENT eventarray[8];

	//Ŀǰ�׽��ָ���
	int total;

};