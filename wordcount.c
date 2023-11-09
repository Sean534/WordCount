#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

//author by Sean Dong

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Error: Expected 1 argument, found %d", argc-1 );
        if (argc != 1)
        {
            printf(":");
            for (int i = 1; i < argc; i++)
            {
                printf(" \"%s\"", argv[i]); 
            }
        }
        printf("\n");
        return -1;
    }

    int fileID = open(argv[1], O_RDONLY);
    if (fileID == -1)
    {
        if (errno == EACCES)
        {
            printf("Error: Read permission denied: \"%s\"", argv[1]);
        }
        else
        {
            printf("Error: Does not exist in directory: \"%s\"", argv[1]);
        }
        printf("\n");
        return -1;
    }

    char buffer;
    int lineCount = 0;
    int wordCount = 0;
    int charCount = 0;
    int inWord = 0;

    while (read(fileID, &buffer, 1) != 0)
    {
        ++charCount;

        if (buffer == '\n')
        {
            ++lineCount;
            if (inWord)
            {
            ++wordCount;
            }
            inWord = 0; // a new line will terminates a word
        }

        else if (isspace(buffer))
        {
            if (inWord)
            {
                ++wordCount;
                inWord = 0; // a whitespace will terminate a word
            }
        }
        
        else
        {
            inWord = 1; // a non-whitespace character indicates we are inside a word
        }
    }

    close(fileID);

    if (lineCount == 0 && wordCount == 0 && charCount == 0)
    {
        printf("Error: File is empty: \"%s\"\n" , argv[1]);
        return -1;
    }

    // Check if the file ends with a word for EOF
    if (inWord)
    {
        ++wordCount;
        if (charCount > 0)
        {
            ++lineCount;
        }
    }

    printf("%d %d %d %s\n", lineCount, wordCount, charCount, argv[1]);
    

}