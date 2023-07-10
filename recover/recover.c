#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    //require argument
    if (argc != 2)
    {
        printf("Usage: ./recover 'file.name'\n");
        return 1;
    }
    //open input file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    //each block is 512 Bytes long
    uint8_t buffer[512];
    int num = 0;
    char filename[8]; //for XXX.jpeg with an extra for \n
    FILE *img = NULL; //initiate empty image file
    while (fread(&buffer, 1, sizeof(buffer), input) == sizeof(buffer))
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            //need to release img memory from previous img when a new one opens
            if (img != NULL)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", num);
            img = fopen(filename, "w");
            num++;
        }

        if (img != NULL)
        {
            fwrite(&buffer, 1, sizeof(buffer), img);
        }
    }

    fclose(img);
    fclose(input);


}
