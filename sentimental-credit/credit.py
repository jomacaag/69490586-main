import re


def main():
    # ask for number
    cardnum = (input("Number: "))
    luhn(cardnum)


def luhn(card):
    # import string as part of list
    cardrev = [int(n) for n in card]
    # reverse the list
    cardrev.reverse()
    # sum every odd number
    sumofodd = sum(cardrev[::2])
    # for every even number double it and add it to list
    cardeven = [2*x for x in cardrev[1::2]]
    # initialize sum
    sumofeven = 0
    # calculate every odd number by deviding by 10 and addint the remainder to itself so that 13/10 =1 +remainder3 => 4
    for x in cardeven:
        sumofeven += x // 10 + x % 10
    # match that card is between 13-16 digits and that the division by 10 has no remainders.
    if (re.match(r"\d{13,16}$", card) and ((sumofeven+sumofodd) % 10 == 0)):
        validate(card)
    else:
        print('INVALID')


def validate(card):
    # these matches cards to their issuer,
    if re.match(r"^4\d{12,15}$", card):
        print('VISA')
    elif re.match(r"^3[4,7]\d{11,14}$", card):
        print('AMEX')
    elif re.match(r"^5[0-5]\d{11,14}$", card):
        print('MASTERCARD')
    else:
        print('INVALID')
    return 0


main()