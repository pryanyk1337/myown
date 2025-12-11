Dokumentácia k projektu

1) Úvodná strana
Názov univerzity: ........................................
Fakulta / katedra: .......................................

Názov zadania (hra): Konzolová 2D hra v jazyku C (Space Invaders – ASCII)
Autor: ...................................................
Rok: 2025
Mesto: ...................................................


2) Znenie zadania
Zadanie smerovalo k vytvoreniu programu v jazyku C s využitím knižnice ncurses. Malo ísť o hru alebo tvorivú konzolovú aplikáciu, ktorá pracuje v dvojrozmernom prostredí a splní aspoň tri prvky z tohto zoznamu: farby, ovládanie bez Enteru, viac úrovní, práca s časom, spracovanie argumentov príkazového riadku alebo práca so súbormi. Očakával sa ucelený projekt, nie iba ilustračný príklad.

Moja implementácia je malá arkádová „space‑invaders“ hra v termináli. Svet je 2D mriežka znakov, objekty sa hýbu po obrazovke a hráč sa im prispôsobuje. Projekt využíva farby, okamžitý vstup z klávesnice, časovo riadené zmeny a viacero úrovní, takže pokrýva požiadavky zadania.


3) Návrh riešenia
Zvolil som čisté C a ncurses. Táto kombinácia sedí k textovej grafike a umožňuje reagovať na stlačené klávesy hneď, bez potvrdenia. Kód som rozdelil do modulov, aby sa dali samostatne rozvíjať:
- main.c – vstupný bod, spustenie a ukončenie ncurses, hlavná slučka aplikácie, prepínanie medzi menu a hrou,
- menu.c – vykreslenie hlavného menu a obrazovky s inštrukciami, obsluha voľby používateľa,
- game_logic.c – herná logika, inicializácia hry a úrovne, pohyb objektov, kolízie, skončenie hry,
- render.c – vykresľovanie hráča, nepriateľov, projektilov, rámčekov a textového rozhrania,
- game.h – spoločné deklarácie, konštanty a farebné páry (napr. COLOR_ENEMY, COLOR_BULLET_PLAYER, COLOR_UI).

Pri štarte sa volá initNcurses(), zapnú sa farby a skryje sa kurzor. Následne beží hlavný cyklus, v ktorom showMenu() vráti voľbu. Ak hráč spustí hru, prebehne initGame() a initLevel(), potom preberá kontrolu gameLoop(). V ňom sa číta vstup, posúvajú sa objekty podľa času, kontrolujú sa kolízie a volá sa vykresľovanie. Chybné voľby v menu sa zvýraznia farebnou hláškou a po krátkej pauze sa menu obnoví. Pri odchode hra zobrazí jednoduché poďakovanie („Thanks for playing! See you, Space Cowboy…“) a korektne ukončí ncurses.

Logika sveta je priamočiara. Hráč sa pohybuje po spodnej časti, nepriatelia postupujú vpred a z času na čas sa objaví nová vlna. Čas riadi rýchlosť pohybu a intervaly udalostí (napms alebo počítanie tickov). Farby pomáhajú čítať situáciu: iná pre hráča, iná pre nepriateľov, ďalšia pre rozhranie. Vďaka oddeleniu renderu od logiky sa dá meniť vzhľad bez zásahu do pravidiel hry.


4) Použitie programu
Kompilácia
Vyžaduje sa gcc a knižnica ncurses (v Linuxe balíček libncurses-dev alebo podobný).

Príklad prekladu:
  gcc main.c menu.c game_logic.c render.c -lncurses -o space_invaders

Spustenie
  ./space_invaders

Po spustení sa objaví hlavné menu. Ovládanie je okamžité, netreba stláčať Enter po každom príkaze.

Ovládanie (predvolené)
  A – pohyb doľava
  D – pohyb doprava
  SPACE – streľba
  ESC – pauza
  Q – návrat do menu

Ukážkové situácie
- Štart hry: postava sa zobrazí na spodku obrazovky, prichádzajú prvé vlny nepriateľov.
- Streľba a kolízie: pri zásahu nepriateľ zmizne a pripočíta sa skóre.
- Časové zmeny: po určitom čase sa mení tempo a nápor, objavujú sa ďalšie vlny.
- Prechod úrovní: po splnení podmienok sa pripraví nová úroveň s náročnejším priebehom.


5) Záver
Zadanie som naplnil formou jednoduchej, ale súdržnej 2D konzolovej hry. Knižnica ncurses zabezpečila plnú kontrolu nad terminálom a umožnila pohodlné ovládanie z klávesnice. Hra používa farby, pracuje s časom a má viac úrovní, takže spĺňa požadované prvky. 

Isté obmedzenia ostávajú. Grafika je čisto textová, správanie nepriateľov je zámerne striedme, výsledky sa zatiaľ neukladajú do súboru a argumenty príkazového riadku som nechal na ďalšiu fázu. To sú prirodzené kroky k rozšíreniu: tabuľka rekordov v súbore, voľba obtiažnosti pri štarte, prípadne načítanie máp z textu. Základ je pripravený a dá sa na ňom stavať bez toho, aby sa museli prepisovať celé moduly.
