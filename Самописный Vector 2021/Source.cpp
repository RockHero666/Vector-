#include<iostream>
#include"my_vector.h"
#include<vector>



class Z
{
public:

	static int x;

	Z() 
	{
		std::cout << "constr\n";
		x++;
	}
	Z(const Z& z)  { std::cout << "copy\n"; }
	Z& operator =(const Z& z) { std::cout << "copy=\n"; return *this; }
	Z( Z&& z)
	{ std::cout << "move\n"; }

	Z& operator =( Z&& z)
	{ std::cout << "move=\n"; return *this;}


	int get()
	{
		return x;
	}


};

int Z::x = 0;




int main()
{
	
	srand(time(0));
	setlocale(0, "");

	std::vector<Z> z(5);
	z.push_back(Z());
	z.push_back(Z());
	std::cout << Z::x<<"\n";
	std::cout << z[0].get() << "\n";


	my::vector<Z> zz(5);
	z.push_back(Z());
	z.push_back(Z());
	std::cout << Z::x << "\n";
	std::cout << zz[0].get() << "\n";
	

	

}