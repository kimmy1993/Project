#include "Coffee.h"
#include <string>
using namespace std;

Coffee::Coffee()
{
	shot=0;
	water=0;
	milk=0;
	bubble=0;
	caramel=0;
	choco=0;
	green_tea=0;
	ice=0;
	mix=0;
}

string Coffee::recipe(int num)
{
	switch(num)
	{
	case 1:
		return "샷";
		break;
	case 2:
		return "샷 물";
		break;
	case 3:
		return "샷 우유";
		break;
	case 4:
		return "샷 우유 거품";
		break;
	case 5:
		return "샷 우유 거품 카라멜";
		break;
	case 6:
		return "우유 초코 얼음";
		break;
	case 7:
		return "샷 우유 초코";
		break;
	case 8:
		return "샷 우유 초코 카라멜";
		break;
	case 9:
		return "우유 녹차";
		break;
	case 10:
		return "샷 물 얼음";
		break;
	case 11:
		return "샷 우유 얼음";
		break;
	case 12:
		return "샷 우유 거품 얼음";
		break;
	case 13:
		return "샷 우유 거품 카라멜 얼음";
		break;
	case 14:
		return "샷 우유 초코 얼음";
		break;
	case 15:
		return "샷 우유 초코 카라멜 얼음";
		break;
	case 16:
		return "우유 녹차 얼음";
		break;
	default:
		return "1~16중에서 고르세요";
		break;
	}
}
bool Coffee::get_shot(int num)
{
	if(num==2)
	{
		if(shot==1)
			return false;
		shot++;
		return true;
	}
	else
	{
		if(shot==0)
			return false;
		shot--;
		return true;
	}
}
bool Coffee::get_water(int num)
{
	if(num==2)
	{
		if(water==2)
			return false;
		water+=2;
		return true;
	}
	else
	{
		if(water==0)
			return false;
		water-=2;
		return true;
	}
}
bool Coffee::get_milk(int num)
{
	if(num==2)
	{
		if(milk==4)
			return false;
		milk+=4;
		return true;
	}
	else
	{
		if(milk==0)
			return false;
		milk-=4;
		return true;
	}
}
bool Coffee::get_bubble(int num)
{
	if(num==2)
	{
		if(bubble==8)
			return false;
		bubble+=8;
		return true;
	}
	else
	{
		if(bubble==0)
			return false;
		bubble-=8;
		return true;
	}
}
bool Coffee::get_caramel(int num)
{
	if(num==2)
	{
		if(caramel==16)
			return false;
		caramel+=16;
		return true;
	}
	else
	{
		if(caramel==0)
			return false;
		caramel-=16;
		return true;
	}
}
bool Coffee::get_choco(int num)
{
	if(num==2)
	{
		if(choco==32)
			return false;
		choco+=32;
		return true;
	}
	else
	{
		if(choco==0)
			return false;
		choco-=32;
		return true;
	}
}
bool Coffee::get_green_tea(int num)
{
	if(num==2)
	{
		if(green_tea==64)
			return false;
		green_tea+=64;
		return true;
	}
	else
	{
		if(green_tea==0)
			return false;
		green_tea-=64;
		return true;
	}
}
bool Coffee::get_ice(int num)
{
	if(num==2)
	{
		if(ice==128)
			return false;
		ice+=128;
		return true;
	}
	else
	{
		if(ice==0)
			return false;
		ice-=128;
		return true;
	}
}

int Coffee::get_mix()
{
	mix=shot+water+milk+bubble+caramel+choco+green_tea+ice;
	return mix;
}

void Coffee::get_reset()
{
	shot=0;
	water=0;
	milk=0;
	bubble=0;
	caramel=0;
	choco=0;
	green_tea=0;
	ice=0;
	mix=0;
}

string Coffee::select_coffee(int mixed)
{
	get_reset();
	switch(mixed)
	{
	case 1:
		return "에스프레소가 완성되었습니다";
		break;
	case 3:
		return "아메리카노가 완성되었습니다";
		break;
	case 5:
		return "카페 라떼가 완성되었습니다";
		break;
	case 13:
		return "카푸치노가 완성되었습니다";
		break;
	case 29:
		return "카라멜 마끼아또가 완성되었습니다";
		break;
	case 37:
		return "카페 모카가 완성되었습니다";
		break;
	case 53:
		return "카라멜 모카가 완성되었습니다";
		break;
	case 68:
		return "녹차 라떼가 완성되었습니다";
		break;
	case 131:
		return "ICE 아메리카노가 완성되었습니다";
		break;
	case 133:
		return "ICE 카페 라떼가 완성되었습니다";
		break;
	case 141:
		return "ICE 카푸치노가 완성되었습니다";
		break;
	case 157:
		return "ICE 카라멜 마끼아또가 완성되었습니다";
		break;
	case 164:
		return "초코 프라카치노가 완성되었습니다";
		break;
	case 165:
		return "ICE 카페 모카가 완성되었습니다";
		break;
	case 181:
		return "ICE 카라멜 모카가 완성되었습니다";
		break;
	case 196:
		return "ICE 녹차 라떼가 완성되었습니다";
		break;
	default:
		return "커피가 레시피에 맞지 않습니다. 커피는 망했습니다";
		break;
	}
}

string Coffee::find_material()
{
	int num=shot+water+milk+bubble+caramel+choco+green_tea+ice;
	string material_ice,material_green_tea,material_choco,material_caramel;
	string material_bubble,material_milk,material_water,material_shot;
	if(num==0)
		return "들어가 있는 재료가 없습니다";
	if(num>=128)
	{
		material_ice="얼음 ";
		num-=128;
	}
	if(num>=64)
	{
		material_green_tea="녹차 ";
		num-=64;
	}
	if(num>=32)
	{
		material_choco="초코 ";
		num-=32;
	}
	if(num>=16)
	{
		material_caramel="카라멜 ";
		num-=16;
	}
	if(num>=8)
	{
		material_bubble="거품 ";
		num-=8;
	}
	if(num>=4)
	{
		material_milk="우유 ";
		num-=4;
	}
	if(num>=2)
	{
		material_water="물 ";
		num-=2;
	}
	if(num>=1)
	{
		material_shot="샷 ";
		num-=1;
	}
	
	string all="들어있는 재료는 "+material_shot+material_water+material_milk+material_bubble+material_caramel+material_choco+material_green_tea+material_ice+"입니다";
	return all;

}