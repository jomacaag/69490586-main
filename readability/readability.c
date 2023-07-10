#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int calculate(string);

int main(void)
{
    //Get text from user

    string text = get_string("Text: ");
    //Calculate level
    int score = calculate(text);

    //Print Results
    if (score < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (score > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", score);
    }

}


int calculate(string text1)
{
    double index = 0;
    double letters = 0;
    double sentences = 0;
    double words = 1;
    int x = 0;

    int length = strlen(text1);


    for (int i = 0; i < length; i++)
    {
        //for every character in string, change it to lowercase
        text1 [i] = tolower(text1[i]);
        //set character to x for if elseses
        x = text1[i];
        //counters
        //First if is letter counters
        if ((x >= 65 && x <= 90) || (x >= 97 && x <= 122))
        {
            letters = letters + 1;
        }
        else if (x == 46 || x == 63 || x == 33)
        {
            sentences = sentences + 1;
        }
        else if (x == 32)
        {
            words = words + 1;
        }

    }

    double L = letters / words * 100.0;
    double S = sentences / words * 100.0;
    index = 0.0588 * L - 0.296 * S - 15.8;




    return round(index);

}
