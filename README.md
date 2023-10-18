# Проект PoroMarker
## Задача: проверка разметки пор
1) Обнаружение "висячих камней" (части материала, не закрепленные к материалу)
2) Сообщение пользователю об ошибке в разметке


Входные данные: сегментированные изображения


Выходные данные: изображения не меняются, но возвращаются ошибки при наличии


Формат работы: библиотека (?)

Подход:
- С помощью расстояний "висячих камней" до ближайшей части материала (чем больше камень и меньше расстояние, тем больше вероятность, что это все таки материал)
- С помощью построения связного графа, где компоненты, которые не свзаны с основной частью, будут считаться висячими камнями


Инструменты:
- OpenCV компоненты связности
