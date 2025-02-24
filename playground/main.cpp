#include <iostream>
#include <cassert>

import tree_algorithms;

namespace {

struct Data {
    int key;
    int value;
};

struct DataCmp {
    bool operator()(const Data& a, const Data& b) const {
        if (a.key == b.key) return a.value < b.value;
        return a.key < b.key;
    }
};

}

int main() {
    std::cout << "Hello from playground!" << std::endl;
    trees::TreapTree<Data, DataCmp> tree;

    const Data a{1, 0};
    const Data b{1, 10};
    const Data c{2, -10};

    tree.insert(a);
    tree.insert(a);

    tree.insert(b);
    tree.insert(c);

    assert(tree.count(a) == 2);
    assert(tree.count(b) == 1);
    assert(tree.count(c) == 1);

    assert(tree.remove(a) && tree.remove(a));
    assert(tree.remove(b));
    assert(tree.remove(c));

    return 0;
}