#pragma once

#ifndef _MYSTL_LISTITERATOR_H_
#define _MYSTL_LISTITERATOR_H_

//code by 20221025

#include "iterator.h"
#include "listnode.h"


namespace mystl {
    //list�������������䣬������������ָ��
    template<class T, class Ref, class Ptr>
    struct _list_iterator {
        typedef _list_iterator<T, T&, T*> iterator; //ָ���ڲ�Ԫ�صĵ�����
        typedef _list_iterator<T, Ref, Ptr> self;   //ָ��node�ĵ�����
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;


        typedef T value_type;
        typedef Ref reference;
        typedef Ptr pointer;

        typedef _list_node<T>* link_type;
        typedef size_t size_type;

        link_type node; //list�ڵ��ָ��

        _list_iterator() {};
        _list_iterator(link_type x) :node{ x } {}
        _list_iterator(const _list_iterator& x) :node{ x.node } { }

        bool operator==(const self& x) const { return node == x.node; }
        bool operator!=(const self& x) const { return node != x.node; }

        //deference
        reference operator*() const { return node->data; }
        pointer   operator->() const { return &(operator*()); }

        //��������ǰ�ƶ�һ��λ��(++i)
        self& operator++() {
            node = node->next;
            return *this;
        }

        //��������ǰ�ƶ�һ��λ��(i++)
        self operator++(int) {
            self tmp = *this;
            ++* this;
            return tmp;
        }

        //�Ե������ݼ�1 (--i)
        self& operator--() {
            node = node->prev;
            return *this;
        }

        //�Ե������ݼ�1 (i--)
        self operator--(int) {
            self tmp = *this;
            --* this;
            return tmp;
        }

        self operator+(int dis) {
            self tmp = *this;
            while (dis-- > 0) {
                tmp = tmp.node->next;
            }
            return tmp;
        }

        self operator-(int dis) {
            self tmp = *this;
            while (dis-- > 0) {
                tmp = tmp.node->prev;
            }
            return tmp;
        }

    };



};//!namesapce mystl


#endif // !_MYSTL_LISTITERATOR_H_


