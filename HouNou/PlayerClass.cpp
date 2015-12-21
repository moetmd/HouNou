#include "DirectX.h"

Player::Player(int d_step)
{
	world_X = GAMEPANEL_WIDTH - 1;
	world_Y = GAMEPANEL_HEIGHT - 1;
	width = height = 96;
	columns = 4;
	startframe = 0;
	endframe = 0;
	foot = 20;
	day_step = d_step;
	night_step = 7 - d_step;
	current_step = day_step;

}

bool Player::Is_InBlood()
{
	if ( WALL[this->world_Y][this->world_X] == 1 )
		return true;
	else
		return false;
}

bool Player::Is_OverPlayer()
{
	int num = 0;
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X == this->world_X && iter->second->world_Y == this->world_Y)
			num++;
	}

	if (num >= 2)
		return true;
	else
		return false;
}

bool Player::Move_Up(bool force_push)
{

	//���ֻʣ��1��������Խ���������
	if (this->current_step == 1)
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			if (this->world_Y - 1 == iter->second->world_Y && this->world_X == iter->second->world_X)
				return false;
		}

	//�߽���
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}
	

	//�ж���һ���Ƿ��ǹ���
	if (this->world_Y - 1 == monster->world_Y && this->world_X == monster->world_X)
		return false;

	//��ʯͷ
	for (map<int, Sprite*>::iterator iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_X == iter->second->world_X && this->world_Y - 1 == iter->second->world_Y)
		{
			if (force_push)
			{
				iter->second->Move_Up(true);
				this->world_Y -= 1;
				return true;
			}
			if ( iter->second->Move_Up(false) )
			{
				this->world_Y -= 1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	//����ǹ����ƣ�����һ�����ҲҪ����
	if (force_push)
	{
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			if (this->world_X == iter->second->world_X && this->world_Y - 1 == iter->second->world_Y)
			{
				iter->second->Move_Up(true);
				this->world_Y -= 1;
				return true;
			}
		}
	}

	this->world_Y -= 1;
	return true;
}


bool Player::Move_Down(bool force_push)
{

	//���ֻʣ��1��������Խ���������
	if (this->current_step == 1)
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			if (this->world_Y + 1 == iter->second->world_Y && this->world_X == iter->second->world_X)
				return false;
		}

	//�߽���
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//�ж���һ���Ƿ��ǹ���
	if (this->world_Y + 1 == monster->world_Y && this->world_X == monster->world_X)
		return false;

	//��ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_X == iter->second->world_X && this->world_Y + 1 == iter->second->world_Y)
		{
			if (force_push)
			{
				iter->second->Move_Down(true);
				this->world_Y += 1;
				return true;
			}
			if (iter->second->Move_Down(false))
			{
				this->world_Y += 1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	this->world_Y += 1;
	return true;
}

bool Player::Move_Left(bool force_push)
{

	//���ֻʣ��1��������Խ���������
	if (this->current_step == 1)
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			if (this->world_Y  == iter->second->world_Y && this->world_X - 1 == iter->second->world_X)
				return false;
		}

	//�߽���
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//�ж���һ���Ƿ��ǹ���
	if (this->world_Y  == monster->world_Y && this->world_X - 1 == monster->world_X)
		return false;

	//��ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_X - 1 == iter->second->world_X && this->world_Y  == iter->second->world_Y)
		{
			if (force_push)
			{
				iter->second->Move_Left(true);
				this->world_X -= 1;
				return true;
			}
			if (iter->second->Move_Left(false))
			{
				this->world_X -= 1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	this->world_X -= 1;
	return true;
}

bool Player::Move_Right(bool force_push)
{

	//���ֻʣ��1��������Խ���������
	if (this->current_step == 1)
		for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
		{
			if (this->world_Y == iter->second->world_Y && this->world_X + 1 == iter->second->world_X)
				return false;
		}

	//�߽���
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//�ж���һ���Ƿ��ǹ���
	if (this->world_Y == monster->world_Y && this->world_X + 1 == monster->world_X)
		return false;

	//��ʯͷ
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (this->world_X + 1 == iter->second->world_X && this->world_Y == iter->second->world_Y)
		{
			if (force_push)
			{
				iter->second->Move_Right(true);
				this->world_X += 1;
				return true;
			}
			if (iter->second->Move_Right(false))
			{
				this->world_X += 1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	this->world_X += 1;
	return true;
}


//���������
bool Player::Push_Up()
{
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X == this->world_X && iter->second->world_Y == this->world_Y - 1)
		{
			//��Һ�����ʯͷ������
			for (map<int, Sprite*>::iterator iter_1 = stones.begin(); iter_1 != stones.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X && iter_1->second->world_Y == this->world_Y - 2)
					return false;
			}

			//��Һ��������������
			for (map<int, Sprite*>::iterator iter_1 = players.begin(); iter_1 != players.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X && iter_1->second->world_Y == this->world_Y - 2)
					return false;
			}

			//��Һ�����Ѫ��������
			if (this->world_Y - 2 < 0 || WALL[this->world_Y - 2][this->world_X] == 1)
			{
				return false;
			}
			
			//
			if (iter->second->Move_Up(false))
				return true;
			else
				return false;
			
		}
	}
}

bool Player::Push_Down()
{
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X == this->world_X && iter->second->world_Y == this->world_Y + 1)
		{
			//��Һ�����ʯͷ������
			for (map<int, Sprite*>::iterator iter_1 = stones.begin(); iter_1 != stones.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X && iter_1->second->world_Y == this->world_Y + 2)
					return false;
			}

			//��Һ��������������
			for (map<int, Sprite*>::iterator iter_1 = players.begin(); iter_1 != players.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X && iter_1->second->world_Y == this->world_Y + 2)
					return false;
			}

			//��Һ�����Ѫ��������
			if (this->world_Y + 2 >= GAMEPANEL_HEIGHT || WALL[this->world_Y + 2][this->world_X] == 1)
			{
				return false;
			}
			else
			{
				if (iter->second->Move_Down(false))
					return true;
				else
					return false;
			}
		}
	}
}

bool Player::Push_Left()
{
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X  == this->world_X - 1 && iter->second->world_Y == this->world_Y)
		{
			//��Һ�����ʯͷ������
			for (map<int, Sprite*>::iterator iter_1 = stones.begin(); iter_1 != stones.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X - 2 && iter_1->second->world_Y == this->world_Y)
					return false;
			}

			//��Һ��������������
			for (map<int, Sprite*>::iterator iter_1 = players.begin(); iter_1 != players.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X - 2 && iter_1->second->world_Y == this->world_Y)
					return false;
			}

			//��Һ�����Ѫ��������
			if (this->world_X - 2 < 0 || WALL[this->world_Y][this->world_X - 2] == 1)
			{
				return false;
			}
			else
			{
				if (iter->second->Move_Left(false))
					return true;
				else
					return false;
			}
		}
	}
}

bool Player::Push_Right()
{
	for (map<int, Sprite*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		if (iter->second->world_X == this->world_X + 1 && iter->second->world_Y == this->world_Y)
		{
			//��Һ�����ʯͷ������
			for (map<int, Sprite*>::iterator iter_1 = stones.begin(); iter_1 != stones.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X + 2 && iter_1->second->world_Y == this->world_Y)
					return false;
			}

			//��Һ��������������
			for (map<int, Sprite*>::iterator iter_1 = players.begin(); iter_1 != players.end(); ++iter_1)
			{
				if (iter_1->second->world_X == this->world_X + 2 && iter_1->second->world_Y == this->world_Y)
					return false;
			}

			//��Һ�����Ѫ��������
			if (this->world_X + 2 >= GAMEPANEL_WIDTH || WALL[this->world_Y][this->world_X + 2] == 1)
			{
				return false;
			}
			else
			{
				if (iter->second->Move_Right(false))
					return true;
				else
					return false;
			}
		}
	}
}