# Расширение

Далее представлен путь написания дерева и внедрение его в тесты и бенчмаркинг. По сути у нас 3 шага:
1. Имплементация
2. Unit-тесты
3. Бенчмаркинг

Далее они и описаны.

### Имплементация
1. Чтобы добавить свое дерево, нужно реализовать имплементора [TreeNode<T>](../src/modules/nodes/TreeNode.cpp) (см. как остальные сделаны) и базового интерфейса дерева [BSTree.cpp](../src/modules/trees/BSTree.cpp). Можно посмотреть как сделаны [TreeNode.cpp](../src/modules/nodes/TreeNode.cpp) и [TreapTree.cpp](../src/modules/trees/TreapTree.cpp)_.
2. Не забываем указать созданный `MyTree.cpp` в [CMakeLists.txt](../src/CMakeLists.txt) модуля [src](../src).
3. У нас используются C++ модули, поэтому нужно немножко потерпеть (а потом еще немножко) и соблюсти структуру в `MyTree.cpp` как в других деревьях.
   1. По C++ модулям можно посмотреть вот эту презентацию на YouTube: [C++ Modules - Getting started today - Andreas Weis - Meeting C++ 2024](https://www.youtube.com/watch?v=NI7dAiWwibY).
   1. **Важно**: открываем [tree_algorithms.ixx](../src/modules/tree_algorithms.ixx) и пишем `export import :MyTree;`. Иначе наше дерево никто не сможет увидеть из вне модуля.
4. **Важно**: _не используйте `new` и `delete` для создания/удаления вершин дерева. Для этого сделаны 2 специальных метода `create()` и `destroy()` внутри дерева_. **Смотри пример ниже**.

Например:

1. Сначала сделали новую ноду:

```cpp
// src/nodes/MyTreeNode.cpp

module;

#include <random>
#include <limits>


export module tree_algorithms:MyTreeNode;
import :TreeNode;

namespace trees::nodes {

    template <class T>
    struct MyTreeNode final : public TreeNode<T> {
        explicit MyTreeNode(const T& value): TreeNode<T>(value) {}

    private:
        void my_custom_method() { /* implementation */ }
    };

}
```

2. Далее создаем новое дерево:

```cpp
// src/trees/MyTree.cpp

module;

export module tree_algorithms:MyTree;
import :BSTree;
// nodes
import :TreeNode;
import :MyTreeNode;

namespace trees {

using namespace nodes;

/**
 * Do NOT forget about documentation!
 */
export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class MyTree final : public BSTree<T, Comp, Alloc> {
    using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<MyTreeNode<T>>;

public:
    explicit MyTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
        : BSTree<T, Comp, Alloc>(comparator, allocator),
        m_node_allocator(this->m_allocator) {}

    void insert(const T& value) override { /* implementation */ }

    bool remove(const T& value) override { /* implementation */ }

    ~MyTree() override {
        this->clear();
    }

private:
    std::pair<TreeNode<T>*, TreeNode<T>*> split_example_method(TreeNode<T>* root, const T& splitValue) const { /* implementation */ }

    TreeNode<T>* merge_example_method(TreeNode<T>* t1_, TreeNode<T>* t2_) const { /* implementation */ }

    // NOTE: use `create`/`destroy` instead of `new`/`delete`
    TreeNode<T>* create(const T& value) override {
        return this->BSTree<T, Comp, Alloc>::template create<MyTreeNode<T>>(m_node_allocator, value);
    }

    void destroy(TreeNode<T>* node) override {
        this->BSTree<T, Comp, Alloc>::template destroy<MyTreeNode<T>>(m_node_allocator, static_cast<MyTreeNode<T>*>(node));
    }

    NodeAllocatorType m_node_allocator;
};

}
```


### Unit-тесты

1. Заходим в [ParameterizedTreeTests.h](../tests/components/ParameterizedTreeTests.h) и пишем такое:
```cpp
template <typename T, typename Comp, typename Alloc> using MyTreeType =
   trees::MyTree<T, Comp, Alloc>;

// ...

using TreeTypeTemplates = ::testing::Types<
   // ...
   TreeTemplateWrapper<MyTreeType>
>;
```
2. Теперь запускаем CMake таргет с тестами через `make test` и должны увидеть строчку, в которой есть `where TypeParam = TreeTemplateWrapper<MyTreeType>`.

### Бенчмаркинг

Тут чуть потерпеть нужно. Но это последнее место.

1. Открываем в [playground/main.cpp](../playground/main.cpp) и пишем в функции `make_tree_by_name` новую if-ветку (**важно, что в условии мы пишем именно `"MyTree"`, потом в yaml-конфиге эту же строку надо будет указать, это **tag** нашего дерева):
```cpp
 std::pair<std::string, tree_ptr<int>> make_tree_by_name(const std::string& name) {
     // ...
     if (name == "MyTree") {
         return { "MyTree<int>", std::make_unique<trees::MyTree<int>>() };
     }
     throw std::runtime_error("Unknown tree type provided: " + name);
 }
```
2. Открываем [configs/benchmarking.yaml](../configs/benchmarking.yaml) и в секцию `trees` дописываем свой tag дерева (**тот же, что в `main.cpp` выше!**):
```yaml
tree_benchmark_config:
  trees:
    ...
    - MyTree
```
3. Теперь пытаемся запустить [scripts/benchmarking.py](../scripts/benchmarking.py), запустив команду (ожидаем, что в CLI выведется вердикт по исполнению для нашего дерева `"MyTree"`):
```bash
# Если мы еще не в virtual env
# Unix (точка в начале не случайна!)
. .venv/bin/activate
make playground
# или: `python ./scripts/benchmarking.py -b ./build/playground/playground_tree_algorithms -c ./configs/benchmarking.yaml`
```

### Наставление

**Если что-то не получилось, то пишете в тг [@Vladislav0Art](https://t.me/Vladislav0Art), [@dmitrii_artuhov](https://t.me/dmitrii_artuhov), [@youka4](https://t.me/youka4). Будем терпеть вместе!**

