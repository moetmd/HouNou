#pragma once

class Sprite
{
public:
	Sprite();
	~Sprite();

	//����Ϸ�����е�λ��
	int world_X;
	int world_Y;

	//֡��������
	int frame, columns;
	//֡��֡��
	int width, height;

	//֡�ײ�����ɫ�Ų��߶�
	int foot;

	//�Ŵ��������ת����
	float scaling, rotation;

	//��ʼ֡������֡
	int startframe, endframe;

	//֡������ʼʱ�䣬�ӳ�
	int starttime, delay;

	//֡��������
	int direction;

	//
	float velx, vely;

	//��ɫ
	D3DCOLOR color;

	//�Ƿ�ɼ�
	bool visibal;

	//��ɫ�ز�ͼ
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
