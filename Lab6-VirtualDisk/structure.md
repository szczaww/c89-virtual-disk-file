# Struktura
### 1. Nagłówek dysku
Rozmiar łączny: ***48 bajty***
- **4 bajty** Rozmiar dysku - y
- **4 bajty** Rozmiar bloku - x
- **4 bajty** Offset bitmapy i-node'ów
- **4 bajty** Offset pierwszego i-node'a
- **4 bajty** Offset bitmapy bloków
- **4 bajty** Offset pierwszego bloku:
### 2. Bitmapa i-node'ów
Rozmiar łączny: ***n bitów***
### 3. i-node'y
Rozmiar łączny: ***n * 80 bajtów***
Rozmiar pojedynczego: 80 bajtów 
- **64 bajty** Nazwa pliku 1: treść
- **4 bajty** Nazwa pliku 2: wskaźnik na blok
- **4 bajty** Flaga ukrycia
- **4 bajty** Rozmiar pliku
- **4 bajty** Adres pierwszego bloku danych pliku
### 4. Bitmapa bloków
Rozmiar łączny: ***n bitów***
### 5. Bloki danych
Rozmiar łączny:  ***n(x + 4 bajty)***
Rozmiar pojedynczego: x + 4 bajtów 
- **x bajtów** Dane
- **4 bajty** następny blok danych pliku
# Rozmiar bloku
Symbole:
y - rozmiar dysku
x - rozmiar danych
n - ilość bloków
$$ y > 28 + n/8 + 80n + n/8 n(x+4) = $$
$$ = 28 + 80n + xn + 4n + n/4 = $$
$$ = 28 + 84.25n + xn $$
$$ y - 28 > (84,25 + x)n $$
Zatem:

$$ (y - 28) / (84,25 + x) > n $$