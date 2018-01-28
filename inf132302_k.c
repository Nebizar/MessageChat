#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>//ipc
#include <sys/ipc.h>//ipc
#include <sys/msg.h>//ipc
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "struktury.h"

int IDgl;
int e = 0;

struct user u;


void logowanie()
{
	int id = msgget(12345678, IPC_CREAT | 0644);
	//printf("%d", id);
	printf("Podaj login: \n");
	scanf("%s", u.username);
	printf("Podaj haslo: \n");
	scanf("%s", u.password);
	struct msgbuf msg;
	struct msgbuf msg1;
	msg.type = 1;
	strcpy(msg.username, u.username);
    strcpy(msg.password, u.password);
	//printf("%s", msg.username);
	msgsnd(id, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(id, &msg1, (sizeof(msg1) - sizeof(long)), 10, 0);
	printf("%s\n", msg1.massage);
    u.ID = msg1.id;
    u.zal =1;
    IDgl = msgget(u.ID,IPC_CREAT|0644);
    printf("%d\n",IDgl);
}

void wyloguj(){
    struct msgbuf msg;
    int check =0;
    printf("Czy na pewno wylogowac? 1 - TAK\n");
    scanf("%d",&check);
    if(check == 1)
    {
        msg.type = 9;
        msg.id = u.ID;
        u.zal = 0;
        msgsnd(IDgl,&msg, (sizeof(msg) - sizeof(long)),0);
    }
}

void getUsers() {
	struct msgbuf msg;
	msg.type = 2;
	msg.id = u.ID;
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	printf("Lista zalogowanych: \n");
	printf("%s \n", msg.massage);
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void getGroups() {
	struct msgbuf msg;
	msg.type = 5;
	msg.id = u.ID;
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	printf("Lista dostepnych grup: \n");
	printf("%s \n", msg.massage);
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void getInGroup() {
	struct msgbuf msg;
	msg.type = 3;
	msg.id = u.ID;
	printf("Podaj nazwe grupy, ktora chcesz sprawdzic: \n");
	scanf("%s", msg.group_name);
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	printf("Lista czlonkow: \n");
	printf("%s \n", msg.massage);
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void enlist() {
	struct msgbuf msg;
	msg.type = 4;
	msg.id = u.ID;
	printf("Podaj nazwe grupy, do ktorej chcesz dolaczyc: \n");
	scanf("%s", msg.group_name);
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	//printf("Lista zalogowanych: \n");
	printf("%s \n", msg.massage);
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void sentOne() {
	struct msgbuf msg;
	msg.type = 6;
	msg.id = u.ID;
	strcpy(msg.username, u.username);
	printf("Odbiorca: \n");
	scanf("%s", msg.username);
	printf("Wiadomosc: \n");
	scanf(" %[^\t\n]s", &msg.massage);
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	printf("%s", msg.massage);
}

void sentMany() {
	struct msgbuf msg;
	msg.type = 7;
	msg.id = u.ID;
	strcpy(msg.username, u.username);
	printf("Odbiorca: \n");
	scanf("%s", msg.group_name);
	printf("Wiadomosc: \n");
	scanf(" %[^\t\n]s", &msg.massage);
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	printf("CHUJ\n");
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	printf("%s", msg.massage);
}

void getout() {
	struct msgbuf msg;
	msg.type = 4;
	msg.id = u.ID;
	printf("Podaj nazwe grupy, ktora chcesz opuscic: \n");
	scanf("%s", msg.group_name);
	msgsnd(IDgl, &msg, (sizeof(msg) - sizeof(long)), 0);
	msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 10, 0);
	//printf("Lista zalogowanych: \n");
	printf("%s \n", msg.massage);
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void escape() {
	int check;
	printf("Czy chcesz wyjsc ?? T/N \n");
	scanf("%d", check);
	if (check == 1 ) e = 1;
}

void recive() {
	struct msgbuf msg;
	int check = 0;
	while (msgrcv(IDgl, &msg, (sizeof(msg) - sizeof(long)), 11, IPC_NOWAIT) > 0) {
		printf("%s: \n", msg.username);
		printf("%s \n\n", msg.massage);
	}
	printf("Nacisnij klawisz aby kontynuowac.\n");
	getchar();
}

void menu()
{

	int choice;
	printf("-------CHAT------\n");
	printf("1.Zaloguj\n");
	printf("2.Wyloguj\n");
	printf("3.Zalogowani uzytkownicy\n");
	printf("4.Uzytkownicy w grupie\n");
	printf("5.Zapisz do grupy\n");
	printf("6.Wypisz z grupy\n");
	printf("7.Lista grup\n");
	printf("8.Wyslij wiadomosc do uzytkownika\n");
	printf("9.Wyslij wiadomosc do grupy\n");
	printf("10.Odbierz\n");
	printf("11.Wyjdz\n");
	scanf("%d", &choice);
	switch (choice)
	{
	case 1:
	{
		logowanie();
		break;
	}
	case 2:
    {
        wyloguj();
        break;
    }
	case 3:
	{
		getUsers();
		break;
	}
	case 4:
	{
		getInGroup();
		break;
	}
	case 5:
	{
		enlist();
		break;
	}
	case 6:
	{
		getout();
		break;
	}
	case 7:
	{
		getGroups();
		break;
	}
	case 8:
	{
		sentOne();
		break;
	}
	case 9:
	{
		sentMany();
		break;
	}
	case 10: {
		recive();
		break;
	}
	case 11:{
		escape();
		break;
	}
	default:
		printf("Brak opcji\n");
	}

}

int main()
{
    u.zal = 0;
	//printf("elo");
	while (e != 1)
	{
		menu();
	}

	return 0;
}
