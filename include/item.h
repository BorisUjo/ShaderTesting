#pragma once
#include <iostream>

struct StatModifier
{
	uint16_t health;
	uint16_t attackShield;
	uint16_t magicShield;

	uint16_t attackDamage;
	uint16_t magicDamage;
};


struct ItemData
{
	uint8_t id;
	StatModifier itemStats;
	std::string iconPath;
};



class Item
{
private:
	ItemData data;
public:
	uint8_t get_id() { return data.id; }
	uint8_t get_health() { return data.itemStats.health; }
	uint8_t get_attack() { return data.itemStats.attackDamage; }
	uint8_t get_shield() { return data.itemStats.attackShield; }

};
