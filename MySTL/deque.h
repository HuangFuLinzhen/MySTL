#pragma once
#ifndef  _MYSTL_DEQUE_H_
#define _MYSTL_DEQUE_H_

//codeby 20221030

#include "allocator.h"
#include "dequeIterator.h"
#include "_uninitialized.h"
#include "construct.h"
#include "_algorithm.h"

namespace mystl {
	template<class T, class alloc_ = Allocator<T>, size_t BufSize = 0>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef _deque_iterator<T, T&, T*, BufSize> iterator;

		typedef pointer* map_pointer;

		//配置一个元素大小,value
		typedef Allocator<value_type> data_allocator;
		//配置一个指针大小,map
		typedef Allocator<pointer> map_allocator;

	protected:
		iterator start;
		iterator finish;

		//data member
		map_pointer map;      //map首地址
		size_type map_size;   //map存储的大小


	public:
		deque() :start{}, finish{}, map_size{ 0 }, map{ 0 }{
			create_map_and_nodes(0);
		}
		deque(int n, const value_type& x) :start{}, finish{}, map_size{ 0 }, map{ 0 }{
			fill_initialize(n, x);
		}
		~deque() {

		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		reference operator[] (size_type n) {
			return *(start + n);
		}

		reference front() { return *start; }
		reference back() {
			return *(finish - 1);
		}

		size_type size() const { return finish - start; }

		size_type max_size() const { return size_type(-1); }
		bool empty() const { return finish == start; }

		void push_back(const value_type& t) {
			if (finish.cur != finish.last - 1) {
				construct(finish.cur, t);
				++finish.cur;
			}
			else { //需要配置新的缓冲区
				push_back_aux(t);
			}
		}

		void push_front(const value_type& t) {
			if (start.cur != start.first) {
				construct(start.cur - 1, t);
				--start.cur;
			}
			else {//第一缓冲区已经没有备用空间了
				push_front_aux(t);
			}
		}

		void pop_back() {
			if (finish.cur != finish.first) {
				--finish.cur;
				destroy(finish.cur);
			}
			else {
				pop_back_aux();
			}
		}

		void clear() {
			for (map_pointer node = start.node + 1; node < finish.node; ++node) {
				destroy(*node, *node + buffer_size());
				data_allocator::deallocate(*node, buffer_size());
			}

			if (start.node != finish.node) {
				destroy(start.cur, start.last);
				destroy(finish.first, finish.cur);
				data_allocator::deallocate(finish.first, buffer_size()); //保留头缓冲区
			}
			else {
				destroy(start.cur, finish.cur); //将缓冲区内的元素全部析构
			}
			finish = start;// 调整状态
		}

	protected:

		static size_t buffer_size() { return _deque_buf_size(BufSize, sizeof(T)); }

		static size_t _deque_buf_size(size_t n, size_t sz) {
			return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
		}
		pointer alloc_node() {
			return data_allocator::allocate(buffer_size());
		}
		void dealloc_node(pointer ptr) {
			data_allocator::deallocate(ptr);
		}
		int initial_map_size() {
			return 8;
		}

		void fill_initialize(size_type n, const value_type& value) {
			create_map_and_nodes(n);
			map_pointer cur;
			for (cur = start.node; cur < finish.node; ++cur)
				//已经获得了内存，构造buffer的开始节点，结束节点，初始值
				unitialized_fill(*cur, *cur + buffer_size(), value);
			unitialized_fill(finish.first, finish.cur, value);
		}

		void create_map_and_nodes(size_type num_elements) {
			size_type num_nodes = num_elements / buffer_size() + 1;
			map_size = max(initial_map_size(), num_nodes + 2);
			map = map_allocator::allocate(map_size);

			map_pointer nstart = map + (map_size - num_nodes) / 2;
			map_pointer nfinish = nstart + num_nodes - 1;
			map_pointer cur;

			for (cur = nstart; cur <= nfinish; cur++) {
				*cur = alloc_node();
			}
			start.set_node(nstart);
			finish.set_node(nfinish);
			start.cur = start.first;
			finish.cur = finish.first + num_elements % buffer_size();
		}

		void reserve_map_at_back(size_type node_to_add = 1) {
			if (node_to_add + 1 > map_size - (finish.node - map)) {
				reallocate_map(node_to_add, false);
			}
		}
		void reserve_map_at_front(size_type node_to_add = 1) {
			if (node_to_add > start.node - map) {
				reallocate_map(node_to_add, true);
			}
		}
		void reallocate_map(size_type node_to_add, bool add_at_front) {

			size_type old_num_nodes = finish.node - start.node + 1;
			size_type new_num_nodes = old_num_nodes + node_to_add;

			map_pointer new_nstart;
			if (map_size > 2 * new_num_nodes) {
				//偏到一边去了，把他移到正中间来
				new_nstart = map + (map_size - new_num_nodes) / 2
					+ (add_at_front ? node_to_add : 0);
				if (new_nstart < start.node)
					copy(start.node, finish.node + 1, new_nstart);
				else
					copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
			}
			else {
				size_type new_map_size = map_size + std::max(map_size, node_to_add) + 2;
				map_pointer new_map = map_allocator::allocate(new_map_size);
				new_nstart = new_map + (new_map_size - new_num_nodes) / 2
					+ (add_at_front ? node_to_add : 0);
				copy(start.node, finish.node + 1, new_nstart);
				map_allocator::deallocate(map, map_size);
				map = new_map;
				map_size = new_map_size;
			}

			start.set_node(new_nstart);
			finish.set_node(new_nstart + old_num_nodes - 1);
		}
		void pop_back_aux() {
			deallocate_node(finish.first);
			finish.set_node(finish.node - 1);
			finish.cur = finish.last - 1;
			destroy(finish.cur);
		}
		void pop_front_aux() {
			deallocate_node(start.last);
			start.set_node(start.node + 1);
			start.cur = start.first;
			destroy(start.cur);
		}
		void push_back_aux(const value_type& t) {
			value_type t_copy = t;
			reserve_map_at_back(); //是否需要重换一个map
			*(finish.node + 1) = alloc_node(); //配置新的map节点

			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}

		void push_front_aux(const value_type& t) {
			value_type t_copy = t;
			reserve_map_at_front();
			*(start.node - 1) = alloc_node();

			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}


	};










}//namespace mystl
#endif // ! _MYSTL_DEQUE_H_
