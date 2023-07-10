import csv
import sys


def main():
    # Check for command-line usage, print error and usage if incorrect num of arguments
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py databases/data.csv sequences/#.txt")
        return 1

    # Read database file into a variable
    data = []
    with open(sys.argv[1]) as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            data.append(row)

    STRs = data[0][1::]

    # Read DNA sequence file into a variable
    seq = ()
    with open(sys.argv[2]) as txtfile:
        seq = txtfile.read()

    # TODO: Find longest match of each STR in DNA sequence
    matches = []
    for n in STRs:
        long_m = longest_match(seq, n)
        matches.append([n, long_m])

    # TODO: Check database for matching profiles
    for row in range(0, len(data)-1):
        num_matches = 0
        for n in range(0, len(STRs)):
            # print(data[row+1][n+1],matches[n][1])
            if int(data[row+1][n+1]) == matches[n][1]:
                num_matches += 1
            # print (num_matches)
            if num_matches == len(STRs):
                print(data[row+1][0])
                sys.exit()

    print('No Match')
    return 0


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
