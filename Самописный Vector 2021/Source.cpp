#include<iostream>
//#include<vector>
#include"my_vector.h"


int main()
{
	srand(time(0));
	setlocale(0, "");

	my::vector <int> X{5,4,7,3,3};
	
	std::cout << "Емкость = " << X.capacity() << " Количество элементов = " << X.size() << std::endl;
	std::cout << "Применяем X.shrink_to_fit()\n";
	X.shrink_to_fit();
	std::cout << "Емкость = " << X.capacity() << " Количество элементов = " << X.size() << std::endl;
	std::cout << "Применяем X.emplace_back()\n";
	X.emplace_back(2, 3);
	std::cout << "Емкость = " << X.capacity() << " Количество элементов = " << X.size() << std::endl;
}