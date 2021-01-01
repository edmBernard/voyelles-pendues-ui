

def getLengthWithoutVowels(word):
    return sum([1 for letter in word if letter not in "aeiouy"])

def strip_vowels(word):
    return "".join(letter for letter in word if letter in "bcdfghjklmnpqrstvwxz")

def strip_consonant(word):
    return "".join(letter if letter not in "bcdfghjklmnpqrstvwxz" else "*" for letter in word)


def main():
    with open("liste.de.mots.francais.frgut.txt", "r") as infile:
        all_valid_words = []
        for line in infile:
            word = line.rstrip()

            if getLengthWithoutVowels(word) >= 3:
                all_valid_words.append(word)

    with open("valid_word_without_vowels.txt", "w") as outfile_without_vowels:
        with open("valid_word_without_consonants.txt", "w") as outfile_without_consonant:
            with open("valid_word.txt", "w") as outfile:
                for word in all_valid_words:
                    outfile_without_vowels.write(strip_vowels(word) + "\n")
                    outfile_without_consonant.write(strip_consonant(word) + "\n")
                    outfile.write(word + "\n")


    print(len(all_valid_words))
    print(all_valid_words[0])

if __name__ == "__main__":
    main()