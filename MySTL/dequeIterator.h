#pragma once
#ifndef _MYSTL_DEQUEITERATOR_H_
#define _MYSTL_DEQUEITERATOR_H_
//codeby 20221030
#include "iterator.h"

namespace mystl {
	template<class T, class Ref, class Ptr, size_t BufSize>
	struct _deque_iterator {
		typedef _deque_iterator<T, T&, T*, BufSize> iterator;
		typedef _deque_iterator<const T, const T&, const T*, BufSize> const_iterator;

		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;   
		typedef Ptr pointer;    
		typedef Ref reference;  
		typedef size_t size_type;  
		typedef ptrdiff_t difference_type;
		typedef pointer* map_pointer;

		typedef _deque_iterator self;

		//与容器保持连接
		T* cur;    //迭代器所指向缓冲区现行元素
		T* first;  //迭代器所指的缓冲区的头
		T* last;   //迭代器所指的缓冲区的尾
		map_pointer node;

		static size_type buffer_size() { return _deque_buf_size(BufSize, sizeof(T)); }
		
		static size_type _deque_buf_size(size_type n, size_type sz) {
			return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
		}

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		reference operator*()const { return *cur; }
		pointer operator->()const { return &(operator*()); }

		difference_type operator-(const self& x)const {
			return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		
		self& operator++() {
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}

		self operator++(int) {
			self tem = *this;
			++* this;
			return tem;
		}

		self& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			cur--;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}

		self& operator+=(difference_type n) {
			difference_type diff = n + cur - first;
			if (diff < (difference_type)buffer_size() && diff > 0) {
				cur += n;
				return *this;
			}

			if (diff > 0) {
				difference_type left = n - (last - cur);
				difference_type node_diff = left / (difference_type)buffer_size();
				set_node(node + node_diff);
				cur = left % (difference_type)buffer_size();
				return *this;
			}

			if (diff < 0) {
				difference_type left = n - (cur - first);
				difference_type node_diff = left / (difference_type)buffer_size();
				set_node(node - node_diff);
				cur = last - (left % (difference_type)buffer_size());
				return *this;
			}
		}

		self operator+(difference_type n) const {
			self tem = *this;
			return tem += n;
		}

		self& operator-=(difference_type n) { return *this += -n; }

		self operator-(difference_type n) const {
			self tmp = *this;
			return tmp -= n;
		}

		//reference operator[](difference_type n) const { return *(*this + n);}

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return cur != x.cur; }
		bool operator<(const self& x) const {
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}


	};




}//!namespace mystl
#endif // ! _MYSTL_DEQUEITERATOR_H_

