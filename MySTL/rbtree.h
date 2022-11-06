#pragma once
#ifndef _MYSTL_RBTREE_H_
#define _MYSTL_RBTREE_H_

//codeby 20221103

#include "rbtreeIterator.h"
#include "allocator.h"
#include "construct.h"
#include "pair.h"

namespace mystl {

	template<class Key, class Value, class KeyOfValue, class Compare>
	class rb_tree {
	protected:
		typedef void* void_pointer;
		typedef _rb_tree_node_base* base_ptr;
		typedef _rb_tree_node<Value> rb_tree_node;
		typedef Allocator<rb_tree_node> rb_tree_node_alloc;

	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef const value_type& const_reference;

		typedef rb_tree_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef _rb_tree_color_type color_type;
		typedef value_type& reference;

	public:
		typedef _rb_tree_iterator<value_type, reference, pointer> iterator;
		typedef _rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

	protected:
		//rbtree ֻ���������ݱ���
		size_type node_count; //���ϵĽڵ����
		link_type header;  //ʵ���ϵ�a trick����root��Ϊ���ڵ㣬header�����Һ��Ӷ���root
		Compare key_compare; //����Ľڵ��ֵ��С�Ƚ�׼��

	public:

		rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
			init();
		}

		~rb_tree() {
			clear();
			put_node(header);
		}

	public:
		Compare key_comp() const { return key_compare; }
		iterator begin() const { return leftmost(); }
		iterator end() const { return header; } //RBtree���ص�Ϊheader��ָ��λ��
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type count() const { return node_count; }
		size_type max_size() const { return size_type(-1); }

	protected:
		link_type get_node() {
			return rb_tree_node_alloc::allocate();
		}
		void put_node(link_type ptr) {
			rb_tree_node_alloc::deallocate(ptr);
		}
		void destroy_node(link_type p) {
			destroy(&p->value_field);    //��������,�� value_field �ĵ�ַ�����ú���
			put_node(p);                 //�ͷ��ڴ�
		}

		link_type create_node(const value_type& x) {
			link_type tmp = get_node();  //���ÿռ�
			construct(&tmp->value_field, x); //��������
			return tmp;
		}

		link_type clone_node(link_type x) {  //��ֵһ��node
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = nullptr;
			tmp->right = nullptr;
			return tmp;
		}

		void clear(){}

		//�����ȡheader�ĳ�Ա
		link_type& root() const { return (link_type&)header->parent; }
		link_type& leftmost() const { return (link_type&)header->left; }
		link_type& rightmost() const { return (link_type&)header->right; }

		//����6��������������ȡ�ýڵ�x�ĳ�Ա
		static link_type& left(link_type x)
		{
			return (link_type&)(x->left);
		}

		static link_type& right(link_type x)
		{
			return (link_type&)(x->right);
		}

		static link_type& parent(link_type x)
		{
			return (link_type&)(x->parent);
		}

		static reference value(link_type x)
		{
			return x->value_field;
		}

		static const Key& key(link_type x)
		{
			return KeyOfValue() (value(x));
		}

		static color_type& color(link_type x)
		{
			return (color_type&)(x->color);
		}

		//����6��������������ȡ��x�ĳ�Ա
		static link_type& left(base_ptr x)
		{
			return (link_type&)(x->left);
		}

		static link_type& right(base_ptr x)
		{
			return (link_type&)(x->right);
		}

		static link_type& parent(base_ptr x)
		{
			return (link_type&)(x->parent);
		}

		static reference value(base_ptr x)
		{
			return ((link_type)x)->value_field;
		}

		static const Key& key(base_ptr x)
		{
			return KeyOfValue() (value(link_type(x)));
		}

		static color_type& color(base_ptr x)
		{
			return (color_type&)(link_type(x)->color);
		}

		static link_type minimum(link_type x) {
			return (link_type)_rb_tree_node_base::minimum(x);
		}

		static link_type maximum(link_type x) {
			return (link_type)_rb_tree_node_base::maximum(x);
		}


	

	private:

		//��ֵ�����         �����ĸ��ڵ�    ��ֵ
		iterator __insert(base_ptr x_, base_ptr y_, const Value& v) {
			link_type y = (link_type)y_;
			link_type x = (link_type)x_;
			link_type z;
			if (y == header || x != nullptr || key_compare(KeyOfValue()(v), key(y))) {
				z = create_node(v);
				left(y) = z;
				if (y == header) {
					root() = z;
					rightmost() = z;
				}
				else if (y == leftmost()) {
					leftmost() = z;
				}
			}
			else {
				z = create_node(v);
				right(y) = z;
				if (rightmost() == y) {
					rightmost() = z;
				}
			}
			parent(z) = y;
			left(z) = nullptr;
			right(z) = nullptr;

			__rb_tree_rebalance(z, header->parent);
			++node_count;
			return iterator(z);
		}

		void init() {
			header = get_node(); //����һ���ڵ�ռ䣬��headerָ����
			color(header) = __rb_tree_red; //headerΪ��ɫ������header��root

			root() = 0;
			leftmost() = header; //��header�����ӽڵ�Ϊ�Լ�
			rightmost() = header; //��header���ҽڵ�Ϊ�Լ�
		}

	public:
		//�������REtree�нڵ��һ�޶� �������ʧ�ܷ��ص���ָ��ýڵ�ĵ�������
		pair<iterator, bool> insert_unique(const value_type& v) {
			link_type y = header;
			link_type x = root();

			bool comp = true;

			while (x != nullptr) {
				y = x;
				comp = key_compare(KeyOfValue()(v), key(x));
				x = comp ? left(x) : right(x);
			}
			iterator j = iterator(y);
			if (comp) {
				if (j == begin()) {
					return pair<iterator, bool>(__insert(x, y, v), true);
				}
				else
					--j;
			}
			if(key_compare(key(j.node), KeyOfValue()(v)))
				return pair<iterator, bool>(__insert(x, y, v), true);

			return  pair<iterator, bool>(j,false);
		}
	
		template<class iterator>
		void insert_unique(iterator first, iterator last) {
			for (auto it = first; it != last; it++) {
				insert_unique(*it);
			}
		}

		//�������RB-tree������ڵ�ֵ�ظ�
		iterator insert_equal(const value_type& v) {
			link_type y = header;
			link_type x = root(); //�Ӹ��ڵ㿪ʼ
			while (x != 0) {
				y = x;
				x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
			}
			return __insert(x, y, v);
		}
		//����������ƽ�⣨��ɫ ��ת���Σ�
		//����1 Ϊ�����ڵ㣬 ����2 Ϊroot
		inline void __rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
			x->color = __rb_tree_red; //�½ڵ�Ϊ��ɫ
			while (x != root && x->parent->color == __rb_tree_red) {//���ڵ�Ϊ��
				if (x->parent == x->parent->parent->left) {//���ڵ�Ϊ�游�ڵ����ڵ�
					//���ڵ�Ϊ�游�ڵ�֮���ӽڵ�
					_rb_tree_node_base* y = x->parent->parent->right;//��yΪ�����ڵ�
					if (y && y->color == __rb_tree_red) { //�����ڵ���ڣ���Ϊ��
						x->parent->color = __rb_tree_black;//���ĸ��ڵ�Ϊ��
						y->color = __rb_tree_black; //���Ĳ����ڵ�Ϊ��
						x->parent->parent->color = __rb_tree_red;//�����游�ڵ�Ϊ��
						x = x->parent->parent;
					}
					else {//�޲����ڵ�򲮸��ڵ�Ϊ��
						if (x == x->parent->right) {//����½ڵ�Ϊ���ڵ�֮�ҽڵ�
							x = x->parent;
							__rb_tree_rotate_left(x, root); //��һ����Ϊ������
						}
						x->parent->color = __rb_tree_black;
						x->parent->parent->color = __rb_tree_red;
						__rb_tree_rotate_right(x->parent->parent, root); //��һ����Ϊ������
					}
				}
				else { //���ڵ�Ϊ�游�ڵ�֮���ӽڵ�
					_rb_tree_node_base* y = x->parent->parent->left;//��yΪ�����ڵ�
					if (y && y->color == __rb_tree_red) { //�в����ڵ㣬��Ϊ��ɫ
						x->parent->color = __rb_tree_black;//���ĸ��ڵ�Ϊ��
						y->color = __rb_tree_black; //���Ĳ����ڵ�Ϊ��
						x->parent->parent->color = __rb_tree_red;//�����游�ڵ�Ϊ��
						x = x->parent->parent; //�������ϼ��
					}
					else { //�޲����ڵ㣬�򲮸��ڵ�Ϊ��
						if (x == x->parent->left) { //����½ڵ�Ϊ���ڵ�֮���ӽڵ�
							x = x->parent;
							__rb_tree_rotate_right(x, root);//��һ����Ϊ������
						}
						x->parent->color = __rb_tree_black; //�ı���ɫ
						x->parent->parent->color = __rb_tree_red;
						__rb_tree_rotate_left(x->parent->parent, root);//��һ����Ϊ������
					}
				}

			} //end of while
			root->color = __rb_tree_black; //root�ڵ���ԶΪ��
		}


		inline void	__rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base*& root) {
			//x Ϊ��ת��
			_rb_tree_node_base* y = x->right;
			x->right = y->left;
			if (y->left != 0)
				y->left->parent = x;
			y->parent = x->parent;

			if (x == root)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		inline void
			__rb_tree_rotate_right(_rb_tree_node_base* x,
				_rb_tree_node_base*& root)
		{

			//xΪ��ת��
			_rb_tree_node_base* y = x->left;
			x->left = y->right;
			if (y->right != 0)
				y->right->parent = x;
			y->parent = x->parent;

			//��y��ȫ����x��λ�ã����뽫�丸�ڵ�Ĺ�ϵ��ȫ���չ�����
			if (x == root)
				root = y;
			else if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;
			y->right = x;
			x->parent = y;
		}

		iterator find(const Key& k) {
			link_type y = header;  //last node which is not less than k
			link_type x = root();  //current node

			while (x != 0)
				if (!key_compare(key(x), k))  //�����Ƿ�� k ���ǵĻ���������ȥ
					y = x, x = left(x); //ע���﷨��
				else
					x = right(x);
			iterator j = iterator(y);
			return (j == end() || key_compare(k, key(j.node))) ? end() : j;
		}











	};





}//!namespace mystl

#endif // !_MYSTL_RBTREE_H_

