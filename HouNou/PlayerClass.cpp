#include "DirectX.h"

bool Player::Move_Up()
{

	//边界检测
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
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
			if ( iter->second->Move_Up() )
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


bool Player::Move_Down()
{

	//边界检测
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
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
			if (iter->second->Move_Down())
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

bool Player::Move_Left()
{

	//边界检测
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
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
			if (iter->second->Move_Left())
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

bool Player::Move_Right()
{

	//边界检测
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
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
			if (iter->second->Move_Right())
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