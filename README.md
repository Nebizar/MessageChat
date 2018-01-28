# Messsage Chat

## KOMPILACJA I URUCHOMIENIE:

gcc inf132302_s.c -o serwer
./serwer

lub

serwer.sh

W osobnym terminalu:

gcc inf132302_k.c -o klient
./klient

lub 

klient.sh

## PLIKI:

inf132302_struct.h
Zawiera strukture wiadomoœci msgbuf oraz strukture przetrzymuj¹c¹ dane o konkretnym u¿ytkowniku.
Struktury u¿ywane zarówno w programie klienta jak i serwera.

inf132302_k.c
Program klienta. Zawiera funkcje odpowiadaj¹ce za poszczególne funkcjonalnosci oprogramowania
oraz g³ówn¹ funkcjê main zawieraj¹c¹ menu wraz z wywo³ywaniem ¿¹danych funkcji.

inf132302_s.c
Program serwera. Zawiera g³ówn¹ funkcje programu, która w pêtli nieskoñczonej sprawdza ¿¹dania
poszczególnych u¿ytkowników i jeœli takie siê pojawi to obs³uguje je. Znajduje siê te¿ tam funkcja
pobieraj¹ca dane konfiguracyjne z pliku setup.txt.

setup.txt
Plik konfiguracyjny w postaci:

Nazwa_U¿ytkownika
Haslo              //x 9
Nazwa_grupy
nr_czlonka1
nr_czlonka2        //x 3

Ostrze¿enie! 
W pliku konfiguracyjnym dla ka¿dej grupy musi zostaæ podanych 2 nale¿¹cych cz³onków.



