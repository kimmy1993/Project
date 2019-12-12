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
		return "��";
		break;
	case 2:
		return "�� ��";
		break;
	case 3:
		return "�� ����";
		break;
	case 4:
		return "�� ���� ��ǰ";
		break;
	case 5:
		return "�� ���� ��ǰ ī���";
		break;
	case 6:
		return "���� ���� ����";
		break;
	case 7:
		return "�� ���� ����";
		break;
	case 8:
		return "�� ���� ���� ī���";
		break;
	case 9:
		return "���� ����";
		break;
	case 10:
		return "�� �� ����";
		break;
	case 11:
		return "�� ���� ����";
		break;
	case 12:
		return "�� ���� ��ǰ ����";
		break;
	case 13:
		return "�� ���� ��ǰ ī��� ����";
		break;
	case 14:
		return "�� ���� ���� ����";
		break;
	case 15:
		return "�� ���� ���� ī��� ����";
		break;
	case 16:
		return "���� ���� ����";
		break;
	default:
		return "1~16�߿��� ������";
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
		return "���������Ұ� �ϼ��Ǿ����ϴ�";
		break;
	case 3:
		return "�Ƹ޸�ī�밡 �ϼ��Ǿ����ϴ�";
		break;
	case 5:
		return "ī�� �󶼰� �ϼ��Ǿ����ϴ�";
		break;
	case 13:
		return "īǪġ�밡 �ϼ��Ǿ����ϴ�";
		break;
	case 29:
		return "ī��� �����ƶǰ� �ϼ��Ǿ����ϴ�";
		break;
	case 37:
		return "ī�� ��ī�� �ϼ��Ǿ����ϴ�";
		break;
	case 53:
		return "ī��� ��ī�� �ϼ��Ǿ����ϴ�";
		break;
	case 68:
		return "���� �󶼰� �ϼ��Ǿ����ϴ�";
		break;
	case 131:
		return "ICE �Ƹ޸�ī�밡 �ϼ��Ǿ����ϴ�";
		break;
	case 133:
		return "ICE ī�� �󶼰� �ϼ��Ǿ����ϴ�";
		break;
	case 141:
		return "ICE īǪġ�밡 �ϼ��Ǿ����ϴ�";
		break;
	case 157:
		return "ICE ī��� �����ƶǰ� �ϼ��Ǿ����ϴ�";
		break;
	case 164:
		return "���� ����īġ�밡 �ϼ��Ǿ����ϴ�";
		break;
	case 165:
		return "ICE ī�� ��ī�� �ϼ��Ǿ����ϴ�";
		break;
	case 181:
		return "ICE ī��� ��ī�� �ϼ��Ǿ����ϴ�";
		break;
	case 196:
		return "ICE ���� �󶼰� �ϼ��Ǿ����ϴ�";
		break;
	default:
		return "Ŀ�ǰ� �����ǿ� ���� �ʽ��ϴ�. Ŀ�Ǵ� ���߽��ϴ�";
		break;
	}
}

string Coffee::find_material()
{
	int num=shot+water+milk+bubble+caramel+choco+green_tea+ice;
	string material_ice,material_green_tea,material_choco,material_caramel;
	string material_bubble,material_milk,material_water,material_shot;
	if(num==0)
		return "�� �ִ� ��ᰡ �����ϴ�";
	if(num>=128)
	{
		material_ice="���� ";
		num-=128;
	}
	if(num>=64)
	{
		material_green_tea="���� ";
		num-=64;
	}
	if(num>=32)
	{
		material_choco="���� ";
		num-=32;
	}
	if(num>=16)
	{
		material_caramel="ī��� ";
		num-=16;
	}
	if(num>=8)
	{
		material_bubble="��ǰ ";
		num-=8;
	}
	if(num>=4)
	{
		material_milk="���� ";
		num-=4;
	}
	if(num>=2)
	{
		material_water="�� ";
		num-=2;
	}
	if(num>=1)
	{
		material_shot="�� ";
		num-=1;
	}
	
	string all="����ִ� ���� "+material_shot+material_water+material_milk+material_bubble+material_caramel+material_choco+material_green_tea+material_ice+"�Դϴ�";
	return all;

}