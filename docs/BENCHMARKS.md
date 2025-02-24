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
    Total time: 8588ms
    Total insert time: 4721.42ms, Average insert time: 0.00944551ms
    Total remove time: 2630.33ms, Average remove time: 0.00525918ms
    
    === Report for 'SplayTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 5584.9ms
    Total insert time: 2284.1ms, Average insert time: 0.0045695ms
    Total remove time: 2117.74ms, Average remove time: 0.00423428ms
    
    === Report for 'TreapTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 4336.97ms
    Total insert time: 1886.56ms, Average insert time: 0.00377418ms
    Total remove time: 1339.78ms, Average remove time: 0.0026788ms
    ```
   Выводы: все имплементации написаны на указателях, причем в Splay и AVL деревьях разыменований существенно больше, чем в Treap,
   из-за этого они работают медленнее. При этом AVL в своей реализации методов `insert` и `remove` использует `std::vector`
   для сохранения пути от корня до нужной вершины, что сильно замедляет работу из-за динамических переаллокаций внутри вектора.