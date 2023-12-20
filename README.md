# Проект PoroMarker https://github.com/emilakper/poromarker
## Задача: проверка разметки пор
1) Обнаружение "висячих камней" (части материала, не закрепленные к материалу)
2) Сообщение об ошибке в разметке


Входные данные: сегментированные изображения (маски)


Выходные данные: Сообщения об ошибке и маски с отмеченной компонентой связности, где есть ошибка

Формат работы: библиотека, программная компонента

Подход:
- С помощью построения связного графа, где компоненты, которые не свзаны с основной частью, будут считаться висячими камнями
- Подсчет количества вершин в компоненте

## Детальное описание:
Классификация ошибок: висячий камень – dandling rock
 

Поиск висячих камней: Сначала выделяются компоненты связности на одной маске, далее таким образом обрабатываются остальные маски. 
 
![image](https://github.com/RomadovaIrina/misis2023f-22-04-romadova-i-o/assets/113946752/bf8a25e2-8c44-428f-aeee-e18d1732102d)


После выделения компонент связности на каждой маске строится связный граф по выделенным компонентам. Теперь на связность- наличие ребра, указывает пересечение компонент связности на двух соседних слоях.
 
![image](https://github.com/RomadovaIrina/misis2023f-22-04-romadova-i-o/assets/113946752/57bda934-bdb2-4fc4-92de-af0acfd86d07)


После построения связного графа проводится его анализ. При наличии таких компонент связности, которые связаны только с порой (то есть отсутствует путь до этой компоненты) сохраняется информация о данном висячем камне.
 
![image](https://github.com/RomadovaIrina/misis2023f-22-04-romadova-i-o/assets/113946752/4aafbb88-a7c5-4991-a1c1-352938adbd93)


Далее, после анализа, сохраняется информация об ошибках и передается дальше.
Для упрощения задачи было решено анализировать компоненты связности, основываясь на их весе, то есть количестве вершин, входящих в эту компоненту. Таким образом, пользователь определит пороговый вес, начиная с которого компонента связности графа будет считаться висячим камнем.

## Описание текущего состояния: 
-	Осуществлен поиск компонент связности отдельно на каждом слое
-	Построен и проанализирован граф по всем слоям
-	Информация о висячих камнях сохраняется
-	Подготовлен набор тестовых данных
-	В системе хранения версий находится первая версия функционала библиотеки (не собирается)
## Описание планов:
-	Разбить библиотеку на файлы
-	Подготовить и описать вторую версию тестовых данных
-	Подготовить документацию
-	Подготовить инструкции по сборке  и тестированию



