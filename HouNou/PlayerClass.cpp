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


bool Player::Move_Up(bool force_push)
{

	//边界检测
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}
	

	//判断下一个是否是怪物
	if (this->world_Y - 1 == monster->world_Y && this->world_X == monster->world_X)
		return false;

	//推石头
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
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

	this->world_Y -= 1;
	return true;
}


bool Player::Move_Down(bool force_push)
{

	//边界检测
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//判断下一个是否是怪物
	if (this->world_Y + 1 == monster->world_Y && this->world_X == monster->world_X)
		return false;

	//推石头
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

	//边界检测
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//判断下一个是否是怪物
	if (this->world_Y  == monster->world_Y && this->world_X - 1 == monster->world_X)
		return false;

	//推石头
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

	//边界检测
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		if (force_push)
		{
			this->killed = true;
			return true;
		}
		return false;
	}


	//判断下一个是否是怪物
	if (this->world_Y == monster->world_Y && this->world_X + 1 == monster->world_X)
		return false;

	//推石头
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