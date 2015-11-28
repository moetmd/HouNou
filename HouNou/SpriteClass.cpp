#include "DirectX.h"


Sprite::Sprite()
{
	world_X = 0;
	world_Y = 0;

	frame = 0;
	columns = 1;
	width = 0;
	height = 0;
	foot = 0;
	scaling = 1.0f;
	rotation = 0;
	startframe = 0;
	endframe = 0;
	starttime = 0;
	delay = 100;
	direction = 1;
	velx, vely = 0.0f;
	color = D3DCOLOR_XRGB(255, 255, 255);
	visibal = true;

	LPDIRECT3DTEXTURE9 img = NULL;
}

Sprite::~Sprite()
{
	if(this->img)
	this->img->Release();
}

bool Sprite::Set_img(LPCTSTR filename)
{
	this->img = LoadTexture(filename);
	if (!img)
		return false;
}

bool Sprite::Move_Up()
{
	this->world_Y -= 1;
	return true;
}

bool Sprite::Move_Down()
{
	this->world_Y += 1;
	return true;
}

bool Sprite::Move_Left()
{
	this->world_X -= 1;
	return true;
}

bool Sprite::Move_Right()
{
	this->world_X += 1;
	return true;
}

float Sprite::Get_RealPosX()
{
	this->real_X = TILEWIDTH * (this->world_X + 1) + TILEWIDTH / 2 - this->width / 2;
	return this->real_X;
}

float Sprite::Get_RealPosY()
{
	this->real_Y = TILEHEIGHT * (this->world_Y + 1) + TILEHEIGHT / 2 - (this->height - foot);
	return this->real_Y;
}

void Sprite::Draw()
{
	Sprite_Animate(this->frame, this->startframe, this->endframe,
		this->direction, this->starttime, this->delay);
	Sprite_Transform_Draw(this->img, this->Get_RealPosX(), this->Get_RealPosY(), this->width,
		this->height, this->frame, this->columns, 0, 1.0f, this->color);
}











