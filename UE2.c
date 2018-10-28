#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

#define MAX_INPUT 255
#define MAX_PATH 255
#define MAX_WORDS 25

void shell();
char **split(char *str,char *delim);
void end_14();
int wo_14();
int cd();
void info_14();
char *checkBackground(char *str);

char **cmdv;
int runBackground = 0;

//Startet die shell Funktion
int main(){
	shell();
	return 0;
}

//Beendet das programm
void end_14(){
	printf("Hope to see you again!\n\n");
	exit(1);
}

//PWD Funktion
int wo_14(){	
	char cwd[MAX_PATH];
	//Abfragen des Current Working Dir mit getcwd
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//Ausgabe des Current Working Directory
    	printf("Current working dir: %s\n", cwd);
	} 
	else {
		//Fehler sollte das CWD nicht abgefragt werden können
		perror("Get current working dir error!\n");
		return 1;
   }
   return 0;
	}

int cd(){
	char cwd[MAX_PATH];	
	//Falls das 2 Element der angegebenen Parameter leer sein Fehlermeldung gültigen Pfad angeben
	if (cmdv[1] != NULL){		
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			chdir(cmdv[1]);
			wo_14();
		} 
		else {
			//Fehler sollte das neue Working Directory nicht gewechselt werden können
			perror("Couldn´t find folder!\n");
			return 1;
	   }
	}
	else
		printf("Enter a valid path to switch!\n");
	return 0;
}

void info_14(){
	printf("\nSysteminfo:\n");	
}
	
char **split(char *str,char *delim){
	int i = 0;	
	//Temporäres Array zum speichern der Commandline Argumente
	char **tmparr= malloc(16 * sizeof(char *));
	//FM im falle das malloc fehlschlägt
	if (tmparr == NULL) {
        perror("Malloc failed!\n");
        exit(1);
    }
	
	//initialisieren strtok und speichern in einer temp variablen
	char *tmp = strtok(str,delim);	
	while(tmp != NULL)	{
		//anschließend nach den definierten delimitern trennen und ins tmparray speichern	
		tmparr[i++]= tmp;
		tmp = strtok(NULL, delim);
	}
	//Sicherheitshalber am Ende noch eine NULL hinzufügen für kontroll zwecke die eventuell später benötigt werden
	tmparr[i] = NULL;
	return tmparr;
}

char *checkBackground(char *str){
	//Kontrolliert ob 1 Character ein & ist und somit der Prozess im Hintergrund laufen soll
	//Anschließend wird beim Argument noch das & entfernt und der Bool Wert von runBackground auf 1 gesetzt
	if(str[0] == '&'){
		runBackground = 1;	
		char *tmp = str + 1;
		str = tmp;
	}
	//Soll nicht im Hintergrund laufen = 0
	else
		runBackground = 0;
	return str;
}

void shell(){
	char eingabe[MAX_INPUT];
	printf("Start my Shell\n");	
	// PGRP & Signale bearbeiten
	for(;;){
		printf("14-Workingdirectory:>");
		fgets(eingabe,MAX_INPUT,stdin);
		
		//Entwertungen markieren
		cmdv = split(eingabe," \t\n");
		//Kontrolle ob Hintergrund prozess und entfernen des &
		cmdv[0] = checkBackground(cmdv[0]);
		
		//Abfrage ob eine Build in Option benutzt werden soll und diese anschließend ausführen
		if(strcmp(cmdv[0],"end_14") == 0)
			end_14();		
		if(strcmp(cmdv[0],"wo_14") == 0){
			wo_14();
			continue;
		}		
		if(strcmp(cmdv[0],"cd") == 0){
			cd();
			continue;
		}		
		if(strcmp(cmdv[0],"info_14") == 0){
			info_14();
			continue;
		}		
		//Sonderzeichen suchen und cmdv verarbeiten
		//festlegen ob Prozzess im Hintergrund laufen soll
		
		//
		
		//Befehle kontrollieren ob vorhanden und ausführen mit exec
		
		// sonst Fork aufruf
		
		pid_t child;
		switch(child=fork()){
			case -1: perror("fork");
				break;
			case 0: //Childprocess
				//Kontrolle ob Hintergund oder nicht
				//Signale für den ChildProcess richtigstellen
				execvp(cmdv[0],cmdv);
				perror(cmdv[0]);
				
				exit(1);
				//break;
			default: //Bin ich selber und ggfs warten auf beendigung des childprocess
				
				printf("LALA");
				/*if(!(imHintergrund)){
					//waitPID(...Tochter...);	
				}
				else{
					continue;
				}
				break;*/
		}
	free(cmdv);
	}
	//Return 0;
}