//IS16134  Andreas Kolan

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
char *wo_14();
void cd();
void info_14();
void getpath();
void setpath_14(const char *path);
void addtopath_14(const char *path);
char *checkBackground(char *str);
void printhelp();

void sigint_handler(int signo);

char **cmdv;
int runBackground = 0;

void sigint_handler(int signo) {
	printf("\n");
    exit(1);
}

void printhelp(){
	//Liefert eine Liste der verfügbaren Kommandos zurück
	printf("\nBuild in Funktionen:\n\n    14-ende - Beendet die Shell\n    14-wo - liefert das Working Directory\n    14-info - liefert Systeminformationen\n");
	printf("    getpath - liefert die PATH Variable\n    14-setpath Pfad - überschreibt die PATH Variable mit dem gelieferten Pfad\n    14-addtopath Pfad - fügt den angegeben Pfad der PATH Variable hinzu\n");
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
char *wo_14(){	
	static char cwd[MAX_PATH];
	
	//Abfragen des Current Working Dir mit getcwd
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//Ausgabe des Current Working Directory
    	return cwd;
	} 
	else {
		//Fehler sollte das CWD nicht abgefragt werden können
		return "ERROR get PATH";
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
	printf("    UID: %d\n",getuid());
	printf("    EUID: %d\n",geteuid());
	printf("    PID: %d\n",getpid());
	printf("    CWD: %s\n",wo_14());
	printf("    PATH: %s\n\n",getenv("PATH"));
}

void setpath_14(const char *path){
	//Einfach überschreiben der bestehenden PATH Variable mit setenv und ausgebe
	setenv("PATH",path,1);  
	printf("PATH=%s\n",getenv("PATH"));
}

void getpath(){
	//Liefert die aktuelle PATH Variable zurück
	printf("PATH: %s\n",getenv("PATH"));
}

void addtopath_14(const char *addpath){
	//PATH Variable laden und einen : anhängen
	char *path = getenv("PATH");
	char *newpath = strcat(path,":");
	//Speichern und ausgeben der neuen PATH Variable
	setenv("PATH",strcat(newpath,addpath),1);   
	printf("PATH %s\n",getenv("PATH"));	
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
	
	//Signale ignorieren
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	for(;;){
		runBackground = 0;
		printf("14-%s:>",wo_14());		
		
		//Einlesen des Userinputs
		fgets(eingabe,MAX_INPUT,stdin);
		
		//Abfangen falls ein leerer Input erfolgt
		if((eingabe[0] == '\n') || (eingabe[0] == '\t')){
			continue;
		}
		//Argumente zerlegen
		cmdv = split(eingabe," \t\n");
		
		//Kontrolle ob Hintergrund prozess und entfernen des &
		cmdv[0] = checkBackground(cmdv[0]);		
		
		//Abfrage ob eine Build in Option benutzt werden soll und diese anschließend ausführen
		if(strcmp(cmdv[0],"14-ende") == 0){
			free(cmdv);
			runBackground = 0;
			end_14();
		}
		else if(strcmp(cmdv[0],"14-wo") == 0){
			printf("CWD: %s\n",wo_14());
			free(cmdv);
			runBackground = 0;
			continue;
		}		
		else if(strcmp(cmdv[0],"cd") == 0){
			cd();
			free(cmdv);
			runBackground = 0;
			continue;
		}		
		else if(strcmp(cmdv[0],"14-info") == 0){
			info_14();
			free(cmdv);
			runBackground = 0;
			continue;
		}
		else if(strcmp(cmdv[0],"14-addtopath") == 0 && cmdv[1] != NULL){
			addtopath_14(cmdv[1]);
			free(cmdv);
			runBackground = 0;
			continue;
		}
		else if(strcmp(cmdv[0],"14-setpath") == 0 && cmdv[1] != NULL){
			setpath_14(cmdv[1]);
			free(cmdv);
			runBackground = 0;
			continue;
		}
		else if(strcmp(cmdv[0],"getpath") == 0){
			getpath();
			free(cmdv);
			runBackground = 0;
			continue;
		}
		else if (strcmp(cmdv[0],"help") == 0){
			printhelp();
			free(cmdv);
			runBackground = 0;
			continue;
		}
		
		//Start Childprocess
		pid_t child;
		switch(child=fork()){
			case -1: perror("fork");
				break;
			case 0: //Childprocess
				//Kontrolle ob Hintergund (1 = Hintergrund) oder nicht
				//Signale für den ChildProcess richtigstellen
				if(runBackground == 1){	
					//Wenn Hintergrund dann Ignore Signale
					signal(SIGINT, SIG_IGN);
					signal(SIGQUIT, SIG_IGN);
					
					//Ausführen des übergebenen Programms
					if(execvp(cmdv[0],cmdv) < 0){
						perror(cmdv[0]);
						exit(1);
					}					
				}
				else{
					//Wenn kein Hintergrund dann Standardverhalten Signale
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					
					//Ausführen des übergebenen Programms
					if(execvp(cmdv[0],cmdv) < 0){
						perror(cmdv[0]);
						exit(1);
					}
				}
				
			default: //Bin ich selber und ggfs warten auf beendigung des childprocess
				if(runBackground == 1){	
					//Wenn Hintergrund dann Ignore Signale und lauf weiter zur neuerlichen befehleingabe
					signal(SIGCHLD,SIG_IGN);
					signal(SIGINT, SIG_IGN);
					signal(SIGQUIT, sigint_handler);
				}
				else{
					//Wenn kein Hintergrund dann Standardverhalten Signale
					signal(SIGCHLD,SIG_DFL);
					signal(SIGINT, SIG_IGN);
					signal(SIGQUIT, sigint_handler);
					
					//warten das Childprozess fertig wird
					waitpid(child, &stat_loc, WUNTRACED);
					printf("P2 Current PID: %d and Child PID: %d\n",getpid(), child);
				}
		}
		
		//Speicher freigeben und runBackground wieder auf 0 setzen
		free(cmdv);
		runBackground = 0;
	}
	//Return 0;
}
