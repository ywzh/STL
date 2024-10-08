#ifndef SGI_STL_HASH_MULTI_SET_H
#define SGI_STL_HASH_MULTI_SET_H

#include "01-config/stl_config.h"
#include "02-allocator/stl_alloc.h"

/* 
 * hash_multiset和hash_set实现上的唯一的差别在于,前者的元素插入采用底层机制
 * hashtable的insert_equal(),后者则是采用insert_unique()
 */

template <class Value,
          class HashFcn = hash<Value>,
          class EqualKey = equal_to<Value>,
          class Alloc = alloc>
class hash_multiset {
private:
    typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
    ht rep;
public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;

    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;

    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_reference reference;

    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_iterator const_iterator;
    typedef typename ht::const_reference const_reference;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }
public:
    //缺省使用大小为100的表格,将被hash table调整为最接近且较大之质数
    hash_multiset() : rep(100, hasher(), key_equal()) {}
    explicit hash_multiset(size_type n) : rep(n, hf, key_equal()) {}
    hash_multiset(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
    hash_multiset(size_type n, const hasher& hf, const key_equal& eql) :
        rep(n, hf, eql) {} 
    
    //以下插入操作全部使用insert_equal(),允许键值重复
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l)
        : rep(100, hasher(), key_equal()) { rep.insert_equal(f, l); }
    
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n)
        : rep(n, hasher(), key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n, const hasher& hf)
        : rep(n, hf, key_equal()) { rep.insert_equal(f, l); }
    template <class InputIterator>
    hash_multiset(InputIterator f, InputIterator l, size_type n,
                  const hasher& hf, const key_equal& eql) :
        rep(n, hf, eql) { rep.insert_equal(f, l); }
public:
    //所有操作几乎都有hash table的对应版本,传递调用即可
    size_type size() const { return rep.size(); }
    szie_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    void swap(hash_multiset& hs) { rep.swap(hs.rep); }
    friend bool operator == __STL_NULL_TMPL_ARGS (const hash_multiset&, const hash_multiset&);

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }
public:
    iterator insert(const value_type& obj) { return rep.insert_equal(obj); }
    template <class InputIterator>
    void insert(InputIterator f, InputIterator l) { rep.insert_equal(f, l); }
    iterator insert_noresize(const value_type& obj) { return rep.insert_equal_noresize(obj); }
    iterator find(const key_type& key) const { return rep.find(key); }
    size_type count(const key_type& key) const { return rep.count(key); }

    pair<iterator, iterator> equal_range(cosnt key_type& key) const
    { return rep.equal_range(key); }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator i) { rep.erase(i); }
    void erase(iterator f, iterator l) { rep.erase(f, l); }
    void clear() { rep.clear(); }
public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

template <class Val, class HashFcn, class EqualKey, class Alloc>
inline bool operator ==(const hash_multiset<Val, HashFcn, EqualKey, Alloc>hms1,
const hash_multiset<Val, HashFcn, EqualKey, Alloc>hms2) { return hms1.rep == hms2.rep; }

#endif // SGI_STL_HASH_MULTI_SET_H

// hash_multiset的使用方式与hash_set一致