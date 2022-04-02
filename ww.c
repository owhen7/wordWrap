#include "wwheader.h"
#include "limits.h"

int boolDirectory;

int main(int argc, char **argv)
{
	char nameOfFile[30]; //Declare an array of max length 30 for the file name. This could be changed to be bigger.
	int pageWidth = atoi(argv[1]); //This is the width of the page.

	//If there are less than 3 arguments, we can assume we were sent only the integer.
	if(argc < 3)
	{
		strcpy(nameOfFile, "readStandardIn"); //Flag the name of the file as "readStandardIn"
		boolDirectory = 0;
	}
	else
	{
		strcpy(nameOfFile, argv[2]); //Put argv[2] into it.

		//First, we need to check whether the user has specified a text file or a directory.
		boolDirectory = isDirectory(nameOfFile);
	}


	//If the initial file that the user gave us was just a text file,
	if(isDirectory(nameOfFile) == 0) // If the file is not a folder, it should be a text file.
	{
		//If the file is a text file, we should call wordWrap on it one time and then we're done.
		wordWrapTextFile(nameOfFile, pageWidth);
	}

	//If the file is a directory, we should call wordWrap on all of the text files inside of it.
	if(isDirectory(nameOfFile) == 1)
	{

		DIR *directory;
		struct dirent *dir;
		directory = opendir(nameOfFile);
		chdir(nameOfFile); //This line is important. We need to change the working directory so we can open the text files inside.
		if(directory)
		{
			while ((dir = readdir(directory)) != NULL) //Iterate through all of the files (including folders) in the sub-directory
			{
				if(isDirectory(dir->d_name) == 0) //If the file is not a folder, it should be a text file.
				{
				    if((strstr(dir->d_name, "wrap.") == NULL)) //if the string doesn't contain "wrap." as prefix
					{
						if(!(*dir->d_name == '.')){     //if the string doesn't contain "." as prefix
                            printf("Working on %s\n", dir->d_name); //Print it out for debug purposes,
                            wordWrapTextFile(dir->d_name, pageWidth);
						}
                    }

				}
			}
			closedir(directory);
		}
			printf("Done! Check the directory for the new files.\n");
	}

	return EXIT_SUCCESS;
}

//This is small function that determines whether a file is directory or not.
int isDirectory(const char *filePath)
{
	struct stat statbuff;
	if (stat(filePath, &statbuff) != 0)
		return 0; //Returns 0 if the file is not a directory.
	return S_ISDIR(statbuff.st_mode); //Return a 1 if it is a directory.
}

int wordWrapTextFile(char* argument2, int wrapWidth) //nameOfFile is just the name of the file (that the user gave us).
{
    char nameOfFile[30]; //Declare an array of max length 30 for the file name. This could be changed to be bigger.
	strcpy(nameOfFile, argument2); //Put argv[2] into it.

	int fd, fd2, bytes;
	if(strcmp(nameOfFile, "readStandardIn") == 0) //If the file name has been set to "readStandardIn" to mark standard in needs to be read,
	{
		fd = 0; //File descriptor for standard in.
	}
	else //Otherwise, proceed as normal. Output an error message and try to open the file.
	{
		fd = open(nameOfFile, O_RDONLY);
		if(fd == -1)
		{
		    perror(nameOfFile);
		    return EXIT_FAILURE;
		}
	}


    char *buff = malloc(INT_MAX);   //our buffer
    char *nfile;                    //an array to store information read from file.
    int pos, prevIndex;

            prevIndex = 0;
            nfile=NULL;
            int crntlen = 0;
            //Reading the entire file
            while ((bytes = read(fd,buff,BUFSIZE)) > 0) {
                for(pos = 0; pos < bytes; pos++){
                        //if we find a whitespace, check if we need to normalize it
                        if((buff[pos] == ' ')){
                            if(prevIndex>1){
                            //only read the whitespace if the previous character was not a whitespace
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
                            //if we encounter a newline character, check if we need to normalize it
                            if(buff[pos] == '\n')
                            {
                                if(prevIndex>2){
                                //only accept the newline character if the previous character was not a whitespace, or the previous two characters were not the newline characters
                                    if(!((nfile[prevIndex-1] == ' ')||((nfile[prevIndex-1] == '\n')&&(nfile[prevIndex-2] == '\n'))))
                                    {
                                       nfile = realloc(nfile, crntlen+1);
                                       memcpy(&nfile[prevIndex],&buff[pos],1);
                                       crntlen += 1;
                                       prevIndex++;
                                    }
                                }
                            }
                            else{
                                //if we get a null operator, ignore it
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
            if(!(nfile[prevIndex-1]=='\n')){
                nfile = realloc(nfile, 1+crntlen);
                nfile[prevIndex] = '\n';
                crntlen += 1;
            }
            close(fd);

if(boolDirectory==1){           //if we are working on a directory, create files with wrap. prefix

    const char * prefix = "wrap.";
    int needed = strlen( nameOfFile ) + strlen( prefix ) + 1;

    char * filename;
    filename = malloc(needed * sizeof(char));

    strcpy( filename, prefix );
    strcat( filename, nameOfFile );
    fd2 = open(filename, O_RDWR|O_CREAT, 0777);
    if (fd2 == -1)
        {
            perror(filename);
            return EXIT_FAILURE;
        }

    free(filename);
}

    int exppos=0;
    int wordstart = 0;
    int wordend;
    int wordlen;
    char *newlinechar;
    newlinechar = malloc(5);
    *newlinechar = '\n';
    //go through the entire character array read from the input file
    for(int k=0;k<crntlen;k++){
        if(nfile[k] == ' ' || nfile[k] == '\n'){    //if we encounter any whitespace, that means its the end of a word.
            wordend = k;
            wordlen = wordend-wordstart+1;
            exppos += wordlen;
            if((exppos-1)>wrapWidth){       //if we cannot fit the word in current line then insert a new line and adjust position variable accordingly
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
                if(!(nfile[wordstart-1]=='\n'))
                printf("\n");
                for(bytes = 0; bytes<wordlen;bytes++)
                printf("%c",nfile[wordstart+bytes]);
                }
                wordstart += wordlen;
            }
            else{                           //if the word can be put in current line, write it out
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
    close(fd2);
    free(buff);
    free(newlinechar);
    free(nfile);

    return EXIT_SUCCESS;
}


