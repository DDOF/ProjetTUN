#include "iftun.h"
#include "extremite.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LEN 256

int main (int argc, char** argv){
	if (argc != 2) {
		printf("usage : tun.cfg %s\n",argv[0]);
		exit(1);
	}

	char tun[128], inip[128],inport[128],options[128],outport[128],outip[256];
	FILE* config_fp ;
	char line[MAX_LINE_LEN + 1] ;
	char* token;

	config_fp = fopen( argv[1], "r" ) ;

	while( fgets( line, MAX_LINE_LEN, config_fp ) != NULL )
	{
		token = strtok( line, "\t =\n\r" ) ;
		if( token != NULL && token[0] != '#' )
		{

			if (strcmp(token ,"tun") == 0 ) {
				token= strtok( NULL, "\t =\n\r" ) ;
				sprintf(tun,"%s",token );
			}
			else if (strcmp(token ,"inip") ==0 ) {
				token= strtok( NULL, "\t =\n\r" ) ;
				sprintf(inip,"%s",token );
			}
			else if (strcmp(token,"inport") ==0 ) {
				token= strtok( NULL, "\t =\n\r" ) ;
				if (token) {
					sprintf(inport,"%s",token );
				}
			}
			else if (strcmp(token,"options") == 0 ) {

				token= strtok( NULL, "\t =\n\r" ) ;
				if (token)
				sprintf(options,"%s",token );
			}
			else if (strcmp(token ,"outip") ==0 ) {

				token= strtok( NULL, "\t =\n\r" ) ;
				sprintf(outip,"%s",token );
			}
			else if (strcmp(token,"outport") == 0 ) {
				token= strtok( NULL, "\t =\n\r" ) ;
				sprintf(outport,"%s",token );
			}
		}

	}
	int pid;
	int fd = 0;
	fd = tun_alloc(tun);
	char cmd[256];
	sprintf(cmd,"./configure-tun.sh %s %s",tun,inip);
	system(cmd);

	if ((pid = fork ()) == 0) {
		ext_out(fd,inport);
		exit(1);
	}
	getchar(); //Attente autre bout du tunnel
	ext_in(outip,outport,fd);
	exit(1);

}