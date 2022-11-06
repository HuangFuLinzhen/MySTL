#pragma once
#ifndef _MYSTL_LISTNODE_N_
#define _MYSTL_LISTNODE_N_

namespace mystl {

	template <class T>
	struct _list_node {  //listÊÇË«ÏòµÄ
		typedef _list_node* void_pointer;
		void_pointer prev{ nullptr };
		void_pointer next{ nullptr };
		T data{ 0 };
		/*;
		_list_node() {}
		_list_node(const T& x) :data{ x } {}*/
		

	};



}//!namespace mystl

#endif // !_MYSTL_LISTNODE_N_

