#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int votecount;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void merge_sort(int i, int j, pair pairs2[], pair temp[]);
bool has_cycle(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // number of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    //candidates array
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        votecount = 0 ;
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //Make sure name voted is one of the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            //     printf("votcount++ ");
            ranks[rank] = i;
            votecount++;
        }
    }
    if (votecount == (rank + 1))
    {
        //printf("votcount: %i rank: %i \n",votecount,rank+1);
        return true;
    }
    else
    {
        return false;
    }
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }

        }

    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp[pair_count];
    merge_sort(0, pair_count - 1, pairs, temp);
    return;
}

void merge_sort(int i, int j, pair pairs2[], pair temp[])
{
    // i = the first index of array, j = the last index of array
    if (j <= i)
    {
        return;
    }
    int mid = (i + j) / 2;
    merge_sort(i, mid, pairs, temp);
    merge_sort(mid + 1, j, pairs, temp);
    int low = i;
    int high = mid + 1;
    for (int k = i; k < j + 1; k++)
    {
        int left_winner = pairs2[low].winner;
        int left_loser = pairs2[low].loser;
        int right_winner = pairs2[high].winner;
        int right_loser = pairs2[high].loser;
        if (low == mid + 1) // left array reaches its end
        {
            temp[k] = pairs2[high];
            high++;
        }
        else if (high == j + 1) // right array reaches its end
        {
            temp[k] = pairs2[low];
            low++;
        }
        else if (preferences[left_winner][left_loser] > preferences[right_winner][right_loser])
        {
            temp[k] = pairs2[low];
            low++;
        }
        else
        {
            temp[k] = pairs2[high];
            high++;
        }
    }
    for (int k = i; k < j + 1; k++)
    {
        // copy sorted array into original pair
        pairs2[k] = temp[k];
    }
}

bool has_cycle(int winner, int loser)
{
    if (locked[loser][winner] == true)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i] == true && has_cycle(winner, i))
        {
            return true;
        }
    }
    return false;
}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        if (!has_cycle(winner, loser))
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int i;
    for (i = 0; i < candidate_count; i++)
    {
        int x = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                x = 1;
            }
        }

        if (x == 0)
        {
            break;
        }

    }
    printf("%s\n", candidates[i]);
    return;
}