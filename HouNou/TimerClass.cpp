
#include "DirectX.h"



Timer::Timer(DWORD time)
{
	c_time = timeGetTime();
	t_time = time;
}

Timer::~Timer()
{
}

bool Timer::TimeOut()
{
	if (timeGetTime() - c_time > t_time)
	{
		c_time = timeGetTime();
		return true;
	}
	else
		return false;
}

void Timer::SetTimeOut(DWORD time)
{
	t_time = time;
}