#include<stdio.h>
#include <stdlib.h>
#include<string.h>

void GetInput(char* usename, char *password) {
	printf("please input username:");
	scanf("%s", usename);
	printf("please input password:");
	scanf("%s", password);
}

int Login(char *username, char *password) {
	char admin_username[] = "admin";
	char admin_password[] = "12345678";
	
	if(!strcmp(username, admin_username)) {
		if(!strcmp(password, admin_password)) {
			return 3;
		}
		else {
			return 2;
		}
	}
	else {
		return 1;
	}
}

int main()
{
	char username[10];
	char password[10];
	GetInput(username, password);
	switch(Login(username, password)) {
		case 1: 
			printf("username: %s error\n", username);
			break;
		case 2:
			printf("password: %s error\n", password);
			break;
		case 3:
			printf("OK\n");
	}
}

