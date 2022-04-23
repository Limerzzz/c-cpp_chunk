#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 4096


int main(int argc, char** argv){
	if(argc < 3){
		exit(1);	
	}
	int in_fd = open(argv[1],O_RDONLY);
	if(in_fd == -1){
		perror("fail to open file!");
		exit(1);	
	}
	int out_fd = open(argv[2], O_WRONLY|O_CREAT);
	if(out_fd == -1){
		perror("fail to open file!");
		exit(1);	
	}
	char buf[BUF_SIZE];
	ssize_t size = 0;
	while(1){
		bzero(buf, BUF_SIZE);
		size = read(in_fd, buf, BUF_SIZE);
		if(size == -1){
			perror("read file error !");
			exit(1);	
		}	
		write(out_fd, buf, size);
		if(size < BUF_SIZE)
			break;
	}
	close(in_fd);
	close(out_fd);
	return 0;
}
