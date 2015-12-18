#include "DirectX.h"

//������������
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

Monster::Monster()
{
	width = height = 96;
	columns = 4;
	foot = 20;
	world_X = 0;
	world_Y = 0;
	face_to = 2;
	kill = 0;
}

//Monster��
void Monster::Get_CurrentStep()
{
	int steps[] = { 1, 2, 5, 7, 7, 8, 8, 10 };
	srand(timeGetTime());
	this->step = steps[rand() % 8];
}

//�ж�
// bool Monster::Action()
// {
// 	//�Ȼ�ȡ�ж�����
// 	int current_step = 0;
// 	int step = this->Get_CurrentStep();
// 	switch (step)
// 	{
// 		//����鵽ɱ�˿�
// 	case 1:
// 	case 2:
// 		while (kill < step && current_step <= 20) //��ɱ����С����Ҫɱ���� ����С��20 ʱһֱѭ��
// 		{
// 			this->face_to = this->Look_Around();
// 			if (this->Move())
// 			{
// 				current_step++;
// 				this->Draw();
// 			}
// 		}
// 		return true;
// 		break;
// 
// 	default:
/*		while (step > 0)*/
// 		{
// 			this->face_to = this->Look_Around();
// 			if (this->Move())
// 			{
// 				step--;
// 				this->Draw();
// 			}
// 		}
// 		return true;
// 		break;
// 	}
// 
// }

//�۲�
int Monster::Look_Around()
{
	switch (this->face_to)
	{
	case UP:
		return this->Change_Direction(this->Look_Left(), this->Look_Right(), this->Look_Up(), this->face_to);
		break;

	case DOWN:
		return this->Change_Direction(this->Look_Right(), this->Look_Left(), this->Look_Down(), this->face_to);
		break;

	case LEFT:
		return this->Change_Direction(this->Look_Down(), this->Look_Up(), this->Look_Left(), this->face_to);
		break;

	case RIGHT:
		return this->Change_Direction(this->Look_Up(), this->Look_Down(), this->Look_Right(), this->face_to);
		break;

	}
}

int Monster::Change_Direction(int A, int B, int C, int Face_to)
{
	if (A == 99)
	{
		if (B > C)
		{
			return Face_to;
		}
		else
			if (B < C)
			{
				Face_to += 1;
			}
			else
				return Face_to;
	}
	else
		if (B == 99)
		{
			if (A > C)
			{
				return Face_to;
			}
			else
				if (A < C)
				{
					Face_to -= 1;
				}
				else
					return Face_to;

		}
		else
			if (C == 99)
			{
				if (A > B)
				{
					Face_to += 1;
				}
				else
					if (A < B)
					{
						Face_to -= 1;
					}
					else
						return Face_to;
			}

	if (Face_to > 4)
	{
		Face_to = 1;
	}
	if (Face_to < 1)
	{
		Face_to = 4;
	}

	return Face_to;
}

//�ƶ�
bool Monster::Move()
{
	switch (this->face_to)
	{
	case UP:
		return this->Move_Up();
		break;
	case DOWN:
		return this->Move_Down();
		break;
	case LEFT:
		return this->Move_Left();
		break;
	case RIGHT:
		return this->Move_Right();
		break;
	}
}

//�۲�ĳ������
int Monster::Look_Up()
{
	int distance = GAMEPANEL_HEIGHT;

	//�ڹ������Ϸ���Ѱ�����
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_Y < this->world_Y && iter->second->world_X == this->world_X)
		{
			if (this->world_Y - iter->second->world_Y < distance)
			{
				distance = this->world_Y - iter->second->world_Y;
			}
		}
	}

	//���֮����ʯͷ
	if (distance != GAMEPANEL_HEIGHT)
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (iter->second->world_Y < this->world_Y && iter->second->world_X == this->world_X)
			{
				if (this->world_Y - iter->second->world_Y < distance)
				{
					return 99;
				}
			}
		}
	else
		return 99;

	return distance;
}

int Monster::Look_Down()
{
	int distance = GAMEPANEL_HEIGHT;

	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_Y > this->world_Y && iter->second->world_X == this->world_X)
		{
			if (iter->second->world_Y - this->world_Y < distance)
			{
				distance = iter->second->world_Y - this->world_Y;
			}
		}
	}

	//���֮����ʯͷ
	if (distance != GAMEPANEL_HEIGHT)
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (iter->second->world_Y > this->world_Y && iter->second->world_X == this->world_X)
			{
				if (iter->second->world_Y - this->world_Y < distance)
				{
					return 99;
				}
			}
		}
	else
		return 99;

	return distance;
}

int Monster::Look_Left()
{
	int distance = GAMEPANEL_WIDTH;

	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if ( iter->second->world_X < this->world_X && iter->second->world_Y == this->world_Y)
		{
			if (this->world_X - iter->second->world_X < distance)
			{
				distance = this->world_X - iter->second->world_X;
			}
		}
	}

	//���֮����ʯͷ
	if (distance != GAMEPANEL_WIDTH)
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (iter->second->world_X < this->world_X && iter->second->world_Y == this->world_Y)
			{
				if (this->world_X - iter->second->world_X < distance)
				{
					return 99;
				}
			}
		}
	else
		return 99;

	return distance;
}

int Monster::Look_Right()
{
	int distance = GAMEPANEL_WIDTH;

	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X > this->world_X && iter->second->world_Y == this->world_Y)
		{
			if (iter->second->world_X - this->world_X < distance)
			{
				distance = iter->second->world_X - this->world_X;
			}
		}
	}

	if (distance != GAMEPANEL_WIDTH)
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (iter->second->world_X > this->world_X && iter->second->world_Y == this->world_Y)
			{
				if (iter->second->world_X - this->world_X  < distance)
				{
					return 99;
				}
			}
		}
	else
		return 99;


	return distance;
}


//��ĳ�������ƶ��ƶ�
bool Monster::Move_Up()
{
	//�߽���
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//���ǰ������ң�����ұ��ж�Ϊ��
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y - 1 == iter->second->world_Y && 
			this->world_X == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_Y -= 1;
			this->kill += 1;
			return true;
		}
	}

	//��ʯͷ �� ʯͷ��������
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_Y - 1 == iter->second->world_Y &&
			this->world_X == iter->second->world_X)
		{
			iter->second->Move_Up(true);
			this->world_Y -= 1;
			return true;
		}
	}


	//����������Ѫ��
	//�ж���һ���Ƿ���Ѫ��
	int x = this->world_X;
	int y = this->world_Y;
	bool flag = false;
	while (true)
	{
		if (WALL[y - 1][x] == 1)
		{
			y -= 1;
			flag = true;
		}
		else
			break;
	}

	if (flag)//�����һ����Ѫ�صĻ�
	{
		//�ж�Ѫ�����Ƿ���ʯͷ
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (
				x == iter->second->world_X
				&& y - 1 == iter->second->world_Y
				)
			{
				iter->second->Move_Up(true);
				this->world_X = x;
				this->world_Y = y - 1;
				return true;
			}
		}

		//�ж�Ѫ�����Ƿ������
		for (iter = players.begin(); iter != players.end(); ++iter)
		{
			if (
				x == iter->second->world_X
				&& y - 1 == iter->second->world_Y
				)
			{
				iter->second->killed = true;
				this->world_X = x;
				this->world_Y = y - 1;
				this->kill += 1;
				return true;
			}
		}
	}

	//�����һ���ǿյ�
	this->world_Y -= 1;
	return true;

}

bool Monster::Move_Down()
{
	//�߽���
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//���ǰ������ң�����ұ��ж�Ϊ��
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y + 1 == iter->second->world_Y &&
			this->world_X == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_Y += 1;
			this->kill += 1;
			return true;
		}
	}

	//��ʯͷ �� ʯͷ��������
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_Y + 1 == iter->second->world_Y &&
			this->world_X == iter->second->world_X)
		{
			iter->second->Move_Down(true);
			this->world_Y += 1;
			return true;
		}
	}


	//����������Ѫ��
	//�ж���һ���Ƿ���Ѫ��
	int x = this->world_X;
	int y = this->world_Y;
	bool flag = false;
	while (true)
	{
		if (WALL[y + 1][x] == 1)
		{
			y += 1;
			flag = true;
		}
		else
			break;
	}

	if (flag)//�����һ����Ѫ�صĻ�
	{
		//�ж�Ѫ�����Ƿ���ʯͷ
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (
				x == iter->second->world_X
				&& y + 1 == iter->second->world_Y
				)
			{
				iter->second->Move_Down(true);
				this->world_X = x;
				this->world_Y = y + 1;
				return true;
			}
		}

		//�ж�Ѫ�����Ƿ������
		for (iter = players.begin(); iter != players.end(); ++iter)
		{
			if (
				x == iter->second->world_X
				&& y + 1 == iter->second->world_Y
				)
			{
				iter->second->killed = true;
				this->world_X = x;
				this->world_Y = y + 1;
				this->kill += 1;
				return true;
			}
		}
	}

	//�����һ���ǿյ�
	this->world_Y += 1;
	return true;


}

bool Monster::Move_Left()
{
	//�߽���
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//���ǰ������ң�����ұ��ж�Ϊ��
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X - 1 == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_X -= 1;
			this->kill += 1;
			return true;
		}
	}

	//��ʯͷ �� ʯͷ��������
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X - 1 == iter->second->world_X)
		{
			iter->second->Move_Left(true);
			this->world_X -= 1;
			return true;
		}
	}


	//����������Ѫ��
	//�ж���һ���Ƿ���Ѫ��
	int x = this->world_X;
	int y = this->world_Y;
	bool flag = false;
	while (true)
	{
		if (WALL[y][x - 1] == 1)
		{
			x -= 1;
			flag = true;
		}
		else
			break;
	}

	if (flag)//�����һ����Ѫ�صĻ�
	{
		//�ж�Ѫ�����Ƿ���ʯͷ
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (
				x - 1 == iter->second->world_X
				&&y == iter->second->world_Y
				)
			{
				iter->second->Move_Left(true);
				this->world_X = x - 1;
				this->world_Y = y;
				return true;
			}
		}

		//�ж�Ѫ�����Ƿ������
		for (iter = players.begin(); iter != players.end(); ++iter)
		{
			if (
				x - 1 == iter->second->world_X
				&& y == iter->second->world_Y
				)
			{
				iter->second->killed = true;
				this->world_X = x - 1;
				this->world_Y = y;
				this->kill += 1;
				return true;
			}
		}
	}

	//�����һ���ǿյ�
	this->world_X -= 1;
	return true;


}

bool Monster::Move_Right()
{
	//�߽���
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//���ǰ������ң�����ұ��ж�Ϊ��
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X + 1 == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_X += 1;
			this->kill += 1;
			return true;
		}
	}

	//��ʯͷ �� ʯͷ��������
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X + 1 == iter->second->world_X)
		{
			iter->second->Move_Right(true);
			this->world_X += 1;
			return true;
		}
	}

	//����������Ѫ��
	//�ж���һ���Ƿ���Ѫ��
	int x = this->world_X;
	int y = this->world_Y;
	bool flag = false;
	while (true)
	{
		if (WALL[y][x + 1] == 1)
		{
			x += 1;
			flag = true;
		}
		else
			break;
	}

	if (flag)//�����һ����Ѫ�صĻ�
	{
		//�ж�Ѫ�����Ƿ���ʯͷ
		for (iter = stones.begin(); iter != stones.end(); ++iter)
		{
			if (
				x + 1 == iter->second->world_X
				&&y == iter->second->world_Y
				)
			{
				iter->second->Move_Right(true);
				this->world_X = x + 1;
				this->world_Y = y;
				return true;
			}
		}

		//�ж�Ѫ�����Ƿ������
		for (iter = players.begin(); iter != players.end(); ++iter)
		{
			if (
				x + 1 == iter->second->world_X
				&& y == iter->second->world_Y
				)
			{
				iter->second->killed = true;
				this->world_X = x + 1;
				this->world_Y = y;
				this->kill += 1;
				return true;
			}
		}
	}

	//�����һ���ǿյ�
	this->world_X += 1;
	return true;


}