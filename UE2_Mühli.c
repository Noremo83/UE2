#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h> 
#include <unistd.h> 

#define MAX_INPUT 255
#define MAX_WORDS 25

void shell();
char *split(char *str,char *delim);


int main(){
	
	shell();
	return 0;
}
	
char *split(char *str,char *delim){
	int i = 0;
	char *cmdv = malloc(MAX_INPUT * sizeof(char));
	//strtok benutzen
	for(i=0, cmdv[i] = strtok(str,delim); cmdv[i]; ++i,cmdv[i] = strtok(NULL,delim))
		;
	printf("%s\n",*cmdv);
	return cmdv;
}

void shell(){
	char eingabe[MAX_INPUT];
	char *cmdv[MAX_WORDS];
	
	
	printf("Start my Shell\n");	
	// PGRP & Signale bearbeiten
	
	for(;;){
		printf("Prompt: ");
		while(fgets(eingabe,MAX_INPUT,stdin)){
		
		//Entwertungen markieren
		cmdv = split(eingabe," \t\n");
			int i;
			for(i=0;(sizeof(cmdv) / sizeof(char*));i++){
				printf("%c\n",cmdv[i]);
			}
			
		}	
		//Sonderzeichen suchen und cmdv verarbeiten
		//festlegen ob Prozzess im Hintergrund laufen soll
		int imHintergrund = 0;
		
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
				if(!(imHintergrund)){
					//waitPID(...Tochter...);	
				}
				else{
					continue;
				}
				break;
		}	
	}
	printf("Stop my Shell\n");
	//Return 0;
	
}