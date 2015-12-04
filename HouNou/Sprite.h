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

	//�Ƿ�����
	bool killed;
	
	//����
	bool out_of_map;

	//��ɫ�ز�ͼ
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




//�����
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


//������
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



//ʯͷ��
class Stone :public Sprite
{
public:
	
	void Draw();
	bool Move_Up(bool);
	bool Move_Down(bool);
	bool Move_Left(bool);
	bool Move_Right(bool);
};
