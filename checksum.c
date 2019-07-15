//
// Created by xosel on 13/07/2019.
//

#include "checksum.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

char* makeChecksum(char* filename){
    int status;

    int fd_pipeFillToPare[2];
    if (pipe(fd_pipeFillToPare) < 0){
        printf("Error al crear la pipe!\n");
        exit(-1);
    }


    int sonPID = fork();
    switch (sonPID){
        case -1:
            // Error
            printf("Error al crear el fork!\n");
            exit(-1);
            break;
        case 0:
            printf("Soc el fill!\n");
            // Tanquem fd de les pipes que no utilitzarem
            close(fd_pipeFillToPare[PIPE_READ]);

            dup2(fd_pipeFillToPare[PIPE_WRITE], 1);

            execl("/usr/bin/md5sum", "md5sum", filename, (char*)0);
            // Sens folla al programa (nomes al fill)!!

            break;
        default:
            // Pare

            close(fd_pipeFillToPare[PIPE_WRITE]);

            waitpid(sonPID, &status, WUNTRACED);

            char* md5sum = (char*)malloc(sizeof(char) * 32);
            read(fd_pipeFillToPare[PIPE_READ], md5sum, sizeof(char) * 32);

            printf("CHECKSUM: %s \n", md5sum);

            close(fd_pipeFillToPare[PIPE_READ]);

            printf("Tot ha anat be!\n");

            return md5sum;

            break;
    }
}