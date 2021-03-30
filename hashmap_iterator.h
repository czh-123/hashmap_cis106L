#ifndef HASHMAPITERATOR_H
#define HASHMAPITERATOR_H
/* Milestone 6 (optional): implement iterators for your HashMap class */
#include <iterator>
#include <hashmap.h>
//using namespace std;
template<typename K,typename M,typename H>
class HashMap;



template<typename K,typename  M,typename H>
struct hashmap_iterator{
public:

    using iterator = hashmap_iterator<K,M,H>;
    //using const_iterator = __hashmap_const_iterator<K,M,H>;
    using hashmap =  HashMap<K,M,H>;
    using node_type =typename hashmap::node;   //指向node

    //!! required
    using iterator_category = std::forward_iterator_tag ;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using value_type =  typename hashmap::value_type;
    using reference = value_type& ;
    using pointer  = value_type* ;

    node_type* cur;
    hashmap* hm;

    hashmap_iterator(node_type* cur,hashmap *hm): cur(cur),hm(hm) {}
    hashmap_iterator( ) { }

    reference operator*() const {return cur->value;}
    pointer operator->() const {return &(operator*()); }

    bool operator== (const iterator& it) const { return cur == it.cur;}
    bool operator!= (const iterator& it) const { return cur != it.cur;}
    iterator& operator=(const iterator& it) {cur = it.cur; hm = it.hm; return *this;}

    iterator& operator++(){ //前加加

        if (cur == nullptr)
            return *this;

        const node_type* old = cur;
        cur = cur->next;
        if (cur == nullptr){
            size_type index = hm->_hash_function(old->value.first) % hm->bucket_count();
            // better -- SGI STL
            while(cur == nullptr && ++index < hm->bucket_count()){
                cur = hm->_buckets_array[index];
            }
        }
        return *this;
    }

    iterator operator++(int){   //后加加
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

};

template<typename K,typename M,typename H>
struct hashmap_const_iterator{
public:
    using iterator = hashmap_iterator<K,M,H>;
    using const_iterator = hashmap_const_iterator<K,M,H>;
    using hashmap =  HashMap<K,M,H> ;
    using node_type =typename hashmap::node;   //指向node

    using iterator_category = std::forward_iterator_tag;
    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using value_type = typename hashmap::value_type;
    using reference = const value_type &;
    using pointer = const value_type *;



    const node_type* cur;
    const hashmap* hm;

    hashmap_const_iterator(const node_type* cur,const hashmap* hm ):cur(cur),hm(hm) {}
    hashmap_const_iterator(const iterator& it): cur(it.cur),hm(it.hm) {}

    reference operator*() const {return cur->value;}
    pointer operator->() const {return &(operator*());}

    bool operator== (const iterator& it) const { return cur == it.cur;}
    bool operator!= (const iterator& it) const { return cur != it.cur;}

    bool operator!= (const_iterator& it) const {return cur != it.cur;}
    bool operator== (const_iterator& it) const { return cur == it.cur;}
    bool operator!= (const const_iterator& it) const {return cur != it.cur;}
    bool operator== (const const_iterator& it) const { return cur == it.cur;}

    const_iterator& operator++(){ //前加加

        if (cur == nullptr)
            return *this;
    // copy from iterator type -> const type
        const node_type* old = cur;
        cur = cur->next;
        if (cur == nullptr){
            size_type index = hm->_hash_function(old->value.first) % hm->bucket_count();
            while(cur == nullptr && ++index < hm->bucket_count()){
                cur = hm->_buckets_array[index];
            }
        }
        return *this;
    }

    const_iterator operator++(int){
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }
};







// "White. A blank page or canvas. His favorite. So many possibilities...""









#endif // HASHMAPITERATOR_H
