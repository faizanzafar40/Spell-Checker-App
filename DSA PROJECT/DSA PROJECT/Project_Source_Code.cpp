#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <conio.h>
#include <algorithm>
#include <map>
#include <time.h>
#include <windows.h>
#include <utility>

using namespace std;

//changes the text colour of console screen
void colour(){
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(outputHandle, &csbi);
	csbi.wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	csbi.wAttributes |= (FOREGROUND_RED | FOREGROUND_INTENSITY);
	SetConsoleScreenBufferInfoEx(outputHandle, &csbi);
}

//spell checking class which has a spell check function 
class SpellCheck {
private:
	string str;		// String search from the the dic class for formatting
	string temp;    // Stores Retrived word from the word list. 
	int cSeq;		// Contineous sequence of letters
	int pCseq;      // Previous Contineous Sequence of letters;
	int lMatch;     // Variable for Character Match.
	int gMatch;     // Variable for greatest character match.
	int seq;        // Variable for sequenced Character Match.
	int Gseq;       // Greatest sequenced Character Match.
	int Wlength;    // Variable for Length of Character
	vector<string>::iterator itr;
public:
	SpellCheck(){
		cSeq = 0;
		pCseq = 0;
		lMatch = 0;
		gMatch = 0;
		seq = 0;
		Gseq = 0;
	} // Default Constructor Initializes all variables to zero.	

	//spell check function which basically takes a string as parameter which is actually the word entered by the user for checking its spelling
	//The other parameter is a vector which stores all the words in a trie and then this vector is used to find words similar to entered word and
	//returns the closest match
	//this function returns a string which stores the suggested word which is similar to the word enetered by the user.
	string spell(string word,vector<string> words){ 		
		pCseq = 0;
		gMatch = 0;
		Gseq = 0;
		str.clear();
		itr = words.begin();
		while (itr!=words.end()){
			temp = *itr;
				cSeq = lMatch = seq = 0;            // Assigns All seq to 0;
				//finding out all the words in the vector whose length is less than or more than the length of entered word by a factor 2
				if (temp.length() >= word.length() - 2 && temp.length() <= word.length() + 2)  // Filters out words for faster processing
				{
					// Check if the queried word is shorter or longer then retrieved word.
					if (word.length() > temp.length()) {
						Wlength = temp.length();
					}
					else {
						Wlength = word.length();
					}
					// Start of Contineous Sequence Match
					for (int i = 0; i < Wlength; i++) {
						if (word[i] == temp[i]){
							cSeq++;
						}
						else{
							if (i < Wlength / 2){
								seq = 0;
							}
							else{
								break;
							}
						}
					}
					// End of Contineous Sequence Match

					// Start of Sequence Match
					for (int i = 0; i < Wlength; i++){
						if (word[i] == temp[i]){
							seq++;
						}
					}
					// End of Sequence Match

					// Start of Character Match.
					for (int i = 0; i < Wlength; i++){
						for (int j = 0; j < Wlength; j++){
							if (word[i] == temp[j]){
								lMatch++;
								break;
							}
						}
					}
					// End of Character Match
					if (cSeq>pCseq && seq>Gseq && lMatch > gMatch){
						pCseq = cSeq;
						lMatch = gMatch;
						Gseq = seq;
						str = temp;
					}
				}
				itr++;
			}
		return str;
	}
}; 

//node class which has the properties and functions reagarding the node of the trie
class Node{
private:
	//mcontent is the character of a word which has a separate node of its own
	char mContent;                 //single chracters of word
	bool word_end;                  //End point of Word. Marks the position where th word is ended
	vector<Node*> mChildren;       //child of each content. 
public:	
	//constructor
	Node() {
		mContent = ' ';
		word_end = false;
	}
	//returns single char of a word
	char content(){
		return mContent;
	}
	//set each characters of a word (called in a loop). All character of a word are stores in separate nodes hence this function takes a char
	//as a parameter and stores it in a node
	void setContent(char c){
		mContent = c;
	}
	//returns if the word has ended or the loop is at some character of the word
	bool wordend() {
		return word_end;
	}
	//if word has ended sets words marker true
	void setwordend(){
		word_end = true;
	}
	//finds a comeplete word character by character. This function takes each character of entered word as a parameter and compare it in the trie. 
	//if all the characters of words are matched with a wor din the trie then it returns the pointers of each character that is matched.
	Node* findChild(char c){
		for (int i = 0; i < mChildren.size(); i++)
		{
			Node* tmp = mChildren.at(i);
			if (tmp->content() == c)
			{
				return tmp;
			}
		}

		return NULL;
	}
	//pushes a child in the children vector. Takes a object pointer as a parameter and push it in a a vector
	void appendChild(Node* child){
		mChildren.push_back(child);
	}
	//returns the vector which contains the children
	vector<Node*> children(){
		return mChildren;
	}
};

//This class basically creates, load, add words, and find words in a trie
class Trie {
private:
	Node* root;                      //pointer object of class node
	SpellCheck check;                //spell check object to check spelling of a word
	multimap <string, string> map_1; //8 Letter words
	multimap <string, string> map_2; //4 Letter words
	multimap <string, string> map_3; //6 Letter words
	vector <string> words;
public:
	//contructor
	Trie(){
		root = new Node();
	}
	//add words in a trie. Takes a string as input and add it in a trie
	void addWord(string word){
		Node* current = root;
		//if word length is 0
		if (word.length() == 0)
		{
			current->setwordend(); // an empty word
			return;
		}
		//for loop till words end
		for (int i = 0; i < word.length(); i++)
		{
			//check if word exist
			Node* child = current->findChild(word[i]);
			if (child != NULL)
			{
				current = child;
			}
			//if word does nt exist
			else
			{
				Node* tmp = new Node();
				tmp->setContent(word[i]);
				current->appendChild(tmp);
				current = tmp;
			}
			//setting end of word
			if (i == word.length() - 1)
				current->setwordend();
		}
	}
	//returns true or false for if word exists or not. returns true or false depending on wether a word exists in the trie or not
	bool searchWord(string word){
		Node* current = root;
		while (current != NULL)
		{
			//for loop for words length
			for (int i = 0; i < word.length()  ; i++)
			{
				//check if word exist
				Node* tmp = current->findChild(word[i]);
				//if world is already in there return false else move on
				if (tmp == NULL){
					return false;
				}
				current = tmp;
				
			}
			//if matching word found return true else false
			if (current->wordend()){
				return true;
			}
			else{
				return false;
			}
		}
		//return false if word not found
		return false;
	}
	//find function that returns suggested or origial word. This function first calls the searchword function to see if the word is avaialable 
	// in the trie or not.. if the word is not availabke then this function calls the spell check function.
    string find(string word,vector<string>words){
		string suggestion;
		//if word found
		if (searchWord(word) == true){
			return word;
		}
		//if word not found
		if (searchWord(word) == false){		
			suggestion = check.spell(word,words);
			return suggestion;
		}
	}
	//Loads words in a trie. This function loads all the words from a file into a trie
	void Load(){
		fstream file;   //file variable
		string line;    //stores a line from file
		string s1;      //stores word from line
		string s2;      //stores s1 in alphabetical order
		int count = 0;
		int per=0;
		file.open("Words.txt", file.in);      //opens file
		if (file.is_open()){                  //if file found and opened
			while (!file.eof()){              //till file not end
				getline(file, line);          // get a line from file and store in line
				addWord(line);                //add the word in trie
				count++;
				if (count % 1000 == 0){
					if (per <= 100){
						cout << "\r" << per++ << "% LOADED";
					}
				}
				words.push_back(line);
				//if line length is less than 8
				if (line.length() <= 8) {
					//storing line in s1
					s1.assign(line);
					//sorting s1
					sort(line.begin(), line.end());//The words are sorted here
					//storing sorted line in s2
					s2.assign(line);
					//insert all 8 and less than 8 letter words in map1
					map_1.insert(pair<string, string>(s2, s1));
					if (line.length() <= 4) {
						//insert all 4 and less than 4 letter words in map1
						map_2.insert(pair<string, string>(s2, s1));
					}
					if (line.length() <= 6) {
						//insert all 6 and less than 6 letter words in map1
						map_3.insert(pair<string, string>(s2, s1));
					}
				}
			}
		}
		file.close();
		//file closed
	}
	//returns map that contains 8 and less than 8 letter words
	multimap<string, string> returnmap1(){
		return map_1;
	}
	//returns map that contains 4 and less than 4 letter words
	multimap<string, string> returnmap2(){
		return map_2;
	}
	//returns map that contains 6 and less than 6 letter words
	multimap<string, string> returnmap3(){
		return map_3;
	}
	//returns vector including all the words in a trie;
	vector<string> returnwords(){
		return words;
	}
};

//This class solves all the anagram consisting of 4,6 and 8 letters. This class also consists of a game in which user have to enter all possible
//words that can be made with the given anagram
class Anagram{
private:	
	int timer, y;         
	vector <char> v1;
	vector <char> v2;
	vector<string>str;             //Vector for storing all 8 letter words
public:
	Anagram(){
		timer = 0;
		y = 0;
	}
	//return a random value
	int Random() {
		return (rand() % 26 + 1);
	}
	//anagram solving game. This games takes a map as an input which has all the words with lengths less than equal to 8. The user than guesses 
	//the words that can be made with the generated anagram
	void Game(multimap <string, string> map1){
		int count;
		string line;  //stores line from file
		vector <int> v(8);   //stores random integers from random function
		while (str.size()<1) {
			for (int i = 0; i < 8; i++) {
				v[i] = Random();
			}
			//creating a random anagram
			for (int i = 0; i < 8; i++) {
				v1.push_back('a' + (v[i] - 1));
				v2.push_back(v1[i]);
			} 
			string nam(v1.begin(), v1.end());
			sort(nam.begin(), nam.end());
			sort(v1.begin(), v1.end());
			sort(v.begin(), v.end());
			y = 0;
			for (auto it = map1.begin(); it != map1.end(); ++it) {
				char *buffer = new char[it->first.length() + 1];
				it->first.copy(buffer, it->first.length());
				buffer[it->first.length() + 1] = '\0';
				if (y > 0) {
					v2.clear();
					for (int i = 0; i < 8; i++) {
						v2.push_back(v1[i]);
					}
				}
				y = 0;
				count = 0;
				if (nam.length() >= it->first.length()) {
					while (count != 1) {
						for (int j = 0; j < v2.size(); j++) {
							if (buffer[y] == v2[j]) { //check if a match iss found in the vector
								y++;
								v2.erase(v2.begin() + (j));
								break;
							}
							else {
								if (j == (v2.size() - 1)) {
									count = 1;
									break;
								}
							}
							if (y == (it->first.length())) {
								count = 1;
							}
						}
					}
					if (y == (it->first.length())) {
						str.push_back(it->second);     //stores answer in str
					}
				}
			}
		}
		y = 0;
		count = 0;
		vector<double>f1;
		int tempc = 0;//temporary counter//
		vector<string>s1;
		double temp;
		vector<int>i1;
		int i = 0;
		//open high scores files
		ifstream in("High Scores.txt");
		if (!in) {
			cout << "Could not open File!\n";
		}
		//storing name scores and time
		while (!in.eof()) {
			in >> line;
			s1.push_back(line);
			in >> temp;
			i1.push_back(temp);
			in >> temp;
			f1.push_back(temp);
			i++;
			tempc++;
			if (in.eof()) {
				i1.erase(i1.begin() + (tempc - 1));
				s1.erase(s1.begin() + (tempc - 1));
				f1.erase(f1.begin() + (tempc - 1));
				break;
				tempc--;
			}
			if (i1[tempc - 1] < 0) {
				tempc = 0;
				i1.erase(i1.begin());
				s1.erase(s1.begin());
				f1.erase(f1.begin());
				break;
			}
		}
		in.close();
		//game
		cout << "The Purpose Of This Game Is For You To Guess As Many Words As Possible\nPoints Are Assigned As Follows:\n8 For 8 Letter Words\n7 For 7 Letter Words And So On\nIf Your Score Ties With The High Score, The Person With The Best Time Wins!\n";
		cout << "You Can Also Enter 'EXIT!' If You Want To Quit At Any Time\n";
		if (s1.size() >= 1) {
			cout << "\n\t\t\t\tPrevious High Scores\n";
			cout << "\t\t\t\tNAME\tSCORE\tTIME\n\t\t\t\t\n";
			int tempc2 = 0;
			//printing top 3 positions
			if (s1.size() < 3) {
				tempc2 = s1.size();
			}
			else {
				tempc2 = 3;
			}
			for (int i = 0; i < tempc2; i++) {
				cout << "\t\t\t\t" << s1[i] << "\t" << i1[i] << "\t" << f1[i];
				cout << "\n";
			}
		}
		cout << "\n\n\t\t\t\t";
		for (int i = 0; i < 10; i++) {
			cout << "-";
		}
		cout << "\n\t\t\t\t|ANAGRAM |\n\t\t\t\t";
		for (int i = 0; i < 8; i++) {
			if (i == 0) {
				cout << "|";
			}
			cout << v1[i];
		}
		cout << "|\n\t\t\t\t";
		for (int i = 0; i < 10; i++) {
			cout << "-";
		}
		int break1 = 0;
		cout << endl;
		int *score = new int[str.size()];
		string str2 = "exit!";
		vector<string>str3;
		string enter = "RED";//Stores user input//
		int counter = 0;
		timer = clock(); // start counting time
		while (enter != str2 && counter != str.size()) {
			cin >> enter;
			//enter a string
			transform(enter.begin(), enter.end(), enter.begin(), tolower);
			for (int i = 0; i < str.size(); i++) {
				if (break1 == 1) {
					break;
				}
				for (int i = 0; i < str3.size(); i++) {
					//if already found
					if (enter.compare(str3[i]) == 0) {
						cout << "\n\n\t\t\t\tWord Has Already Been Guessed!\n";
						break1 = 1;
						break;
					}
				}
				//match found
				if (enter.compare(str[i]) == 0) {
					cout << "\n\n\t\t\t\tMATCH FOUND!" << endl;
					score[counter] = str[i].length();
					str3.push_back(str[i]);
					counter++;
					break;
				}
				//wrong answer
				else {
					if (i == (str.size() - 1) && enter != str[i] && enter != str2) {
						cout << "\n\n\t\t\t\tWrong Answer!\n\t\t\t\tTry again!\n";
						break;
					}
				}
			}
			break1 = 0;
		}
		//calculate sum
		int sum = 0;
		for (int i = 0; i < counter; i++) {
			sum += score[i];
		}
		//calculate time
		timer = clock() - timer;
		double count3 = timer / 1000;
		if (counter != str.size()) {
			cout << "\n\t\t\t\tYour Score Is " << sum << endl;
			cout << "\t\t\t\tTime Elapsed: " << count3 << endl;
		}
		//if all words guessed
		if (counter == str.size()) {
			cout << "\n\n\t\t\t\tCONGRATULATIONS!\n\t\t\t\tAll Words Were Guessed!\n\t\t\t\tYour Score Is " << sum << endl;
		}
		vector<string>::iterator sp = s1.begin();//to place the name at a particular position in a vector
		vector<int>::iterator ip = i1.begin();
		vector<double>::iterator fp = f1.begin();
			for (i = 0; i < s1.size(); i++) {
				if (sum>i1[i]) {
					if (sum > i1[0]) {
						system("cls");
						//new high score
						cout << "\t\t\t\tNEW HIGH SCORE!\n";
						Sleep(1000);
						cout << "Please Enter Your Name:\n";
						cin >> enter;
						sp += (i);            //name iterator increased
						ip += (i);            //score iterator increasd
						fp += (i);            //tim iterator increased
						s1.insert(sp, enter);  //stores new name
						i1.insert(ip, sum);    //stores new score
						f1.insert(fp, count3); //stores new time
						break;
					}
					//if u placed above someone
					cout << "You Placed Above " << s1[i] << "!\nYour Position Is: " << i + 1 << endl;
					cout << "Please Enter Your Name:\n";
					cin >> enter;
					sp += (i);            //name iterator increased
					ip += (i);            //score iterator increasd
					fp += (i);            //tim iterator increased
					s1.insert(sp, 1, enter);  //stores new name
					i1.insert(ip, 1, sum);    //stores new score
					f1.insert(fp, 1, count3); //stores new time
					break;
				}
				else {
					if (i == (s1.size() - 1)) {
						//showing if u placed last
						cout << "You placed " << s1.size() + 1 << "!" << endl;
						cout << "Please Enter Your Name:\n";
						cin >> enter;
						sp += (i);            //name iterator increased
						ip += (i);            //score iterator increasd
						fp += (i);            //tim iterator increased
						s1.insert(sp, 1, enter);  //stores new name
						i1.insert(ip, 1, sum);    //stores new score
						f1.insert(fp, 1, count3); //stores new time
						break;
					}
				}
				if (sum == i1[i]) {
					if (timer < f1[i]) {
						//if two persons score is same but your time was better
						cout << "You And " << s1[i] << " Tied But Since Your Time Was Better, You Replace Him In That Position i.e." << i + 1 << endl;
						cout << "Please Enter Your Name:\n";
						cin >> enter;
						sp += (i);            //name iterator increased
						ip += (i);            //score iterator increasd
						fp += (i);            //tim iterator increased
						s1.insert(sp, 1, enter);  //stores new name
						i1.insert(ip, 1, sum);    //stores new score
						f1.insert(fp, 1, count3); //stores new time
						break;
					}
				}
				if (sum == i1[i] && sum != i1[i + 1] && i + 1<s1.size()) {
					if (timer > f1[i]) {
						//if two person have score but his/her time was better
						cout << "You And " << s1[i] << " Tied But His Completion Time Was Faster!\nYou Placed " << i + 1 << endl;
						cout << "Please Enter Your Name:\n";
						cin >> enter;
						sp += (i);            //name iterator increased
						ip += (i);            //score iterator increasd
						fp += (i);            //tim iterator increased
						s1.insert(sp, enter);  //stores new name
						i1.insert(ip, sum);    //stores new score
						f1.insert(fp, count3); //stores new time
						break;
					}
				}
			}
		string random1;
		int random2;
		double random3;
		if (s1.size() == 0) {
			//if no entries in s1
			cout << "No Entries Previously Existed!\n" << endl;
			cout << "Please Enter Your Name To Create A New Entry!\n";
			cin >> enter;
			sp += (i);            //name iterator increased
			ip += (i);            //score iterator increasd
			fp += (i);            //tim iterator increased
			s1.insert(sp, 1, enter);  //stores new name
			i1.insert(ip, 1, sum);    //stores new score
			f1.insert(fp, 1, count3); //stores new time
		}
		for (int u = 1; u < s1.size(); u++) {
			for (int i = 0; i < s1.size(); i++) {
				if ((i + 1) < s1.size()) {
					if (i1[i] < i1[i + 1]) {
						random1 = s1[i + 1];
						s1[i + 1] = s1[i];
						s1[i] = random1;
						random2 = i1[i + 1];
						i1[i + 1] = i1[i];
						i1[i] = random2;
						random3 = f1[i + 1];
						f1[i + 1] = f1[i];
						f1[i] = random3;
					}
					if (i1[i] == i1[i + 1] && f1[i] > f1[i + 1]) {
						random1 = s1[i + 1];
						s1[i + 1] = s1[i];
						s1[i] = random1;
						random2 = i1[i + 1];
						i1[i + 1] = i1[i];
						i1[i] = random2;
						random3 = f1[i + 1];
						f1[i + 1] = f1[i];
						f1[i] = random3;
					}
				}
			}
		}
		ofstream out;
		out.open("High Scores.txt");
		int tempc1 = 0;
		cout << "\t\t\t\tNAME\tSCORE\tTIME\n\t\t\t\t\n";
		while (tempc1 < 10 && tempc1<s1.size()) {     //printing all the high scores
			out << s1[tempc1] << " " << i1[tempc1] << " " << f1[tempc1] << endl;
			cout << "\t\t\t    " << tempc1 + 1 << ")" << s1[tempc1] << "\t" << i1[tempc1] << "\t" << f1[tempc1] << endl;
			tempc1++;
		}
		out.close();
		//if all words weren't guessed
		if (counter != str.size()) {
			cout << "All Possible Words Are:" << endl;
		}
	}
	//4 letter anagram solver. This function takes a map as parameter and which contains all the words with length less than equal to 4. The user
	//enter an anagram and and this function solves it
	vector <string> Solve4(char escape, multimap <string, string> map2){
		int check = 0;    //check if entered anagram contains 4 letters
		int count;  //counter
		string ana4;    //user unput variable
		cout << "\n\n\n\n\n\n\n\t\t\t\tREADY FOR ENTRY!\n";
		Sleep(1000);
		system("cls");
		cout << "Enter A 4 Letter Anagram.\n";
		//take user input
		while (check != 1) {
			cin >> ana4;
			if (ana4.length() > 4 || ana4.length() < 4) {
				cout << "Error Invalid Length!\nPlease Try Again:\n";
				check = 0;
			}
			else {
				check = 1;
			}
		}
		//transform to lower case
		transform(ana4.begin(), ana4.end(), ana4.begin(), tolower);//transforms to lowercase
		vector<char>u1(ana4.begin(), ana4.end());//vector to store user entries character by character//
		v2.clear();
		for (int i = 0; i < 4; i++) {
			v2.push_back(u1[i]);
		}
		sort(u1.begin(), u1.end());
		escape = u1[3] + 1;
		for (auto it = map2.begin(); it != map2.end(); ++it) {
			char *buffer = new char[it->first.length() + 1];    //converting to character pointer
			it->first.copy(buffer, it->first.length());
			buffer[it->first.length() + 1] = '\0';
			if (y > 0) {
				v2.clear();
				for (int i = 0; i < 4; i++) {
					v2.push_back(u1[i]);  //pushing u1 to v2
				}
			}
			y = 0;
			count = 0;
			if (escape == buffer[0]) {
				break;
			}
			if (ana4.length() >= it->first.length()) {
				while (count != 1) {
					for (int j = 0; j < v2.size(); j++) {
						if (buffer[y] == v2[j]) { //look for character match
							y++;
							v2.erase(v2.begin() + (j));
							break;
						}
						else {
							if (j == (v2.size() - 1)) {  //if found for length less than that of v2 by 1
								count = 1;
								break;
							}
						}
					}
					if (y == (it->first.length())) {  //if found for complete word length
						count = 1;
						break;
					}
				}
				if (y == (it->first.length())) {   //push answer
					str.push_back(it->second);
				}
			}
		}
		y = 0;
		count = 0;
		return str; //return answer
	}
	//function to solve anagram containing 6 letters. This function takes a map as parameter and which contains all the words with length less than equal to 6. The user
	//enter an anagram and and this function solves it
	vector <string> Solve6(char escape, multimap <string, string> map3){
		int check = 0;    //check user input
		int count;
		string ana6;    //stores user input
		cout << "\n\n\n\n\n\n\n\t\t\t\tREADY FOR ENTRY!\n";
		Sleep(1000);
		system("cls");
		cout << "Enter A 6 Letter Anagram.\n";
		//taking user input
		while (check != 1) {
			cin >> ana6;
			if (ana6.length() > 6 || ana6.length() < 6) {
				cout << "Error Invalid Length!\nPlease Try Again:\n";
				check = 0;
			}
			else {
				check = 1;
			}
		}
		transform(ana6.begin(), ana6.end(), ana6.begin(), tolower);//transforms to lowercase
		vector<char>u1(ana6.begin(), ana6.end());//vector to store user entries character by character//
		v2.clear();
		for (int i = 0; i < 6; i++) {     //pushing u1 into v2
			v2.push_back(u1[i]);
		}
		sort(u1.begin(), u1.end());
		escape = u1[5] + 1;
		for (auto it = map3.begin(); it != map3.end(); ++it) {
			char *buffer = new char[it->first.length() + 1];     //converting into character pointer
			it->first.copy(buffer, it->first.length());
			buffer[it->first.length() + 1] = '\0';
			if (y > 0) {
				v2.clear();
				for (int i = 0; i < 6; i++) {
					v2.push_back(u1[i]);   //pushin u1 to v2
				}
			}
			y = 0;
			count = 0;
			if (escape == buffer[0]) {
				break;
			}
			if (ana6.length() >= it->first.length()) {
				while (count != 1) {
					for (int j = 0; j < v2.size(); j++) {
						if (buffer[y] == v2[j]) { //check for character match//
							y++;
							v2.erase(v2.begin() + (j));
							break;
						}
						else {     
							if (j == (v2.size() - 1)) {    //if found for length less than that of v2 by 1
								count = 1;
								break;
							}
						}
					}
					if (y == (it->first.length())) {  //if found for complete word length
						count = 1;
						break;
					}
				}
				if (y == (it->first.length())) {  //push answer
					str.push_back(it->second);
				}
			}
		}
		y = 0;
		count = 0;
		return str;  //return answer
	}
	//function to solve anagram containing 8 letters. This function takes a map as parameter and which contains all the words with length less than equal to 8. The user
	//enter an anagram and and this function solves it
	vector <string> Solve8(char escape, multimap <string, string> map1){
		int check = 0;  //check user input
		int count;     //counter
		string ana8;    //takes user input
		cout << "\n\n\n\n\n\n\n\t\t\t\tREADY FOR ENTRY!\n";
		Sleep(1000);
		system("cls");
		cout << "Enter A 8 Letter Anagram.\n";
		//taking user input
		while (check != 1) {
			cin >> ana8;
			if (ana8.length() > 8 || ana8.length() < 8) {
				cout << "Error Invalid Length!\nPlease Try Again:\n";
				check = 0;
			}
			else {
				check = 1;
			}
		}
		transform(ana8.begin(), ana8.end(), ana8.begin(), tolower);//transforms to lowercase
		vector<char>u1(ana8.begin(), ana8.end());//vector to store use entries character by character//
		v2.clear();
		for (int i = 0; i < 8; i++) {
			v2.push_back(u1[i]);
		}
		sort(u1.begin(), u1.end());
		escape = u1[7] + 1;
		for (auto it = map1.begin(); it != map1.end(); ++it) {
			char *buffer = new char[it->first.length() + 1];  //converting into char pointer
			it->first.copy(buffer, it->first.length());
			buffer[it->first.length() + 1] = '\0';
			if (y > 0) {
				v2.clear();
				for (int i = 0; i < 8; i++) {
					v2.push_back(u1[i]);   //pushing u1 in v2
				}
			}
			y = 0;
			count = 0;
			if (escape == buffer[0]) {
				break;
			}
			if (ana8.length() >= it->first.length()) {
				while (count != 1) {
					for (int j = 0; j < v2.size(); j++) {
						if (buffer[y] == v2[j]) { //check for character match//
							y++;
							v2.erase(v2.begin() + (j));
							break;
						}
						else {  
							if (j == (v2.size() - 1)) {   //if found for length less than that of v2 by 1
								count = 1;
								break;
							}
						}
						if (y == (it->first.length())) {  //if found for complete word match
							count = 1;
							break;
						}
					}
				}
				if (y == (it->first.length())) {
					str.push_back(it->second);   //pushes answer
				}
			}
		}
		y = 0;
		count = 0;
		return str;  //return answer
	}
	//function to generate and autosolve. This functions automatically generates an 8 ltter anagram and solves it automatically.
	vector <string> AutoSolve(string nam, char escape, multimap <string, string> map1){
		int count;
		cout << "\n\n\n\t\t\t\t";
		for (int i = 0; i < 10; i++) {
			cout << "-";
		}
		cout << "\n\t\t\t\t|ANAGRAM |\n\t\t\t\t";
		for (int i = 0; i < 8; i++) {
			if (i == 0) {
				cout << "|";
			}
			cout << v1[i];
		}
		cout << "|\n\t\t\t\t";
		for (int i = 0; i < 10; i++) {
			cout << "-";
		}
		for (auto it = map1.begin(); it != map1.end(); ++it) {
			char *buffer = new char[it->first.length() + 1];    //converting into character pointer 
			it->first.copy(buffer, it->first.length());
			buffer[it->first.length() + 1] = '\0';
			if (y > 0) {
				v2.clear();
				for (int i = 0; i < 8; i++) {
					v2.push_back(v1[i]);    //pushing v1 in v2
				}
			}
			y = 0;
			count = 0;
			if (escape == buffer[0]) {
				break;
			}
			if (nam.length() >= it->first.length()) {
				while (count != 1) {
					for (int j = 0; j < v2.size(); j++) {
						if (buffer[y] == v2[j]) { //check for character match//
							y++;
							v2.erase(v2.begin() + (j));
							break;
						}
						else {  //if j is less than length of v2 by 1
							if (j == (v2.size() - 1)) {
								count = 1;
								break;
							}
						}
					}      //if found for complete length
					if (y == (it->first.length())) {
						count = 1;
						break;
					}
				}
				if (y == (it->first.length())) {
					str.push_back(it->second); //push answer
				}
			}
		}
		return str;
	}
	//function to solve anagram. This function have 3 parameters which has all the words with letters of length 4, 6 or 8 and less.
	//this function calls the other functions which then solves the anagram
	void Solve(multimap <string, string> map1, multimap <string, string> map2, multimap <string, string> map3){
		srand(time(NULL));
		vector <int> v(8);
		int corr = 0;
		int option = 0;
		//taking user input
		while (option != 6) {
			cout << "Welcome To Anagram Solver And Anagram Solving Game.\nHere You Can Solve Anagrams That Contains 4, 6 Or 8 Letters.\n";
			cout << "You Can Also Play Anagram Solving Game Where You Will Have To Guess As Many\nWords As You Can That Can Be Made Using The Given Anagram.\n";
			cout << "You Can Also Ask The CPU To Randomly Generate An Anagram And Solve It\nAutomatically.\n";
			cout << "\n\n";
			
			cout << "****************************|ANAGRAM SOLVER & GAME|****************************\n";
			
			cout << endl << " 1) Enter a 8 Letter Anagram" << endl << " 2) Enter a 6 Letter Anagram" << endl << " 3) Enter a 4 Letter Anagram" << endl << " 4) Play A Game to Solve 8 letter Anagram" << endl << " 5) Automatically Generate and Solve 8 Letter Anagram" << endl << " 6) Exit Anagram Solver" << endl;
			while (corr != 1) {
				cin >> option;
				if (option > 6 || option < 1) {
					cout << "Error Invalid Entry!\nPlease Try Again:\n";
					corr = 0;
				}
				else {
					corr = 1;
				}
			}
			system("cls");
			timer = clock();//Acts as a stopwatch//
			for (int i = 0; i < 8; i++) {
				v[i] = Random();
			}//stores random integer
			for (int i = 0; i < 8; i++) {
				v1.push_back('a' + (v[i] - 1));
				v2.push_back(v1[i]);
			}
			//Creating an Anagram
			string nam(v1.begin(), v1.end());
			sort(nam.begin(), nam.end());
			sort(v1.begin(), v1.end());
			sort(v.begin(), v.end());
			char escape = 'a' + v[7];
			y = 0;
			string line2;

			/////////////////////////////////////////
			if (option == 1) {     //call function to solve 8 letter anagram
				str = Solve8(escape, map1);
			}
			if (option == 2) {     //call function to solve 6 letter anagram
				str = Solve6(escape, map3);
			}
			if (option == 3) {     //call function to solve 4 letter anagram
				str = Solve4(escape, map2);
			}
			if (option == 4) {     //call function to play game
				Game(map1);
			}
			if (option == 5) {     //call function to auto generate and solve
				str = AutoSolve(nam, escape, map1);
			}
			if (option == 6) {   //exit
				break;
			}
			//////////////////////////////////////////
			//storing answer in str
			for (int u = 1; u < str.size(); u++) {
				for (int i = 0; i < str.size(); i++) {
					if ((i + 1) < str.size()) {
						if (str[i].length() > str[i + 1].length()) {
							string tmp;
							tmp = str[i + 1];
							str[i + 1] = str[i];
							str[i] = tmp;
						}
					}
				}
			}
			y = 0;
			timer = clock() - timer;
			int countx = 0;
			int e = 1;
			//printing answers
			while (countx != 1 && e < str.size()){
				//printing all two letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 1 && str[e].length() < 3) {
						cout << "\nAll 2 Letter words:\n";
						countx = 1;
					}
					while (e < str.size() && str[e].length() > 1 && str[e].length() < 3) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 3 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 2 && str[e].length() < 4) {
						countx = 1;
						cout << "\nAll 3 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 2 && str[e].length() < 4) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 4 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 3 && str[e].length() < 3) {

						countx = 1;
						cout << "\nAll 4 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 3 && str[e].length() < 5) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 5 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 4 && str[e].length() < 6) {
						countx = 1;
						cout << "\nAll 5 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 4 && str[e].length() < 6) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 6 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 5 && str[e].length() < 7) {
						countx = 1;
						cout << "\nAll 6 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 5 && str[e].length() < 7) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 7 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 6 && str[e].length() < 8) {
						countx = 1;
						cout << "\nAll 7 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 6 && str[e].length() < 8) {
						cout << str[e] << "\t";
						e++;
					}
				}
				//printing 8 letter words
				if (e < str.size()) {
					if (e < str.size() && str[e].length() > 7 && str[e].length() < 9) {
						countx = 1;
						cout << "\nAll 8 Letter Words:\n";
					}
					while (e < str.size() && str[e].length() > 7 && str[e].length() < 9) {
						cout << str[e] << "\t";
						e++;
					}
				}
				e++;
			}
			//printing time taken
			if (option != 3 || option != 4) {
				timer = timer / 1000;
				float f = timer % 1000;
				cout << "\n\n\n\t\tTime Elapsed " << timer << ":" << f << endl;
			}
			e = 1;
			countx = 0;
			str.clear();
			corr = 0;
			string stop;
			cout << "Enter Any Key To Exit\n";
			cin >> stop;
			system("cls");
		}
	}
};

//main function contains the main interface and calls the function according to users option
int main(){	
	string stop;
	string word1;
	string wordf;
	int option = 0;
	int correct = 0;
	char c;
	Anagram A;
	Trie* trie = new Trie();
	cout << "****************Welcome To Our Spell Checker And Anagram Solver****************\n\n\nPlease Wait For A While\nLOADING..." << endl << endl;
	trie->Load();
	cout << endl << "LOADED SUCCESFULLY!" << endl;
	Sleep(500);
	system("cls");
	while (option != 3){
		c = NULL;
		colour();
		cout << "Hello :) \n\nWelcome To Our World Of Letters Where You Can Check Spellings Of Words That You Use Everyday In Your Life." << endl;
		cout << "Here You Can Also Solve Any Anagram That Contains 4, 6 Or 8 Letters." << endl;
		cout << "Also There is a little ""Word Game"" which we hope you'll definitely enjoy" << endl << endl;
		cout << "\n\n********************************** MAIN MENU **********************************\n\n";
		cout << "Please Enter Your Choice\n 1) Spell Checker \n 2) Anagram Solver & Anagram Solving Game \n 3) Exit" << endl;
		//Taking user Input
		while (correct != 1) {
			cin >> option;
			if (option > 3 || option < 1) {
				cout << "Error: Invalid entry!\nPlease try again.\n";
				correct = 0;
			}
			else {
				correct = 1;
			}
		}
		system("cls");
		if (option == 1){
			cout << "*********************************SPELL CHECKER*********************************\n\n";
			cout << "\nEnter A Word To Check. " << endl;
			cin >> wordf;
			//converting to lowercase
			transform(wordf.begin(), wordf.end(), wordf.begin(), tolower);
			word1 = trie->find(wordf,trie->returnwords());
			if (word1 == wordf){
				cout << "\nYou Entered Correct Spelling" << endl;
			}
			else{
				transform(word1.begin(), word1.end(), word1.begin(), toupper);
				cout << "You Entered " << "'" << wordf << "'" << " But I Think You Wrongly Entered Spelling Of  " << endl << "'" << word1 << "'";
				cout << endl;
			}
		}
		if (option == 2){	
			//calling anagram solver
			A.Solve(trie->returnmap1(),trie->returnmap2(),trie->returnmap3());
		}
		if (option == 3){
			cout << "Are You Sure You Really Want To Quit?\n\nEnter Y To Exit.\nEnter N To Go Back To Main Menu." << endl;
			cin >> c;

			if (c == 'Y' || c == 'y'){
				break;
			}

			if (c == 'N' || c == 'n'){
				option=0;
			}
		}
		correct = 0;
		if (c != 'Y' || c != 'y'){
			cout << "Enter Any Key To Go Back To Main Menu" << endl;
			cin >> stop;
		}
		system("cls");
		option = 0;
	}
	delete trie;
}