#include "iftun.h"
#include "extremite.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv){
	if (argc != 3) {
	    printf("usage : ext_out tunXXX %s\n",argv[0]);
	    exit(1);
  	}
		int pid;
		int fd = 0;
		fd = tun_alloc(argv[2]);
		char cmd[128];
		sprintf(cmd,"./configure-tun.sh %s",argv[2]);
  		system(cmd);
  		
  		if ((pid = fork ()) == 0) {
  			ext_out(fd);
  			exit(1);
  		}
		getchar();
		ext_in(argv[1],fd);
		return 0;

}