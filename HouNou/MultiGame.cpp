#include "DirectX.h"


MultiGame::MultiGame()
{
	flag = -1;
	model = -1;
	ul = 1;
	total = 0;

	player_num = 0;
	role_num = 0;
}

MultiGame::~MultiGame()
{
	::closesocket(c[0].s);
	::WSACleanup();
}

bool MultiGame::Game_Init()
{
	GameClass::Game_Init();

	//2��4λ���
	if (total <= 3)
	{
		for (int i = 0; i <= total; ++i)
		{
			map<int, Player*> temp;
			for (int j = 0; j < 2; ++j)
			{
				Player* player = new Player();
				player->Set_img(L"GameMedia\\010_00.png");
				player->current_step -= 1;

				temp[j] = player;
			}
			AllPlayer[i] = temp;
		}
	}

	//5��7λ���
	if (total >= 4)
	{
		for (int i = 0; i <= total; ++i)
		{
			map<int, Player*> temp;
			for (int j = 0; j < 2; ++j)
			{
				Player* player = new Player();
				player->Set_img(L"GameMedia\\010_00.png");
				player->current_step -= 1;
				temp[j] = player;
			}
			AllPlayer[i] = temp;
		}
	}

	current_turn = PLAYER_TURN;

	return true;
}


void MultiGame::Game_Update(HWND window)
{
	//make sure the Direct3D device is valid
	if (!g_pd3dDevice)
		return;
	/*
		���㿪ʼ
	*/

	//ɾ�������ʯͷ
	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end();)
	{
		if (iter->second->out_of_map || WALL[iter->second->world_Y][iter->second->world_X] == 1)
			stones.erase(iter++);
		else
			++iter;
	}

	//�����map��ɾ���������Ľ�ɫ
	for (map<int, map<int, Player*>>::iterator iter = AllPlayer.begin(); iter != AllPlayer.end();)
	{
		//���ĳ�����������
		if (iter->second.empty() || iter->second.size() == 0)
		{
			AllPlayer.erase(++iter);
		}

		for (map<int, Player*>::iterator iter_in = iter->second.begin(); iter_in != iter->second.end();)
		{
			if (iter_in->second->killed)
				iter->second.erase(iter_in++);
			else
				++iter_in;
		}
		++iter;
	}

	//������������Ϊ0������Ϸ����
	if (AllPlayer.empty() || AllPlayer.size() == 0)
	{
		game_over = true;
		g_currentGUI = GAME_LOSE;
		g_sound_bgm.Stop();
	}

	if (escape_num >= 3)
	{
		game_over = true;
		g_currentGUI = GAME_WIN;
		g_sound_bgm.Stop();
	}

	//����ǵڶ��֣������ʣ��Ľ�ɫ
// 	if (rounds == 2)
// 	{
// 		players[3] = player_3;
// 		players[4] = player_4;
// 
// 		players[7] = ai_player_3;
// 		players[8] = ai_player_4;
// 
// 		Human_Players[3] = player_3;
// 		Human_Players[4] = player_4;
// 
// 		AI_Players[3] = ai_player_3;
// 		AI_Players[4] = ai_player_4;
// 
// 	}

	//����ָ����㣬׼����һ����Ϸѭ��
	if (current_rounds != rounds)
	{
		//���ݻغ���ȷ����ɫ���ƶ�����
		for (map<int, map<int, Player*>>::iterator iter = AllPlayer.begin(); iter != AllPlayer.end(); ++iter)
		{
			for (map<int, Player*>::iterator iter_in = iter->second.begin(); iter_in != iter->second.end(); ++iter_in)
			{
				rounds % 2 == 0 ?
					iter_in->second->current_step = iter_in->second->night_step :
					iter_in->second->current_step = iter_in->second->day_step;
			}
			
		}

// 		hp_iter = Human_Players.begin();
// 		ai_iter = AI_Players.begin();
	}

	current_rounds = rounds;

	/*
		�������
	*/

	//update input devices
	g_pDInput->GetInput();

	char text[10];
	if (g_pDInput->IsKeyDown(DIK_UPARROW))
	{
		text[0] = '-';
		text[1] = '1';

	}

	if (g_pDInput->IsKeyDown(DIK_DOWNARROW))
	{

	}

	if (g_pDInput->IsKeyDown(DIK_LEFTARROW))
	{

	}

	if (g_pDInput->IsKeyDown(DIK_RIGHTARROW))
	{

	}





}

void MultiGame::Game_Render(HWND hwnd)
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

	

	for (map<int, map<int, Player*>>::iterator iter_1 = AllPlayer.begin();
			iter_1 != AllPlayer.end(); ++iter_1)
	{
		for (map<int, Player*>::iterator iter_2 = iter_1->second.begin();
				iter_2 != iter_1->second.end(); ++iter_2)
		{
			if (iter_2->second->visibal)
			{
				iter_2->second->Draw();

				formatRect.top = iter_2->second->Get_RealPosY();
				formatRect.left = iter_2->second->Get_RealPosX();
				wchar_t g_strStep[30] = { 0 };
				int charCount = swprintf_s(g_strStep, 20, _T("%2d"), ((Player*)(iter_2->second))->current_step);
				g_pFont->DrawText(NULL, g_strStep, charCount,
					&formatRect, DT_TOP | DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
			}
		}
	}

	cursor->Draw();

	spriteobj->End();

}

int MultiGame::GetConnection(char* ip)
{
	memcpy(add, ip, sizeof(add));
	WSADATA wsaData;
	int ret;
	if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)//�ĳ�if((ret=WSAStartup(0x101,&wsaData))!=0) Ҳ���ԣ������ֽڸ߽��ֽ�ָ��С�汾(�޶���)��,��λ�ֽ�ָ�����汾��
	{
		return -1;
	}
	
	acceptSocket = socket(AF_INET, SOCK_STREAM, 0);//�������� socket
	if (acceptSocket == INVALID_SOCKET)
	{
		return -1;
	}

	//���÷�������ַ��Ϣ
	serv.sin_family = AF_INET;//���ø�ʽΪAF_INET��ָ����Э��أ���ʹ��Socket�����ֻ����AF_INET
	serv.sin_port = htons(12248);//���ü����˿���12248�����������ӵķ������ļ����˿�(������������˰󶨵ļ����˿�һ��)
	serv.sin_addr.s_addr = inet_addr(add);//����Ҫ�󶨵ķ�������ip��ַ
	if (connect(acceptSocket, (SOCKADDR *)&serv, sizeof(serv)) == INVALID_SOCKET)//���������������TCP����
	{
		return 1;//�޷����ӶԷ�ip������model������Ϊ�����Զ���������ģʽ׼��
	}
	else
		return 0;
}

bool MultiGame::client_receive()
{
	int nrecv;
	nrecv = ::recv( *( (SOCKET *)(&acceptSocket) ), buff, 1024, 0);
	if (nrecv > 0)
	{
		buff[nrecv] = '\0';
	}
	else
	{
		return false;
	}

	if (nrecv == SOCKET_ERROR)	//���Ӵ���
	{
		return false;
	}

	return true;
}

bool MultiGame::client_send(char* text)
{
	memset(buff, 0, sizeof(buff));
	memcpy(buff, text, sizeof(text));

	int ret = -1;

	ret = send(acceptSocket, buff, sizeof(buff), 0);
	if (ret == SOCKET_ERROR)
		return false;

	return true;
}


bool MultiGame::server_receive(LPVOID lp)
{
	return true;
}

bool MultiGame::server_send(LPVOID lp, char* text)
{
	SOCKET *s = (SOCKET *)lp;

	int rett;
	memset(buff, 0, sizeof(buff));
	memcpy(buff, text, sizeof(text));
	flag = 1;


	rett = ::send(*s, buff, sizeof(buff), 0);//��������
	if (rett == SOCKET_ERROR)
	{
		return false;
	}
	
// 	//�ر�socket
// 	closesocket(*s);
	return true;
}

bool MultiGame::StartServer()
{
	c[0].s = socket(AF_INET, SOCK_STREAM, 0);

	//�����첽
	::ioctlsocket(c[0].s, FIONBIO, (u_long*)&ul);

	//���IP���˿�
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12248);
	c[0].sin = sin;

	//�󶨵�ַ�˿����׽���
	if (bind(c[0].s, (sockaddr *)&c[0].sin, sizeof(c[0].sin)) == SOCKET_ERROR)
	{
		return false;
		return 0;
	}

	//��������
	listen(c[0].s, 5);

	//printf("\n�������Ѿ�����,���Խ�������!\n");

	//�����¼�����
	WSAEVENT event = ::WSACreateEvent();

	//��Ӧ�¼����������׽��ֶ�Ӧ
	::WSAEventSelect(c[0].s, event, FD_ACCEPT | FD_CLOSE);

	//�����׽��ּ�������
	eventarray[total] = event;

	//������1
	total++;
	nxx = sizeof(xx);

	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (::connect(acceptSocket, (struct sockaddr *)&serv, sizeof(serv)) == INVALID_SOCKET)
	{
		return false;
	}

	flag = 1;
}

bool MultiGame::ProcessLink()
{
	//�����ж����ϵȴ�
	int n = ::WSAWaitForMultipleEvents(total, eventarray, false, WSA_INFINITE, FALSE);
	//��ÿ���������WSAWaitForMultipleEvents��ȷ��״̬
	n = n - WSA_WAIT_EVENT_0;//����ȴ�����������
	for (int i = n;i < total;i++)
	{
		n = ::WSAWaitForMultipleEvents(1, &eventarray[i], FALSE, 0, FALSE);
		if (n == WSA_WAIT_FAILED || n == WSA_WAIT_TIMEOUT)
		{
			continue;
		}
		else
		{
			//��ȡ�¼�
			WSANETWORKEVENTS event;
			//�鿴����ʲô�¼�
			::WSAEnumNetworkEvents(c[i].s, eventarray[i], &event);
			//���������¼�
			if (event.lNetworkEvents&FD_ACCEPT)
			{
				//���û��������
				if (event.iErrorCode[FD_ACCEPT_BIT] == 0)
				{
					//�������������
					if (total >= 6)
					{
						return false;
					}
					SOCKET s = ::accept(c[i].s, (sockaddr *)&xx, &nxx);
					WSAEVENT e = ::WSACreateEvent();
					//��Ӧ�׽��֡��¼�������
					::WSAEventSelect(s, e, FD_READ | FD_WRITE | FD_CLOSE);
					c[total].s = s;
					c[total].sin = xx;
					eventarray[total] = e;

					//֪ͨ������
// 					for (int x = 1;x < total;x++)
// 					{
// 						int nsend = ::send(c[x].s, buff, strlen(buff), 0);
// 					}
					total++;
				}
			}
			//����READ�¼�
			else if (event.lNetworkEvents&FD_READ)
			{
				//���û��������
				if (event.iErrorCode[FD_READ_BIT] == 0)
				{
					int nrecv = ::recv(c[i].s, buff, 1024, 0);
					if (nrecv > 0)
					{
						//�ַ�����'\0'��β
						buff[nrecv] = '\0';
						if (flag == 0)
						{

						}
						else
							flag = 0;
						for (int x = 1;x < total;x++)
						{
							//if (i != x)//�����͸�������
								int nsend = ::send(c[x].s, buff, strlen(buff), 0);
						}
					}
				}
			}
			//����Ͽ��¼�
			else if (event.lNetworkEvents&FD_CLOSE)
			{
				//���û��������
				//if(event.iErrorCode[FD_CLOSE_BIT]==0)
				{
					//�ر��׽���
					::closesocket(c[i].s);
					//�����׽��֡���������
					//ÿ���׽��֡�����ǰ��
					for (int j = i;j < total - 1;j++)
					{
						c[j].s = c[j + 1].s;
						c[j].sin = c[j + 1].sin;

						eventarray[j] = eventarray[j + 1];

					}
					total--;

					printf("����%s��%d�ſͻ��ѶϿ�!\n", inet_ntoa(c[i].sin.sin_addr), i);

// 					//֪ͨ������
// 					for (int x = 1;x < total;x++)
// 					{
// 						int nsend = ::send(c[x].s, info, strlen(info), 0);
// 					}
				}
			}
		}
	}
}

