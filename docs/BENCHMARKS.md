# Benchmarks

Бенчмарк прогоняется как часть юнит-тестов (таргет `test_tree_algorithms`.
Смотреть файл [`tests/test_tree_algorithms.cpp`](../tests/test_tree_algorithms.cpp) на тест `TEST(TreesBenchmark, MeasureExecutionTimes)`.

### Описание

- Для каждого дерева вызываются операции `insert` и `remove` равновероятностно, суммарное число операций: 1M.
- Выводится:
  - Среднее время выполнение операций для каждого дерева
  - Общее суммарное время исполнения
- Запуск с флагами cmake release сборки

### Результаты

1. **Linux: WSL2 Ubuntu 22.04** (Intel(R) Core(TM) i7-8550U CPU @ 1.80GH)
    ```txt
    === Report for 'AVLTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 12700ms
    Total insert time: 6836.7ms, Average insert time: 0.0136773ms
    Total remove time: 3881.96ms, Average remove time: 0.00776173ms
    
    === Report for 'SplayTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 7863.11ms
    Total insert time: 3250.65ms, Average insert time: 0.00650314ms
    Total remove time: 2937.7ms, Average remove time: 0.00587374ms
    
    === Report for 'TreapTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 4808.83ms
    Total insert time: 2099.25ms, Average insert time: 0.00419969ms
    Total remove time: 1506.55ms, Average remove time: 0.00301224ms
    ```
   Выводы: все имплементации написаны на указателях, причем в Splay и AVL деревьях разыменований существенно больше, чем в Treap,
   из-за этого они работают медленнее. При этом AVL в своей реализации методов `insert` и `remove` использует `std::vector`
   для сохранения пути от корня до нужной вершины, что сильно замедляет работу из-за динамических переаллокаций внутри вектора.