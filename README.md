# Műveletek nagy számokkal
## Bevezető
Az alkalmazás egy egész számokból álló matematikai formulát értékel ki. A cél egy olyan adattárolási módszer kifejlesztése volt, aminél nem állhat fenn túlcsordulás, vagy ha mégis, akkor az fizikailag lehetetlen lenne. **Jelenleg az összeadás (+), kivonás (-), szorzás (\*), hatványozás (^) és a faktoriális (!) van megvalósítva.** A további célok közé tartozott a moduláris felépítés, valamint a testreszabhatóság. Jelenleg az alkalmazás az stdinről olvas, és az stdoutba ír.
## Fordított lengyel jelölés
A fordított lengyel jelölést (angolul Reverse Polish Notation - RPN) Jan Łukasiewicz lengyel jelöléséről kapta a nevét. A lengyel jelölés ún. prefix operátorokat használ (az operátor az operandusok előtt vannak). **Az RPN** hasonló módon, de **postfix operátorokat használ (az operátor az operandusok után van).** Mind a két ábrázolási módszer legnagyobb előnye a **zárójelek elhagyása, és a precedencia elhagyása (sorban értékeljük ki)**.
A problémát egyedül az okozta, hogy az emberek hagyományosan infix operátorokat használnak (az operátor az operandusok között van), viszont ez a számítógép számára nem értelmezhető, vagy csak nagyon nehézkesen. **A postfix operátorokkal viszont nagyon szépen és egyszerűen tud dolgozni az eszköz a verem használatával.** A postfix és az infix algoritmus közötti váltást az ún. Shunting-yard algoritmussal oldjuk meg.
## Shunting-yard algoritmus
A Shunting-yard algoritmus (magyarul rendezőudvar algoritmusként lehetne nevezni) E. W. Dijkstra nevéhez kötődik. A célja egyértelmű: **Infix matematikai formulákat alakít postfixre.** Ez egy viszonylag egyszerű, de mégis komplex algoritmus. Két fajta adatstruktúrát használ: **verem és várólista.**

**A verem LIFO (Last In First Out), míg a várólista FIFO (First In First Out).** Egyik típus sem létezik C-ben, viszont megvalósításuk egyszerű: láncolt listával. Magára az algoritmus működésére itt nem térnék ki, az interneten, valamint a forráskódban le van írva.
## Hatékonyság
Röviden: **Lassú és helyigényes.** Hasonlítsuk össze a C-ből megismert int változóval, és annak variációjával. A szám ebben az algoritmusban szövegként van ábrázolva, valamint külön egységben tároljuk az előjelet. Emiatt a legkisebb számjegy is tulajdonképpen 4 bájtot foglal (1 előjel + 1 számjegy + 1 lezáró nulla + 1 pointer a szövegre). Egy számjegyhez elég egy char típus, ami 1 bájtot jelent. Ez négyszeres növekedés. Nézzük a legnagyobb típust (unsigned long long int).

A maximális szám itt a 2^64 - 1, ami 18 446 744 073 709 551 615, ez 20 számjegy. Ebben az esetben ez 23 bájtot jelent (1 előjel + 20 számjegy + 1 lezáró nulla + 1 pointer a szövegre). Ezt unsigned long long inttel 8 bájton el tudjuk tárolni. Ez két és félszeres növekedés. Ebből szépen látszik, hogy minden esetben sokkal több helyet foglal. **Ez az ára annak, hogy nem fenyeget a túlcsordulás veszélye (racionális keretek között).** Lassabb is, mint az integerekkel számolás. Az ok az, hogy a stringműveletek lassabbak, valamint a számítógép integer számok kiszámolására lett optimalizálva.
## Korlátok és a size\_t
A cél a túlcsordulás kivédése volt, de mivel minden véges, ezért itt sem lehet végtelen számokkal számolni. Többször említve lett, hogy a számokat stringként ábrázoljuk, és stringműveletekkel dolgozzuk fel. Ehhez használjuk a string.h-t, azon belül is az strcmp implementációját. Nem egységes az strcmp függvény megvalósítása, viszont valószínűsíthetjük, hogy a függvényben van valamilyen szintű ciklikusság. Azt is valószínűsíteni lehet, hogy az inkrementált változó size\_t típusban van tárolva. Erről a size\_t-ről annyit lehet tudni, hogy unsigned, azaz a méretét nem tudjuk.

Az én gépem esetén unsigned long int, induljunk ki ebből. Mivel minden számjegy egy karakter, egy karakter pedig egy bájton van ált. tárolva, ezért ebből kiindulva 2^64 - 1 számjegyig működőképes a program. Ez 18 446 744 073 709 551 615 bájtot jelent, ami durván 16,8 millió terabájt. **Ez azt jelenti, hogy 16,8 millió terabájtnyi RAM-nál fog túlcsordulni a program.** Tudomásom szerint nincsenek a piacon 16,8 millió terabájtos RAM modulok, ezért ez a probléma nem fenyeget minket egy 'kis' ideig. Nem garantálom, hogy ekkor méretnél is a kívánt számot fogjuk megkapni, mivel eszköz hiányában nem lehetett kitesztelni. A program fel van készülve a memória lefolglalás hibáira.
## Érdekesség
Implementáltam a faktoriálist is a programba. A 8!-t a másodperc törtrészében ki tudta számolni. A (8!)!-t körülbelül 4 perc alatt tudta kiszámolni a számítógépem (2016-os MacBook Pro i7-6700HQ). A ((8!)!)!-t lehetetlen kiszámolni. Az eredmény körülbelül 10^160000 számjegyet fog tartalmazni. A jelenleg ismert univerzum nagyjából 10^80 atommal rendelkezik. **Szóval ahhoz, hogy ezt a számot el tudjuk tárolni, az univerzum összes atomjának tartalmaznia kell annyi részegységet, mint az univerzum összes atomja. Ez még mindig nem lenne elég, ezért a kapott összes részegységbe még belesűrítjük az univerzum összes atomját, így már elegendő kapacitással fog rendelkezni az univerzum, hogy eltárolhassa a ((8!)!)! számjegyeit.** Ehhez persze idő is kell, valószínűleg nem lenne elég az univerzum keletkezésétől eltelt idő a kiszámolásához.
## Telepítés
A program telepítése egyszerű.
```C
make
```
```C
//Kijelzi a részszámításokat is
make debug
```
```C
//Alapértelmezetten ezresenként tagoljuk az eredményt, ezzel a kapcsolóval azonban ki lehet ezt a funkciót kapcsolni.
make bare
```

### Futtatás

```C
bin/bigint
```
## Hasznos linkek
https://en.wikipedia.org/wiki/Reverse_Polish_notation

https://en.wikipedia.org/wiki/Shunting-yard_algorithm

http://wcipeg.com/wiki/Shunting_yard_algorithm

https://www.youtube.com/watch?v=7ha78yWRDlE

https://www.youtube.com/watch?v=LQ-iW8jm6Mk

https://www.youtube.com/watch?v=zMDm2MbRG6w