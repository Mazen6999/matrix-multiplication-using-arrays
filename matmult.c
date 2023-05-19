#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define Item(X, I, J, NCOL) X[((J) + (I) * (NCOL))]

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int *A, int *B, int *C, int l, int m, int n)
{
    for (int i = 0; i < l; i++)
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            for (int k = 0; k < m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
struct arg_struct
{
    int *arg1;
    int *arg2;
    int *arg3;
    int arg4;
    int arg5;
    int arg6;

} *args;
int counter = 0;
int V1counter = -1;
int counterV1 = 0;
/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */

void *v1_threadmatmul(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;

    int *A = args->arg1;
    int *B = args->arg2;
    int *C = args->arg3;
    int l = args->arg4;
    int m = args->arg5;
    int n = args->arg6;

    int y[m+n][n+m];
    int res[l][n];
    int sum = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            y[i][j] = B[i + j * n];
        }
    }
    

            sum = 0;
            for (int k = 0; k < m; k++)
            {
                sum = sum + (A[k] * y[counterV1%(n)][k]);
            }
            res[V1counter][counterV1%n] = sum;


    for (int j = 0; j < n; j++)
    {
        C[counterV1] = res[V1counter][counterV1%(n)];
    } 
    counterV1++;
}


void matmult_v1(int *A, int *B, int *C, int l, int m, int n)
{
    args = malloc(sizeof(struct arg_struct) * 1);
    args->arg1 = A;
    args->arg2 = B;
    args->arg3 = C;
    args->arg4 = l;
    args->arg5 = m;
    args->arg6 = n;

    pthread_t thread_id[(l*n)];
    for (int i = 0; i < (l*n); i++)
    {
        args = malloc(sizeof(struct arg_struct) * 1);
        if(counterV1%n==0){V1counter++;}
        args->arg1 = A + (m * V1counter);
        args->arg2 = B;
        args->arg3 = C;
        args->arg4 = l;
        args->arg5 = m;
        args->arg6 = n;
    

        pthread_create(&thread_id[i], NULL, &v1_threadmatmul, args);
        usleep(10);

    }

    for (int j = 0; j < (l*n); j++)
    {
        pthread_join(thread_id[j], NULL);
    }
}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */



void *v2_threadmatmul(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;

    int *A = args->arg1;
    int *B = args->arg2;
    int *C = args->arg3;
    int l = args->arg4;
    int m = args->arg5;
    int n = args->arg6;

    int y[m+n][n+m];
    int res[l][n];
    int sum = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            y[i][j] = B[i + j * n];
        }
    }

    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum = 0;
            for (int k = 0; k < m; k++)
            {
                sum = sum + (A[k] * y[j][k]);
            }
            res[i][j] = sum;
        }
    }

    for (int j = 0; j < n; j++)
    {
        C[j + counter * n] = res[0][j];
    }
    counter++;
}

void matmult_v2(int *A, int *B, int *C, int l, int m, int n)
{
    args = malloc(sizeof(struct arg_struct) * 1);
    args->arg1 = A;
    args->arg2 = B;
    args->arg3 = C;
    args->arg4 = l;
    args->arg5 = m;
    args->arg6 = n;

    pthread_t thread_id[l];
    for (int i = 0; i < l; i++)
    {
        args = malloc(sizeof(struct arg_struct) * 1);
        args->arg1 = A + (m * i);
        args->arg2 = B;
        args->arg3 = C;
        args->arg4 = l;
        args->arg5 = m;
        args->arg6 = n;

        pthread_create(&thread_id[i], NULL, &v2_threadmatmul, args);
        usleep(10);     
    }

    for (int j = 0; j < l; j++)
    {
        pthread_join(thread_id[j], NULL);
    }
}
