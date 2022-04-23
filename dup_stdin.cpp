/*
 * @Author: Limer
 * @Date: 2022-04-15 16:41:24
 * @LastEditors: Limer
 * @LastEditTime: 2022-04-19 21:57:45
 * @Description:
 */
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

int main() {
  int pipefd[2];
  pipe(pipefd);
  close(STDIN_FILENO);
  dup(pipefd[0]);
  char buf[1024];
  bzero(buf, 1024);
  scanf("%s", buf);
}
