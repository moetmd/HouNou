#pragma once

class Sprite
{
public:

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

	//是否死亡
	bool killed;
	
	//出界
	bool out_of_map;

	//角色素材图
	LPDIRECT3DTEXTURE9 img;

	Sprite();
	~Sprite();

	bool Set_img(LPCTSTR filename);

	virtual bool Move_Up();
	virtual bool Move_Down();
	virtual bool Move_Left();
	virtual bool Move_Right();

	virtual bool Move_Up(bool);
	virtual bool Move_Down(bool);
	virtual bool Move_Left(bool);
	virtual bool Move_Right(bool);

	float Get_RealPosX();
	float Get_RealPosY();

	virtual void Draw();

private:
	float real_X;
	float real_Y;
};




//玩家类
class Player :public Sprite
{
public:
	int day_step;
	int night_step;
	int current_step;

	

	virtual bool Move_Up(bool);
	virtual bool Move_Down(bool);
	virtual bool Move_Left(bool);
	virtual bool Move_Right(bool);
};


//怪物类
class Monster :public Sprite
{
public:
	int face_to;
	int kill;
	int step;
	int current_step;
	bool Action();



	void Get_CurrentStep();

	bool Move();
	int Look_Around();

private:
	int Look_Up();
	int Look_Down();
	int Look_Left();
	int Look_Right();

	bool Move_Up();
	bool Move_Down();
	bool Move_Left();
	bool Move_Right();

	int Change_Direction(int A, int B, int C, int Face_to);
};



//石头类
class Stone :public Sprite
{
public:
	
	void Draw();
	bool Move_Up(bool);
	bool Move_Down(bool);
	bool Move_Left(bool);
	bool Move_Right(bool);
};
