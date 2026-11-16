# Akinator

Консольная игра-угадайка на C. База знаний - бинарное дерево на указателях,
узлы которого хранят вопросы и ответы. Поддерживает сохранение/загрузку деревьев
в текстовые файлы и опциональное озвучивание через Festival (русский голос).

Реализация игры с угадываением загаданных слов

# Особенности
- База данных реализована на бинарных деревьях
- Есть возможность сохранения БД в файл и чтение из него
- Добавлено озвучивание вопросов с помощью festival
- Есть возможность получить определение слова при его наличии в БД
- А также сравнение 2 слов и "на сколько % ворон крыса"
- Реализован графический дамп дерева БД в лог-файл

дамп:
<img width="857" height="633" alt="image" src="https://github.com/user-attachments/assets/2b38e1bd-44fe-4cd3-866f-9d356ae82e01" />


# Структура данных

Основа - база данных в виде бинарного дерева на указателях.
Вершина содержит вопрос и два указателя на потомков - left и right. Если ответ на вопрос "Да", то акинатор рссматривает левое поддерево, если "нет" - правое
Текст вопроса и нода хранятся в динамической памяти с помощью strdup и calloc соответственно

структура:
```c
typedef struct Node {
    char *text;          // вопрос
    struct Node *left;   // "да"
    struct Node *right;  // "нет"
} Node;
```

База данных хранится в формате filanme.tree


# Файлы

- main.c - сама программа акинатора
- tree.c \ tree.h - реализация бинарного дерева
- log.c  \ log.h - логи

# Сборка
```
g++ main.c tree.c log.c common.c stack.c -o ./akinator
```

# Запуск
```
./akinator
```

# Интерфейс программы
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>>
```

В 1 режиме акинатор будет задавать вопросы:
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 1
Это животное?
[Y/n] Y
Полторашка?
[Y/n] Y
This was: Полторашка. I'm very clever!
```
Если акинатор не знает это слово, он попросит добавить его в БД:
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 1
Это животное?
[Y/n] Y
Полторашка?
[Y/n] n
:( I don't know... What was that?
>>> Собака
What is the difference? Собака...
>>> Гавкает
```
Во 2 режиме он может дать определение слова, если оно есть в БД
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 2
Enter a name, if I know it, I will find and say what it is
>>> Полторашка
Полторашка is: Это животное, не Гавкает.
```
3 и 4 режим просят ввести название файла и соответсвенно сохранит\загрузит БД
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 3
Enter a filename to save
>>> readme.tree
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 4
Enter a filename to load
>>> readme.tree
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>>
```
5 режим сравнивает 2 слова:
```
=====Akinator ver 0.1=====
What do you want to do?
1) Guess      2) Descritption
3) Save       4) Load
5) Difference 6) Exit
>>> 5
Enter a first name, if I know it, I will find and say what it is
>>> Полторашка
Enter a second name, if I know it, I will find and say what it is
>>> Собака
Полторашка and Собака are same in: Это животное, но Полторашка is не Гавкает, а Собака is Гавкает
Полторашка на 50.000000% Собака
```
6 - выход
