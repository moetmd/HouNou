#include "DirectX.h"

//Stone类
void Stone::Draw()
{
	Sprite_Transform_Draw(this->img, this->Get_RealPosX(), this->Get_RealPosY(), this->width,
		this->height, this->frame, this->columns, 0, 1.0f, this->color);
}

bool Stone::Move_Up()
{
	//判断下一个是否是墙
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		return false;
	}

	//判断下一个是否是石头
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y - 1 == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是玩家
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y - 1 == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是怪物
	if (this->world_X == monster->world_X
		&& this->world_Y - 1 == monster->world_Y)
	{
		return false;
	}

	//判断下一个是否是血池
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

	//判断血池外是否是石头
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

	//判断血池外是否是玩家
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
	//判断是否是血池 完毕



	this->world_Y -= 1;
	return true;
}

bool Stone::Move_Down()
{
	//判断下一个是否是墙
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT - 1 || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		return false;
	}

	//判断下一个是否是石头
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y + 1 == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是玩家
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X == iter->second->world_X
			&& this->world_Y + 1 == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是怪物
	if (this->world_X == monster->world_X
		&& this->world_Y + 1 == monster->world_Y)
	{
		return false;
	}

	//判断下一个是否是血池
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

	//判断血池外是否是石头
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

	//判断血池外是否是玩家
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
	//判断是否是血池 完毕



	this->world_Y += 1;
	return true;
}

bool Stone::Move_Left()
{
	//判断下一个是否是墙
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		return false;
	}

	//判断下一个是否是石头
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X - 1== iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是玩家

	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X - 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是怪物
	if (this->world_X - 1 == monster->world_X
		&& this->world_Y == monster->world_Y)
	{
		return false;
	}

	//判断下一个是否是血池
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

	//判断血池外是否是石头
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

	//判断血池外是否是玩家
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
	//判断是否是血池 完毕



	this->world_X -= 1;
	return true;
}

bool Stone::Move_Right()
{
	//判断下一个是否是墙
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		return false;
	}

	//判断下一个是否是石头
	map<int, Sprite*>::iterator iter;
	for (iter = stones.begin(); iter != stones.end(); ++iter)
	{
		if (
			this->world_X + 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是玩家

	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (
			this->world_X + 1 == iter->second->world_X
			&& this->world_Y == iter->second->world_Y
			)
		{
			return false;
		}
	}

	//判断下一个是否是怪物
	if (this->world_X + 1 == monster->world_X
		&& this->world_Y == monster->world_Y)
	{
		return false;
	}

	//判断下一个是否是血池
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

	//判断血池外是否是石头
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

	//判断血池外是否是玩家
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
	//判断是否是血池 完毕



	this->world_X += 1;
	return true;
}