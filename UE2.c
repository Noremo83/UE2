#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include<unistd.h> 
#include<signal.h>

#define MAX_INPUT 255
#define MAX_PATH 255
#define MAX_WORDS 25

void shell();
char **split(char *str,char *delim);
void end_14();
void wo_14();
void cd();
void info_14();
void setpath_14(char *path);
void addtopath_14(const char *path);
char *checkBackground(char *str);
void sigint_handler(int signo);

char **cmdv;
int runBackground = 0;

void sigint_handler(int signo) {
	printf("\n");
    exit(1);
}

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
void wo_14(){	
	char cwd[MAX_PATH];
	//Abfragen des Current Working Dir mit getcwd
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//Ausgabe des Current Working Directory
    	printf("Current working dir: %s\n", cwd);
	} 
	else {
		//Fehler sollte das CWD nicht abgefragt werden können
		perror("Get current working dir error!\n");
   }
}

void cd(){
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
	   }
	}
	else
		printf("Enter a valid path to switch!\n");
}

void info_14(){
	//Liefert Systeminformationen
	printf("\nSysteminfo:\n");
	printf("UID: %d\n",getuid());
	printf("EUID: %d\n",geteuid());
	printf("PID: %d\n",getpid());
	wo_14();
	printf("PATH: %s\n",getenv("PATH"));
}

void setpath_14(char *path){

}
void addtopath_14(const char *path){
	printf("%s\n",path);
	char *newpath = strcat("PATH=$PATH:",path);
	//printf("%s\n",newpath);
	execvp("export",newpath);	
	
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
	int stat_loc;
	
	printf("\nStart my Shell\n\n");	
	
	//signal(SIGINT, sigint_handler);
	signal(SIGINT, SIG_IGN);
	// PGRP & Signale bearbeiten
	for(;;){
		runBackground = 0;
		printf("14-Workingdirectory:>");			
		
		fgets(eingabe,MAX_INPUT,stdin);
		
		//Argumente zerlegen
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
		if(strcmp(cmdv[0],"addtopath_14") == 0 && cmdv[1] != NULL){
			addtopath_14(cmdv[1]);
			continue;
		}
		//Start Childprocess
		pid_t child;
		switch(child=fork()){
			case -1: perror("fork");
				break;
			case 0: //Childprocess
				//Kontrolle ob Hintergund oder nicht
				//Signale für den ChildProcess richtigstellen
				signal(SIGINT, SIG_DFL);
				if(execvp(cmdv[0],cmdv) < 0){
					perror(cmdv[0]);				
					printf("### Child ###\nCurrent PID: %d and Child PID: %d\n",
               		getpid(), child);
				}
				
			default: //Bin ich selber und ggfs warten auf beendigung des childprocess
				if(runBackground == 1){
					waitpid(child, &stat_loc, WUNTRACED);
					printf("### Parrent ###\nCurrent PID: %d and Child PID: %d\n",
               		getpid(), child);
				}
				else{
					printf("### Parrent ###\nCurrent PID: %d and Child PID: %d\n",
               		getpid(), child);
					continue;
				}
				//continue;
				waitpid(child, &stat_loc, WUNTRACED);
				break;
		}
		free(cmdv);
	}
	//Return 0;
}