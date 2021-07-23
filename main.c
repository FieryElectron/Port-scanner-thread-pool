#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>
#include "thpool.h"

#define ServerIP "45.33.32.156"

pthread_mutex_t mut;

int counter = 0;

void task(int i)
{
    int sockfd;
    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket error\n");
        return;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(i);

    if (inet_pton(AF_INET, ServerIP, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error\n");
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        //        printf("port: %d close\n", i);
        close(sockfd);
    }
    else
    {
        printf("\rport: % 5d -----------------> open\n", i);
        close(sockfd);
    }

    pthread_mutex_lock(&mut);
    //    printf("Thread #%u --->%d\n", (int)pthread_self(),counter);
    ++counter;
    pthread_mutex_unlock(&mut);
}

int main()
{
    pthread_mutex_init(&mut, NULL);
    threadpool thpool = thpool_init(1000);

    for (int i = 0; i < 65536; i++)
    {
        thpool_add_work(thpool, (void *)task, i);
    };

    while (1)
    {
        pthread_mutex_lock(&mut);
        printf("\rIn progress %.2f%% %d", (float)counter / 65535.0 * 100, counter);
        if (counter >= 65535)
        {
            break;
        }
        pthread_mutex_unlock(&mut);
    }
    printf("\nKilling threadpool\n");
    thpool_destroy(thpool);
    printf("End!\n");

    return 0;
}
