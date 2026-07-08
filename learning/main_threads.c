#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// void* routine(void *ret)
// {
//     // int *val = ret;
//     // *val = 100;
//     *(int *)ret = 100;
//     return NULL;
// }

// void* routine()
// {
//     int *val = malloc(sizeof(int));
//     *val = 1002;
//     return val;
// }

// int main(int ac, char *av[])
// {
//     pthread_t th;

//     int *ret;
//     // ret = 1;
//     // printf("ret = %d\n", ret);
//     pthread_create(&th, NULL, routine, &ret);
//     pthread_join(th, (void**)&ret);
//     printf("ret = %d\n", *ret);
// }


// int var = 0;
// pthread_mutex_t mutex;

// void* routine()
// {
//     int i = 0;
//     while (i < 1000000)
//     {
//         i++;
//     }
//     pthread_mutex_lock(&mutex);
//     var += i;
//     pthread_mutex_unlock(&mutex);
    
//     return NULL;
// }

// int main(int ac, char *av[])
// {
//     pthread_t th[4];

//     pthread_mutex_init(&mutex, NULL);
//     for (int i = 0; i < 4; i++)
//     {
//         pthread_create(&th[i], NULL, routine, NULL);
//     }
//     for (int i = 0; i < 4; i++)
//     {
//         pthread_join(th[i], NULL);
//     }
//     pthread_mutex_destroy(&mutex);
//     printf("%d\n", var);


//     return 0;
// }

// typedef struct
// {
//     int index;
//     int *destination;
// } ARGS;


// int numbers[] = {0,444,8,9,7,5,6,878,2,3};
// void *pick_number(void* arg)
// {
//     ARGS *args = arg;
//     // printf("%d\n", args->index);
//     args->destination[args->index] = numbers[args->index];
//     // printf("i = %d\n", i);
//     // print the thread id that will edite the number
//     // printf("thread id = %ld\n", pthread_self());
//     // printf("value = %d\n", *value);
//     return NULL;
// }

// int main(int ac, char *av[])
// {
//     pthread_t th[10];

//     int arr[10];
//     ARGS arg[10];
//     for (int i = 0; i < 10; i++)
//     {
//         arg[i].index = i;
//         arg[i].destination = arr;
//         pthread_create(&th[i], NULL, pick_number, &arg[i]);
//     }
//     for (int i = 0; i < 10; i++)
//     {
//         pthread_join(th[i], NULL);
//     }
//     for (int i = 0; i < 10; i++)
//     {   
//         printf("%d\n",arr[i]);
//     }
//     return 0;
// }

// int numbers[] = {1,2,3,4, 10, 1};

// pthread_mutex_t mutex;

// typedef struct{
//     int start;
//     int end;
// } ARGS;

// void* routine(void* args)
// {
//     // printf("%d\n", *(int *)num);
//     int *sum;
//     sum = malloc(4);
//     ARGS ar = *(ARGS *)args;
    
//     for (int i = ar.start; i < ar.end; i++)
//     {
//         *sum += numbers[i];
//     }
//     printf("sum = %d\n", *sum);

//     // printf("%ld\n", sizeof(numbers) / 4);
//     return sum;
// }


// int main(int ac, char *av[])
// {
//     pthread_t th[2];
//     ARGS args[2];
//     int start = 0;
//     int sum = 0;
//     int *tmp = 0;
//     for (int i = 0; i < 2; i++)
//     {
//         args[i].start = start;
//         args[i].end = start + (sizeof(numbers) / sizeof(int)) / 2;
//         start = args[i].end;
//         pthread_create(&th[i], NULL, routine, &args[i]);
//     }
//     for (int i = 0; i < 2; i++)
//     {

//         pthread_join(th[i], (void**)&tmp);
//         sum += *tmp;
//     }
//     printf("MAIN THREAD SUM = %d\n", sum);
//     return 0;
// }

pthread_mutex_t mutex;
pthread_cond_t fuelCond;

int fuel = 0;

void* fuel_filling()
{
    for (int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&mutex);
        fuel += 12;
        printf("Filled Fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&fuelCond);
        sleep(1);
    }
    printf("\n");
    return NULL;
}

void* car()
{
    pthread_mutex_lock(&mutex);
    while (fuel < 40)
    {
        printf("WAIT FOR THE FUEL TO BE AVAILABLE\n");
        pthread_cond_wait(&fuelCond, &mutex);
    }
    fuel -= 40;
    printf("Remaining Fuel %d\n", fuel);
    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main(int ac, char *av[])
{
    pthread_t th[5];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&fuelCond, NULL);
    for (int i = 0; i < 5; i++)
    {
        if (i == 4)
            pthread_create(&th[i], NULL, fuel_filling, NULL);
        else
        {
            pthread_create(&th[i], NULL, car, NULL);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&fuelCond);
    return 0;
}
