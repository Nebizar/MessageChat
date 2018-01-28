#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>//ipc
#include <sys/ipc.h>//ipc
#include <sys/msg.h>//ipc
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "struktury.h"

struct group {
	char name[64];
	int list[9];
};

struct user uList[9];
struct group gList[3];
int queList[9]; 

void setup()
{
	FILE* file = fopen("setup.txt", "r");
	char buffer[64];
	int id = 234567;
	for (int i = 0; i < 9; i++)
	{
		fgets(buffer, 64, file);
		buffer[strlen(buffer) - 1] = '\0';
		strcpy(uList[i].username, buffer);
		fgets(buffer, 64, file);
		buffer[strlen(buffer) - 1] = '\0';
		strcpy(uList[i].password, buffer);
		uList[i].ID = id;
		uList[i].zal = 0;
		id += 10;
	}
	int index;
	for (int i = 0; i < 3; i++)
	{
		fgets(buffer, 64, file);
		buffer[strlen(buffer) - 1] = '\0';
		strcpy(gList[i].name, buffer);
		for (int j = 0; j < 9; j++)
		{
			gList[i].list[j] = -1;
		}
		fgets(buffer, 64, file);
		buffer[strlen(buffer) - 1] = '\0';
		index = atoi(buffer);
		gList[i].list[index] = uList[index].ID;
		fgets(buffer, 64, file);
		buffer[strlen(buffer) - 1] = '\0';
		index = atoi(buffer);
		gList[i].list[index] = uList[index].ID;
	}
	for (int i = 0; i < 9; i++)
	{
		printf("%s \t %s \t %d \t %d \n", uList[i].username, uList[i].password, uList[i].ID, uList[i].zal);
	}
	fclose(file);
	for (int i = 0; i < 9; i++) {
		queList[i]= msgget(uList[i].ID, IPC_CREAT | 0644);
	}
}


int main()
{
    //printf("WAWAW\n");
    //struct msgbuf msg1;
	setup();
	int pom;
	int id;
	struct msgbuf msg;
	id = msgget(12345678, IPC_CREAT | 0644);
	while (1) {
		for (int a = 0; a < 9; a++)
		{
			//LOGOWANIE - OBSLUGA TYPU 1
			pom = 0;
			pom = msgrcv(id, &msg, (sizeof(msg) - sizeof(long)), 1, IPC_NOWAIT);
			if (pom > 0) {
				struct msgbuf msg1;
				msg1.type = 10;
				int checker = 0;
				for (int i = 0; i < 9; i++) {
					if (strcmp(msg.username, uList[i].username) == 0 && strcmp(msg.password, uList[i].password) == 0)
					{
						checker = 1;
						if (uList[i].zal == 0) {
							printf("login success");
							uList[i].zal = 1;
							msg1.id = uList[i].ID;
							strcpy(msg1.massage, "Zalogowano!!!\n");
							//queList[0] = msgget(msg1.id, IPC_CREAT | 0644);
							msgsnd(id, &msg1, (sizeof(msg1) - sizeof(long)), 0);
							printf("%d\n", queList[0]);
						}
						else {
							strcpy(msg1.massage, "Uzytkownik juz zalogowany!!!\n");
							msgsnd(id, &msg1, (sizeof(msg1) - sizeof(long)), 0);
						}
					}
				}
				if (checker == 0) {
					strcpy(msg1.massage, "Uzytkownik nie istnieje!!!\n");
					msgsnd(id, &msg1, (sizeof(msg1) - sizeof(long)), 0);
				}
			}
			//WYLOGOWANIE - OBSLUGA TYPU 9
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 9, IPC_NOWAIT);
			if (pom > 0) {
				uList[a].zal = 0;
				printf("Wylogowano\n");
			}
			//ZALOGOWANI - OBSLUGA TYPU 2
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 2, IPC_NOWAIT);
			if (pom > 0) {
				struct msgbuf msg2;
				msg2.type = 10;
				strcpy(msg2.massage, "");
				for (int i = 0; i < 9; i++) {
					if (uList[i].zal == 1) {
						strcat(msg2.massage, uList[i].username);
						strcat(msg2.massage, "\t");
					}
				}
				msgsnd(queList[a], &msg2, (sizeof(msg2) - sizeof(long)), 0);
			}
			//LISTA GRUP - O. T. 5
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 5, IPC_NOWAIT);
			if (pom > 0) {
				struct msgbuf msg5;
				msg5.type = 10;
				strcpy(msg5.massage, "");
				for (int i = 0; i < 3; i++) {
					strcat(msg5.massage, gList[i].name);
					strcat(msg5.massage, "\t");
				}
				msgsnd(queList[a], &msg5, (sizeof(msg5) - sizeof(long)), 0);
				//printf("cos poszlo\n");
			}
			//USERS W GROUP - O. T. 3
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 3, IPC_NOWAIT);
			if (pom > 0) {
				//printf("JESTEM\n");
				struct msgbuf msg3;
				int checker = 0;
				msg3.type = 10;
				strcpy(msg3.massage, "");
				for (int i = 0; i < 3; i++) {
					if (strcmp(gList[i].name, msg.group_name) == 0) {
						//printf("SUKCES\n");
						checker = 1;
						for (int j = 0; j < 9; j++)
						{
							if (gList[i].list[j] != -1) {
								strcat(msg3.massage, uList[j].username);
								strcat(msg3.massage, "\t");
							}
						}
					}
				}
				if (checker == 0) {
					strcpy(msg3.massage, "Blad w nazwie grupy.\n");
				}
				msgsnd(queList[a], &msg3, (sizeof(msg3) - sizeof(long)), 0);
				printf("cos poszlo\n");
			}
			//JOIN/QUIT GROUP - O. T. 4
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 4, IPC_NOWAIT);
			if (pom > 0) {
				struct msgbuf msg4;
				msg4.type = 10;
				for (int i = 0; i < 3; i++)
				{
					if (strcmp(msg.group_name, gList[i].name) == 0) {
						if (gList[i].list[a] == uList[a].ID) {
							gList[i].list[a] = -1;
							strcpy(msg4.massage, "Usunieto z grupy\n");
							msgsnd(queList[a], &msg4, (sizeof(msg4) - sizeof(long)), 0);
						}
						else {
							gList[i].list[a] = uList[a].ID;
							strcpy(msg4.massage, "Dodano do grupy\n");
							msgsnd(queList[a], &msg4, (sizeof(msg4) - sizeof(long)), 0);
						}
					}
				}

			}
			//MESSAGE TO ONE - O. T. 6
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 6, IPC_NOWAIT);
			if (pom > 0) {
				char from[64];
				int checker = 0;
				strcpy(from, uList[a].username);
				struct msgbuf msg6;
				struct msgbuf msgback;
				msgback.type = 10;
				for (int i = 0; i < 9; i++)
				{
					if (strcmp(msg.username, uList[i].username) == 0)
					{
						msg6.type = 11;
						strcpy(msg6.username, from);
						strcpy(msg6.massage, msg.massage);
						printf("%d\n", i);
						msgsnd(queList[i], &msg6, (sizeof(msg6) - sizeof(long)), 0);
						strcpy(msgback.massage, "Wyslano pomyslnie");
						msgsnd(queList[a], &msgback, (sizeof(msgback) - sizeof(long)), 0);
						checker = 1;
						break;
					}
				}
				if (checker == 0) {
					strcpy(msgback.massage, "Wystapil blad");
					msgsnd(queList[a], &msgback, (sizeof(msgback) - sizeof(long)), 0);
				}
			}
			//MESSAGE TO MANY - O. T. 7
			pom = 0;
			pom = msgrcv(queList[a], &msg, (sizeof(msg) - sizeof(long)), 7, IPC_NOWAIT);
			if (pom > 0) {
				char from[64];
				int checker = 0;
				strcpy(from, uList[a].username);
				struct msgbuf msg7;
				struct msgbuf msgback;
				msgback.type = 10;
				for (int i = 0; i < 3; i++)
				{
					if (strcmp(msg.group_name, gList[i].name) == 0)
					{
						checker = 1;
						for (int j = 0; j < 9; j++)
						{
							if (gList[i].list[j] != -1) {
								msg7.type = 11;
								strcpy(msg7.username, from);
								strcpy(msg7.massage, msg.massage);
								printf("%d\n", j);
								msgsnd(queList[j], &msg7, (sizeof(msg7) - sizeof(long)), 0);
								strcpy(msgback.massage, "Wyslano pomyslnie");
								msgsnd(queList[a], &msgback, (sizeof(msgback) - sizeof(long)), 0);

							}
						}
					}
				}
				if (checker == 0) {
					strcpy(msgback.massage, "Wystapil blad");
					msgsnd(queList[a], &msgback, (sizeof(msgback) - sizeof(long)), 0);
				}
			}
		}
	}
	//printf("123");

	return 0;
}
