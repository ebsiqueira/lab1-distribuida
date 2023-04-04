#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char op[3], key[5], value[10], cmd[21], rtn[10];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // address.sin_addr.s_addr = inet_addr("");
    address.sin_port = htons(9734);

    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        perror("oops: client");
        exit(1);
    }
    while (1)
    {
        printf("type operation (put or get): ");
        scanf("%s", &op);
        if (strcmp(op, "put") == 0)
        {
            printf("type key: ");
            scanf("%s", &key);
            printf("type value: ");
            scanf("%s", &value);
            strcpy(cmd, op);
            strcat(cmd, "(");
            strcat(cmd, key);
            strcat(cmd, ",");
            strcat(cmd, value);
            strcat(cmd, ")");
            write(sockfd, &cmd, sizeof(cmd));
        }
        else
        {
            if (strcmp(op, "get") == 0)
            {
                printf("type key: ");
                scanf("%s", &key);
                strcpy(cmd, op);
                strcat(cmd, "(");
                strcat(cmd, key);
                strcat(cmd, ")");
                write(sockfd, &cmd, sizeof(cmd));
                read(sockfd, &rtn, sizeof(rtn));
                printf("%s\n", &rtn);
            }
            else
            {
                if (strcmp(op, "quit") == 0)
                {
                    char quit = 'q';
                    write(sockfd, &quit, 1);
                    break;
                }
                else
                {
                    printf("invalid operation\n");
                }
            }
        }
    }
    close(sockfd);
    exit(0);
}
