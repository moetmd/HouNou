#pragma once

//������ͼ
void BuildGameWorld();

//���ƿ�
void DrawTile(LPDIRECT3DSURFACE9 source, int tilenum, int width, int height,
	int columns, LPDIRECT3DSURFACE9 dest, int destx, int desty);

//���ر���
LPDIRECT3DSURFACE9 LoadSurface(LPCTSTR filename);

//���Ʊ���
void DrawSurface(LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source);

//��ͼ����
void  ScrollScreen();

//ȡ��λͼ��С
D3DXVECTOR2 GetBitmapSize(LPCTSTR filename);

//���ز���
LPDIRECT3DTEXTURE9 LoadTexture(LPCTSTR filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));

//draw functions
//����Sprite֡
void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty,
	int framenum, int framew, int frameh, int columns);

//Sprite֡��������
void Sprite_Animate(int &frame, int startframe, int endframe,
	int direction, int &starttime, int delay);

//Sprite���ζ�������
void Sprite_Transform_Draw(LPDIRECT3DTEXTURE9 image, int x, int y, int width,
	int height, int frame, int columns, float rotation, float scaling, D3DCOLOR color);

//physics functions 
//��ײ���
int Collision(SPRITE sprite1, SPRITE sprite2);
bool CollisionD(SPRITE sprite1, SPRITE sprite2);