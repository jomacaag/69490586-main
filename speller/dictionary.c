
// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include "dictionary.h"



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 400;
int word_count = 0;
int exitcode = 0;
// Hash table
node *table[N];

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize the hash table with NULL values.
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file for reading.
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        // If the file cannot be opened, return false to indicate failure.
        return false;
    }

    // Read words from the dictionary file and insert them into the hash table.
    char words[LENGTH + 1];
    int x = 0;
    while (fscanf(dict, "%s", words) == 1)
    {
        // Convert the word to uppercase.
        for (int i = 0; i < strlen(words); i++)
        {
            words[i] = toupper(words[i]);
        }

        // Get the hash value for the word.
        x = hash(words);

        // Allocate memory for a new node to store the word.
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            // If memory allocation fails, return false to indicate failure.
            return false;
        }

        // Copy the word to the new node's word field.
        strcpy(n->word, words);
        // Insert the new node at the head of the linked list at the index corresponding to the hash value.
        n->next = table[x];
        table[x] = n;
        // Increment the word count.
        word_count++;
    }

    // Close the dictionary file.
    fclose(dict);
    // Return true to indicate success.
    return true;
}





// Hashes word to a number
unsigned int hash(const char *word)
{
    // Create a temporary buffer to store the word.
    char temp[LENGTH + 1];
    // Copy the word to the temporary buffer.
    strcpy(temp, word);

    // Convert all characters in the buffer to uppercase.
    for (int i = 0; i < strlen(temp); i++)
    {
        temp[i] = toupper(temp[i]);
    }

    // Get the length of the buffer.
    int len = strlen(temp);

    // Calculate the hash value using the first, last, and second-to-last characters in the buffer.
    int y = temp[0] + temp[len - 1] + temp[len - 2];

    // Take the hash value modulo N to get a number between 0 and N-1.
    int h = y % N;

    return h;
}



// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Create a temporary buffer to store the word.
    char temp[LENGTH + 1];
    // Copy the word to the temporary buffer.
    strcpy(temp, word);
    // Convert all characters in the buffer to uppercase.
    for (int i = 0; i < strlen(temp); i++)
    {
        temp[i] = toupper(temp[i]);
    }

    // Get the hash value for the word.
    int x = hash(temp);
    // Look up the node at the index corresponding to the hash value.
    node *n = table [x];

    // If the index is empty, the word is not in the hash table.
    if (n == NULL)
    {
        return false;
    }

    // Traverse the linked list at the index to check if the word is present.
    n = table[x];
    while (n != NULL)
    {
        // Compare the word with the current node's word (ignoring case).
        if (strcasecmp(n->word, temp) == 0)
        {
            // If the words match, the word is in the hash table.
            return true;
        }
        // Move to the next node in the linked list.
        n = n->next;
    }

    // If we reach here, the word is not in the hash table.
    return false;
}
// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// This function unloads the dictionary from memory, freeing all dynamically allocated memory.
// It returns true if successful, else false.
bool unload(void)
{
    // Traverse the hash table, freeing each linked list.
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];
        while (n != NULL)
        {
            // Save a pointer to the current node.
            node *temp = n;
            // Move to the next node in the linked list.
            n = n->next;
            // Free the current node.
            free(temp);
        }
        // Set the index in the hash table to NULL to indicate that it is empty.
        table[i] = NULL;
    }
    // Return true to indicate that the function was successful.
    return true;
}