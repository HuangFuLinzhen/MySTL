#pragma once
#ifndef _MYSTL_LIST_H_
#define _MYSTL_LIST_H_

//codeby 20221025
#include "listiterator.h"
#include "allocator.h"
#include "construct.h"

namespace mystl {
	template<class T>
	class list {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;

		typedef _list_node<T> list_node;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef list_node* link_type;

		typedef _list_iterator<T, T&, T*>   iterator;
		typedef Allocator<list_node> alloc;

	private:
		link_type node;  //list是一个环状双向链表，一个指针即可表示整个环状双向链表，指向尾端的空白节点
		size_type cursize;
		void empty_initialize() {
			node = get_node();
			--cursize;
			node->next = node;
			node->prev = node;
		}

		link_type get_node() {
			++cursize;
			return alloc::allocate();
		}

		void put_node(link_type x) {
			--cursize;
			alloc::deallocate(x);
		}

		//产生一个节点，并且带有元素值
		link_type create_node(const T& x) {
			link_type newListNode = get_node();
			construct(&newListNode->data, x); //所指的数据，创建
			return newListNode;
		}

		//销毁一个元素点 析构并释放
		void destroy_node(link_type d) {
			destroy(&d->data);
			
			put_node(d);
		}
	public:

		list() { empty_initialize(); } //产生空的链表
		~list() {
			clear();
			erase(end());
		}


		iterator begin() const { return node->next; }
		iterator end() const { return node; }
		bool empty() const { return node->next == node; }
		size_type size() const {
			return cursize;
		}

		reference front() { return *begin(); }
		reference back() { return *(--end()); }

		iterator erase(iterator position) {
			link_type next_node = position.node->next;
			link_type prev_node = position.node->prev;
			prev_node->next = next_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

		iterator insert(iterator position, const T& x) {
			link_type tmp = create_node(x);
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			position.node->prev->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}


		//尾部插入节点
		void push_back(const T& x) { //在list尾部空元素处添加数据元素
			insert(end(), x);
		}
		//头部插入节点
		void push_front(const T& x) {
			insert(begin(), x);
		}

		//删除头部节点
		void pop_front() {
			erase(begin());
		}
		//删除尾部节点
		void pop_back() {
			erase(--end());
		}

		//清除所有list节点
		void clear() {
			iterator s = begin();
			while (s != end()) {
				s = erase(s);
			}
			//空list的状态
			node->next = node;
			node->prev = node;
		}



		void swap(list& x) {
			link_type tmp = x.node;
			x.node = this->node;
			this->node = tmp;
		}




	};



}//!namespace mystl


#endif // !_MYSTL_LIST_H_


