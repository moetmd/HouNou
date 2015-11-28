#pragma once

class Sprite
{
public:

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

	Sprite();
	~Sprite();

	bool Set_img(LPCTSTR filename);

	virtual bool Move_Up();
	virtual bool Move_Down();
	virtual bool Move_Left();
	virtual bool Move_Right();

	float Get_RealPosX();
	float Get_RealPosY();

	void Draw();

private:
	float real_X;
	float real_Y;
};




//�����
class Player :public Sprite
{
public:
	int day_step;
	int night_step;
	int current_step;

	virtual bool Move_Up();
	virtual bool Move_Down();
	virtual bool Move_Left();
	virtual bool Move_Right();
};


//������
class Monster :public Sprite
{
public:
	int Get_CurrentStep();
};



//ʯͷ��
class Stone :public Sprite
{
public:
	void Draw();
	virtual bool Move_Up();
	virtual bool Move_Down();
	virtual bool Move_Left();
	virtual bool Move_Right();
};
