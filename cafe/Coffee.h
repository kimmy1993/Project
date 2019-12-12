#pragma once
#include <string>
using namespace std;

class Coffee
{
private:
	int shot;
	int water;
	int milk;
	int bubble;
	int caramel;
	int choco;
	int green_tea;
	int ice;
	int mix;
public:
	Coffee();
	string recipe(int);
	bool get_shot(int);
	bool get_water(int);
	bool get_milk(int);
	bool get_bubble(int);
	bool get_caramel(int);
	bool get_choco(int);
	bool get_green_tea(int);
	bool get_ice(int);
	int get_mix();
	void get_reset();
	string find_material();
	string select_coffee(int);
	
};