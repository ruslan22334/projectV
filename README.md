# projectV

На текущий момент, main содержит запуск теста на DNN и попытку вызвать функцию insertSite из voronoi_graph (которая не завершает работу корректно), на остальные структуры тестов нет

DNN реализован как KDtree + медленная реализация для проверки 
текущий граф Вороного должен храниться явно в виде списка вершин (вставка вершины не работает), DCEL фактически отсутствует
так как insertSite не доделан, большие ячейки в алгоритме как объект отсутствуют, но для их работы написаны DCR (просто линейный, этого вроде должно быть достаточно) и YardTree (реализовано с декартовым деревом), но пока не используются

