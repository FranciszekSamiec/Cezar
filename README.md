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
tylko wynik modulo 10^9 + 7.

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
10^9 + 7.

## Przykład

Dla danych wejściowych:  
```text
3 3  
GRG  
? 1 3  
O 1 2  
? 1 3  
```
poprawnym wynikiem jest:  
```text
6  
5  
```
## Rozwiązanie

W rozwiązaniu korzystam z drzewa [splay](https://en.wikipedia.org/wiki/Splay_tree). Węzeł reprezentuje żołnierza a porządek infiksowy na drzewie wyznacza kolejność żołnierzy (operacje splay na drzewie nie zaburzają tego porządku). Każdy węzeł podsumowuje informacje zawarte w poddrzewie tego węzła (które to opisuje pewien przedział zołnierzy). Na przykład w poniższym drzewie korzeń opisuje przedział [1, 5] natomiast węzeł 2 opisuje przedział [1, 3]. 
```text
     4  
    / \  
   2   5  
  / \  
 1   3  `
```
  Ponadto każdy węzeł zawiera informacje ile jest poprawnych ciągów żołnierzy kończących się na R/G i zaczynajcych się na R/G w danym segmencie (opisywanym przez ten węzeł). Dodatkowo każdy węzeł wie ile ma lewych i prawych potomków po to żeby utrzymać porządek. Numeracja nie zadziałałaby gdyż dopuszczamy operacje odwracania. Szukamy tak jak w drzewie BST. operacja splay(n, x) znajduje nam wierzcholek x w poddrzewie z korzeniem n i wykonuje odpowiednie rotacje przemieszczajac wierzcholek x do korzenia (x jest nowym korzeniem). Mamy jeszcze flage w każdym węźle, która mówi czy segment opisywany przez poddrzewo jest odwrócony czy nie. Jeżeli więc chcielibyśmy zapytać ile jest poprawnych ciągów z szeregu [2, 4] to najpierw wykonujemy splay(n,2) (n - korzeń całego drzewa) potem odcinamy lewe poddrzewo aktualizując informacje wierzchołka o numerze 2, następnie robimy splay(n, 4) (szukamy w prawym poddrzewie 4 i "spleujujemy" tę 4) odcinamy prawe poddrzewo (aktualizując info tak samo jak przy odcinaniu lewego) i w korzeniu tego już środkowego drzewa jest interesująca nas informacja. Po odczytaniu informacji łączymy wszystkie drzewa oczywiście znów aktualizując wszelkie informacje w węzłach. Aktualizacja informacji w węźle jest niedroga (O(1)). Jeżeli natrafimy na flagę reverse to "spychamy" ją w dół do synów jednocześnie odwracając role synów - lewy staje się prawym a prawy lewym. Zamiana flagi to polega na inkrementacji mod 2. 
  Początkowo drzewo jest niezbalansowane - budując drzewo tworzy się po prostu długa gałąź lewych synów jednak wykonania splay szybko balansują strukturę dzięki czemu operacje typu ? i O są wykonywane w zamortyzowanym koszcie O(log(n)), koszt pamięciowy to O(n) gdzie n to liczebność szeregu żołnierzy. Całkowity koszt czasowy zatem to O(mlog(n) + n) dodatkowe n bierze się z konieczności zbudowania drzewa na którym będziemy operować. 
  
Mamy następujące drzewo:
```text
      G
     / \
    G   G
   /   / \
  R   R   R
[narodowosc, index]
Dla uproszczenia wszystkie flagi reverse są równe 0, indexy są wykorzystywane tylko na początku przy budowie drzewa kiedy nie ma jeszcze
ilości lewych i prawych potomków, pokazują one wyraźnie porządek infiksowy - kolejność żołnierzy oraz zachowanie operacji splay przy wykonaniu zapytania typu "?".
```
Przykład wykoniania zapytania ? 2 4
```text
pierwszy **splay**:
     G
    / \
   R   G
        \  
         G
        / \
       R   R
odcięcie lewego i splay 4 elementu:
     R
    / \
   G   G
    \   \
     G   R
