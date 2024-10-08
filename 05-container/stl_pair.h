#ifndef SGI_STL_PAIR_H
#define SGI_STL_PAIR_H

//仅是为了rb_tree内使用新加的
template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    pair() : first(T1()), second(T2()) {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};

#endif // SGI_STL_PAIR_H