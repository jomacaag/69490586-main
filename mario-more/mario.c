#include <cs50.h>
#include <stdio.h>



int main(void)
{
    int height;
    do
    {

        //promt user for height
        height = get_int("Desired height: ");

        if (height >= 1 && height <= 8)
        {
            //rows
            for (int i = 0; i < height; i++)
            {
                //Columns:
                //adding space before bricks
                for (int k = height - 1 - i; k > 0; k--)
                {
                    printf(" ");
                }
                //adding brick
                for (int j = 0; j <= i; j++)
                {
                    printf("#");
                }
                //adding space in middle
                printf("  ");
                //printing right side of pyramid
                for (int j = 0; j <= i; j++)
                {
                    printf("#");
                }
                printf("\n");

            }
        }
    }
    while (height < 1 || height > 8);
}


