#include <iostream>
#include "list.h"
#include "deque.h"
#include <list>
#include "rbtree.h"
#include "set.h"
#include "map.h"
using std::cout;
using std::endl;
class test {
public:
	test() {
		cout << "构造" << endl;
	}
	test(const test& t) {
		cout << "拷贝" << endl;
	}
	test(test&& t) noexcept{
		cout << "右值" << endl;
	}
	~test() {
		cout << "析构" << endl;
	}
};


int main() {

	//mystl::deque<int> mystl(5,6);
	////test* a = new test();
	//for (int i = 0; i < 6; i++) {
	//	mystl.push_back(1);
	//	mystl.push_front(2);
	//}
	//for (auto i : mystl) {
	//	cout << i << ' ';
	//}
	////delete a;
	////mystl.clear();
	//cout << "-----------" << endl;

	/*mystl::rb_tree<int, int, identity<int>, less<int>> r;
	r.insert_unique(1);
	r.insert_unique(3);
	r.insert_unique(5);
	r.insert_unique(77);
	r.insert_unique(1);
	r.insert_unique(7);
	r.insert_unique(55);
	r.insert_unique(44);
	r.insert_unique(3);
	for (auto& i : r) {
		cout << i << ' ';
	}*/

	/*mystl::set<int> r;
	r.insert(1);
	r.insert(1);
	r.insert(3);
	r.insert(5);
	r.insert(77);
	r.insert(1);
	r.insert(7);
	r.insert(55);
	r.insert(44);
	r.insert(3);
	for (auto& i : r) {
		cout << i << ' ';
	}
	mystl::set<int> s(r.begin(), r.end());
	
	for (auto& i : s) {
		cout << i << ' ';
	}*/
	mystl::map<int, char> m;
	for (int i = 0; i < 10; i++) {
		m[i] = i + 'a';
	}
	for (auto& i : m) {
		cout << i.first << i.second << ' ';
	}
	for (int i = 0; i < 10; i++) {
		cout<<m[i];
	}
	return 0;
}