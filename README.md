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
Zawiera strukture wiadomo�ci msgbuf oraz strukture przetrzymuj�c� dane o konkretnym u�ytkowniku.
Struktury u�ywane zar�wno w programie klienta jak i serwera.

inf132302_k.c
Program klienta. Zawiera funkcje odpowiadaj�ce za poszczeg�lne funkcjonalnosci oprogramowania
oraz g��wn� funkcj� main zawieraj�c� menu wraz z wywo�ywaniem ��danych funkcji.

inf132302_s.c
Program serwera. Zawiera g��wn� funkcje programu, kt�ra w p�tli niesko�czonej sprawdza ��dania
poszczeg�lnych u�ytkownik�w i je�li takie si� pojawi to obs�uguje je. Znajduje si� te� tam funkcja
pobieraj�ca dane konfiguracyjne z pliku setup.txt.

setup.txt
Plik konfiguracyjny w postaci:

Nazwa_U�ytkownika
Haslo              //x 9
Nazwa_grupy
nr_czlonka1
nr_czlonka2        //x 3

Ostrze�enie! 
W pliku konfiguracyjnym dla ka�dej grupy musi zosta� podanych 2 nale��cych cz�onk�w.



