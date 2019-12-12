#include <iostream>
#include "Coffee.h"
using namespace std;

void main()
{
	Coffee coffee;
	int num=0;
	int recipe_num=0;
	int select_num=0;
	int mix_coffee=0;
	while(1)
	{
		cout<<"1.������ 2.���ֱ� 3.��ứ�� 4.���Ȯ�� 5.�����(�ڵ�����) 6.���� 7.����"<<endl;
		cin>>num;
		switch(num)
		{
		case 1:
			cout<<"������ �ѹ��� ������"<<endl;
			cout<<"1.����������  2.�Ƹ޸�ī��  3.ī���  4.īǪġ�� 5.ī���_��Ű�ƶ�"<<endl;
			cout<<"6.����_����īġ��  7.ī���ī  8.ī���_��ī  9.������  10.ICE_�Ƹ޸�ī��"<<endl;
			cout<<"11.ICE_ī���  12.ICE_īǪġ��  13.ICE_ī���_�����ƶ�  14.ICE_ī��_��ī"<<endl;
			cout<<"15.ICE_ī���_��ī  16.ICE_������"<<endl;
			cout<<"���ÿ�: ";
			cin>>recipe_num;
			cout<<coffee.recipe(recipe_num)<<endl;
			system("pause");
			system("cls");
			break;
		case 2:
		case 3:
			cout<<"��Ḧ ������"<<endl;
			cout<<"1.��  2.��  3.����  4.��ǰ  ";
			cout<<"5.ī���  6.����  7.����  8.����"<<endl;
			cout<<"���ÿ�: ";
			cin>>select_num;
			switch(select_num)
			{
			case 1:
				if(coffee.get_shot(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 2:
				if(coffee.get_water(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 3:
				if(coffee.get_milk(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 4:
				if(coffee.get_bubble(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 5:
				if(coffee.get_caramel(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 6:
				if(coffee.get_choco(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 7:
				if(coffee.get_green_tea(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			case 8:
				if(coffee.get_ice(num))
					cout<<"�Ǿ����ϴ�"<<endl;
				else
					cout<<"�����߽��ϴ�"<<endl;
				break;
			default:
				cout<<"�߸����̽��ϴ�"<<endl;
				break;
			}
			system("pause");
			system("cls");
			break;
		case 4:
			cout<<coffee.find_material()<<endl;
			break;
		case 5:
			mix_coffee=coffee.get_mix();
			cout<<coffee.select_coffee(mix_coffee)<<endl;
			system("pause");
			system("cls");
			break;
		case 6:
			coffee.get_reset();
			cout<<"���µǾ����ϴ�"<<endl;
			system("pause");
			system("cls");
			break;
		case 7:
			break;
		default:
			cout<<"�ٽð�����"<<endl;
			system("pause");
			system("cls");
			break;
		}
		if(num==7)
		{
			cout<<"����Ǿ����ϴ�"<<endl;
			break;
		}
	}
}