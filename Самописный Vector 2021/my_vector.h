#pragma once
//#define debag  // ������������ ��� ������
#define C17 // �������� �� 14 ��� variadic templates
#pragma warning(disable : 4996)  // ��������� std::iterator

namespace my
{



	template<class T> //////////////////////////////////////////////ALLOCATOR////////////////////////////////
	class Allocator;

	template<class T>
	class Allocator  // �������� ������
	{
	public:
		T* pointer = nullptr; // ��������� �� ������������ ������ ������� ���������� ��� �������������
	public:
		Allocator();
		Allocator(const Allocator& copy_alloc); // ��������� �����������

		T* allocate(size_t size); // ���������� ��������� �� ������� ������  ������ size
		void   deallocate();
		size_t max_size() const;
		bool operator ==(const Allocator<T>& alloc);
		bool operator !=(const Allocator<T>& alloc);
		Allocator& operator = (const Allocator<T>&) = default;
		
	};				//////////////////////////////////////////////ALLOCATOR END////////////////////////////////



	template<class T, class Alloc = Allocator<T>> ///////////////////////////////////VECTOR///////////////////////
	class vector
	{

	private:
		size_t _capacity = 0; // ����������� (�������� ���������� ��������� �����)
		size_t _size = 0;     // �������������� ������������� ������ �������
		T* ptr = nullptr;    // ��������� �� �������������� ������
		Alloc allocator;     // ��������� ���������� ��� ���������� ������� 
		


	public:
		
			template<class U=T>
			class iterator : public std::iterator<std::forward_iterator_tag,T> /////////////////ITERATOR/////////////
			{
			private:
				U* ptr= nullptr;
				int position = 0;
			public:
				iterator(U* ptr);
				bool operator == (const iterator<U>& ptr);
				bool operator != (const iterator<U>& ptr);
				bool operator > (const iterator<U>& ptr);
				T& operator *  ();
				iterator<U>& operator++();
				iterator<U>& operator--();
				iterator<U> operator++(int);
				iterator<U> operator--(int);
				void operator=(U& value);
				iterator<U> operator - (int x);
				iterator<U> operator + (int x);
				int get_position();

			};																/////////////////ITERATOR END/////////////

			vector();											//  ����������� ��� ���������� �������� ������ �� 8 �����
			vector(size_t size, const Alloc& alloc = Alloc() );  //  ����������� ���������� size ����� � capacity size*2 ���� size>8 ��� = 8  ��� size<8
			vector(const vector& vect);		//  ����������� ����������� � ������ ������������ ������ ������� � ���������� ������
			~vector(); //����������
			vector(vector&& vect)noexcept; // ����������� �����������
			vector(const std::initializer_list<T>& list); // ����������� � ���������� ����� �������������

			iterator<T> begin(); // �������� ������ ptr
			iterator<T> end();   // �������� ������ ptr + count
			iterator<T> begin()const; // �������� ������ ptr
			iterator<T> end()const;   // �������� ������ ptr + count

			


			void push_back(const T& value); // ������� ��� emplace_back

			void resize(size_t x); // � ������������ ������ � �������������� ���������� ����� ���������� ������� �������� �������
			void clear(); 
			size_t size();
			void assign(size_t new_size, const T& value);
			T at(size_t value);
			void emplace_back(T&& value);

#ifdef C14
			void emplace_back(T& value);
			template<class...Args>
			void emplace_back(T&& val, Args && ... args);
#endif 
#ifdef C17
			template<class...Args>
			void emplace_back(Args && ... args);
#endif 


			

			template<class...Args>
			void emplace(vector<T, Alloc>::iterator<T> &_where,Args && ... args);
			void emplace(vector<T, Alloc>::iterator<T> &_where, T&& value);


			void insert(vector<T, Alloc>::iterator<T>& _where, T&& value);

			void pop_back();

			void erase(vector<T, Alloc>::iterator<T>& first, vector<T, Alloc>::iterator<T>& last);
			void erase(vector<T, Alloc>::iterator<T>& first);


			T front();
			T back();
			T* data();
			size_t capacity();
			bool empty();

			T& operator [](size_t position)const;
			vector& operator = (const vector<T, Alloc>& vect);
			vector& operator = ( vector<T, Alloc>&& vect) noexcept ; 
			vector& operator = (const std::initializer_list<T>& list);


	};

	//�����������  � ����� �� �����
	//��������  ���� �����  (�������)
	//�������� �����
	//��� ������  � �������� 
	// ������������� � stl   �������


	
}////////////////////////////////////////////////////VECTOR END/////////////////////////////////////////



using namespace my;







///////////////////////////���� ����������/////////////////////////////////////////


template<class T>
my::Allocator<T>::Allocator()
{
}

template<class T>
my::Allocator<T>::Allocator(const Allocator& copy_alloc)
{
	this->pointer = copy_alloc.pointer;
}

template<class T>
T* my::Allocator<T>::allocate(size_t size)
{
	pointer = new T[size];
	return pointer;
}



template<class T>
void my::Allocator<T>::deallocate()
{
	if (pointer == nullptr)
		return;
	else
	{
		delete[] pointer;
		pointer = nullptr;
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
	return this->pointer == alloc.pointer;
}

template<class T>
 bool my::Allocator<T>::operator!=(const Allocator<T>& alloc)
{
	 return this->pointer != alloc.pointer;
}





////////////////////////////////////////////////////////////////////////////////////////////














///////////////////////////���� �������/////////////////////////////////////////

template<class T, class Alloc>
my::vector<T, Alloc>::vector()
{
#ifdef debag
	std::cout << " \n" << "����������� �� �������" << " \n";
#endif
}

template<class T, class Alloc>
my::vector<T, Alloc>::vector(size_t size, const  Alloc& alloc) :_size(size), allocator(alloc)
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
		ptr[i] = 0;
	}
#ifdef debag
	std::cout << " \n" << "����������� � �����������" << " \n";
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
	std::cout << " \n" << "����������� �����������" << " \n";
#endif

}

 template<class T, class Alloc>
  my::vector<T, Alloc>::~vector()
 {
	  if (ptr != nullptr)
	  {
		  allocator.deallocate();
#ifdef debag
		  std::cout << " \n" << "���������� �������� " << " \n";
#endif
		  return;
	  }

#ifdef debag

	  std::cout << " \n" << "���������� ���������� �� �� �������� " << " \n";
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
	  std::cout << " \n" << "����������� �����������" <<  " \n";
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
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::begin()
{
	
	return my::vector<T, Alloc>::iterator<T>(ptr);
}

template<class T, class Alloc>
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::end()
{

	return my::vector<T, Alloc>::iterator<T>(ptr+ _size);
}

template<class T, class Alloc>
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::begin() const
{

	return my::vector<T, Alloc>::iterator<T>(ptr);
}

template<class T, class Alloc>
my::vector<T, Alloc>::iterator<T> my::vector<T, Alloc>::end() const
{

	return my::vector<T, Alloc>::iterator<T>(ptr + _size); 
}

template<class T, class Alloc>
 void my::vector<T, Alloc>::push_back(const T& value)
{

	/*if (_size >= _capacity)
	{
		resize(_capacity);
	}

	ptr[_size] = value;
	
	_size++;*/
	 T val = value;
	 emplace_back(std::move(val));

}


 template<class T, class Alloc>
 void my::vector<T, Alloc>::resize(size_t x)
 {
	 if (x < _capacity && x != _capacity)  // ������ ���� (resize)
	 {
		 _size = x;
	 }
	 else if (x > _capacity) //������ ����� (resize)
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
	 else if(x== _capacity)  // ���������� �� ����������
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
 size_t my::vector<T, Alloc>::size()
 {
	 return _size;

 }

 template<class T, class Alloc>
  void my::vector<T, Alloc>::assign(size_t new_size, const T& value)
 {
	  clear();
	  for (int i = 0; i < new_size; i++)
	  {
		  push_back(value);
	  }
 }


 


  template<class T, class Alloc>
   T my::vector<T, Alloc>::at(size_t value)
  {
	   if (value < _size)
		   return ptr[value];
	   else
		   throw std::out_of_range("����� �� ������� �������");
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
			 if(last>first) // ������ �������� �� ���� � ������
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
	T my::vector<T, Alloc>::front()
	{
		return ptr[0];
	}

	template<class T, class Alloc>
	T my::vector<T, Alloc>::back()
	{
		return ptr[_size-1];
	}

	template<class T, class Alloc>
	T* my::vector<T, Alloc>::data()
	{
		return ptr;
	}

	template<class T, class Alloc>
	 size_t my::vector<T, Alloc>::capacity()
	{
		return _capacity;
	}

	 template<class T, class Alloc>
	 inline bool my::vector<T, Alloc>::empty()
	 {
		 return _size == 0;
	 }




   template<class T, class Alloc>
   T& my::vector<T, Alloc>::operator[](size_t position) const
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
		   std::cout << " \n" << "���������� ������������" << " \n";
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
	   vect.allocator.pointer = nullptr;
	   ptr = vect.ptr;
	   vect.ptr = nullptr;
#ifdef debag
	   std::cout << " \n" << "���������� ������������" << " \n";
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
 ////////////////////////////////////////////////////////////////////////////











//////////////////////////////////////////�������� �������////////////////////


template<class T, class Alloc>
template<class U>
 my::vector<T, Alloc>::iterator<U>::iterator(U* ptr):ptr(ptr)
{
}

template<class T, class Alloc>
template<class U>
 bool my::vector<T, Alloc>::iterator<U>::operator==(const iterator<U>& ptr)
{
	return this->ptr == ptr.ptr;
}
 

template<class T, class Alloc>
template<class U>
 bool my::vector<T, Alloc>::iterator<U>::operator!=(const iterator<U>& ptr)
{
	return this->ptr != ptr.ptr;
}


 template<class T, class Alloc>
 template<class U>
  bool my::vector<T, Alloc>::iterator<U>::operator>(const iterator<U>& ptr)
 {
	 return this->ptr > ptr.ptr;
 }

template<class T, class Alloc>
template<class U>
 T& my::vector<T, Alloc>::iterator<U>::operator*()
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
  my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator-(int x)
  {
	 
	  auto temp = this;
	  this->ptr -= x;
	  this->position -= x;
	  return *temp;
  }

  template<class T, class Alloc>
  template<class U>
  my::vector<T, Alloc>::iterator<U> my::vector<T, Alloc>::iterator<U>::operator+(int x)
  {
	  auto temp = this;
	  this->ptr += x;
	  this->position += x;
	  return *temp;
  }

  
  template<class T, class Alloc>
  template<class U>
  inline int my::vector<T, Alloc>::iterator<U>::get_position()
  {
	  return position;
  }