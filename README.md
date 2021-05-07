# Network-Load-Balancer
Test task

Парсера как такового нет, конфигурационный файл требует соблюдения строгого порядка
1 строка количество узлов для пересылки, затем N строк с реквезитами узлов.
Далее через пустую строку еще строка реквизита, это адрес и порт откуда принимаем посылки на вход балансировщика.
Последняя строка со словом FREQ, это значение ограничивающей частоты прихода сообщений.
Ограничение реализовано через засыпание основного потока в цикле приема/отправки сообщений.

Для теста использовались самописные сервера. Предполагалось, что посылки на вход балансировщика бывают хотя бы раз в секунду, т.е. N - целое число.

