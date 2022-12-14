# QtTable
Find a path on a board using Qt

Чтобы запустить приложение, необходимо:
  1. Загрузить файлы.
  2. Запустить main.cpp

Инструкция по работе с программой:
  - Снизу в окнах X и Y задаются размеры поля. При вводе в о кна размер 0, высветится ошибка.
  - Для того, чтобы сгенерировать поле нужно сначала прописать размеры поля в диапазоне от 0 до 99 и нажать на кнопку PushButton.
  - При одном нажатии на сгенерированном поле появится изначальная точка в виде эллипса, затем при перемещении по полю мышкой будет отрисовываться путь из выбранной ячейки до ячейки, на которую наведён курсор.
  - Путь подсчитывается с помощью алгоритма в ширину(bfs).
  - Путь и подсчет поля высчитываются в отдельных потоках.
  - Изменить масштаб поля можно с помощью колесика мыши.
  - При выходе из программы текущие параметры поля сохранятся и при повторном запуске они восстановятся.

Пример работы программы:
![alt text](https://github.com/stupakdm/QtTable/blob/main/TableBoard.JPG)
