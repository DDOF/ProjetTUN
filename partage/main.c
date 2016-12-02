#include "iftun.h"
#include "extremite.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv){
	if (argc == 3) {


		int fd = 0;
		fd = tun_alloc(argv[2]);
		char cmd[128];
		sprintf(cmd,"./configure-tun.sh %s",argv[2]);
  		system(cmd);

		ext_in(argv[1],fd);

		return 0;
	}
	else if (argc == 1) {
		ext_out();
		return 0;

	}
	else{
		printf("usage : hote tun_name %s\n",argv[0]);
		exit(1);
	}
}