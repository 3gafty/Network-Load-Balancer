# Network-Load-Balancer(Сетевой балансировщик нагрузки)
Постановка задачи:
Требуется разработать разработать сетевой балансировщик нагрузки, работающий в системе Linux, который бы обладал следующими возможностями:
1) читать настройки из конфигурационного файла произвольного формата;
2) принимать UDP-датаграммы с определенного порта(задается файлом конфигурации);
3) перенаправлять датаграммы из п.2 на один из нескольких узлов(серверов) таким образом, чтобы нагрузка на узлы распределялась равномерно.(адреса узлов задаются файлом конфигурации);
4) ограничивать нагрузку на узлы, общее количество датаграмм проходящих через балансировщик не должно превышать N в секунду(задается файлом конфигурации), лишние датаграммы отклоняются.

Задание должно быть выполнено на языке C++ произвольного стандарта(уместное применение ООП/STL приветствуется), с использованием POSIX API для работы с сетью.
Применение библиотек и фреймворков наподобие Qt или boost при выполнении задания не допускается.


Парсера как такового нет, конфигурационный файл требует соблюдения строгого порядка
1 строка количество узлов для пересылки, затем N строк с реквезитами узлов.
Далее через пустую строку еще строка реквизита, это адрес и порт откуда принимаем посылки на вход балансировщика.
Последняя строка со словом FREQ, это значение ограничивающей частоты прихода сообщений(N из задания). Можно при желании прикрутить json либо xml, но лень.

Важное замечание, что частота носит мгновенный характер, т.е. в любой момент вермени если выбрать временное окно шириной 1 секунда на временной шкале, то в этом окне должно быть не более N отправок.

