#include<iostream>
#include<vector>
#include"my_vector.h"







int main()
{
	srand(time(0));
	setlocale(0, "");

	//std::cout << "������ ��������"<<std::endl<<std::endl;
	//
	//my::vector<int> gg1,gg2,gg3,gg4,gg5;

	//std::cout << "����� ��������" << std::endl << std::endl;


	//gg1.emplace_back( 5,34,2,7,33 );
	//gg2.emplace_back(5, 34, 2, 7, 33);
	//gg3.emplace_back(5, 34, 2, 7, 33);
	//gg4.emplace_back(5, 34, 2, 7, 33);
	//gg5.emplace_back(5, 34, 2, 7, 33);

	//std::cout << "������ ��������" << std::endl << std::endl;
	//my::vector<my::vector<int>> X;
	//
	//std::cout << "����� ��������" << std::endl << std::endl;

	//std::cout << "����������" << std::endl << std::endl;
	//X.emplace_back(gg1,gg2,gg3,gg4,gg5);
	//my::vector<my::vector<int>> Y(std::move(X));

	//std::cout << "�����" << std::endl << std::endl;

	//for (int i = 0; i < X.size(); i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		std::cout << Y[i][j] << " ";
	//
	//	}
	//	std::cout << std::endl;
	//}



	//

	//


	//std::cout << "����������" << std::endl << std::endl;
	//	
	//	
	//



	//my::vector<int> X{1,2,3,4,5};
		
	my::vector<int> X{2,56,52};

	X = { 2,3,4,6 };

		for (auto const& x : X)
		{
			std::cout << x <<" ";
		}
		std::cout << std::endl;


		X.emplace_back(2);


}