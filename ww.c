#include "wwheader.h"
#include "limits.h"

int main(int argc, char **argv)
{
    printf("%d\n",argc);
    printf("%s\n",argv[0]);
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);

    int fd, fd2, bytes;
    const int newLineASCII = 10;
    /*struct stat fileStat;
fstat(fd, &fileStat); */// Don't forget to check for an error return in real code
// Allocate enough to hold the whole contents plus a '\0' char.
    char *buff = malloc(INT_MAX);
    char *nfile;
    int start, pos, prevIndex;

    if (argc > 2)
    {
        fd = open(argv[2], O_RDONLY);
        if (fd == -1)
        {
            perror(argv[2]);
            return EXIT_FAILURE;
        }
    }
    else
    {
        fd = 0;
    }
    //for(int k=0;k<INT_MAX;k++){
            prevIndex = 0;
            nfile=NULL;
            int crntlen = 0;
            while ((bytes = read(fd,buff,1)) > 0) {

            //printf("Read %d bytes\n", bytes);
            //printf("%s\n",buff);
            for(pos = 0; pos < bytes; pos++){
                    if((buff[pos] == ' ')){
                        if(prevIndex>1){
                        if(!((nfile[prevIndex-1] == ' ')||(nfile[prevIndex-1] == '\n')))
                        {
                           nfile = realloc(nfile, 1+crntlen);
                           memcpy(&nfile[prevIndex],&buff[pos],1);
                           crntlen += 1;
                           prevIndex++;
                        }
                        }
                    }
                    else {
                            if(buff[pos] == '\n')
                        {
                        //printf("%d\n",prevIndex);
                        if(prevIndex>1){
                            if(!((nfile[prevIndex-1] == ' ')||((nfile[prevIndex-1] == '\n'))))
                            {
                               nfile = realloc(nfile, 1+crntlen+1);
                               memcpy(&nfile[prevIndex],&buff[pos],1);
                               //printf("%c\n",nfile[prevIndex]);
                               crntlen += 1;
                               prevIndex++;
                               /*nfile[prevIndex+1] = '\0';
                               crntlen += 1;
                               prevIndex+=2;*/
                            }
                            }
                        }
                        else{
                            if(!(buff[pos]=='\0')){
                        nfile = realloc(nfile, 1+crntlen);
                        memcpy(&nfile[prevIndex],&buff[pos],1);
                        crntlen += 1;
                        prevIndex++;
                            }
                    }
                    }

                    }

            }

            for (int i = 0; i < crntlen; ++i) {
                printf("%c\n", nfile[i]);
            }
    printf("\nCurrent Length: %d\n",crntlen);
            printf("Current prevIndex: %d\n",prevIndex);

    close(fd);

    const char * prefix = "wrap.";
    int needed = strlen( argv[2] ) + strlen( prefix ) + 1;

    char filename[needed];

    strcpy( filename, prefix );
    strcat( filename, argv[2] );
    fd2 = open(filename, O_RDWR|O_CREAT, 0777);
    if (fd2 == -1)
        {
            perror(filename);
            return EXIT_FAILURE;
        }
    bytes = write(fd2, nfile, prevIndex);
    if (bytes == -1)
    {
            perror(filename);
            return EXIT_FAILURE;
    }
    close(fd2);
return EXIT_SUCCESS;
}
