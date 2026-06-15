# Spell Checker & Anagram Solver

A C++ console application that suggests a correction for a misspelled word and
solves (and plays games with) anagrams. It is built around a **trie** as the
primary data structure.

I wrote this as my data-structures course project to get hands-on with tries
and to see how a simple matching heuristic can stand in for a "did you mean…?"
suggestion. It loads a ~109,000-word English dictionary into a trie at startup
and uses it both to look words up and to power the anagram features.

## Tech stack

- **Language:** C++ (C++11)
- **Core data structures:** trie (dictionary + lookups), `std::multimap` keyed
  by sorted letters (anagram buckets), `std::vector`
- **Platform:** Windows — the coloured prompts, timing and screen clearing use
  the Win32 console API (`<windows.h>`)
- **Build:** CMake (also opens directly as a folder in Visual Studio)

## Features

- **Spell checker.** Enter a word; if it is in the dictionary I confirm it,
  otherwise I suggest the closest match. For example, `staike` or `stakei`
  returns a suggestion like `stack`. I only compare against words within two
  letters of the input length, then rank candidates by their leading run of
  matching letters, their in-place matches, and their shared letters.
- **Anagram solver.** Enter a 4, 6 or 8 letter anagram and I list every word
  that can be formed from those letters, grouped by length.
- **Auto-solve.** I generate a random 8-letter anagram and solve it for you.
- **Anagram game.** I generate a random 8-letter anagram and you guess as many
  valid words as you can against the clock. Scores (8 points for an 8-letter
  word, 7 for a 7-letter word, and so on) and completion times are saved to a
  high-score table, with ties broken by the faster time.

## Sample output

The main menu greets you after the dictionary loads:

```
****************Welcome To Our Spell Checker And Anagram Solver****************

********************************** MAIN MENU **********************************

Please Enter Your Choice
 1) Spell Checker
 2) Anagram Solver & Anagram Solving Game
 3) Exit
```

Choosing the spell checker and entering a misspelling reports the suggestion:

```
*********************************SPELL CHECKER*********************************

Enter A Word To Check.
staike
You Entered 'staike' But I Think You Wrongly Entered Spelling Of
'STACK'
```

## Prerequisites

- A C++ compiler with C++11 support (the project originally targeted the
  Visual Studio 2013 toolset)
- Windows (the program uses the Win32 console API)
- CMake 3.15 or newer

## Build and run

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the resulting executable **from the project root** so it can find the
dictionary and high-score files under `data/`:

```bash
./build/spell-checker        # path may be ./build/Debug/spell-checker.exe with MSVC
```

You can also open the folder directly in Visual Studio (File → Open → Folder),
which picks up `CMakeLists.txt` automatically.

## Project structure

```
.
├── CMakeLists.txt        # build definition
├── LICENSE               # MIT license
├── README.md
├── data/
│   ├── words.txt         # the dictionary loaded into the trie
│   └── high_scores.txt   # anagram-game high scores (read and updated at runtime)
└── src/
    └── main.cpp          # the whole program: trie, spell checker, anagram solver & game
```

## How it works

- **Spell checker.** When you enter a word I determine its length, pull every
  dictionary word within ±2 of that length, and score each one. The score
  combines the leading run of matching characters, the number of characters
  that match in place, and the number of shared characters; the best match is
  returned. The complexity of trie operations is roughly O(n·m) in the number
  and length of words, which fits this use case well.
- **Anagram solver.** While loading the dictionary I bucket the short words
  into multimaps keyed by their letters sorted alphabetically (so `bed` is
  stored under `bde`). To solve an anagram I sort its letters and match them
  against those keys; when every letter of a key is present in the input, the
  word behind that key is a valid answer.

## What I learned

This was where tries clicked for me — storing the dictionary character by
character makes membership tests cheap, and reusing the same structure (plus a
sorted-letter index) for anagrams showed me how the right data layout can make
a second feature almost fall out of the first. The matching heuristic for
suggestions is deliberately simple, and tightening it would be a good next step.

## License

This project is released under the [MIT License](LICENSE).
