/*
 * @Author: Limer
 * @Date: 2022-03-10 22:25:34
 * @LastEditors: Limer
 * @LastEditTime: 2022-03-10 22:27:13
 * @Description: 
 */
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>


void sig_handler(int sig);

int main(){
	int ret = -1;
	int pipefd[2];
	ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
	return 0;
}

void sig_handler(int sig){
	int save_errno = errno;
	int msg = sig;
	send(pipefd[1], (char*)&msg, 1, 0);
	errno = save_errno;
}

void addsig(int sig){
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = sig_handler;
	sa.sa_flags |= SA_RESATRT;
	sigfillset(&sa.sa_mask);	
}

