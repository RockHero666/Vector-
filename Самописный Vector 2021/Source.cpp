#include<iostream>
#include"my_vector.h"


int main()
{
	
	srand(time(0));
	setlocale(0, "");
	my::vector<int> z{2,2,1,1};
	my::vector<int> zz{2,2,1,0};
	

	std::cout <<(z<=zz);

}