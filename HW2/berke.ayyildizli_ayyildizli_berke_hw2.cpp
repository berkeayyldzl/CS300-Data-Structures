//
//Created by Berke Ayyildizli 31018
//
//CS300_HW2

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include "Tree.h" //my main tree class

using namespace std;

struct DocumentItem { //DocumentItem struct to hold the each WordItem object's information data such as count and document names.

    string documentName;
    int count;

};

struct WordItem { //WordItem struct to hold each word in the tree, each object take a string, the actual read word, and a DocumentItem vector, holding values like count and document name.

    string word;
    vector<DocumentItem> documentList;

    WordItem() {} //default constructor

    WordItem(string newWord, string docName) : word(newWord), documentList(1){//paramaterized constructor, i needed this bc when using the tree's find function, i needed a WordItem object.

        documentList[0].documentName = docName;
        documentList[0].count = 1;
    }

};

bool find(vector<string> wordList, const string & word){ //unused find method for finding if a word is present in the given vector, later i switched to using default find, method with wordItem objects.
    for (int i=0; i < wordList.size(); i++){
        if (wordList[i] == word){
            return true;
        }
    }
    return false;
}

void correctString(string & word){ //the function to convert all characters to lowercase, if a character is not alphabetic, it makes it blank.

    for (int i = 0; i < word.length() ; ++i) {

        if(isalpha(word[i])){

            word[i] = tolower(word[i]);

        }
        else{
            word[i] = ' ';

        }
    }
}

bool operator<(const WordItem& lhs, const WordItem& rhs) { //the operator overload for the < to work in methods like find and insert
    return lhs.word < rhs.word;
}

bool operator == (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the == to work in methods like find and insert
    return lhs.word == rhs.word;
}



bool operator != (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the != to work in methods like find and insert
    return lhs.word != rhs.word;
}

int main() {

    int fileAmount;
    cout<<"Enter number of input files: ";
    cin>>fileAmount;

    vector<string> fileNames; //i am holding all the fileNames in a vector of strings.

    for (int j = 0; j < fileAmount; j++) {

        string fileName;
        cout << "Enter " << j + 1 << ". file name: ";
        cin >> fileName;
        fileNames.push_back(fileName); //pushing each fileName into the fileNames vector.

    }

    cout<<endl;

    AvlTree<WordItem> mainTree(WordItem("", {}));//forming a blank AVL tree, with parameters as blank words and blank document names, because of the constructor given by the lecture slides, this is the only way i was able to correctly make a tree.

    for (int i = 0; i < fileNames.size(); i++) { //While starting the homework, i did something like this, but since there is no input checks, it never returns unable to open file, but left it anyway.

        ifstream file(fileNames[i]);
        if (!file.is_open()) {
            cout << "Unable to open file: " << fileNames[i] << endl;
        }

        string read; //for the each word of the document

        //constructors below are for debug and test purposes, before i was able to correctly implement the tree, i tried to form it like this, leaving it anyway so that maybe later i can come back and check them out.

        //const WordItem WordItem_NOTFOUND(-1); //means that the word is nonexistent in the tree
        //AvlTree<WordItem> mainTree;
        //WordItem currentItem = NULL;

        while (file >> read) {
            correctString(read); //making each word lowercase and discarding non-alphabetic characters.

            WordItem newItem(read, fileNames[i]); //forming a new WordItem object, the word is read, and the documentName is the document we are currently on.
            WordItem foundWord = mainTree.find(WordItem{read, {}}); // found word is if the word can be found in my main tree.
            if (read != foundWord.word) { //unless it can't be found
                DocumentItem doc; //forming a new DocumentItem object for the new word.
                doc.count = 1; //making it's count 1.
                doc.documentName = fileNames[i]; //making it's document name our current document name

                newItem.documentList.push_back(doc); //pushing back to the newItem's documentList vector.

                mainTree.insert(newItem); //inserting the newItem.
            }

            else { //if the word is existent somehow

                bool DocumentIsNotExisting = false; //suppose that the word exist but on the same file that we are on.

                for (int j = 0; j < foundWord.documentList.size(); j++) {//iterating that words possible documents

                        if (foundWord.documentList[j].documentName == fileNames[i]) { //if the document of the word is the same as the document we are currently iterating
                            DocumentIsNotExisting = true; //now we know that we are on the same document
                            foundWord.documentList[j].count++; //if so we just need to update its count
                        }

                }

                if (!DocumentIsNotExisting) { //if the word exists but not on our file,

                    DocumentItem newDocumentItemOfTheWord; //we need to push another documentItem object to the WordItem, so we create another one.
                    newDocumentItemOfTheWord.documentName = fileNames[i]; //its file is the file that we have found,
                    newDocumentItemOfTheWord.count = 1; //and again its count is one
                    foundWord.documentList.push_back(newDocumentItemOfTheWord); //pushing back to foundWord

                }

                // ////////////////////////////////////////////////////////////////////////////////////////////////////
                // For this part, i need to make an explanation, with nearly 3 days of debugging in the office hour, we couldn't come up with a solution to correctly//
                // change the count and documentName of a given word, although i took all the methods such as insert and find from lecture slides, i just couldn't make it change my actual tree.//
                // For this reason, after i make all the changes to my foundWord, i just remove it and reinsert. Then the changes are implemented.  //
                // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                mainTree.remove(foundWord); // remove the existing word
                mainTree.insert(foundWord); // insert the updated word back

            }
            //this block is for debug purposes only, i wanted to show the document names and counts of all the elements.
            //WordItem a = mainTree.find(WordItem{read, {}});
            //cout << "-------" << endl;
            //for (int j = 0; j < foundWord.documentList.size(); j++) {
            //    cout << a.documentList[j].count << a.documentList[j].documentName << endl;
            //}
        }

        file.close();

    }

    //WE ARE DONE WITH THE READING PART, CONTINUING WITH THE QUERY PART

    string line;
    getline(cin, line); //taking a line of words

    while (line != "ENDOFINPUT") { //if a line is ENDOFINPUT, it ends the program no matters what

        istringstream iss(line); //creating a stringstream object
        string word;
        vector<string> quarryWords; //a vector to store each word that is taken from the query

        while (iss >> word) { //reading words one by one

            correctString(word); //making them lowercase and discarding non-alphabetic elements
            quarryWords.push_back(word);//pushing them to vector

        }

        if (!quarryWords.empty() && quarryWords[0] == "remove") { //Checks the first word of the quary inputs, if its REMOVE, it removes the next word after it. I haven't made an input check since ta's said that there won't be 2 worded remove queries.

            for (int i = 1; i < quarryWords.size(); i++) {

                string wantedWord = quarryWords[i]; //the word i want to delete
                WordItem wantedItem(wantedWord, {}); //creating a wordItem object from that word.
                mainTree.remove(wantedItem); //removing said word.
                cout << quarryWords[i] << " has been REMOVED" << endl << endl;//giving two endlines because it is wanted in the result page

            }

        }

        else { //if the line isn't ENDOFINPUT or the first word isn't remove, program comes to the output part.

            //HERE I NEED TO MAKE ANOTHER EXPLANATION
            //Again with many hours of debugging and trying to find a way, this is the best solution i can come up with. I know that when starting, i need to have some
            //kind of a count value for keeping the number of words present in every file. With this count i need to check, if it is eqaul to our query words' size, then we can say that every
            //word is indeed on that file. But since i couldn't come up with a proper way to implement it on the start of the else, i am submitting the previous version of my code. Here, since i can't really find out how can i find
            //on which files the words are from the start, i can't really move the "In document x.txt," elsewhere. If i put it on the outer loop, the problem is solved for only 1 query input, but since i know
            //there will be test cases for more than 1 query word, my output becomes something like: " In document a.txt, capitol found 2 times, In document a.txt, impoverished found 1 times ". I thought that leaving
            //it like that may lead to a bigger error than how it currently operates.

            //the operations of this loop will be explained below. Thank you.

            bool ReturnTrueIfAnyOfDocumentsContainsQueryWordsTogether = false; //this is for checking if the given query is altogether in the same file

            for (int IndexOfFile = 0; IndexOfFile < fileNames.size(); IndexOfFile++) { //a loop to check every file

                int count = 0; //this count is for checking every word in my query to see if they are on the same file that we are currently iterating

                for (int IndexOfQuarry = 0; IndexOfQuarry < quarryWords.size(); IndexOfQuarry++) { //iterating through every word in queery

                    WordItem wantedItem(quarryWords[IndexOfQuarry], {}); //again creating a wordItem object to find
                    WordItem foundItem = mainTree.find(wantedItem); //if finded, create a found item object
                    bool IswordFoundInDocument = false; //this is to check if the word finded in the tree is infact on the same document we are iterating

                    for (int DocIndexOfWord = 0; DocIndexOfWord < foundItem.documentList.size(); DocIndexOfWord++) { //this loops iterates through every instance of the found word's documents,

                        if (foundItem.documentList[DocIndexOfWord].documentName == fileNames[IndexOfFile]) { //if the document name of the word is the same as the file that we are currently checking,

                            IswordFoundInDocument = true; //it makes the bool true,
                            count++; //and also increments the count by 1, since with this, we will know if all the query words are on one document.
                            break; //if found breaks

                        }
                    }

                    if (!IswordFoundInDocument) { // if the query word is not present on that file,

                        count = 0; //resets the count so that we know we can't print that filename
                        break; // move to the next document

                    }
                }

                if (count == (quarryWords.size())) { //if the count is equal to the count of every word in the query

                    ReturnTrueIfAnyOfDocumentsContainsQueryWordsTogether = true; //change our bool

                    cout << "in document " << fileNames[IndexOfFile] << ", "; //write the document name

                    for (int i = 0; i < quarryWords.size(); i++) { //iterating through every word

                        WordItem wantedItem(quarryWords[i], {}); //again creating a wantedItem object
                        WordItem foundItem = mainTree.find(wantedItem); //finding it

                        for (int IndexOfDocument = 0; IndexOfDocument < foundItem.documentList.size(); IndexOfDocument++) {//iterating thorough every document of a word

                            if (foundItem.documentList[IndexOfDocument].documentName == fileNames[IndexOfFile]) {//if a words current document is same as our document

                                cout << wantedItem.word << " found " << foundItem.documentList[IndexOfDocument].count << " times"; //finally print out the count and name of the word

                                if (i != quarryWords.size() - 1) { //if the word isn't the last one
                                    cout << ", "; //puts a comma between
                                }
                                break;
                            }
                        }
                    }
                    cout << "." << endl; // adds a dot at the end of the sentence for each document
                }
            }

            if (!ReturnTrueIfAnyOfDocumentsContainsQueryWordsTogether) { //if the words are not on the same document together,

                cout << "No document contains the given query" << endl;

            }

            cout << endl; //for the output syntax
        }

        cout << "Enter queried words in one line: "; //for the question to be asked again,
        getline(cin, line);
    }

    return 0;
}

//below is my first approach to the question, i couldn't find a way though, leaving it in case i will come back to check before midterm

/*
       else {
           for (int i = 0; i < fileNames.size(); ++i) {
               int count = 0;
               WordItem wantedItem(quarryWords[i], {});
               WordItem foundItem = mainTree.find(wantedItem);
               for (int j = 0; j < quarryWords.size(); ++j) {
                   if (find(fileNames ,quarryWords[j])){
                       count++;
                   }
                   else{
                       break;
                   }
               }
               if (count == quarryWords.size()){
                   cout << "In document " << fileNames[i] << ", " << quarryWords[i] << " found " << foundItem.documentList[i].count << " times." << endl;
               }

           }

       }


   }
   */

