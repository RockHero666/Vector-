#include<iostream>
#include"my_vector.h"
#include<vector>
#include<time.h>

struct Y
{   static int y;
    int x=0;

    Y()
    {
        x=y;
        y++;
    }
};
int Y::y=1;


class Z
{
public:

     static int x;

     Z()
     {
       std::cout << "defult constr\n";
       x++;
     }

    Z(int u)
	{
		std::cout << "constr\n";
        x=u;
	}
    Z(const Z& z)
    {
        this->x = z.x;
        std::cout << "copy\n";
    }
    Z& operator =(const Z& z)
    {
        this->x = z.x;
        std::cout << "copy=\n";
        return *this;
    }
	Z( Z&& z)
    {
        this->x = z.x;
        
        std::cout << "move\n";
    }

	Z& operator =( Z&& z)
    {
        this->x = z.x;
       
        std::cout << "move=\n";
        return *this;
    }


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

    my::vector<Z> zz(5);
    zz.push_back(Z());
    zz.push_back(Z());
  //  zz.push_back(Z());
    zz.resize(25);
    zz.push_back(Z());
    zz.shrink_to_fit();

    for (int var = 0; var < 6; ++var) {
        std::cout << zz[var].x << "\n";
    }

    //std::cout << zz[0].get() << "\n";
    std::cout<<"-----\n";




   std::vector<Z> z(5);
    z.push_back(Z());
    z.push_back(Z());
  //  z.push_back(Z());
    z.resize(25);
    z.push_back(Z());
    z.shrink_to_fit();
   for (int var = 0; var < 6; ++var) {
       std::cout << z[var].x << "\n";


      


   }

  //  std::cout<<z.capacity();

   std::cout << "-----\n";



   //my::vector<my::vector<Z>> zz(5);
  // zz[0][0] = Z();



   my::vector<my::vector<Z>> zzz(5);
   zzz[0][0] = Z();




	

	

}

