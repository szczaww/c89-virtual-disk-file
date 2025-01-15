# Struktura
### 1. Nagłówek dysku
Rozmiar łączny: ***20 bajty***
- **4 bajty** Rozmiar dysku - y
- **4 bajty** Offset bitmapy i-node'ów
- **4 bajty** Offset pierwszego i-node'a
- **4 bajty** Offset bitmapy bloków
- **4 bajty** Offset pierwszego bloku:
### 2. Bitmapa i-node'ów
Rozmiar łączny: ***n bajtów***
### 3. i-node'y
Rozmiar łączny: ***n * 80 bajtów***
Rozmiar pojedynczego: 80 bajtów 
- **64 bajty** Nazwa pliku 1: treść
- **4 bajty** Nazwa pliku 2: wskaźnik na blok
- **4 bajty** Flaga ukrycia
- **4 bajty** Rozmiar pliku
- **4 bajty** Adres pierwszego bloku danych pliku
### 4. Bitmapa bloków
Rozmiar łączny: ***n bajtów***
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
$$ y > 20 + n + 80n + n + n(x+4) = $$
$$ = 20 + 80n + xn + 4n + 2n = $$
$$ = 20 + 86n + xn $$
$$ y - 20 > (86 + x)n $$
Zatem:

$$ (y - 20) / (86 + x) > n $$