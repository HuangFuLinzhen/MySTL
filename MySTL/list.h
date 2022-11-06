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
		link_type node;  //list��һ����״˫������һ��ָ�뼴�ɱ�ʾ������״˫������ָ��β�˵Ŀհ׽ڵ�
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

		//����һ���ڵ㣬���Ҵ���Ԫ��ֵ
		link_type create_node(const T& x) {
			link_type newListNode = get_node();
			construct(&newListNode->data, x); //��ָ�����ݣ�����
			return newListNode;
		}

		//����һ��Ԫ�ص� �������ͷ�
		void destroy_node(link_type d) {
			destroy(&d->data);
			
			put_node(d);
		}
	public:

		list() { empty_initialize(); } //�����յ�����
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


		//β������ڵ�
		void push_back(const T& x) { //��listβ����Ԫ�ش��������Ԫ��
			insert(end(), x);
		}
		//ͷ������ڵ�
		void push_front(const T& x) {
			insert(begin(), x);
		}

		//ɾ��ͷ���ڵ�
		void pop_front() {
			erase(begin());
		}
		//ɾ��β���ڵ�
		void pop_back() {
			erase(--end());
		}

		//�������list�ڵ�
		void clear() {
			iterator s = begin();
			while (s != end()) {
				s = erase(s);
			}
			//��list��״̬
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


