#pragma once

class Sprite
{
public:
	Sprite();
	~Sprite();

	//在游戏场景中的位置
	int world_X;
	int world_Y;

	//帧数，列数
	int frame, columns;
	//帧宽，帧高
	int width, height;

	//帧底部到角色脚部高度
	int foot;

	//放大参数，旋转参数
	float scaling, rotation;

	//起始帧，结束帧
	int startframe, endframe;

	//帧动画开始时间，延迟
	int starttime, delay;

	//帧动画方向
	int direction;

	//
	float velx, vely;

	//颜色
	D3DCOLOR color;

	//是否可见
	bool visibal;

	//角色素材图
	LPDIRECT3DTEXTURE9 img;


	bool Set_img(LPCTSTR filename);

	void Move_Up();
	void Move_Down();
	void Move_left();
	void Move_Right();

	float Get_RealPosX();
	float Get_RealPosY();

	void Draw();

private:
	float real_X;
	float real_Y;
};





class Player :public Sprite
{
public:
	int day_step;
	int night_step;
	int current_step;
};

class Monster :public Sprite
{

};

class Stone :public Sprite
{
public:
	void Draw();
};
