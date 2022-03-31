#include "wwheader.h"
#include "limits.h"

int boolDirectory;

int main(int argc, char **argv)
{
	char nameOfFile[30]; //Declare an array of max length 30 for the file name. This could be changed to be bigger.
	strcpy(nameOfFile, argv[2]); //Put argv[2] into it.

	printf("Here is the nameOfFile:%s.\n", nameOfFile); //Print out the string we are calling wordWrapTextFile on.

	int pageWidth = atoi(argv[1]); //This is the width of the page.

	//First, we need to check whether the user has specified a text file or a directory.
//	printf("isDirectory is %d.\n", isDirectory(nameOfFile));
    boolDirectory = isDirectory(nameOfFile);


	//If the initial file that the user gave us was just a text file,
	if(isDirectory(nameOfFile) == 0) // If the file is not a folder, it should be a text file.
	{
		//If the file is a text file, we should call wordWrap on it one time and then we're done.
		wordWrapTextFile(nameOfFile, pageWidth);
	}

	//If the file is a directory, we should call wordWrap on all of the text files inside of it.
	if(isDirectory(nameOfFile) == 1)
	{

		DIR *d;
		struct dirent *dir;
		d = opendir(nameOfFile);
		chdir(nameOfFile); //This line is important. We need to change the working directory so we can open the text files inside.
		if(d)
		{
			while ((dir = readdir(d)) != NULL) //Iterate through all of the files (including folders) in the sub-directory
			{
				printf("%s\n", dir->d_name);
				if(isDirectory(dir->d_name) == 0) //If the file is not a folder, it should be a text file.
				{
					if(strstr(dir->d_name, "wrap.") == NULL) //if the string doesn't contain "wrap."
					{
						printf("%s\n", dir->d_name); //Print it out for debug purposes,
						wordWrapTextFile(dir->d_name, pageWidth);
					}
				}
			}
			closedir(d);
		}
	}

	return EXIT_SUCCESS;
}

int wordWrapTextFile(char* argument2, int wrapWidth) //nameOfFile is just the name of the file (that the user gave us).
{
    char nameOfFile[30]; //Declare an array of max length 30 for the file name. This could be changed to be bigger.
	strcpy(nameOfFile, argument2); //Put argv[2] into it.

printf("\nIS IT A DIRECTORY: %d\n",boolDirectory);

	int fd, fd2, bytes;
	fd = open(nameOfFile, O_RDONLY);
	if(fd == -1)
		{
		    perror(nameOfFile);
		    return EXIT_FAILURE;
		}
	    //remember to check argc length back in main at some point.

    char *buff = malloc(INT_MAX);
    char *nfile;
    int start, pos, prevIndex;

            prevIndex = 0;
            nfile=NULL;
            int crntlen = 0;
            while ((bytes = read(fd,buff,BUFSIZE)) > 0) {
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
                        if(prevIndex>1){
                            if(!((nfile[prevIndex-1] == ' ')||((nfile[prevIndex-1] == '\n')&&(nfile[prevIndex-2] == '\n'))))
                            {
                               nfile = realloc(nfile, 1+crntlen+1);
                               memcpy(&nfile[prevIndex],&buff[pos],1);
                               crntlen += 1;
                               prevIndex++;
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
            nfile = realloc(nfile, 1+crntlen);
            nfile[prevIndex] = '\n';
            crntlen += 1;

    close(fd);
if(boolDirectory==1){

    const char * prefix = "wrap.";
    int needed = strlen( nameOfFile ) + strlen( prefix ) + 1;

    char filename[needed];

    strcpy( filename, prefix );
    strcat( filename, nameOfFile );
    fd2 = open(filename, O_RDWR|O_CREAT, 0777);
    if (fd2 == -1)
        {
            perror(filename);
            return EXIT_FAILURE;
        }
}
    int exppos=0;
    int wordstart = 0;
    int wordend;
    int wordlen;
    char *newlinechar;
    newlinechar = malloc(5);
    *newlinechar = '\n';
    if(boolDirectory==0)
    printf("\nTHE OUTPUT IS: \n\n");
    for(int k=0;k<crntlen;k++){
        if(nfile[k] == ' ' || nfile[k] == '\n'){
            wordend = k;
            wordlen = wordend-wordstart+1;
            exppos += wordlen;
            if((exppos-1)>=wrapWidth){
                exppos = wordlen;
                if(boolDirectory==1){
                bytes = write(fd2,newlinechar,1);
                if (bytes == -1)
                {
                    perror(nameOfFile);
                    return EXIT_FAILURE;
                }
                bytes = write(fd2,&nfile[wordstart],wordlen);
                if (bytes == -1)
                {

                    perror(nameOfFile);
                    return EXIT_FAILURE;
                }
                }
                else{
                printf("\n");
                for(bytes = 0; bytes<wordlen;bytes++)
                printf("%c",nfile[wordstart+bytes]);
                }
                wordstart += wordlen;
            }
            else{
                if(boolDirectory==1){
                bytes = write(fd2,&nfile[wordstart],wordlen);
                if (bytes == -1)
                {

                    perror(nameOfFile);
                    return EXIT_FAILURE;
                }
                }
                else{
                    for(bytes = 0; bytes<wordlen;bytes++)
                    printf("%c",nfile[wordstart+bytes]);
                }
                wordstart += wordlen;
            }

        }
    }
    //bytes = write(fd2, nfile, prevIndex);
    close(fd2);
    free(buff);
    free(newlinechar);
    free(nfile);

    return EXIT_SUCCESS;
}

int isDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0; //Returns 0 if the file is not a directory.
	return S_ISDIR(statbuf.st_mode); //Return a 1 if it is a directory.
}



