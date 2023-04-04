#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define key_size 5
#define value_size 10
#define max_clients 2
#define vault_capacity 10

int global = 0;

pthread_mutex_t store_lock;
char store_key[vault_capacity][key_size];
char store_value[vault_capacity][value_size];

void *user(void *arg);

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t tid[max_clients + 5];
    pthread_mutex_init(&store_lock, NULL);

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, max_clients);
    int i = 0;
    while (1)
    {
        printf("server waiting\n");
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        if (pthread_create(&tid[i++], NULL, user, &client_sockfd) != 0)
        {
            printf("failed to create thread\n");
        }
        else
        {
            printf("thread %i created\n", i);
        }
        if (i > max_clients)
        {
            i = 0;
            while (i < max_clients)
            {
                pthread_join(tid[i++], NULL);
                printf("thread %i finished\n", i);
            }
            i = 0;
        }
    }
}
void *user(void *arg)
{
    int client_sockfd = *((int *)arg);
    char cmd[21];
    char rtn[value_size];
    char key[key_size];
    char value[value_size];
    while (1)
    {
        fflush(stdout);
        read(client_sockfd, &cmd, sizeof(cmd));
        if (cmd[0] == 'p' && cmd[1] == 'u' && cmd[2] == 't')
        {
            char *token = strtok(cmd, ",");
            for (int i = 4; i < sizeof(token); i++)
            {
                key[i - 4] = token[i];
            }
            token = strtok(NULL, "");
            for (int i = 0; i < sizeof(token); i++)
            {
                if (token[i] == ')')
                {
                    break;
                }
                value[i] = token[i];
            }

            pthread_mutex_lock(&store_lock);
            for (int l = 0; l < sizeof(key); l++)
            {
                store_key[global][l] = key[l];
            }
            for (int l = 0; l < sizeof(value); l++)
            {
                store_value[global][l] = value[l];
            }
            global++;
            pthread_mutex_unlock(&store_lock);
        }
        else
        {
            if (cmd[0] == 'g' && cmd[1] == 'e' && cmd[2] == 't')
            {
                for (int i = 4; i < sizeof(cmd); i++)
                {
                    if (cmd[i] == ')')
                    {
                        break;
                    }
                    key[i - 4] = cmd[i];
                }
                pthread_mutex_lock(&store_lock);
                int m = 0;
                for (m; m < vault_capacity; m++)
                {
                    if (strcmp(store_key[m], key) == 0)
                    {
                        break;
                    }
                }
                strcpy(rtn, store_value[m]);
                pthread_mutex_unlock(&store_lock);
                write(client_sockfd, &rtn, sizeof(rtn));
            }
            else
            {
                close(client_sockfd);
                break;
            }
        }
    }
    pthread_exit(NULL);
}