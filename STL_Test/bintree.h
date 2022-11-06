#pragma once
#ifndef  _BINTREE_H_
#include <iostream>

struct node {
	int value;
	node* left{ nullptr };
	node* right{ nullptr };
	node() = default;
	node(const int& x) :value{ x }, left{ nullptr }, right{ nullptr }{}
	~node() = default;
};


class bintree {
private:
	node* root{ nullptr };

	void _post(node* cnode) {
		if (cnode != nullptr) {
			_post(cnode->left);
			std::cout << cnode->value << ' ';
			_post(cnode->right);
		}
	}
public:
	bintree() = default;

	void read() {
		_post(root);
	}
	void insert(const int& x) {
		if (root == nullptr) {
			root = new node(x);
			return;
		}
		node* cnode = root;
		while ((x >= cnode->value && cnode->right != nullptr) ||
			(x < cnode->value && cnode->left != nullptr)) {
			if (x >= cnode->value)cnode = cnode->right;
			else cnode = cnode->left;
		}
		if (x >= cnode->value) {
			cnode->right = new node(x);
		}
		else {
			cnode->left = new node(x);
		}

	}
};





#endif // ! _BINTREE_H_
