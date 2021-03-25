#ifndef HASHMAPITERATOR_H
#define HASHMAPITERATOR_H
/* Milestone 6 (optional): implement iterators for your HashMap class */
#include <iterator>
#include <hashmap.h>
//using namespace std;
template<typename K,typename M,typename H>
class HashMap;

template<typename K,typename  M,typename H>
struct __hashmap_iterator{
    typedef std::forward_iterator_tag iterator_category;
    using iterator = __hashmap_iterator<K,M,H>;
    typedef HashMap<K,M,H> hashmap;
    using differende_type = ptrdiff_t;
    using size_type = size_t;
    using node_pair = typename hashmap::value_type;

    using node_type =typename hashmap::node;   //指向node
    using reference = node_type& ;
    using pointer  = node_type* ;

    pointer cur;
    hashmap* hm;

    __hashmap_iterator(pointer cur,hashmap *hm): cur(cur),hm(hm) {}
    __hashmap_iterator( ) { }

    node_pair& operator*() const {return cur->value;}
    node_pair* operator->() const {return &(operator*()); }

    bool operator== (const iterator& it) const { return cur == it.cur;}
    bool operator!= (const iterator& it) const { return cur != it.cur;}
    iterator& operator=(const iterator& it) {cur = it.cur; hm = it.hm; return *this;}

    iterator& operator++(){ //前加加

        if (cur == nullptr)
            return *this;

        const pointer old = cur;
        cur = cur->next;
        if (cur == nullptr){
            size_type index = hm->_hash_function(old->value.first) % hm->bucket_count();
            //std::cout<<index<<std::endl<<std::endl;
            index++;
            while(index < hm->bucket_count() && hm->_buckets_array[index] == nullptr){
                index++;
            }
            if (index < hm->bucket_count())
                cur = hm->_buckets_array[index];
        }
        return *this;
    }

    iterator operator++(int){   //后加加
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

};







// "White. A blank page or canvas. His favorite. So many possibilities...""









#endif // HASHMAPITERATOR_H
