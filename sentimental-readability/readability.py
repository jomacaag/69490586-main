from cs50 import get_string

# main function


def main():
    text = get_string('Text: ')
    score = calculate(text)
    if score < 1:
        print('Before Grade 1')
    elif score > 16:
        print('Grade 16+')
    else:
        print(f'Grade {score}')


# calculate function that recieves text
def calculate(text):
    # initalized all to floating numbers
    letters = 0.0
    index = 0.0
    sentences = 0.0
    # first word added because it only checks for spaces in beteween words, hence one is missed.
    words = 1.0

    for i in text:
        if i.isalpha():
            letters += 1
        if i == ' ':
            words += 1
        if i in ['.', '!', '?']:
            sentences += 1

            # formula to chek
    L = letters / words * 100.0
    S = sentences / words * 100.0
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)


main()