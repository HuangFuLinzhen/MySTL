#pragma once
#ifndef _MYSTL_ALLOC_H_
#define _MYSTL_ALLOC_H_


namespace mystl {
    /*
  * �ռ�������
  */
    class alloc {
    private:
        enum { _ALIGN = 8 }; //С��������ϵ��߽�
        enum { _MAX_BYTES = 128 }; //С�����������
        enum { _NFREELIST = _MAX_BYTES / _ALIGN }; //freelist�ĸ���
        enum { _NOBJS = 20 };//ÿ�����ӵĽڵ�����

        union obj { //free list �ڵ�
            union obj* free_list_next;
            char client[1];
        };

        static obj* volatile free_list[_NFREELIST];

        static char* start_free; //�ڴ����ʼ��ַ
        static char* end_free; //�ڴ�ؽ�����ַ
        static size_t heap_size;

        //������Ҫ�������С��ѡ��freelist���
        static size_t FREELIST_INDEX(size_t bytes) {
            return ((bytes + _ALIGN - 1) / _ALIGN - 1);
        }
        //37

        //��bytes �ϵ���8�ı���
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + _ALIGN - 1) & ~(_ALIGN - 1));
        }
       

        //����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
        static void* refill(size_t bytes);

        //����һ���ռ䣬������_NOBJS����СΪsize������
        static char* chunk_alloc(size_t bytes, size_t& nobjs);

    public:
        static void* allocate(size_t bytes);

        static void deallocate(void* ptr, size_t bytes);

        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
    };
}// !namespace mystl
#endif // !_MYSTL_ALLOC_H_
