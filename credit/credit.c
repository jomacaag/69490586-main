#include <cs50.h>
#include <stdio.h>
#include <string.h>


int luhn(void);
int validate(void);


//initialize variables
long n = 0;
long div = 0;
long x = 0;
long doub = 0;
long total = 0;
int length = 0;
int valid = 0;
char str[20];

int main(void)
{

    //ask for number
    n = get_long("Number: ");
    //calculate length of number

    sprintf(str, "%ld", n);
    length = strlen(str);
    //check imput length to be betweeen 13 and 16
    if (length >= 13 && length <= 16)
    {
        //calculate Luhn's Algorithm
        luhn();
        //check if invalid, also assign card if algorithm is good
        if (total % 10 != 0)
        {
            printf("INVALID\n");
        }
        else
        {
            validate();
        }
    }
    else
    {
        printf("INVALID\n");
    }
}






int luhn(void)
{
    //set div to number
    div = n;
    //calculate Luhn's Algorithm
    //loop through the numbers, with division/10 which truncates off last digit and modulus which gets remaider
    for (int i = 0; i < length; i++)
    {
        //gets last digit
        x = div % 10;
        //because calculation is alternating numbers: starting from second to last

        //if the last number is in an odd position
        if (i % 2 != 0)
        {
            //double that digit and then add the digits of itself together, ie. 13->1+3
            doub = x * 2;
            //everything gets added to the total
            total = total + doub / 10 + doub % 10;
        }
        //if its an even position
        else
        {
            //adds even even placed digit to total
            total = total + x;
        }
        //truncates
        div = div / 10;
    }

    return 0;

}



int validate(void)
{
    if (str[0] == '4')
    {
        printf("VISA\n");
    }
    else if (str[0] == '3' && (str[1] == '4' || str[1] == '7'))
    {
        printf("AMEX\n");
    }

    else if (str[0] == '5' && (str[1] >= '1' && str[1] <= '5'))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}