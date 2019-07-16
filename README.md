# Spell Checker Application

## Brief Description

An application built in C++ which provides the user with a suitable suggestion for a misspelled word in a custom text. It makes use of _Tries_ as the primary data structure.

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

works on the algorithm such that when a user enter any anagram it is stored in an array. Our program then, determines the length of anagram and uses the trie to store all the words of that length and less than that length in a multimap. They key of every word in the multimap is actually the alphabetical sort of entered word. Our program then, starts matching the character of entered anagram with the keys of multimap. For example, if a word ‘bed’ is stored in a multimap then its alphabetical sort is ‘bde’. Our program compares all the characters of entered anagram with the key of this word and if all the characters of the key are present in that anagram then the word associated with this key will be returned.