module;

#include <vector>

export module tree_algorithms;


export namespace trees {

void printMessage(int a);

// BSTree<T, Comp, Alloc>
template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class BSTree {
public:
    virtual void build(const T* items, size_t n) = 0;
    virtual void build(const std::vector<T>& items) = 0;

    virtual void insert(const T&) = 0;
    virtual int count(const T&) = 0;
    virtual bool remove(const T&) = 0;
    virtual void clear() = 0;

    // BSTree<T, Comp, Alloc>::Iterator begin() const
    // BSTree<T, Comp, Alloc>::Iterator end() const

    virtual ~BSTree() = default;
};


template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class AVLTree : public BSTree<T, Comp, Alloc> {
public:
    void build(const T* items, size_t n) final override {

    }

    void build(const std::vector<T>& items) final override {

    }

    void insert(const T&) final override {

    }

    int count(const T&) final override {
        return 0;
    }

    bool remove(const T&) final override {
        return false;
    }

    void clear() final override {

    }
};

}