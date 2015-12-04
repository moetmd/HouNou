#include "DirectX.h"

//Stone��
void Stone::Draw()
{
	Sprite_Transform_Draw(this->img, this->Get_RealPosX(), this->Get_RealPosY(), this->width,
		this->height, this->frame, this->columns, 0, 1.0f, this->color);
}

bool Stone::Move_Up(bool force_push)
{
	//�ж���һ���Ƿ���ǽ
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		//��������ƣ������ƽ�ǽ����
		if (force_push)
		{
			this->out_of_map = true;
			return true;
		}
		//��������ϽǺ����½��ƣ������Ƴ���ͼ
		if (this->world_X == 0 && this->world_Y == 0)
		{
			this->out_of_map = true;
			return true;
		}
		return false;
	}

	//�ж���һ���Ƿ���ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y - 1 == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ��ʯͷҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Up(true);
				this->world_Y -= 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ������
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y - 1 == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ�����ҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Up(true);
				this->world_Y -= 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ��ǹ���
	if (this->world_X == monster->world_X
		&& this->world_Y - 1 == monster->world_Y)
	{
		return false;
	}

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

	//�ж�Ѫ�����Ƿ���ʯͷ
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			x == iter->second->world_X
			&& y - 1 == iter->second->world_Y
			)
		{
			this->world_X = x;
			this->world_Y = y;
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
			this->world_X = x;
			this->world_Y = y;
			return true;
		}
	}

	if (flag)
	{
		this->world_X = x;
		this->world_Y = y - 1;
		return true;
	}
	//�ж��Ƿ���Ѫ�� ���



	this->world_Y -= 1;
	return true;
}

bool Stone::Move_Down(bool force_push)
{
	//�ж���һ���Ƿ���ǽ
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		if (force_push)
		{
			this->out_of_map = true;
			return true;
		}
		if (this->world_X == GAMEPANEL_WIDTH - 1 && this->world_Y == GAMEPANEL_HEIGHT - 1)
		{
			this->out_of_map = true;
			return true;
		}
		return false;
	}

	//�ж���һ���Ƿ���ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y + 1 == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ��ʯͷҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Down(true);
				this->world_Y += 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ������
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y + 1 == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ�����ҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Down(true);
				this->world_Y += 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ��ǹ���
	if (this->world_X == monster->world_X
		&& this->world_Y + 1 == monster->world_Y)
	{
		return false;
	}

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

	//�ж�Ѫ�����Ƿ���ʯͷ
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			x == iter->second->world_X
			&& y + 1 == iter->second->world_Y
			)
		{
			this->world_X = x;
			this->world_Y = y;
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
			this->world_X = x;
			this->world_Y = y;
			return true;
		}
	}

	if (flag)
	{
		this->world_X = x;
		this->world_Y = y + 1;
		return true;
	}
	//�ж��Ƿ���Ѫ�� ���



	this->world_Y += 1;
	return true;
}

bool Stone::Move_Left(bool force_push)
{
	//�ж���һ���Ƿ���ǽ
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		if (force_push)
		{
			this->out_of_map = true;
			return true;
		}
		if (this->world_X == 0 && this->world_Y == 0)
		{
			this->out_of_map = true;
			return true;
		}
		return false;
	}

	//�ж���һ���Ƿ���ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X - 1== iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ��ʯͷҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Left(true);
				this->world_X -= 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ������

	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X - 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ��ʯͷҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Left(true);
				this->world_X -= 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ��ǹ���
	if (this->world_X - 1 == monster->world_X
		&& this->world_Y == monster->world_Y)
	{
		return false;
	}

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

	//�ж�Ѫ�����Ƿ���ʯͷ
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			x - 1 == iter->second->world_X
			&& y == iter->second->world_Y
			)
		{
			this->world_X = x;
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
			this->world_X = x;
			this->world_Y = y;
			return true;
		}
	}

	if (flag)
	{
		this->world_X = x - 1;
		this->world_Y = y;
		return true;
	}
	//�ж��Ƿ���Ѫ�� ���



	this->world_X -= 1;
	return true;
}

bool Stone::Move_Right(bool force_push)
{
	//�ж���һ���Ƿ���ǽ
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		if (force_push)
		{
			this->out_of_map = true;
			return true;
		}
		if (this->world_X == GAMEPANEL_WIDTH - 1 && this->world_Y == GAMEPANEL_HEIGHT - 1)
		{
			this->out_of_map = true;
			return true;
		}
		return false;
	}

	//�ж���һ���Ƿ���ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X + 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ��ʯͷҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Right(true);
				this->world_X += 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ������

	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X + 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			//����ǹ����ƣ���һ�����ҲҪ�ƶ�
			if (force_push)
			{
				iter->second->Move_Right(true);
				this->world_X += 1;
				return true;
			}
			return false;
		}
	}

	//�ж���һ���Ƿ��ǹ���
	if (this->world_X + 1 == monster->world_X
		&& this->world_Y == monster->world_Y)
	{
		return false;
	}

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

	//�ж�Ѫ�����Ƿ���ʯͷ
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			x + 1 == iter->second->world_X
			&& y == iter->second->world_Y
			)
		{
			this->world_X = x;
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
			this->world_X = x;
			this->world_Y = y;
			return true;
		}
	}

	if (flag)
	{
		this->world_X = x + 1;
		this->world_Y = y;
		return true;
	}
	//�ж��Ƿ���Ѫ�� ���



	this->world_X += 1;
	return true;
}