#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen()
#include <err.h>
#include <unistd.h> // read(), write(), close()
#include <fcntl.h>

#define BUFFER_SIZE 4096
/*
argc =  how many parameter 
argv[] = array of string pointer
*/
int main(int argc, char *argv[])
{
    int fd, size;
    char *deli;

    if (argc < 3)
        errx(1, "not enough arguements\nusage: %s <files> <delimiter>", argv[0]);

    for (int i = 1; i < argc-1; i++)
    {
        fd = open(argv[i], O_RDONLY); //read only
        deli = argv[argc-1];

        if (fd == -1 && strcmp(argv[i], "-") != 0)
            errx(1, "%s: No such file or directory", argv[i]);
        else if(strlen(deli) != 1)
            errx(1, "only single-character delimiters allowed\nusage: %s <files> <delimiter>", argv[0]);
        else
        {
            char buf[BUFFER_SIZE]; 
            
            if(strcmp(argv[i], "-") == 0)
            {
                while(1)
                {
                    size = read(0, buf, BUFFER_SIZE);   // read from stdin(0) and store in buf
                    if (size == 0)                      // until size = 0
                        break;
                    for (int j = 0; j < size; j++)      // compare with delimiter
                    {
                        if (buf[j] == *deli)
                        buf[j] = '\n';
                    }
                    write(1, buf, size);                // read the buf and write to stdout(1)
                }
            }
            else
            {
                while(1)
                {
                    size = read(fd, buf, BUFFER_SIZE);  // read the file descriptor and put it in buf
                    if (size == 0)
                        break;
                    for (int j = 0; j < size; j++)      // compare with delimiter
                    {
                        if (buf[j] == *deli)
                        buf[j] = '\n';
                    }
                    write(1, buf, size);               // read the buf and write in first parameter (stdout in this case)
                }
            }
        }
        close(fd);
    }
    return 0;
}