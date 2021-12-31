#include<iostream>
#include<vector>
#include"my_vector.h"


int main()
{
	srand(time(0));
	setlocale(0, "");
	my::vector <int> z{1};

	std::cout << z.capacity()<<std::endl;
	z.reserve(400000);
	std::cout << z.capacity()<<" "<<z.size() << std::endl;
	z.emplace_back(1, 1, 1, 1, 1, 25);
	std::cout << z.capacity() << " " << z.size() << std::endl;

	
	
}