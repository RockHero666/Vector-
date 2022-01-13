#pragma once
//#define debag  // раскоментить для дебага
#define C17 // поменять на 14 для variadic templates

namespace my
{


	namespace func
	{
		template<class T, class U>
		struct is_same
		{
			static constexpr bool value = false;
		};

		template<class T>
		struct is_same<T, T>
		{
			static constexpr bool value = true;
		};
	}

	//////////////////////////////////////////////ALLOCATOR////////////////////////////////
	
	template<class T>
	class Allocator 
	{
	public:
	
		T* _ptr=nullptr;									   // указатель на динамическую память который передается при аллоцировании
		Allocator();										   // конструктор по умолчанию
		Allocator(const Allocator& copy_alloc);				   // почленное копирование
		T* allocate(const size_t size);						   // возвращает указатель на область памяти  равную size
		void deallocate();									   // освобождение памяти 
		size_t max_size() const;							   // возвращает максимальное значение для алокации
		bool operator ==(const Allocator<T>& alloc);		   // оператор равенства
		bool operator !=(const Allocator<T>& alloc);		   // оператор неравенства
		Allocator& operator = (const Allocator<T>&) = default; // стандартный оператор присвоения
		
	};//////////////////////////////////////////////ALLOCATOR END////////////////////////////////



	template<class T, class Alloc = Allocator<T> > ///////////////////////////////////VECTOR///////////////////////
	class vector
	{
	private:
		size_t _capacity = 0; // вместимость (реальное количество доступных ячеек)
		size_t _size = 0;     // предпологаемый пользователем размер вектора
		T* ptr = nullptr;    // указатель на аллоцированную память
		Alloc allocator;     // экземпляр аллокатора для управления памятью 
		
	public:
		
			template<class U=T>
			class iterator //////////////////////////////////////////////////////////ITERATOR/////////////
			{
				friend class vector; // полный доступ для контейнера в класс итератора

				using iterator_category = std::random_access_iterator_tag;  //////////////
				using value_type = void;									//
				using difference_type = void;								//	блок для коректной работы итератора с стандартными иструментами с++
				using pointer = void;										//
				using reference = void;										/////////////

			private:
				U* ptr;			 // указатель на ячейку контейнера
				size_t position; // переменная для понимания где сейчас итератор относительно _size контейнера
			public:
				iterator(U* ptr);								 // конструктор принимающий указатель на ячейку контейнера
				bool operator == (const iterator<U>& ptr) const; // оператор равенства
				bool operator != (const iterator<U>& ptr) const; // оператор неравенства
				bool operator > (const iterator<U>& ptr) const;  // оператор больше
				T& operator * () const;							 // оператор разиминования 
				iterator<U>& operator++();						 // префексный инкримент итератора
				iterator<U>& operator--();						 // префексный дикримент итератора
				iterator<U> operator++(int);					 // постфиксный инкримент итератора
				iterator<U> operator--(int);					 // постфиксный дикримент итератора
				void operator=(U& value);						 // оператор присваивания копированием
				iterator<U> operator - (const int x);			 // оператор минус
				iterator<U> operator + (const int x);			 // оператор плюс
				int get_position() const;						 // возвращает позицию оносительно size контейнера
			private:
				iterator(U* ptr,int pos);						 // конструктор для коректной работы end() контейнера

			};////////////////////////////////////////////////////////////////////ITERATOR END/////////////

			vector();									  // конструктор без параметров выделяет память на 8 ячеек
			vector(size_t size, const T& val = T());	  // конструктор выделяющий size ячеек а capacity size*2 если size>8 или = 8  при size<8
			vector(const vector& vect);					  // конструктор копирования с полным копированием данных масивва и аллокацией нового
			~vector();									  // деструктор
			vector(vector&& vect)noexcept;				  // конструктор перемещения
			vector(const std::initializer_list<T>& list); // конструктор с параметром листа инициализации

			iterator<T> begin() const;					  // итератор вернет ptr
			iterator<T> end() const;					  // итератор вернет ptr + count
			void	    push_back(const T& value);		  // обертка для emplace_back
			void	    resize(const size_t x);			  // с возможностью ручной и автоматической реалокации через увеличение вектора методами вектора
			void	    clear();						  // очистка памяти и сброс членов контейнера
			size_t	    size() const;					  // вернет количество элементов в контейнере
			void	    assign(const size_t new_size, const T& value); // заполняет контейнер значением value в количестве ячеек size
			T		    at(const size_t value) const;	  // возвращает данные подобно [] но в случает пустой ячейки вернет  throw std::out_of_range
			void	    emplace_back(T&& value);		  // перемещение в конец контейнера

#ifdef C14
			void emplace_back(T& value);		// перемещение в конец контейнера через  variadic template
			template<class...Args>
			void emplace_back(T&& val, Args && ... args); 
#endif 
#ifdef C17
			template<class...Args>
		    void emplace_back(Args && ... args); // перемещение в конец контейнера через fold expression
#endif 

			void emplace(vector<T, Alloc>::iterator<T>& _where, T&& value); // перемещение через итератор контейнера
			template<class...Args>
			void emplace(vector<T, Alloc>::iterator<T> &_where,Args && ... args); // перемещение через итератор контейнера с fold expression
			void insert(vector<T, Alloc>::iterator<T>& _where, T&& value); // обертка для emplace()
			void pop_back(); // удаляет последний элемент в контейнере
			void erase(vector<T, Alloc>::iterator<T>& first, vector<T, Alloc>::iterator<T>& last); // удаляет элементы с first по last 
			void erase(vector<T, Alloc>::iterator<T>& first); // удаляет элементы с first по конец контейнера


			T      front()    const;		 // вернет первый элемент контейнера
			T      back()     const;		 // вернет последний элемент контейнера
			T*     data()     const;		 // вернет указатель контейнера
			size_t capacity() const;		 // вернет вместимость контейнера
			bool   empty()    const;		 // проверка на опустошенность контейнера
			void   shrink_to_fit();			 // освобождает неиспользованную память уменьшая capacity
			void   swap(vector& vect);		 // меняет два контейнера местами
			void   reserve(const size_t x);  // резервирует память на будующие

			T&     operator [] (const size_t position) const;		   // вернет данные внутри ячейки по индексу position
			vector& operator = (const vector<T, Alloc>& vect);         // оператор присваивания копированием
			vector& operator = ( vector<T, Alloc>&& vect) noexcept ;   // оператор присваивания перемещением
			vector& operator = (const std::initializer_list<T>& list); // оператор присваивания перемещением листа инициализации
			 
			bool operator >  (vector<T, Alloc>& vect); // оператор сравнения
			bool operator <  (vector<T, Alloc>& vect); // оператор сравнения
			bool operator == (vector<T, Alloc>& vect); // оператор сравнения
			bool operator != (vector<T, Alloc>& vect); // оператор сравнения
			bool operator >= (vector<T, Alloc>& vect); // оператор сравнения
			bool operator <= (vector<T, Alloc>& vect); // оператор сравнения
	};
	
}////////////////////////////////////////////////////VECTOR END/////////////////////////////////////////



///////////////////////////блок Аллокатора/////////////////////////////////////////


template<class T>
my::Allocator<T>::Allocator()
{
}

template<class T>
my::Allocator<T>::Allocator(const Allocator& copy_alloc)
{
	this->_ptr = copy_alloc._ptr;
}

template<class T>
T* my::Allocator<T>::allocate(const size_t size)
{
	_ptr = new T[size];
	return _ptr;
}



template<class T>
void my::Allocator<T>::deallocate()
{
	if (_ptr == nullptr)
		return;
	else
	{
		delete[] _ptr;
		_ptr = nullptr;
	}
}



template<class T>
 size_t my::Allocator<T>::max_size() const
{
	return static_cast<size_t>(-1) / sizeof(T);
}

template<class T>
 bool my::Allocator<T>::operator==(const Allocator<T>& alloc)
{
	return this->_ptr == alloc._ptr;
}

template<class T>
 bool my::Allocator<T>::operator!=(const Allocator<T>& alloc)
{
	 return this->_ptr != alloc._ptr;
}

////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////блок Вектора/////////////////////////////////////////////////////

template<class T, class Alloc>
my::vector<T, Alloc>::vector()
{
#ifdef debag
	std::cout << " \n" << "Конструктор по дефолту" << " \n";
#endif
}

template<class T, class Alloc>
my::vector<T, Alloc>::vector(size_t size, const T& val) :_size(size)
{
	
	
	auto lamb = [&]() ->int
	{
		int x=8;
		while (true)
		{
			if (x > size)
				break;
			x *= 2;
		}

		return x;
	};

	_capacity = lamb();

	ptr = allocator.allocate(_capacity);

	for (int i = 0; i < size; i++)
	{
		ptr[i] = std::move(val);
	}
#ifdef debag
	std::cout << " \n" << "Конструктор с аргументами" << " \n";
#endif
}

template<class T, class Alloc>
 my::vector<T, Alloc>::vector(const vector& vect)
{
	this->ptr = this->allocator.allocate(vect._capacity);
	this->_capacity = vect._capacity;
	this->_size = vect._size;

	for (int i = 0; i < vect._size; i++)
	{
		this->ptr[i] = vect.ptr[i];
	}
#ifdef debag
	std::cout << " \n" << "Конструктор копирования" << " \n";
#endif

}

 template<class T, class Alloc>
  my::vector<T, Alloc>::~vector()
 {
	  if (ptr != nullptr)
	  {
		  allocator.deallocate();
#ifdef debag
		  std::cout << " \n" << "Деструктор сработал " << " \n";
#endif
		  return;
	  }

#ifdef debag

	  std::cout << " \n" << "Деструктор запустился но не сработал " << " \n";
#endif
 }


 template<class T, class Alloc>
  my::vector<T, Alloc>::vector(vector&& vect) noexcept
 {
	  _size = vect._size;
	  _capacity = vect._capacity;
	  allocator = vect.allocator;
	  ptr = vect.ptr;
	  vect.ptr = nullptr;
#ifdef debag
	  std::cout << " \n" << "Конструктор перемещения" <<  " \n";
#endif
 }

  template<class T, class Alloc>
  my::vector<T, Alloc>::vector(const std::initializer_list<T>& list)
	:my::vector<T, Alloc>(list.size())
  {
	  clear();
	  for (auto & l : list)
	  {
		  this->push_back(l);
	  }
  }




template<class T, class Alloc>
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::begin() const
{

	return my::vector<T, Alloc>::iterator<T>(ptr);
}

template<class T, class Alloc>
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::end() const
{

	return my::vector<T, Alloc>::iterator<T>(ptr + _size, _size + 1);
}

template<class T, class Alloc>
 void my::vector<T, Alloc>::push_back(const T& value)
{
	 T val = value;
	 emplace_back(std::move(val));
}

 template<class T, class Alloc>
 void my::vector<T, Alloc>::resize(const size_t x) 
 {
	 if (x < _capacity && x != _capacity)  // ручная вниз (resize)
	 {
		 _size = x;
	 }
	 else if (x > _capacity) //ручная вверх (resize)
	 {
		 T* new_ptr = allocator.allocate(x);

		 for (int i = 0; i < _size; i++)
		 {
			 new_ptr[i] = ptr[i];
		 }

		 delete[] ptr;
		 ptr = new_ptr;

		 _capacity = x;
		 _size = x;
	 }
	 else if(x== _capacity)  // реалокация от добавления
	 {
		 if (_capacity == 0)
		 {
			 ptr = allocator.allocate(1);
			 _capacity = 1;
		 }
		 else
		 {
			 T* new_ptr = allocator.allocate(x * 2);

			 for (int i = 0; i < x; i++)
			 {
				 new_ptr[i] = std::move(ptr[i]);
			 }

			 if (ptr != nullptr)
				 delete[] ptr;

			 ptr = new_ptr;

			 _capacity *= 2;
		 }
	 }
 }

 template<class T, class Alloc>
 void my::vector<T, Alloc>::clear()
 {
	 allocator.deallocate();
	 _size = 0;
	 _capacity = 0;

 }


 template<class T, class Alloc>
 size_t my::vector<T, Alloc>::size() const
 {
	 return _size;

 }

 template<class T, class Alloc>
  void my::vector<T, Alloc>::assign(const size_t size, const T& value)
 {
	  clear();
	  for (int i = 0; i < size; i++)
	  {
		  push_back(value);
	  }
 }


 


  template<class T, class Alloc>
   T my::vector<T, Alloc>::at(const size_t value) const
  {
	   if (value < _size)
		   return ptr[value];
	   else
		   throw std::out_of_range("Выход за пределы массива");
  }
   

   template<class T, class Alloc>
    void my::vector<T, Alloc>::emplace_back(T&& value)
   {
		if (_size >= _capacity)
		{
			resize(_capacity);
		}
		ptr[_size] = std::move(value);

		_size++;
   }

#ifdef C14   // variadic templates

	template<class T, class Alloc>
	void my::vector<T, Alloc>::emplace_back(T& value)
	{
		emplace_back(std::forward<T&&>(value));
	}

	template<class T, class Alloc>
	template<class ...Args>
	 void my::vector<T, Alloc>::emplace_back(T&& val,Args && ... args)
	{
		  emplace_back(std::forward<T&&>(val) );
		  emplace_back(std::forward<Args>(args)...);
	}

#endif // C14

#ifdef C17 // fold expression

	 template<class T, class Alloc>
	 template<class ...Args>
	 void my::vector<T, Alloc>::emplace_back(Args && ... args)
	 {
		 (emplace_back(std::forward<T&&>(args)),...);
	 }

#endif // C17

	


	  template<class T, class Alloc>
	   void my::vector<T, Alloc>::emplace(vector<T, Alloc>::iterator<T> &_where, T&& value)
	  {
		   int beg_end = 0;
		   vector<T, Alloc>::iterator<T> new_where = begin();
		  
		   if (_where == begin())
		   {
			   beg_end = 1;
		   }
		   else if (_where == end())
		   {
			   beg_end = 2;
		   }

		  if (_size >= _capacity)
		  {

			  resize(_capacity);

			  
		  }


		  switch (beg_end)
		  {
		  case 0:
			  new_where = begin();
			  _where = new_where + _where.get_position();
			  break;

		  case 1:
			  _where = begin();
			  break;
		  case 2:
			  return;
		  }


		 auto _end = end();
		 auto _befor_end = end();
		 --_befor_end;

		  for (; _end != _where; --_end )
		  {
			 
			  *_end = std::move(*_befor_end);
			  --_befor_end;
		  }
		  *_where = std::move(value);

		  _size++;
	  }

	   template<class T, class Alloc>
	   template<class ...Args>
	   void  my::vector<T, Alloc>::emplace(vector<T, Alloc>::iterator<T> &_where, Args && ...args)
	   {
		   if (_where != nullptr &&   end() > _where)
		   {
			  ( emplace(_where, std::forward<Args>(args)),...);
			   
			   
		   }
		  
	   }

	   template<class T, class Alloc>
	    void my::vector<T, Alloc>::insert(vector<T, Alloc>::iterator<T>& _where, T&& value)
	   {
			if (_where != nullptr && end() > _where)
			   emplace(_where, std::forward<T&&>(value));
	   }

		template<class T, class Alloc>
		void my::vector<T, Alloc>::pop_back()
		{
			if (_size > 0)
			{
				ptr[_size-1].~T();
				_size--;
			}
		}

		template<class T, class Alloc>
		 void my::vector<T, Alloc>::erase(vector<T, Alloc>::iterator<T>& first, vector<T, Alloc>::iterator<T>& last)
		{
		
			 int iter=0;
			 if(last> first && last.get_position() <= this->size()) 
			   for (; first != last; ++first)
			   {
			  	   *first = std::move(*end());
				   iter++;
			   }
			 _size -= iter;
		}

		template<class T, class Alloc>
		 void my::vector<T, Alloc>::erase(vector<T, Alloc>::iterator<T>& first)
		{
			
			 int iter = 0;
			 for (; first != end(); ++first)
			 {
				 *first = std::move(*end());
				 iter++;
			 }
			 _size -= iter;
		}


	template<class T, class Alloc>
	T my::vector<T, Alloc>::front() const
	{
		return ptr[0];
	}

	template<class T, class Alloc>
	T my::vector<T, Alloc>::back() const
	{
		return ptr[_size-1];
	}

	template<class T, class Alloc>
	T* my::vector<T, Alloc>::data() const
	{
		return ptr;
	}

	template<class T, class Alloc>
	 size_t my::vector<T, Alloc>::capacity() const
	{
		return _capacity;
	}

	 template<class T, class Alloc>
	  bool my::vector<T, Alloc>::empty() const
	 {
		 return _size == 0;
	 }


	 template<class T,class Alloc>
	 void my::vector<T,Alloc>::shrink_to_fit()
	 {
		 if (_size == _capacity)
			 return;

		 T* new_ptr = allocator.allocate(_size);
		 _capacity = _size;

		 for (int i = 0; i < _size; i++)
		 {
			 new_ptr[i] = std::move(ptr[i]);
		 }

		 delete[] ptr;

		 ptr = new_ptr;
	 }

	 template<class T, class Alloc>
	 void my::vector<T, Alloc>::swap(vector& vect)
	 {
		 vector<T, Alloc> temp;
		 temp = std::move(*this);
		 *this = std::move(vect);
		 vect = std::move(temp);
	 }

	 template<class T, class Alloc>
	 void my::vector<T, Alloc>::reserve(const size_t x)
	 {
		 if (x == _capacity||x < _capacity)
			 return;

		 T* new_ptr = allocator.allocate(x);
		 _capacity = x;

		 for (int i = 0; i < _size; i++)
		 {
			 new_ptr[i] = std::move(ptr[i]);
		 }

		 delete[] ptr;

		 ptr = new_ptr;
	 }


   template<class T, class Alloc>
   T& my::vector<T, Alloc>::operator[](const size_t position) const
   {
	   return ptr[position];
   }

   template<class T, class Alloc>
   my::vector<T,Alloc>& my::vector<T, Alloc>::operator =(const vector<T, Alloc>& vect) 
   {
	   allocator.deallocate();
	   _size = vect._size;
	   _capacity = vect._capacity;
	   ptr = allocator.allocate(_size);

		   for (int i = 0; i < size(); i++)
		   {
			   ptr[i] = vect.ptr[i];
		   }

#ifdef debag
		   std::cout << " \n" << "Присвоение копированием" << " \n";
#endif
	   return *this;
   }

   template<class T, class Alloc>
   my::vector<T, Alloc>& my::vector<T, Alloc>::operator =( vector<T, Alloc>&& vect) noexcept
   {
	   allocator.deallocate();
	   _size = vect._size;
	   _capacity = vect._capacity;
	   allocator = vect.allocator;
	   vect.allocator._ptr = nullptr;
	   ptr = vect.ptr;
	   vect.ptr = nullptr;
#ifdef debag
	   std::cout << " \n" << "Присвоение перемещением" << " \n";
#endif
	   return *this;
   }
   template<class T, class Alloc>
   my::vector<T, Alloc>& my::vector<T, Alloc>::operator=(const std::initializer_list<T>& list)
   {
	   clear();
	   for (auto& l : list)
	   {
		   this->push_back(l);
	   }
	   return *this;
   }
   template<class T, class Alloc>
   bool my::vector<T, Alloc>::operator>(vector<T, Alloc>& vect)
   {
		   if constexpr (my::func::is_same<decltype(*this),decltype(vect)>::value)
		   {


			   for (int i=0; i < _size; i++)
			   {
				   if (this->ptr[i] == vect.ptr[i])
					   continue;

				   if (this->ptr[i] > vect.ptr[i])
					   return 1;
			   }
			   return 0;
		   }
		   else
		   {
			   return 0;
		   }
   }

   template<class T, class Alloc>
   inline bool my::vector<T, Alloc>::operator<(vector<T, Alloc>& vect)
   {
	   return !(*this > vect);
   }

   template<class T, class Alloc>
   inline bool my::vector<T, Alloc>::operator==(vector<T, Alloc>& vect)
   {
	   if (this->_size != vect._size)
		   return 0;

	   if constexpr (my::func::is_same<decltype(*this), decltype(vect)>::value)
	   {
		   for (int i = 0; i < _size; i++)
		   {
			   if (this->ptr[i] == vect.ptr[i])
				   continue;

				   return 0;
		   }
		   return 1;
	   }
	   else
	   {
		   return 0;
	   }

   }

   template<class T, class Alloc>
   inline bool my::vector<T, Alloc>::operator!=(vector<T, Alloc>& vect)
   {
	   return !(*this == vect);
   }

   template<class T, class Alloc>
   inline bool my::vector<T, Alloc>::operator>=(vector<T, Alloc>& vect)
   {
	   return (*this > vect || *this == vect);
   }

   template<class T, class Alloc>
   inline bool my::vector<T, Alloc>::operator<=(vector<T, Alloc>& vect)
   {
	   return ( *this < vect || *this == vect); 
   }


 /////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////Итератор вектора////////////////////


template<class T, class Alloc>
template<class U>
 my::vector<T, Alloc>::iterator<U>::iterator(U* ptr):ptr(ptr)
{
}



template<class T, class Alloc>
template<class U>
 bool my::vector<T, Alloc>::iterator<U>::operator==(const iterator<U>& ptr) const
{
	return this->ptr == ptr.ptr;
}
 

template<class T, class Alloc>
template<class U>
 bool my::vector<T, Alloc>::iterator<U>::operator!=(const iterator<U>& ptr) const
{
	return this->ptr != ptr.ptr;
}


 template<class T, class Alloc>
 template<class U>
  bool my::vector<T, Alloc>::iterator<U>::operator>(const iterator<U>& ptr) const
 {
	 return this->ptr > ptr.ptr;
 }

template<class T, class Alloc>
template<class U>
 T& my::vector<T, Alloc>::iterator<U>::operator*() const
{
	 if(ptr!=nullptr)
	return *ptr;
}

 template<class T, class Alloc>
 template<class U>
 my::vector<T, Alloc>::iterator<U>& my::vector<T, Alloc>::iterator<U>::operator++()
 {
	 ptr++;
	 position++;
	 return *this;
 }

 template<class T, class Alloc>
 template<class U>
 my::vector<T, Alloc>::iterator<U>& my::vector<T, Alloc>::iterator<U>::operator--()
 {
	 ptr--;
	 position--;
	 return *this;
 }


 template<class T, class Alloc>
 template<class U>
 my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator++ (int)
 {
	 auto temp = *this;
	 ptr++;
	 position++;
	 return temp;
 }


 template<class T, class Alloc>
 template<class U>
 my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator--(int)
 {
	 auto temp = *this;
	 ptr--;
	 position--;
	 return temp;
 }

 
 template<class T, class Alloc>
 template<class U>
  void my::vector<T, Alloc>::iterator<U>::operator=(U& value)
 {
	  *ptr = value;
 }


  template<class T, class Alloc>
  template<class U>
  my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator-(const int x) 
  {
	 
	  auto temp = this;
	  this->ptr -= x;
	  this->position -= x;
	  return *temp;
  }

  template<class T, class Alloc>
  template<class U>
  my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator+(const int x) 
  {
	  auto temp = this;
	  this->ptr += x;
	  this->position += x;
	  return *temp;
  }

  
  template<class T, class Alloc>
  template<class U>
  inline int my::vector<T, Alloc>::iterator<U>::get_position() const
  {
	  return position;
  }

  template<class T, class Alloc>
  template<class U>
  my::vector<T, Alloc>::iterator<U>::iterator(U* ptr,int pos) :ptr(ptr),position(pos) 
  {
	 
  }


  void w() = delete;
	  //Ошибка(активно)	E2923	Предупреждение PCH : конец заголовка не в области видимости файла.PCH - файл IntelliSense не был создан.
  

