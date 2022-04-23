/*
 * @Author: Limer
 * @Date: 2022-02-08 19:31:56
 * @LastEditors: Limer
 * @LastEditTime: 2022-02-08 19:42:39
 * @Description: a basicdemo of thread
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int var = 8;

void* thread_1(void* arg) {
    while (1) {
        printf(
            "this is a new thread which will increase the value of 'var'.\n");
        ++var;
        sleep(1);
    }
    return NULL;
}
void* thread_2(void* arg) {
    while (1) {
        printf("var = %d\n", var);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thid1, thid2;
    pthread_create(&thid1, NULL, thread_1, NULL);
    pthread_create(&thid2, NULL, thread_2, NULL);

    while (1) {
        printf("the main thread:var = %d \n", var);
        sleep(1);
    }
    return 0;
}