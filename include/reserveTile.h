#pragma once
#include <gameObject.h>

class ReserveTile : public GameObject
{
public:
	bool occupied = false;

	inline void setOccupation(bool status)
	{
		occupied = status;
	}
};