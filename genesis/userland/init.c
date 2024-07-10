#include "userland/libs/std.h"
// #include <fcntl.h>

int main(int argc, char *argv[]) {
    char banner[] = "Welcome to init!\n";
    char prompt[] = "[root@os]~# ";
    char resp[] = "\nYou typed: ";
    char input[0x20];
    int count;
    write(1, banner, sizeof(banner));

    int fd = open("0:lol.txt", 1); // 1 = FA_READ
    char contents[0x100];
    int bread = read(fd, contents, sizeof(contents));
    write(1, contents, bread);
    while(1) {
        write(1, prompt, sizeof(prompt));
        count = read(0, input, sizeof(input));
        write(1, resp, sizeof(resp));
        write(1, input, count);
        write(1, "\n", 1);

        if(input[0]=='b' && input[1]=='y' & input[2]=='e') {
            break;
        }
    }

    return 0;
}