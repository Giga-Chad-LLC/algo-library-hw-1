# Требования

1. Библиотека **не является** потоко-безопасной.
1. Библиотека **не является** интрузивной (то есть дерево будет хранить копии переданных элементов и управлять их временем жизни).
1. Библиотека **позволяет** хранить дубликаты элементов в деревьях.
1. Главный интерфейс: `BSTree<T, Comp, Alloc>` (у нас `BST` значит *balanced* search tree, как бы это странно не было. Это из-за того, что планировалось добавить B-tree, который не является бинарным деревом)
   темплейтный класс, определяющий тип хранимых вершин `T`, компоратор для сравнения вершины (по дефолту `std::less<T>`) и аллокатор памяти (по дефолту `std::allocator<T>`, использующий глобавльные операторы `new` и `delete`).
1. Интерфейсные методы:
    - `void build(const T*, size_t)`, `void build(const std::vector<T>&)` строят дерево из элементов, лежаших по указателю (`T` должен быть [`CopyConstructible`](https://en.cppreference.com/w/cpp/named_req/CopyConstructible)) (**есть реализация через `insert` в [BSTree.cpp](../src/modules/trees/BSTree.cpp) за O(n)**).
    - `void insert(const T&)` добавляет элемент в дерево.
    - `size_t count(const T&)` возвращает кол-во элементов в дереве (**есть глупая реализация в [BSTree.cpp](../src/modules/trees/BSTree.cpp) за O(n)**).
    - `bool remove(const T&)` удаляет элементы из дерева, возвращает `true`, если удаление имело место быть.
    - `void clear()` удаляет все элементы дерева.
    - ~~`BSTree<T, Comp, Alloc>::Iterator begin() const` возвращает итератор, указывающий на наименьший элемент дерева.~~
    - ~~`BSTree<T, Comp, Alloc>::Iterator end() const` возвращает итератор, указывающий на следующий элемент после наибольшего.~~
1. ~~Вложенный интерфейс итератора соответствует [`LegacyForwardIterator`](https://en.cppreference.com/w/cpp/named_req/ForwardIterator).~~


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

# Тестирование

## Экспериментальная среда

1. Стресс-тесты, вызывающие 1M операций из набора (`insert`, `remove`) равновероятно с произвольными параметрами на всех структурах данных. Замеряется суммарное время работы по каждому дереву.