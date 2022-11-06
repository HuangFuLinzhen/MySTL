#pragma once
#include <iostream>

typedef bool color_type;
const color_type red = 1;
const color_type black = 0;

struct node{
	
	int value;
	node* parent{ nullptr };
	node* left{ nullptr };
	node* right{ nullptr };
	color_type color{ red };
	node() = default;
	node(const int& x) :value{ x },parent { nullptr }, left{ nullptr }, right{ nullptr }, color{ red }{}
	node(const int& x, node* pare) :value{ x }, parent{ pare }, left{ nullptr }, right{ nullptr }, color{ red }{}
	~node() = default;
};

class rbtree {
	

private:
	node* root;
	size_t count;

public:
	rbtree() :root{ nullptr }, count{ 0 }{};
	~rbtree() = default;

	void insert(const int& v) {
		++count;
		if (root == nullptr) {
			root = new node(v);
			root->color = black;
			return;
		}
		node* tem = root;
		while ((tem->value > v && tem->left != nullptr) || (tem->value <= v && tem->right != nullptr)) {
			if (tem->value > v) {
				tem = tem->left;
			}
			else
				tem = tem -> right;
		}
		if (tem->value > v) {
			tem->left = new node(v, tem);
			blance(tem->left);
		}
		else {
			tem->right = new node(v, tem);
			blance(tem->right);
		}
			


	}
	void read() {
		inorder(root);
	}
	node* getroot() {
		return root;
	}
	static void getvalue(node* x) {
		std::cout << x->color << ' ' << x->value<<std::endl;
	}

private:
	void inorder(node* x) {
		if (x != nullptr) {
			inorder(x->left);
			std::cout << x->value<<' ';
			inorder(x->right);
		}
	}
	
	void blance(node* x) {
		while (x != root && x->parent->color != black) {
			if (x->parent == x->parent->parent->left) {
				node* y = x->parent->parent->right;
				if (y && y->color == red) {
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						LeftRotate(x);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					RightRotate(x->parent->parent);
				}
			}
			else {
				node* y = x->parent->parent->left;
				if (y && y->color == red) {
					x->parent->color = black;
					y->color = black;
					x->parent->parent->color = red;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->left) {
						x = x->parent;
						RightRotate(x);
					}
					x->parent->color = black;
					x->parent->parent->color = red;
					LeftRotate(x->parent->parent);
				}
			}
		}
		root->color = black;
	}
	void RightRotate(node* x) {
		node* y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}
	void LeftRotate(node* x) {
		node* y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

};