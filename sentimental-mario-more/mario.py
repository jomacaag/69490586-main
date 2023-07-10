from cs50 import get_int

while True:

    height = get_int('Desired Height: ')
    # only if input is inside or 1-8 will code exit. else it will keep prompting user for input
    if height >= 1 and height <= 8:
        for j in range(1, height + 1):
            print(' '*(height - j) + '#' * (height - (height - j)) + '  ' + '#' * (height - (height - j)))
        break