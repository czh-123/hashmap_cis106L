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
    typedef std::forward_iterator_tag iterator_category;

    using iterator = hashmap_iterator<K,M,H>;
    //using const_iterator = __hashmap_const_iterator<K,M,H>;
    typedef HashMap<K,M,H> hashmap;
    using node_type =typename hashmap::node;   //指向node

    //!! required
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

template<typename K,typename M,typename H>
struct hashmap_const_iterator{
public:
    typedef std::forward_iterator_tag iterator_category;
    using iterator = hashmap_iterator<K,M,H>;
    using const_iterator = hashmap_const_iterator<K,M,H>;
    typedef HashMap<K,M,H> hashmap;

    using difference_type = ptrdiff_t;
    using size_type = size_t;
    using value_type = typename hashmap::value_type;

    using node_type =typename hashmap::node;   //指向node

    using reference = const value_type &;
    using pointer = const value_type *;
    const node_type* cur;
    const hashmap* hp;
    hashmap_const_iterator(const node_type* cur,const hashmap* hp ):cur(cur),hp(hp) {}
    hashmap_const_iterator(const iterator& it): cur(it.cur),hp(it.hm) {}

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

        const node_type* old = cur;
        cur = cur->next;
        if (cur == nullptr){
            size_type index = hp->_hash_function(old->value.first) % hp->bucket_count();
            //std::cout<<index<<std::endl<<std::endl;
            index++;
            while(index < hp->bucket_count() && hp->_buckets_array[index] == nullptr){
                index++;
            }
            if (index < hp->bucket_count())
                cur = hp->_buckets_array[index];
        }
        return *this;
    }

    const_iterator operator++(int){   //后加加
        const_iterator tmp = *this;
        ++*this;
        return tmp;
    }
};







// "White. A blank page or canvas. His favorite. So many possibilities...""









#endif // HASHMAPITERATOR_H
