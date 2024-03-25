# Анинатор

<image src="for_readme/akinator.jpeg" width=700px>

## Что он делает?
Этот проект является моей реализацией известной игры Akinator! 
Смысл игры заключется в том, что пользователь должен загадать любого персонажа, а Джинн Анишка — главный персонаж игры — должна его отгадать.

## Какой принцип работы?
### 1. Создание дерева
Программа принимает на вход файл со строки. Эта строка имеет определенную структуру, тк способ обхода дерева - pre-order. <b>Например:</b>

```
( <Анинатор крутой проект?> ( <Это неправда((> <nil> <nil> ) ( <Очень приятно:)>  <nil> <nil> )  ) 
```
Разделителями между узлами является открывающаяся и закрывающаяся скобки. Узлы заключаются в символы <b><</b> и <b>></b>. Для обозначения отсутствия узла используется ключевое слово <b>nil</b>.
Листьями всегда являются имена персонажей/людей, а остальные узлы вопросительные предложения(обязательно заканчивающиеся на символ ?). Левое ребро указывает на положительный ответ на вопрос, правое - на отрицательный.

в итоге на основании выражения
```
( <Это мальчик?> ( <Он с фалта?> ( <Он армянин?> ( <Марк> <nil> <nil> ) ( <Он лапушок?> ( <Сережа> <nil> <nil> ) ( <Тимтимыч> <nil> <nil> ) ) ) ( <Он абьюзер?> ( <Вовчик> <nil> <nil> ) ( <Он из 434 gaaaang?> ( <Он чурка?> ( <Чебурашка(рузалич)> <nil> <nil> ) ( <Геныч(дима)> <nil> <nil> ) ) ( <Он мой ментор?> ( <Крис> <nil> <nil> ) ( <Демк> <nil> <nil> ) ) ) ) ) ( <Это я?> ( <Ани> <nil> <nil> ) ( <Она была моей соседкой?> ( <Она моя самая любимая пиздоглазая?> ( <Ксюшик> <nil> <nil> ) ( <Она живет в 13?> ( <Она жжжгучая?> ( <Сашка-изолента> <nil> <nil> ) ( <Лизок> <nil> <nil> ) ) ( <Настя> <nil> <nil> ) ) ) ( <Иришка-сижка> <nil> <nil> ) ) ) )  
```
можно построить дерево:

<image src="tree/dump.png" width=700px>

### 2. Игра

Пользователю на выбор предоставляется 3 режима:
<ul>
 <li>Угадывание персонажа (Guess)</li>
 <li>Сравнение двух персонажей (Compare)</li>
 <li>Вывод информации о персонаже (Info)</li>
</ul>

#### 2.1. Guess

При запуске режима указатель находится на корне дерева. Пользователю предлагается положительный и отрицательный варианты ответа на вопрос в узле. Если ответ <b>yes</b>, то указатель в дереве перемешается на левого ребенка, если <b>no</b> - на правого. Так продолжается до тех пор, пока указатель не дойдет до листика.

#### 2.2. Compare

Пользователь вводит имена двух персонажей. Для их поиска в дерево выполняется обход в глубину. В стеки кладутся узлы и ребра, которые проходились по пути к пероснажам. Например для Ани и Ксюши стеки будут иметь такой вид:

<image src="for_readme/stack1.png" width=600px>

Далее из стеков вынимаются не общие узлы:

<image src="for_readme/stack2.png" width=600px>

На основе этих стеков и выполняется сравнение персонажей.

#### 2.3. Info

Принцип идентичен прошлому режиму, только теперь создается один стек для введенного персонажа:

<image src="for_readme/stack3.png" width=300px>

На основе этого стека выполняется вывод информации о персонаже.

### 3.1. Запись дерева

Данная функция реализуется только в режиме Guess. 
Если Анинатор не сумел определить загаданного персонажа, то можно добавить его! В либо дерево записывается один узел, либо дополнительный вопрос и узел.
Запись дерева в файл производится обратно <b>1-ому пункту</b> чтению.

## Как запустить программу?

```
make
```
Вторым аргументом в данной команде является название файла со строкой-деревом, а третьим названия файла для вывода дерева иначе будет обрабатываться файлы по-умолчанию:

```
./main_ <input файл> <output файл>
```
Для работы с измененным в процессе программы деревом, второй и третий аргументы команды дожны совпадать.
После работы программы можно удалить все объектники командой

```
make clean
```

## Как выглядит результат работы программы?
Пример результата работы режима Guess:

<image src="for_readme/console1.png" width=500px>

Пример результата работы режима Compare:

<image src="for_readme/console2.png" width=400px>

Пример результата работы режима Info:

<image src="for_readme/console3.png" width=500px>