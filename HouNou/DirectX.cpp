//-----------------------------------������˵����----------------------------------------------
//
//------------------------------------------------------------------------------------------------

#include "DirectX.h"


LPDIRECT3DDEVICE9					g_pd3dDevice = NULL;				//Direct3D�豸����
LPD3DXFONT							g_pTextFPS = NULL;    //����COM�ӿ�
LPD3DXFONT							g_pTextAdaperName = NULL;  // �Կ���Ϣ��2D�ı�
LPD3DXFONT							g_pTextHelper = NULL;  // ������Ϣ��2D�ı�
LPD3DXFONT							g_pTextInfor = NULL;  // ������Ϣ��2D�ı�
float								g_FPS = 0.0f;       //һ�������͵ı���������֡����
wchar_t								g_strFPS[50] = { 0 };    //����֡���ʵ��ַ�����
wchar_t								g_strAdapterName[60] = { 0 };   //�����Կ����Ƶ��ַ�����

bool								g_LMBDown = false;      // GUI�е����״̬��Ϣ���������Ƿ��µı�ʶ
int									g_MouseX = 0, g_MouseY = 0;      //�洢����������������

Timer *timer_1;
Timer *timer_2;

MultiGame* multi_game;

LPDIRECT3DSURFACE9 backbuffer = NULL;

//-----------------------------------��Direct3D_Init( )������----------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd, HINSTANCE hInstance)
{

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 2;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;


	//��ȡ�Կ���Ϣ��g_strAdapterName�У������Կ�����֮ǰ���ϡ���ǰ�Կ��ͺţ����ַ���
	wchar_t TempName[60] = L"��ǰ�Կ��ͺţ�";   //����һ����ʱ�ַ������ҷ����˰�"��ǰ�Կ��ͺţ�"�ַ����������ǵ�Ŀ���ַ�����
	D3DADAPTER_IDENTIFIER9 Adapter;  //����һ��D3DADAPTER_IDENTIFIER9�ṹ�壬���ڴ洢�Կ���Ϣ
	pD3D->GetAdapterIdentifier(0, 0, &Adapter);//����GetAdapterIdentifier����ȡ�Կ���Ϣ
	int len = MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, NULL, 0);//�Կ����������Ѿ���Adapter.Description���ˣ�������Ϊchar���ͣ�����Ҫ����תΪwchar_t����
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//�ⲽ������ɺ�g_strAdapterName�о�Ϊ��ǰ���ǵ��Կ���������wchar_t���ַ�����
	wcscat_s(TempName, g_strAdapterName);//�ѵ�ǰ���ǵ��Կ����ӵ�����ǰ�Կ��ͺţ����ַ������棬�������TempName��
	wcscpy_s(g_strAdapterName, TempName);//��TempName�еĽ��������ȫ�ֱ���g_strAdapterName�У��󹦸��~

	
	

	//get a pointer to the back buffer surface
	g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	if (!(S_OK == Objects_Init())) return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

		return S_OK;
}


//-----------------------------------��Object_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init()
{
	//��������
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"��������", &g_pTextAdaperName);
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"΢���ź�", &g_pTextHelper);
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"����", &g_pTextInfor);


	//���������������
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	GUI_Init();

	//Game_Init();

	timer_1 = new Timer(200);

	return S_OK;
}





//-----------------------------------��Direct3D_Update( )������--------------------------------
//	���������Ǽ�ʱ��Ⱦ���뵫����Ҫ��ʱ���õģ��簴���������ĸ��ģ�����������
//--------------------------------------------------------------------------------------------------
void Direct3D_Update(HWND hwnd, FLOAT fTimeDelta)
{
	if(!game_over)
		Game_Update(hwnd);

	if (g_currentGUI == MULTI_GAME_RUN)
	{
		multi_game->Game_Update(hwnd);
	}

	if (!g_pd3dDevice)
		return;
	if(timer_1->TimeOut())
		g_pDInput->GetInput();

	//�ڶ�����Ϸ�����ȡ���룬��������IP��ַ
	if (g_currentGUI == GUI_MULTI_SCREEN)
	{
		GUI_InputUpdate(g_MultiGUI);
	}

	//�������Ӳ�ѡ��ģʽ
	if (g_currentGUI == GUI_MULTI_READY_SCREEN && multi_game->model == -1)
	{
		char* CStr;
		CStr = (char*)malloc((wcslen(dynamicText_buffer) + 1)*sizeof(char));
		size_t converted = 0;
		wcstombs_s(&converted, CStr, wcslen(dynamicText_buffer) + 1, dynamicText_buffer, _TRUNCATE);
		
		multi_game->model = multi_game->GetConnection(CStr);

		if (multi_game->model == -1)
		{
			g_MultiGUI->UpdateDynamicText(g_MultiGUI->GetDynamicTextId()
				, L"���ӳ���", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
		}
		if (multi_game->model == 0)
		{
			g_MultiGUI->UpdateDynamicText(g_MultiGUI->GetDynamicTextId()
				, L"�Կͻ���ģʽ�������ȴ���������ʼ��Ϸ", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
		}
		if (multi_game->model == 1)
		{
			g_MultiGUI->UpdateDynamicText(g_MultiGUI->GetDynamicTextId()
				, L"�Է�����ģʽ�������ȴ��ͻ�������", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
			timer_2 = new Timer(30000);
		} 

	}

	//����Կͻ���ģʽ����
	if (g_currentGUI == GUI_MULTI_READY_SCREEN && multi_game->model == 0)
	{
		if ( multi_game->client_receive())
		{
			char t[2] = {0};
			if (multi_game->buff[0] == 's')
			{
				
				t[0] = multi_game->buff[1];
				multi_game->total = atoi(t);

				multi_game->Game_Init();
				g_currentGUI = MULTI_GAME_RUN;
			}
			t[0] = '*';
			
			multi_game->client_send(t);
		}
	}

	//����Է�����ģʽ����
	if (g_currentGUI == GUI_MULTI_READY_SCREEN && multi_game->model == 1)
	{
		multi_game->ProcessLink();
		if (multi_game->total == 8 || timer_2->TimeOut())
		{
			multi_game->Game_Init();
			g_currentGUI = MULTI_GAME_RUN;
		}
	}



}



//-----------------------------------��Direct3D_Render( )������-------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd, FLOAT fTimeDelta)
{
	//--------------------------------------------------------------------------------------
	//��������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);

	//--------------------------------------------------------------------------------------
	//��ʼ����
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // ��ʼ����

	//--------------------------------------------------------------------------------------
	//��ʽ����
	//--------------------------------------------------------------------------------------


	//�������ȾGUIϵͳ


	switch (g_currentGUI)
	{
	case GUI_MAIN_SCREEN:
		ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GUI_START_SCREEN:
		ProcessGUI(g_StartGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GUI_MULTI_SCREEN:
	case GUI_MULTI_READY_SCREEN:
		ProcessGUI(g_MultiGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case  MULTI_GAME_RUN:
		multi_game->Game_Render(hwnd);
		break;
	case GUI_OPTION_SCREEN:
		ProcessGUI(g_OptionGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GUI_HELP_SCREEN:
		ProcessGUI(g_HelpGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GAME_WIN:
		ProcessGUI(g_WinGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GAME_LOSE:
		ProcessGUI(g_LoseGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	case GAME_RUN:
		Game_Render(hwnd);
		break;
	default:
		ProcessGUI(g_MainGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
		break;
	}

	//-----------------------------������������Ϣ��-----------------------------
	HelpText_Render(hwnd);
	

	//--------------------------------------------------------------------------------------
	//��������
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // ��������

	//--------------------------------------------------------------------------------------
	//��ʾ��ת
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // ��ת����ʾ

}


//-----------------------------------��HelpText_Render( )������-------------------------------
//	��������װ�˰�����Ϣ�ĺ���
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//�ڴ������ϽǴ�����ʾÿ��֡��
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0, 239, 136, 255));

	//��ʾ�Կ�������
	g_pTextAdaperName->DrawText(NULL, g_strAdapterName, -1, &formatRect,
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
}


//-----------------------------------��Get_FPS( )������------------------------------------------
//	���������ڼ���ÿ��֡���ʵ�һ������
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{

	//�����ĸ���̬����
	static float  fps = 0; //������Ҫ�����FPSֵ
	static int    frameCount = 0;//֡��
	static float  currentTime = 0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//����ʱ��

	frameCount++;//ÿ����һ��Get_FPS()������֡������1
	currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

									   //�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if (currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount / (currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		frameCount = 0;//������֡��frameCountֵ����
	}

	return fps;
}



//-----------------------------------��Direct3D_CleanUp( )������--------------------------------
//	��������Direct3D����Դ���������ͷ�COM�ӿڶ���
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextFPS);
	SAFE_RELEASE(g_pd3dDevice);

	GUI_CleanUp();
	Game_Clean();
}