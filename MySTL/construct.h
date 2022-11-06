#pragma once
#ifndef _MYSTL_CONSTRUCT_H_
#define _MYSTL_CONSTRUCT_H_
//coudeby 20221027
#include "type_traits.h"
namespace mystl {

    template<class T1, class T2>
    inline void construct(T1* ptr1, const T2& value) {
        new(ptr1) T1(value); //placement new ���ѻ�ȡ���ڴ��ϴ�������
    }

    template<class T>
    inline void destroy(T* ptr) {
        ptr->~T();
    }

    template<class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator last, _true_type) {}

    template<class ForwardIterator>
    inline void _destory(ForwardIterator first, ForwardIterator last, _false_type) {
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD;
        _destory(first, last, is_POD());
    }

    //���char* wchar_t*���ػ��汾
    inline void destroy(char*, char*) {}
    inline void destroy(wchar_t*, wchar_t*) {}



}//!namespace mystl
#endif // !_MYSTL_CONSTRUCT_H_
