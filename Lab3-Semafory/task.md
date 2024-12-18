# Zadanie 

1) n producentów, każdy z nich jednorazowo produkuje partię towaru o
losowej liczbie sztuk opisanej rozkładem jednostajnym dyskretnym na
przedziale [a; b],
2) m konsumentów, każdy z nich jednorazowo zgłasza zapotrzebowanie na
partię towaru o losowej liczbie sztuk opisanej rozkładem jednostajnym
dyskretnym na przedziale [c; d],
3) magazyn o pojemności k sztuk towaru,
4) producenci przekazują towar konsumentom za pośrednictwem magazynu,
5) producenci i konsumenci obsługują tylko całe partie towaru (producent
nie może wstawić do magazynu części wyprodukowanej partii towaru,
konsument nie może odebrać z magazynu części wymaganej partii towaru),
6) produkcja i zgłaszanie zapotrzebowań na partie towaru odbywają się
nie wcześniej niż po upływie założonego czasu np. 1 sekundy.


# Założenia

System działa w sposób ciągły (zatrzymanie to przerwanie programu).

Magazyn powinien być zrealizowany jako plik tekstowy, w którym zapisana
jest wartość liczbowa reprezentująca liczbę sztuk towaru dostępnego w
magazynie. Zmiana stanu magazynu wiąże się ze zmianą wartości tej liczby
(liczba jest nadpisywana).

Każdy producent zapisuje do odrębnego logu informacje o wyprodukowanych
partiach towaru i wstawionych partiach towaru do magazynu. Zostaje
utworzonych n plików tekstowych.

Każdy konsument zapisuje do odrębnego logu informacje o
zapotrzebowaniach na partie towaru i pobranych partiach towaru z
magazynu. Zostaje utworzonych m plików tekstowych.


# Monitorowanie

watch -n 1 cat magazyn.txt
tail -f konsument1.log
tail -f producent1.log