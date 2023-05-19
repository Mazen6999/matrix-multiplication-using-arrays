#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned state = 0;

int thread1_ret = 0;
void *thread1(void *arg)
{
    unsigned state_copy;
    pthread_mutex_lock(&mutex);
    state_copy = state;
    pthread_mutex_unlock(&mutex);

    while(1) {
        pthread_mutex_lock(&mutex);
        while(state_copy == state)
            pthread_cond_wait(&cond, &mutex);
        state_copy = ++state;

        printf("In Thread: %s\r\n", __func__);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    thread1_ret = 5;
    return &thread1_ret;
}

int thread2_ret = 0;
void *thread2(void *arg)
{
    unsigned state_copy;
    pthread_mutex_lock(&mutex);
    state_copy = ++state;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);

    while (1) {
        pthread_mutex_lock(&mutex);
        while(state_copy == state)
            pthread_cond_wait(&cond, &mutex);
        state_copy = ++state;

        printf("In Thread: %s\r\n", __func__);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    }
    thread2_ret = 5;
    return &thread2_ret;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    void *ret;
    pthread_join(t1, &ret);
    printf("Thread Returned: %d\r\n", *(int *)ret);
    pthread_join(t2, &ret);
    printf("Thread Returned: %d\r\n", *(int *)ret);

    return 0;
}