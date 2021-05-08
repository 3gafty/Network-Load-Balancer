# Network-Load-Balancer
Test task

Парсера как такового нет, конфигурационный файл требует соблюдения строгого порядка
1 строка количество узлов для пересылки, затем N строк с реквезитами узлов.
Далее через пустую строку еще строка реквизита, это адрес и порт откуда принимаем посылки на вход балансировщика.
Последняя строка со словом FREQ, это значение ограничивающей частоты прихода сообщений(N из задания).

Для теста использовались самописные сервера. Исправлено поведение в случае длительного таймаута между посылками.

TODO:
  для правильного завершения цикла приема/отправки можно сделать глобальный флаг, который можно изменить по сигналу SIGINT, SIGTERM, но для этого сокет должен быть не блокирующим, иначе даже при установе влага false следующая проверка условия цикла произойдет после прибытия очередного сообщения. Можно инициировать сообщение самому себе для завершения итерации цикла.

