#include<iostream>
//#include<vector>
#include"my_vector.h"


int main()
{
	srand(time(0));
	setlocale(0, "");
	my::vector<my::vector<int>> z{ my::vector<int>{4,2,3},
								   my::vector<int>{6,7,8} };
	
	std::cout << z.capacity()<<std::endl;
	z.reserve(400);
	std::cout << z.capacity()<<" "<<z.size() << std::endl;
	z.emplace_back(2);
	std::cout << z.capacity() << " " << z.size() << std::endl;
	z[2].push_back(22);
	
	std::cout << z[2][2];

}