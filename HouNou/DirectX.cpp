//-----------------------------------【程序说明】----------------------------------------------
//
//------------------------------------------------------------------------------------------------

#include "DirectX.h"


LPDIRECT3DDEVICE9					g_pd3dDevice = NULL;				//Direct3D设备对象
LPD3DXFONT							g_pTextFPS = NULL;    //字体COM接口
LPD3DXFONT							g_pTextAdaperName = NULL;  // 显卡信息的2D文本
LPD3DXFONT							g_pTextHelper = NULL;  // 帮助信息的2D文本
LPD3DXFONT							g_pTextInfor = NULL;  // 绘制信息的2D文本
float								g_FPS = 0.0f;       //一个浮点型的变量，代表帧速率
wchar_t								g_strFPS[50] = { 0 };    //包含帧速率的字符数组
wchar_t								g_strAdapterName[60] = { 0 };   //包含显卡名称的字符数组

bool								g_LMBDown = false;      // GUI中的鼠标状态信息，鼠标左键是否按下的标识
int									g_MouseX = 0, g_MouseY = 0;      //存储鼠标坐标的两个变量

Timer *timer_1;
Timer *timer_2;

MultiGame* multi_game;

LPDIRECT3DSURFACE9 backbuffer = NULL;

//-----------------------------------【Direct3D_Init( )函数】----------------------------------
//	描述：Direct3D初始化函数，进行Direct3D的初始化
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd, HINSTANCE hInstance)
{

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
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
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;


	//获取显卡信息到g_strAdapterName中，并在显卡名称之前加上“当前显卡型号：”字符串
	wchar_t TempName[60] = L"当前显卡型号：";   //定义一个临时字符串，且方便了把"当前显卡型号："字符串引入我们的目的字符串中
	D3DADAPTER_IDENTIFIER9 Adapter;  //定义一个D3DADAPTER_IDENTIFIER9结构体，用于存储显卡信息
	pD3D->GetAdapterIdentifier(0, 0, &Adapter);//调用GetAdapterIdentifier，获取显卡信息
	int len = MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, NULL, 0);//显卡名称现在已经在Adapter.Description中了，但是其为char类型，我们要将其转为wchar_t类型
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);//这步操作完成后，g_strAdapterName中就为当前我们的显卡类型名的wchar_t型字符串了
	wcscat_s(TempName, g_strAdapterName);//把当前我们的显卡名加到“当前显卡型号：”字符串后面，结果存在TempName中
	wcscpy_s(g_strAdapterName, TempName);//把TempName中的结果拷贝到全局变量g_strAdapterName中，大功告成~

	
	

	//get a pointer to the back buffer surface
	g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	if (!(S_OK == Objects_Init())) return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象的使命完成，我们将其释放掉

		return S_OK;
}


//-----------------------------------【Object_Init( )函数】--------------------------------------
//	描述：渲染资源初始化函数，在此函数中进行要被渲染的物体的资源的初始化
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init()
{
	//创建字体
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"华文中宋", &g_pTextAdaperName);
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"微软雅黑", &g_pTextHelper);
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"黑体", &g_pTextInfor);


	//设置纹理采样参数
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	GUI_Init();

	//Game_Init();

	timer_1 = new Timer(200);

	return S_OK;
}





//-----------------------------------【Direct3D_Update( )函数】--------------------------------
//	描述：不是即时渲染代码但是需要即时调用的，如按键后的坐标的更改，都放在这里
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

	//在多人游戏界面获取输入，用于输入IP地址
	if (g_currentGUI == GUI_MULTI_SCREEN)
	{
		GUI_InputUpdate(g_MultiGUI);
	}

	//尝试连接并选择模式
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
				, L"连接出错！", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
		}
		if (multi_game->model == 0)
		{
			g_MultiGUI->UpdateDynamicText(g_MultiGUI->GetDynamicTextId()
				, L"以客户端模式启动，等待服务器开始游戏", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
		}
		if (multi_game->model == 1)
		{
			g_MultiGUI->UpdateDynamicText(g_MultiGUI->GetDynamicTextId()
				, L"以服务器模式启动，等待客户端连接", 200, 200, D3DCOLOR_XRGB(80, 80, 80));
			timer_2 = new Timer(30000);
		} 

	}

	//如果以客户端模式启动
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

	//如果以服务器模式启动
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



//-----------------------------------【Direct3D_Render( )函数】-------------------------------
//	描述：使用Direct3D进行渲染
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd, FLOAT fTimeDelta)
{
	//--------------------------------------------------------------------------------------
	//清屏操作
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);

	//--------------------------------------------------------------------------------------
	//开始绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->BeginScene();                     // 开始绘制

	//--------------------------------------------------------------------------------------
	//正式绘制
	//--------------------------------------------------------------------------------------


	//处理和渲染GUI系统


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

	//-----------------------------【绘制文字信息】-----------------------------
	HelpText_Render(hwnd);
	

	//--------------------------------------------------------------------------------------
	//结束绘制
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->EndScene();                       // 结束绘制

	//--------------------------------------------------------------------------------------
	//显示翻转
	//--------------------------------------------------------------------------------------
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  // 翻转与显示

}


//-----------------------------------【HelpText_Render( )函数】-------------------------------
//	描述：封装了帮助信息的函数
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//在窗口右上角处，显示每秒帧数
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS());
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount, &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0, 239, 136, 255));

	//显示显卡类型名
	g_pTextAdaperName->DrawText(NULL, g_strAdapterName, -1, &formatRect,
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
}


//-----------------------------------【Get_FPS( )函数】------------------------------------------
//	描述：用于计算每秒帧速率的一个函数
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{

	//定义四个静态变量
	static float  fps = 0; //我们需要计算的FPS值
	static int    frameCount = 0;//帧数
	static float  currentTime = 0.0f;//当前时间
	static float  lastTime = 0.0f;//持续时间

	frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

									   //如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount / (currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		frameCount = 0;//将本次帧数frameCount值清零
	}

	return fps;
}



//-----------------------------------【Direct3D_CleanUp( )函数】--------------------------------
//	描述：对Direct3D的资源进行清理，释放COM接口对象
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextFPS);
	SAFE_RELEASE(g_pd3dDevice);

	GUI_CleanUp();
	Game_Clean();
}