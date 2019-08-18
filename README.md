# Spell Checker Application

## Brief Description

A C++ application which provides the user with a suitable suggestion for a misspelled word in a custom text. It makes use of _Tries_ as the primary data structure.

## Project Details

- This project is basically a spell checker application that helps the user in identifying the correct spellings of a word by giving a suitable (closely-matched) suggestion.

- For example, if a user enters the word 'staike' or 'stakei', the program provides suggestions of appropriate words i.e. 'stack' in this case.

- An additional module of this application is the **Anagram solver**, which allows the user to enter random letters and the program returns a list of all the possible words that can be created from them.

- For the anagram solver, user is asked to input 4, 6 or 8 letter words.

- Consequently, in the second part of this 'game', the program produces random letters and the user has to create all possible words from them.

- A score and time elapsed measure is calculated for each user and stored for retrieval.

## Implementation Details

### Spell Checker Application

- The application uses Tries to store all the words from a file

- Time complexity of tries is O (n*m) and it is best suited to the desired use case.

- This algorithm works such that when a user enters a word, it determines  the length of that word and then extracts all the words fulfilling a certain condition from the main trie and stores them into a vector. The condition being that length must be less than and greater than that of the input word by 2.

- Then the program finds the closest match available to that word available in the vector.

### Anagram Solver

- The anagram that the user inputs is stored in an array.

- The algorithm determines the length of the anagram and uses the trie to store all the words of that length and less in a multimap.

- The key of each word in the multimap is actually the alphabetical sort of the input word.

- The application then matches each character of the input anagram with the keys of the multimap.

- For example, if the word ‘bed’ is stored in a multimap then its alphabetical sort is ‘bde’.

- If all the characters of a particular key are present in the input anagram, then the word associated with this key is returned.
