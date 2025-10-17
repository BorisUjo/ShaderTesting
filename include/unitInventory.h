#pragma once
#include <iostream>
#include <item.h>

template<size_t N>

class UnitInventory
{
public:
	Item items[N];
	uint16_t inventory_size;

	UnitInventory()
	{
		inventory_size = N;
	}
};
