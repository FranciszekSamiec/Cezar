# Cezar

  Jest ciężko. Wercyngetoryks, wódz galijskiej armii, ufortyfikował się w najbliższym mieście. Galijscy sojusznicy z północy obiecali nam dostawę zaopatrzenia, ale pewnie tylko sam Jupiter wie, kiedy i czy w ogóle
ona dotrze. Trzeba zebrać zapasy z okolicy. Cezar polecił Ci sformowanie patroli, które przeczeszą okolice w
poszukiwaniu jedzenia.
  Ustawiłeś swoich żołnierzy jeden długi szereg o długości n. Każdy patrol stanowi pewien niepusty podciąg
żołnierzy - na rozkaz wskazani żołnierze występują z głównego szeregu i ustawiają się w nowy, zachowując
dotychczasową kolejność.
  Cezar przechadza się wzdłuż szeregu i wydaje Ci polecenia:
- `? i j` - polecenie typu ? - oblicz, na ile sposobów można wybrać godny zaufania patrol spośród żołnierzy
pomiędzy i-tym a j-tym włącznie. Jako że liczba sposobów może być bardzo duża, Cezarowi wystarczy
tylko wynik modulo 109 + 7.

Każdy rzymski legionista (’R’) jest godny zaufania. A przynajmniej tak musisz założyć. Jednak niestety, oprócz
Rzymian, Twój oddział zawiera także galijskich sojuszników z północy (’G’). O ile Galowie są podstępni, to w
otoczeniu legionistów są raczej niegroźni. Zatem warunkiem na to, by patrol był godny zaufania, jest to, by
dwóch Galów nie stało obok siebie. W ten sposób nie będą mogli ze sobą swobodnie spiskować. Divide et impera,
czyli dziel i rządź. Jako że rządzenie powinno polegać na łączeniu, to w wolnym tłumaczeniu: Split&Join.
  Cezar wie, że nie może tasować Galów zupełnie bezkarnie. By korzystnie przebudować szereg i uniknąć
podejrzeń o dyskryminację (co mogłoby go narazić na bunt galijskich sojuszników, albo, co gorsza, wizytę
z rzymskiego HRu), Cezar postanowił, że będzie operował na szeregu używając następującego, etnicznieneutralnego rozkazu:
- `O i j` - polecenie typu O - odwróć kolejność żołnierzy między i-tym, a j-tym włącznie.
Dzięki temu poleceniu Cezar będzie mógł osiągnąć dowolną zaplanowaną przez siebie kolejność w szeregu.
  
 ## Wejście
 
 W pierwszym wierszu wejścia podane są dwie liczby n i m (1 ≤ n, m ≤ 100 000) określające odpowiednio
liczebność szeregu i liczbę zapytań Cezara. Następny wiersz zawiera opis początkowego stanu szeregu, czyli
n znaków (’R’ lub ’G’) Kolejne m wierszy jest w formie t i j (t ∈ {?, O}, i ≤ j, 1 ≤ i, j ≤ n), każdy z nich
definiuje pojedyncze polecenie Cezara.

## Wyjście

Na każde polecenie typu ? odpowiedz jedną liczbą w wierszu oznaczającą liczbę sposobów na wybranie godnego
zaufania patrolu z żołnierzy z zadanego przedziału. Jako że odpowiedź może być duża, wynik wypisz modulo
109 + 7.

## Przykład

Dla danych wejściowych:  
3 3  
GRG  
? 1 3  
O 1 2  
? 1 3  

poprawnym wynikiem jest:  
6  
5  

## Rozwiązanie

W rozwiązaniu korzystam z drzewa [splay](https://en.wikipedia.org/wiki/Splay_tree). Węzeł reprezentuje żołnierza a porządek infiksowy na drzewie wyznacza kolejność żołnierzy (operacje splay na drzewie nie zaburzają tego porządku). Każdy węzeł podsumowuje informacje zawarte w poddrzewie tego węzła (które to opisuje pewien przedział zołnierzy). Na przykład:
           4    
          / \  
         2   5  
        / \  
       1   3  
węzeł 2 reprezentuje przedział [1, 3] a węzeł 4 reprezentuje cały ciąg [1, 5] (szereg żołnierzy).
