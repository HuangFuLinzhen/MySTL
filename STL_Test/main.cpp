//#include "bintree.h"
#include "rbtree.h"
int main() {
	rbtree bt;
	bt.insert(2);
	bt.insert(3);
	bt.insert(6);
	bt.insert(4);
	bt.insert(11);
	bt.insert(5);
	bt.insert(99);
	bt.insert(1);
	bt.insert(0);
	bt.insert(33);
	bt.insert(34);
	bt.insert(35);
	bt.insert(36);
	bt.insert(37);
	bt.insert(45);
	bt.read();
	std::cout << std :: endl;
	node* root = bt.getroot();
	rbtree::getvalue(root->left->left->left);
	rbtree::getvalue(root->left->left->right);
	rbtree::getvalue(root->left->right->right);
	rbtree::getvalue(root->right->left);
	rbtree::getvalue(root->right->right);
	rbtree::getvalue(root->right->right->right->left);

	
}