#pragma once


#ifndef _MTSTL_ITERATOR_H
#define _MTSTL_ITERATOR_H

//code by 20221025
#include <cstddef>
namespace mystl {

	//迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template <class T, class Distance>
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef Distance					difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template<class Categoty, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
		struct iterator {
		typedef Categoty iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};

	//traits特征提取
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference 			reference;
	};

	//针对原生指针而设计的traits偏特化
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	//pointer to const 的特化版本
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag 	iterator_category;
		typedef T 							value_type;
		typedef ptrdiff_t 					difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	//判断iterator类型
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& it) {
		typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	//判断迭代器的value_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator& it) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//获取迭代器的distance_type
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		difference_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}






};//!namespace mystl
#endif // !_MTSTL_ITERATOR_H

