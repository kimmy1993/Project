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
		cout<<"1.레시피 2.재료넣기 3.재료빼기 4.재료확인 5.만들기(자동리셋) 6.리셋 7.종료"<<endl;
		cin>>num;
		switch(num)
		{
		case 1:
			cout<<"레시피 넘버를 고르세요"<<endl;
			cout<<"1.에스프레소  2.아메리카노  3.카페라떼  4.카푸치노 5.카라멜_마키아또"<<endl;
			cout<<"6.초코_프라카치노  7.카페모카  8.카라멜_모카  9.녹차라떼  10.ICE_아메리카노"<<endl;
			cout<<"11.ICE_카페라떼  12.ICE_카푸치노  13.ICE_카라멜_마끼아또  14.ICE_카페_모카"<<endl;
			cout<<"15.ICE_카라멜_모카  16.ICE_녹차라떼"<<endl;
			cout<<"고르시오: ";
			cin>>recipe_num;
			cout<<coffee.recipe(recipe_num)<<endl;
			system("pause");
			system("cls");
			break;
		case 2:
		case 3:
			cout<<"재료를 고르세요"<<endl;
			cout<<"1.샷  2.물  3.우유  4.거품  ";
			cout<<"5.카라멜  6.초코  7.녹차  8.얼음"<<endl;
			cout<<"고르시오: ";
			cin>>select_num;
			switch(select_num)
			{
			case 1:
				if(coffee.get_shot(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 2:
				if(coffee.get_water(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 3:
				if(coffee.get_milk(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 4:
				if(coffee.get_bubble(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 5:
				if(coffee.get_caramel(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 6:
				if(coffee.get_choco(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 7:
				if(coffee.get_green_tea(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			case 8:
				if(coffee.get_ice(num))
					cout<<"되었습니다"<<endl;
				else
					cout<<"실패했습니다"<<endl;
				break;
			default:
				cout<<"잘못고르셨습니다"<<endl;
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
			cout<<"리셋되엇습니다"<<endl;
			system("pause");
			system("cls");
			break;
		case 7:
			break;
		default:
			cout<<"다시고르세요"<<endl;
			system("pause");
			system("cls");
			break;
		}
		if(num==7)
		{
			cout<<"종료되엇습니다"<<endl;
			break;
		}
	}
}