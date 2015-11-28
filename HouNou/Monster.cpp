#include "DirectX.h"

//MonsterÀà
int Monster::Get_CurrentStep()
{
	int steps[] = { 1, 2, 5, 7, 7, 8, 8, 10 };
	srand(timeGetTime());
	return steps[rand() % 8];
}