#include "DirectX.h"

//定义上下左右
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

//Monster类
int Monster::Get_CurrentStep()
{
	int steps[] = { 1, 2, 5, 7, 7, 8, 8, 10 };
	srand(timeGetTime());
	return steps[rand() % 8];
}

//行动
bool Monster::Action()
{
	//先获取行动步数
	int current_step = 0;
	this->step = this->Get_CurrentStep();
	switch (step)
	{
		//如果抽到杀人卡
	case 1:
	case 2:
		while (kill < step && current_step <= 20) //在杀人数小于需要杀人数 或步数小于20 时一直循环
		{
			this->face_to = this->Look_Around();
			if(this->Move())
				current_step++;
			
		}
		break;

	default:
		while (step > 0)
		{
			this->face_to = this->Look_Around();
			if (this->Move())
				step--;
		}
		break;
	}

}

//观察
int Monster::Look_Around()
{
	switch (this->face_to)
	{
	case UP:
		return this->Change_Direction(Look_Left(), Look_Right(), Look_Up(), face_to);
		break;

	case DOWN:
		return this->Change_Direction(Look_Right(), Look_Left(), Look_Down(), face_to);
		break;

	case LEFT:
		return this->Change_Direction(Look_Down(), Look_Up(), Look_Left(), face_to);
		break;

	case RIGHT:
		return this->Change_Direction(Look_Up(), Look_Down(), Look_Right(), face_to);
		break;

	}
}

int Monster::Change_Direction(int A, int B, int C, int Face_to)
{
	if (A < B)
	{
		if (A < C)
		{
			switch (Face_to)
			{
			case UP:
				return LEFT;
				break;
			case RIGHT:
				return UP;
				break;
			case DOWN:
				return RIGHT;
				break;
			case LEFT:
				return DOWN;
				break;

			}
		}
		else
		{
			return Face_to;
		}
	}
	else 
	{
		if (B > C)
			return Face_to;
		else
		{
			switch (Face_to)
			{
			case UP:
				return RIGHT;
			case RIGHT:
				return DOWN;
			case DOWN:
				return LEFT;
			case LEFT:
				return UP;
			}
		}
	}
	
	return Face_to;
}

//移动
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

//观察某个方向
int Monster::Look_Up()
{
	int distance = GAMEPANEL_HEIGHT;

	//在怪物向上方向寻找玩家
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

	//如果之间有石头
	if (distance != GAMEPANEL_HEIGHT)
		for (iter = stones.begin(); iter != players.end(); ++iter)
		{
			if (iter->second->world_Y < this->world_Y && iter->second->world_X == this->world_X)
			{
				if (this->world_Y - iter->second->world_Y < distance)
				{
					return 0;
				}
			}
		}
	else
		return 0;

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

	//如果之间有石头
	if (distance != GAMEPANEL_HEIGHT)
		for (iter = stones.begin(); iter != players.end(); ++iter)
		{
			if (iter->second->world_Y > this->world_Y && iter->second->world_X == this->world_X)
			{
				if (iter->second->world_Y - this->world_Y < distance)
				{
					return 0;
				}
			}
		}
	else
		return 0;

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

	//如果之间有石头
	if (distance != GAMEPANEL_WIDTH)
		for (iter = stones.begin(); iter != players.end(); ++iter)
		{
			if (iter->second->world_X < this->world_X && iter->second->world_Y == this->world_Y)
			{
				if (this->world_X - iter->second->world_X < distance)
				{
					return 0;
				}
			}
		}
	else
		return 0;

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
		for (iter = stones.begin(); iter != players.end(); ++iter)
		{
			if (iter->second->world_X > this->world_X && iter->second->world_Y == this->world_Y)
			{
				if (iter->second->world_X - this->world_X  < distance)
				{
					return 0;
				}
			}
		}
	else
		return 0;


	return distance;
}


//向某个方向移动移动
bool Monster::Move_Up()
{
	//边界检测
	if (this->world_Y - 1 < 0 || WALL[this->world_Y - 1][this->world_X] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//如果前面是玩家，则玩家被判定为出界
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y - 1 == iter->second->world_Y && 
			this->world_X == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_Y -= 1;
			return true;
		}
	}

	//推石头 和 石头后面的玩家
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


}

bool Monster::Move_Down()
{
	//边界检测
	if (this->world_Y + 1 > GAMEPANEL_HEIGHT || WALL[this->world_Y + 1][this->world_X] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//如果前面是玩家，则玩家被判定为出界
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y + 1 == iter->second->world_Y &&
			this->world_X == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_Y += 1;
			return true;
		}
	}

	//推石头 和 石头后面的玩家
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

}

bool Monster::Move_Left()
{
	//边界检测
	if (this->world_X - 1 < 0 || WALL[this->world_Y][this->world_X - 1] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//如果前面是玩家，则玩家被判定为出界
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X - 1 == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_X -= 1;
			return true;
		}
	}

	//推石头 和 石头后面的玩家
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
}

bool Monster::Move_Right()
{
	//边界检测
	if (this->world_X + 1 > GAMEPANEL_WIDTH - 1 || WALL[this->world_Y][this->world_X + 1] == -1)
	{
		this->world_X = GAMEPANEL_WIDTH - 1 - this->world_X;
		this->world_Y = GAMEPANEL_HEIGHT - 1 - this->world_Y;
		return true;
	}


	//如果前面是玩家，则玩家被判定为出界
	map<int, Sprite*>::iterator iter;
	for (iter = players.begin(); iter != players.end(); ++iter)
	{
		if (this->world_Y == iter->second->world_Y &&
			this->world_X + 1 == iter->second->world_X)
		{
			iter->second->killed = true;
			this->world_X += 1;
			return true;
		}
	}

	//推石头 和 石头后面的玩家
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
}