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
    Total time: 7815.41ms
    Total insert time: 4269.16ms, Average insert time: 0.00854072ms
    Total remove time: 2407.34ms, Average remove time: 0.00481333ms
    
    === Report for 'SplayTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 9781.5ms
    Total insert time: 4123.87ms, Average insert time: 0.00825006ms
    Total remove time: 3787.97ms, Average remove time: 0.00757381ms
    
    === Report for 'TreapTree<int>' ===
    Operations total: 1000000
    Insert: 499859
    Remove: 500141
    Total time: 5549.72ms1
    Total insert time: 2384.65ms, Average insert time: 0.00477065ms
    Total remove time: 1700.91ms, Average remove time: 0.00340086ms
    ```
   Выводы: все имплементации написаны на указателях, причем в Splay и AVL деревьях разыменований существенно больше, чем в Treap,
   из-за этого они работают медленнее. При этом AVL имеет меньше вращений и переподвешиваний поддеревьев, чем AVL, поэтому работает шустрее.