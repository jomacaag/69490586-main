#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int in_arg(string cipher);



//input arguments, argc is number of inputed arguments
int main(int argc, string key[])
{
    //Check number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //if number of arguments good then:
    else
    {
        //run key check, and get return 0 or 1 as an integer code
        int code = in_arg(key[1]);
        //if no errors on keycheck then:
        if (code == 0)
        {
            //get plaintext input
            string ptext = get_string("plaintext:  ");
            //count length of input including all characters
            int plen = strlen(ptext);
            printf("ciphertext: ");
            //calculate enctryption
            //for each character in user inputed plaintext,
            for (int k = 0; k < plen; k++)
            {
                //if character is alphabet then:
                if (isalpha(ptext[k]))
                {
                    //if character in plaintext is lowercase:
                    if (islower(ptext[k]))
                    {
                        //print a new character, lowercase with new refrence to provided key, eg a=97 so new character woudl be the key[0]
                        printf("%c", tolower(key[1][ptext[k] - 97]));
                    }
                    else
                    {
                        //print a new character, uppercase with new refrence to provided key, eg B=66 so new character would be the key[0]
                        printf("%c", toupper(key[1][ptext[k] - 65]));
                    }
                }
                else
                {
                    //if character is not  a letter then print the carachter as it was inputed
                    printf("%c", ptext[k]);
                }
            }
            //new line at end of ciphered text
            printf("\n");
            return 0;
        }
        else
        {
            //if there were errors on keycheck, then:
            return 1;
        }
    }






}




int in_arg(string cipher)
{
    //get length of the key
    int ciph_len = strlen(cipher);
    //get new string so we can get lowercase  and compare to itself for no repeated letter... else if not then could have repeated letter as in 'R' and 'r'
    string lower_ciph = cipher;
    //counter for repeated letters
    int y = 0;
    //two things, 1.make all letters lowercase and 2. if any character is not a letter, eg. '!' then it adds a counter,
    for (int n = 0; n < ciph_len; n++)
    {
        lower_ciph[n] = tolower(cipher[n]);
        if (!isalpha(lower_ciph[n]))
        {
            y++;
        }
    }
    //if non character counter is not 0 -in other words- if there is a character that is not a letter then throw error
    if (y != 0)
    {
        printf("Key must contain only letters");
        return 1;
    }
    //if no repeated letters and only letters present in key then:
    else
    {
        //counter for number of same letters in a key
        int count = 0;
        //check if key is 26 characters long.
        if (ciph_len != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        else
        {
            //this loop loops itself 26 by 26 times, and counts anytime a letter is used once, eg. if a letter is used twice it will count it twice:
            //example, say the letter t was used twice [t,T...] it will count++ when lower_ciph[i/0] == lower_ciph[j/0] as well as lower_ciph[i/0] == lower_ciph[j/1] then again twice at: lower_ciph[i/1] == lower_ciph[j/0] as well as lower_ciph[i/1] == lower_ciph[j/1]
            for (int i = 0; i < 26; i++)
            {
                for (int j = 0; j < 26; j++)
                {
                    if (lower_ciph[j] == lower_ciph[i])
                    {
                        count++;
                    }
                }
            }
            //if the letter not matched only once and giving a count of more than 26 then error
            if (count != 26)
            {
                printf("Each letter must be used only once.\n");
                return 1;
            }
            // if the key has only letters AND the key is 26 carachters in length AND no letter is used more than once, then:
            else
            {
                return 0;
            }
        }
    }
}




