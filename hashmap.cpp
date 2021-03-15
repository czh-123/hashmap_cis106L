/*
* Assignment 2: HashMap template implementation (BASIC SOLUTION)
*      Notes: this file is what we call a .tpp file. It's not a .cpp file,
*      because it's not exactly a regular source code file. Instead, we are
*      defining the template definitions for our HashMap class.
*
*      TODO: write a comment here.
*
*      You'll notice that the commenting provided is absolutely stunning.
*      It was really fun to read through the starter code, right?
*      Please emulate this commenting style in your code, so your grader
*      can have an equally pleasant time reading your code. :)
*/

#include "hashmap.h"

//each node in bucket represent a list

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap() : HashMap(kDefaultBuckets) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::HashMap(size_t bucket_count, const H& hash) :
   _size(0),
   _hash_function(hash),
   _buckets_array(bucket_count, nullptr) { }

template <typename K, typename M, typename H>
HashMap<K, M, H>::~HashMap() {
   clear();
}

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::size() const noexcept {
   return _size;
}

template <typename K, typename M, typename H>
inline bool HashMap<K, M, H>::empty() const noexcept {
   return size() == 0;
}

template <typename K, typename M, typename H>
inline float HashMap<K, M, H>::load_factor() const noexcept {
   return static_cast<float>(size())/bucket_count();
};

template <typename K, typename M, typename H>
inline size_t HashMap<K, M, H>::bucket_count() const noexcept {
   return _buckets_array.size();
};

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::contains(const K& key) const noexcept {
   return find_node(key).second != nullptr;
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::clear() noexcept {
   for (auto& curr : _buckets_array) {
       while (curr != nullptr) {
           auto trash = curr;
           curr = curr->next;
           delete trash;
       }
   }
   _size = 0;
}

template <typename K, typename M, typename H>
std::pair<typename HashMap<K, M, H>::value_type*, bool>
           HashMap<K, M, H>::insert(const value_type& value) {
   const auto& [key, mapped] = value;
   auto [prev, node_to_edit] = find_node(key);
   size_t index = _hash_function(key) % bucket_count();

   if (node_to_edit != nullptr) return {&(node_to_edit->value), false};
   _buckets_array[index] = new node(value, _buckets_array[index]);
   // !!!!!!!! 每次添加到头部 bucket记录头部

   ++_size;
   return {&(_buckets_array[index]->value), true};
}

template <typename K, typename M, typename H>
M& HashMap<K, M, H>::at(const K& key)  {
   auto [prev, node_found] = find_node(key);
   if (node_found == nullptr) {
       throw std::out_of_range("HashMap<K, M, H>::at: key not found");
   }
   return node_found->value.second;
}

template <typename K, typename M, typename H>
const M& HashMap<K, M, H>::at(const K& key) const {
   auto [prev, node_found] = find_node(key);
   if (node_found == nullptr) {
       throw std::out_of_range("HashMap<K, M, H>::at: key not found");
   }
   return node_found->value.second;
}

template <typename K, typename M, typename H>
typename HashMap<K, M, H>::node_pair HashMap<K, M, H>::find_node(const K& key) const {
   size_t index = _hash_function(key) % bucket_count();
   auto curr = _buckets_array[index];
   node* prev = nullptr; // if first node is the key, return {nullptr, front}
   while (curr != nullptr) {
       const auto& [found_key, found_mapped] = curr->value;
       if (found_key == key) return {prev, curr};
       prev = curr;
       curr = curr->next;
   }
   return {nullptr, nullptr}; // key not found at all.
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::debug() const {
   std::cout << std::setw(30) << std::setfill('-') << '\n' << std::setfill(' ')
             << "Printing debug information for your HashMap implementation\n"
             << "Size: " << size() << std::setw(15) << std::right
             << "Buckets: " << bucket_count() << std::setw(20) << std::right
             << "(load factor: " << std::setprecision(2) << load_factor() << ") \n\n";

   for (size_t i = 0; i < bucket_count(); ++i) {
       std::cout << "[" << std::setw(3) << i << "]:";
       auto curr = _buckets_array[i];
       while (curr != nullptr) {
           const auto& [key, mapped] = curr->value;
           // next line will not compile if << not supported for K or M
           std::cout <<  " -> " << key << ":" << mapped;
           curr = curr->next;
       }
       std::cout <<  " /" <<  '\n';
   }
   std::cout << std::setw(30) << std::setfill('-') << '\n';
}

template <typename K, typename M, typename H>
bool HashMap<K, M, H>::erase(const K& key) {
    auto [prev, node_to_erase] = find_node(key);
    if (node_to_erase == nullptr) {
        return false;
    } else {
        size_t index = _hash_function(key) % bucket_count();
        (prev ? prev->next : _buckets_array[index]) = node_to_erase->next;
        delete node_to_erase;
        --_size;
        return true;
    }
}

template <typename K, typename M, typename H>
void HashMap<K, M, H>::rehash(size_t new_bucket_count) {
    if (new_bucket_count == 0) {
        throw std::out_of_range("HashMap<K, M, H>::rehash: new_bucket_count must be positive.");
    }

    std::vector<node*> new_buckets_array(new_bucket_count);
    /* Optional Milestone 1: begin student code */

        // Hint: you should NOT call insert, and you should not call
        // new or delete in this function. You must reuse existing nodes.

        //node.first (key) % new_count to get the list and "insert" "del"
    std::vector<node*> old_buckets = _buckets_array;
    this->_buckets_array = new_buckets_array;
    for (auto &bnode:old_buckets){
        while(bnode != nullptr){
            const auto &[key,mapped_key] = bnode->value;
            size_t index = _hash_function(key)%new_bucket_count;

            auto nxt_node = bnode->next;
            bnode->next = _buckets_array[index];
            _buckets_array[index] = bnode;

            bnode = nxt_node;
        }
    }
}

template<typename K,typename M,typename H>
M& HashMap<K,M,H>::operator[](const K &key){
    auto [prev,curr] = find_node(key);
    if (curr == nullptr){
        size_t index = _hash_function(key)%bucket_count();
        _buckets_array[index] = new node({key,M()},_buckets_array[index]);
        _size++;
        //std::cout<<_buckets_array[index]->value.first<<"  "<<_buckets_array[index]->value.second<<std::endl;
        return _buckets_array[index]->value.second;
    }
    else{
        return curr->value.second;
    }
}



template<typename K,typename M,typename H>
std::ostream& operator<<(std::ostream& os,const HashMap<K,M,H>& map){
    os<<"{";
    size_t count = 0;
    for ( auto node:map._buckets_array){
        while (node != nullptr){
            if (count != 0)
                os<<", ";
            else
                ++count;
            os<<node->value.first<<":"<<node->value.second;
            //std::cout<<node->value.first<<":"<<node->value.second<<std::endl;
            node = node->next;
        }
    }
    os<<"}";
    return os;
}

template<typename K,typename M,typename H>
bool operator==(const HashMap<K,M,H>& lhs,const HashMap<K,M,H>& rhs){
    if (lhs.size() != rhs.size() )
        return false;
    for (auto node:lhs._buckets_array){
        while(node != nullptr){
            auto key = node->value.first;
            auto [prev,curr] = rhs.find_node(key);
            if (curr == nullptr || curr->value.second != node->value.second)
                return false;
            //if (rhs.at(static_cast<const K>(node->value.first)) != node->value.second)
            //    return false;
            node = node->next;
        }
    }
    for (auto node:rhs._buckets_array){
        while(node != nullptr){
            auto key = node->value.first;
            auto [prev,curr] = lhs.find_node(key);
            if (curr == nullptr || curr->value.second != node->value.second)
                return false;
            node = node->next;
        }
    }
    return true;
}

template<typename K,typename M,typename H>
bool operator!= (const HashMap<K,M,H>& lhs,const HashMap<K,M,H>& rhs){
    if (lhs.size() != rhs.size())
        return true;
    for (auto node:lhs._buckets_array){
        while(node != nullptr){
            auto key = node->value.first;
            auto [prev,curr] = rhs.find_node(key);
            if (curr == nullptr || curr->value.second != node->value.second)
                return true;
            node = node->next;
        }
    }
    for (auto node:rhs._buckets_array){
        while(node != nullptr){
            auto key = node->value.first;
            auto [prev,curr] = lhs.find_node(key);
            if (curr == nullptr || curr->value.second != node->value.second)
                return true;
            node = node->next;
        }
    }
    return false;
}

// milestone 3
template<typename K,typename M,typename H>
HashMap<K,M,H>::HashMap(const HashMap<K,M,H>& cpy) noexcept:
    _size(cpy.size()),
    _hash_function(cpy._hash_function),
    _buckets_array(std::vector<node*>(cpy.bucket_count(),nullptr)){
    for (size_t index = 0;index<cpy.bucket_count();index++){
        auto lnode = cpy._buckets_array[index];
        while(lnode != nullptr){
            _buckets_array[index] = new node(lnode->value,_buckets_array[index]);
            lnode = lnode->next;
        }
    }
}

template<typename K,typename M,typename H>
HashMap<K,M,H>& HashMap<K,M,H>::operator= (const HashMap<K,M,H>& rhs){
    if (this == &rhs)
        return *this;
    clear();
    //HashMap(rhs);   为什么报错 redefi 和 shadows a parameter？
    _size = rhs.size();
    _hash_function = rhs._hash_function;
    _buckets_array = std::vector<node*>(rhs.bucket_count(),nullptr);
    for (size_t index = 0;index<rhs.bucket_count();index++){
        auto lnode = rhs._buckets_array[index];
        while(lnode != nullptr){
            _buckets_array[index] = new node(lnode->value,_buckets_array[index]);
            lnode = lnode->next;
        }
    }
    return *this;
}

template<typename K,typename M,typename H>
HashMap<K,M,H>::HashMap(HashMap<K,M,H>&& other) noexcept :
    _size(other._size),
    _hash_function(other._hash_function){
    if (this == &other)
        return ;
    _buckets_array = std::move(other._buckets_array);
    other._size = 0;
    }

template<typename K,typename M,typename H>
HashMap<K,M,H>& HashMap<K,M,H>::operator=(HashMap<K,M,H>&& rhs) noexcept {
     //实在不知道怎么优化了
    if  (this != &rhs){

        _size = std::move(rhs._size);
        _buckets_array = std::move(rhs._buckets_array);
    /*
    _buckets_array = std::vector<node*>(rhs.bucket_count(),nullptr);

    //_buckets_array.clear();
    for (size_t i=0;i<rhs.bucket_count();i++){
        //_buckets_array.push_back(std::move(rhs._buckets_array[i]));
        _buckets_array[i] = rhs._buckets_array[i];
        rhs._buckets_array[i] = nullptr;
    }
    */
    }
    return *this;

}

template <typename K,typename M,typename H>
HashMap<K,M,H>::HashMap(std::initializer_list<value_type> hl):
_buckets_array(kDefaultBuckets,nullptr)
    {
    _size = 0;
    for (auto p:hl){
    insert(p);
    }
}

template <typename K,typename M,typename H>
HashMap<K,M,H>::HashMap(std::initializer_list<typename std::vector<std::pair<K,M>>::iterator > &&hl):
_buckets_array(kDefaultBuckets,nullptr)
    {
    _size = 0;
    auto p = *hl.begin();
    auto e = *(hl.end()-1);
    /*
    std::cout<<(*e).first<<"  "<<(*e).second<<std::endl;
    std::cout<<"begin"<<std::endl;
    for (auto p:hl){
        std::cout<<(*p).first<<"   "<<(*p).second<<std::endl;
    }
    std::cout<<hl.size();
    std::cout<<"end"<<std::endl;
    */
    while (p != e){
        insert(*p);
        p++;
        //std::cout<<(*p).first<<(*p).second<<std::endl;
    }
}


/*
    Milestone 2-3: begin student code

    Here is a list of functions you should implement:
    Milestone 2
        - operator[]
        - operator<<
        - operator== and !=
        - make existing functions const-correct

    Milestone 3
        - copy constructor
        - copy assignment
        - move constructor
        - move assignment
*/
