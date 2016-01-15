#pragma once



class Timer
{
public:

	Timer(DWORD time);
	~Timer();

	bool TimeOut();
	void SetTimeOut(DWORD time);

private:
	DWORD c_time;
	DWORD t_time;

};

