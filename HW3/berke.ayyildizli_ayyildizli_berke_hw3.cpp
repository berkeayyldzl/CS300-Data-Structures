//
//Created by Berke Ayyildizli 31018
//
// sucourse user name berke.ayyildizli
//
// berke.ayyildizli_ayyildizli_berke_hw3.cpp
//
//CS300_HW3

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <chrono>
#include "Tree.h" //my main tree class
#include "Hash.h" //my main hash table class

using namespace std;



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

void correctString(string &theWord) { //function to delete non-alphabetical and make all characters lowercase
    for (int i = 0; i < theWord.length(); i++) {
        if (isalpha(theWord[i])) {
            theWord[i] = tolower(theWord[i]); // converts uppercase to lowercase
        } else {
            theWord[i] = ' '; // Treat non-alphabetic characters as separators
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
        cin.ignore();
        fileNames.push_back(fileName); //pushing each fileName into the fileNames vector.

    }



    //binary search tree part

    AvlTree<WordItem> mainTree(WordItem("", {}));//forming a blank bst tree, with parameters as blank words and blank document names, because of the constructor given by the lecture slides, this is the only way i was able to correctly make a tree.

    for (int i = 0; i < fileNames.size(); i++) { //While starting the homework, i did something like this, but since there is no input checks, it never returns unable to open file, but left it anyway.

        ifstream file(fileNames[i]);
        if (!file.is_open()) {
            cout << "Unable to open file: " << fileNames[i] << endl;
        }
        string line1; //for the each word of the document
        string word1;
        while (getline(file, line1)) {
            correctString(line1); //making each word lowercase and discarding non-alphabetic characters.
            istringstream ss(line1);
            while (ss>>word1) {

                WordItem newItem(word1,fileNames[i]); //forming a new WordItem object, the word is word1, and the documentName is the document we are currently on.
                WordItem foundWord = mainTree.find(WordItem{word1, {}}); // found word is if the word1 can be found in my main tree.
                if (word1 != foundWord.word) { //unless it can't be found
                    DocumentItem doc; //forming a new DocumentItem object for the new word.
                    doc.count = 1; //making its count 1.
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

                    mainTree.remove(foundWord); // remove the existing word
                    mainTree.insert(foundWord); // insert the updated word back

                }
            }

        }

        file.close();

    }

    //Hash table part

    HashTable<WordItem> myTable(WordItem("",{})); //constructing the hash table

    for (int i = 0; i < fileNames.size(); i++) { //While starting the homework, i did something like this, but since there is no input checks, it never returns unable to open file, but left it anyway.

        ifstream file(fileNames[i]);
        if (!file.is_open()) {
            cout << "Unable to open file: " << fileNames[i] << endl;
        }

        string line; //for the each word of the document
        string word;
        while (getline(file, line)) {
            correctString(line);
            istringstream ss(line);
            while (ss>>word) {

                WordItem WordItemObject; //word item object to find
                WordItemObject.word = word;

                DocumentItem DocumentItemObject; //document item object to hold the data of the word item
                DocumentItemObject.documentName = fileNames[i];

                WordItem foundItemHash = myTable.find(WordItemObject);

                if (foundItemHash == WordItem("", {})) { //if the word is not present

                    DocumentItemObject.count = 1;

                    WordItemObject.documentList.push_back(DocumentItemObject);

                    myTable.insert(WordItemObject);

                }

                else { //if the word is present

                    bool DocumentIsNotExisting = false; //suppose that the word exist but on the same file that we are on.

                    for (int j = 0; j < foundItemHash.documentList.size(); j++) {//iterating that words possible documents

                        if (foundItemHash.documentList[j].documentName == fileNames[i]) { //if the document of the word is the same as the document we are currently iterating
                            DocumentIsNotExisting = true; //now we know that we are on the same document
                            foundItemHash.documentList[j].count++; //if so we just need to update its count
                            myTable.update(foundItemHash,j); //since upgrading the count outside the table like this is not possible, i wrote a function in the Hash.h, the workings is explained on the header file

                        }

                    }

                    if (!DocumentIsNotExisting) { //if the word exists but not on our file,

                        myTable.update2(WordItemObject,fileNames[i]); //since pushing another document item vector is not possible outside the class, i again implemented another function to create and pushback another documentItem onto wordItem, it is explained on the header

                        //DocumentItem newDocumentItemOfTheWordItemObject; //we need to push another documentItem object to the WordItem, so we create another one.
                        //newDocumentItemOfTheWordItemObject.documentName = fileNames[i]; //its file is the file that we have found,
                        //newDocumentItemOfTheWordItemObject.count = 1; //and again its count is one
                        //WordItemObject.documentList.push_back(newDocumentItemOfTheWordItemObject); //pushing back to foundWord

                        //cout<<newDocumentItemOfTheWordItemObject.documentName<<endl;
                        //cout<<newDocumentItemOfTheWordItemObject.count<<endl;

                        //cout<<WordItemObject.word<<endl;
                        //cout<<WordItemObject.documentList[0].documentName<<endl;
                        //cout<<WordItemObject.documentList[0].count<<endl;

                        //comments here are left behind for debug purposes

                    }

                }
            }
        }
        file.close();
    }
    cout<<endl<<endl;


    cout<<"After preprocessing, the unique word count is "<< myTable.wordCountHash()<<". Current load ratio is "<<myTable.loadRatioHash()<<endl;


    //now the query part.

    cout << "Enter queried words in one line: ";
    //cin.clear();
    //cin.ignore();

    string line;
    getline(cin, line); //taking a line of words


    istringstream iss(line); //creating a stringstream object
    string word;
    vector<string> quarryWords; //a vector to store each word that is taken from the query
    int count = 0; //for the number of words in the query

    while (iss >> word) { //reading words one by one
         correctString(word);
        stringstream wordStream(word);
        string segment;
        while (getline(wordStream, segment, ' ')) { //parsing the line by the blanks
            if (!segment.empty()) {
                quarryWords.push_back(segment);
                count++;
            }
        }

    }

    int k = 20; //IMPORTANT! SINCE MY COMPUTER HAS ONLY 8GB OF RAM, MAKING THIS 100 WOULD ONCE IN EVERY 10 TRIES GAVE THE
    // terminate called after throwing an instance of 'std::length_error'
    //what():  basic_string::_M_create
    //error, but in every case, the hash table is much faster than the bst, you can change the k value to try, but since the point is to show that hash tables are
    //much faster than the binary search trees, i used 20 as my k value to not make my computers memory fill


    auto start = std::chrono::high_resolution_clock::now(); //timer for bst
    for (int i = 0; i < k; i++)
    {
        for (int y = 0; y < fileNames.size(); ++y) { //bst query

            int countForCheck = 0; //a counter to check if the all query words are in the same file we are currently on

            for (int z = 0; z < count ; ++z) {

                WordItem WordItemObjectToFind; //forming a word item object
                WordItemObjectToFind.word = quarryWords[z];

                DocumentItem DocumentItemObjectToFind; //forming a document item object
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemBSTForQuery = mainTree.find(WordItemObjectToFind); //finding the word without a file

                for (int j = 0; j < foundItemBSTForQuery.documentList.size(); ++j) {

                    const DocumentItem &docItem = foundItemBSTForQuery.documentList[j]; //creating a document item object and assign it to the document list of the wanted word
                    if (docItem.documentName == fileNames[y]) { //if the files are the same
                        countForCheck++; //increment the count for one
                        break;
                    }

                }

            }

            if ((count != countForCheck) &&  y==0){ //no document contains the words

                if(i==0){ //to make it print only for once
                    cout<<"No document contains the given query"<<endl;
                }



            }

            else if(count == 1 && countForCheck == 1 ){ //we have only one word and document contains the query

                WordItem WordItemObjectToFind; //again forming a worditem to find the word in the tree
                WordItemObjectToFind.word = quarryWords[i];

                DocumentItem DocumentItemObjectToFind;
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemBSTForQuery = mainTree.find(WordItemObjectToFind);

                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "<< foundItemBSTForQuery.word<<" found "<<foundItemBSTForQuery.documentList[y+1].count<<" times."<<endl; //the print formation
                }


            }
            else if(count > 1 && countForCheck > 1 &&  count == countForCheck){ //if there are more than 1 word and if the document contains all of them

                int ifInSameDocument = 0;

                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "; //prints the document name
                }

                for (int j = 0; j < quarryWords.size() ; ++j) {

                    WordItem WordItemObjectToFind; //creating a word item object to search
                    WordItemObjectToFind.word = quarryWords[j];

                    WordItem foundItemBSTForQuery = mainTree.find(WordItemObjectToFind);



                    for (int l = 0; l <foundItemBSTForQuery.documentList.size() ; ++l) { //this counter is created before but is not used, it holds the data of the words that are on the same document

                        if (foundItemBSTForQuery.documentList[l].documentName == fileNames[y] ){
                            ifInSameDocument++;
                        }

                    }
                    if(i==0){
                        cout<<foundItemBSTForQuery.word<<" found "<<foundItemBSTForQuery.documentList[y+1].count<<" times"; //output formation
                    }


                    if (j < quarryWords.size() -1){
                        if(i==0){
                            cout<<", ";
                        }

                    }
                    else
                        if(i==0){
                            cout<<".";
                        }
                }
                if (i==0){
                    cout<<endl;
                }

            }

        }

    }

    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now(); //timer for hash
    for (int i = 0; i < k; i++)
    {
        for (int y = 0; y < fileNames.size(); ++y) { //hash query

            int countForCheck = 0; //a counter to check if the all query words are in the same file we are currently on

            for (int q = 0; q < count ; ++q) {

                WordItem WordItemObjectToFind; //forming a word item object
                WordItemObjectToFind.word = quarryWords[q];

                DocumentItem DocumentItemObjectToFind; //forming a document item object
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind); //finding the word without a file

                if ( fileNames[y] == foundItemHashForQuery.documentList[y].documentName){ //if the files are the same
                    countForCheck++; //increment the count by one
                }

            }

            if ((count != countForCheck) && y ==0){ //no document contains the words
                if(i==0){ //to print only once
                    cout<<"No document contains the given query"<<endl;
                }


            }

            else if(count == 1 && countForCheck == 1 ){ //we have only one word and the document contains the query
                WordItem WordItemObjectToFind; //creating a word item object
                WordItemObjectToFind.word = quarryWords[i];

                DocumentItem DocumentItemObjectToFind;
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind); //finding the word
                if(i==0){
                    cout<<"in Document "<<fileNames[y]<<", "<< foundItemHashForQuery.word<<" found "<<foundItemHashForQuery.documentList[y].count<<" times."<<endl;
                }

            }
            else if(count > 1 && countForCheck > 1  && count == countForCheck){ //if there are more than 1 word and if the document contains all of them

                int ifInSameDocument = 0;

                if(i==0){ //printing the document name
                    cout<<"in Document "<<fileNames[y]<<", ";
                }

                for (int j = 0; j < quarryWords.size() ; ++j) {

                    WordItem WordItemObjectToFind; //creating a worditem object
                    WordItemObjectToFind.word = quarryWords[j];

                    WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind);



                    for (int l = 0; l <foundItemHashForQuery.documentList.size() ; ++l) { //this counter is created before but is not used, it holds the data of the words that are on the same document

                        if (foundItemHashForQuery.documentList[l].documentName == fileNames[y] ){
                            ifInSameDocument++;
                        }

                    }
                    if(i==0){
                        cout<<foundItemHashForQuery.word<<" found "<<foundItemHashForQuery.documentList[y].count<<" times"; //output formation
                    }


                    if (j < quarryWords.size() -1){
                        if(i==0){
                            cout<<", ";
                        }
                    }
                    else
                        if(i==0){
                            cout<<".";
                        }

                }
                if(i==0){
                    cout<<endl;
                }

            }

        }

    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << BSTTime.count() / k << "\n"; //time of bst
    cout << "\nTime: " << HTTime.count() / k << "\n"; //time of hash

    cout<<"Speed Up: "<<(BSTTime.count() * 1.0) / (HTTime.count() * 1.0); //the speed up time

    return 0;
}

