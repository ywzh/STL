#ifndef SGI_STL_HASH_MAP_H
#define SGI_STL_HASH_MAP_H

#include "02-allocator/stl_alloc.h"
#include "05-container/stl_hash_fun.h"
#include "05-container/stl_hashtable.h"

//hash map接口转调hashtable接口
//RB-tree有自动排序功能而hashtable没有,所以map元素有自动排序而hash_map没有
//map的特点是每个元素都同时拥有一个实值(value)和一个键值(key).这一点在hash_map中也是一样的.
//hash_map的使用方式与map完全相同

template <class Key, 
          class T,
          class HashFcn = hash<Key>,
          class EqualKey = equal_to<Key>,
          class Alloc = alloc>
class hash_map
{
private:
    //以下使用的select1st定义于<stl_function.h>中
    typedef hashtable<pair<const Key, T>, Key, HashFcn,
                      select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
    ht rep; //底层机制以hash table完成
public:
    typedef typename ht::key_type key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher_type hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;

    typedef typename ht::pointer pointer;
    typedef typename ht::iterator iterator;
    typedef typename ht::reference reference;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_iterator const_iterator;
    typedef typename ht::const_reference const_reference;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }
public:
    // 缺省使用大小为100的表格,将由hash table调整为最接近且较大的质数
    hash_map() : rep(100, hasher(), key_equal()) {}
    explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_map(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
    hash_map(size_type n, const hasher& hf, const key_equal& eql) :
        rep(n, hf, eql) {}
    
    //以下插入操作全部使用insert_unique(), 不允许键值重复
    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l) :
        rep(100, hasher(), key_equal()) { rep.insert_unique(f, l); }

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n) :
        rep(n, hasher(), key_equal()) { rep.insert_unique(f, l); }

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf) :
        rep(n, hf, key_equal()) { rep.insert_unique(f, l); }

    template <class InputIterator>
    hash_map(InputIterator f, InputIterator l, size_type n, 
             const hasher& hf, const key_equal& eql) :
        rep(n, hf, eql) { rep.insert_unique(f, l); }
public:
    //所有的操作几乎都有hash table对应版本,传递调用就行
    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_map& hs) { rep.swap(hs.rep); }
    friend bool operator == __STL_NULL_TMPL_ARGS (const hash_map&, const hash_map&);

    iterator begin() { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }
public:
    pair<iterator, bool> insert(cosnt value_type& obj) { return rep.insert_unique(obj); }

    template <class InputIterator>
    void insert(InputIterator f, InputIterator l) { rep.insert_unique(f, l); }

    pair<iterator, bool> insert_noresize(const value_type& obj) { 
        return rep.insert_unique_noresize(obj); 
    }

    iterator find(const key_type& key) { return rep.find(key); }
    const_iterator find(const key_type& key) const { return rep.find(key); }

    T& operator[](const key_type& key) {
        return rep.find_or_insert(value_type(key, T())).second;
    }

    size_type count(const key_type& key) const { return rep.count(key); }
    pair<iterator, iterator> equal_range(const key_type& key) { return rep.equal_range(key); }
    pair<const iterator, const_iterator> equal_range(const key_type& key) { return rep.equal_range(key); }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator i) { return rep.erase(i); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }
public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

template <class Key, class T, class HashFcn, class EqualKey, class Alloc>
inline bool operator ==(const hash_map<Key, T, HashFcn, EqualKey, Alloc>& hm1,
                        const hash_map<Key, T, HashFcn, EqualKey, Alloc>& hm2)
{
    return hm1.rep == hm2.rep;
}

#endif // SGI_STL_HASH_MAP_H


/*
 * ======== TEST DEMO =========
 
#include <iostream>
#include <hash_map>
#include <cstring>

using namespace std;

struct {
    bool operator(const char* s1, const char* s2) const { return strcmp(s1, s2); }
};

int main()
{
    hash_map<const char*, int, hash<const char*>, eqstr> days;

    days["january"] = 31;
    days["february"] = 28;
    days["march"] = 31;
    days["april"] = 30;
    days["may"] = 31;
    days["june"] = 30;
    days["july"] = 31;
    days["august"] = 31;
    days["september"] = 30;
    days["october"] = 31;
    days["november"] = 30;
    days["december"] = 31;

    cout << "september -> " << days["september"] << endl; // 30
    cout << "june      -> " << days["june"] << endl;      // 30
    cout << "february -> " << days["february"] << endl;   // 28
    cout << "december -> " << days["december"] << endl;   // 31

    hash_map<const char*, int, hash<const char*>, eqstr>::iterator c = days.begin();
    hash_map<const char*, int, hash<const char*>, eqstr>::iterator e = days.end();

    for (; c != e; ++c)
        cout << c->first << ' '
    // september june july may january february december march april november october august
}

 */