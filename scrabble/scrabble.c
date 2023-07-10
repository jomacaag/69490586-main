#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Compare scores and Print the winner
    if (score1 == score2)
    {
        printf("Tie!\n");
    }
    else if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else
    {
        printf("Player 2 wins!\n");
    }
}



// TODO: Compute and return score for string
int compute_score(string word)
{
    //initialize and reset x and y
    int y = 0;
    int x = 0;
    //get length of string
    int length = strlen(word);
    //loop through string
    for (int i = 0; i < length; i++)
    {
        //for every character in string, change it to lowercase
        word [i] = tolower(word[i]);
        //set index to start at 'a' = 0
        x = word[i] - 97;
        //Only add y if carachter in array is a-z anything else will be excluded to summation of points
        if (x >= 0 && x <= 25)
        {
            //using the points array add the points of the word, indexed now with x 
            y = y + POINTS[x];
        }
    }
    //return the value of y back to where it was called
    return y;
}
