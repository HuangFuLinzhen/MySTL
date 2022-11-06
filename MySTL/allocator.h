#pragma once


#ifndef _MYSTL_ALLOCATOR_
#define _MYSTL_ALLOCATOR_

//codeby 20221024

namespace mystl {


	//code by _20221024_
	template<class T>
	class Allocator {
	public:
		typedef T            value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t       size_type;
		typedef ptrdiff_t    difference_type;

	public:
		static T* allocate();
		static T* allocate(size_type n);

		static void deallocate(T* ptr);
		static void deallocate(T* ptr, size_type n);

		static void construct(T* ptr);
		static void construct(T* ptr, const T& value);
		static void construct(T* ptr, T&& value);

		template <class... Args>
		static void construct(T* ptr, Args&& ...args);

		inline void destroy(T* ptr);
		static void destroy(T* first, T* last);
	};



	template<class T>
	inline T* Allocator<T>::allocate()
	{
		return static_cast<T*>(::operator new(sizeof(T)));
	}

	template<class T>
	inline T* Allocator<T>::allocate(size_type n)
	{
		if (0 == n) {
			return nullptr;
		}
		return static_cast<T*>(::operator new(sizeof(T) * n));
	}

	template<class T>
	inline void Allocator<T>::deallocate(T* ptr)
	{
		::operator delete(ptr);
	}

	template<class T>
	inline void Allocator<T>::deallocate(T* ptr, size_type /* n */)
	{
		::operator delete(ptr);
	}

	template<class T>
	inline void Allocator<T>::construct(T* ptr)
	{
		::new(ptr) T();
	}

	template<class T>
	inline void Allocator<T>::construct(T* ptr, const T& value)
	{
		::new(ptr) T(value);
	}

	template<class T>
	inline void Allocator<T>::construct(T* ptr, T&& value)
	{
		::new(ptr) T(std::move(value));
	}

	template<class T>
	inline void Allocator<T>::destroy(T* ptr)
	{
		ptr->~T();
	}

	template<class T>
	inline void Allocator<T>::destroy(T* first, T* last)
	{
		for (; first != last; ++first)
			first->~T();
	}

	template<class T>
	template<class ...Args>
	inline void Allocator<T>::construct(T* ptr, Args && ...args)
	{
		::new(ptr) T(std::forward<Args>(args)...);
	}

}

#endif // !_MYSTL_ALLOCATOR_
