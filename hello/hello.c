#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // pulls user input as a string, name
    string name = get_string("What is your name? :");
    //prints words as well as a place holder for name, %s -> name
    printf("Hello %s, Welcome to CS50\n", name);
}