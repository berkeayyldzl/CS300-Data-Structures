//
//Created by Berke Ayyildizli 31018
//
// sucourse user name berke.ayyildizli
//
// berke.ayyildizli_ayyildizli_berke_hw4.cpp
//
//CS300_HW4

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <chrono>
#include "Tree.h" //my main tree class
#include "Hash.h" //my main hash table class
#include "Sort.h"
#include "randgen.h"

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

bool operator <= (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the <= to work in methods like find and insert
    return lhs.word <= rhs.word;
}

bool operator > (const WordItem& lhs, const WordItem& rhs) { //the operator overload for the > to work in methods like find and insert
    return lhs.word > rhs.word;
}



WordItem findVec(vector<WordItem> vec, WordItem word) { //this is the custom find method, looks one by one on the sorted vector to add
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == word) {
            return vec[i];
        }
    }
    WordItem notfound = WordItem("", {}); //if cant found returns a blank
    return notfound;
}


WordItem binarySearch(const vector<WordItem>& sortedArray, const WordItem& target) { //the binary search algorithm, modified some parts from the general to suit my needs
    int low = 0;
    int high = sortedArray.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (sortedArray[mid].word == target.word) { //now checks the words
            return sortedArray[mid]; //and returns the whole object
        } else if (sortedArray[mid].word < target.word) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    WordItem notFound = WordItem("", {}); //if cant be found
    return notFound;
}

WordItem& updateVector(vector<WordItem>& vec, const WordItem& x, int j, const vector<string>& fileNames) { //the new update vector, to both update count, and push back new documents to a word
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == x) { //if it can be found on the document

            for (int k = 0; k < vec[i].documentList.size(); ++k) {
                if (vec[i].documentList[k].documentName == fileNames[j]) {
                    vec[i].documentList[k].count++; //increase its count
                    return vec[i];//return the object
                }
            }

            DocumentItem newDocumentItem; //if the word is not in our document
            newDocumentItem.documentName = fileNames[j];  //create a new document
            newDocumentItem.count = 1;
            vec[i].documentList.push_back(newDocumentItem); //push back to the documentList
            return vec[i];
        }
    }

    WordItem newWordItem; //if the word is not present on the vector
    newWordItem.word = x.word; //create a new WordItem object
    DocumentItem newDocumentItem;
    newDocumentItem.documentName = fileNames[j];
    newDocumentItem.count = 1;
    newWordItem.documentList.push_back(newDocumentItem); //push back the new document
    vec.push_back(newWordItem);//push back the word
    return vec.back();//and return the last pushed element
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

                    }

                }
            }
        }
        file.close();
    }
    //cout<<endl<<endl;


    //vector part
    //The same logic, but since i couldn't change the vector objects in the main just like hash table, i needed to write an update and find function, to find and update WordItem objects.

    vector<WordItem> vec;

    for (int i = 0; i < fileNames.size(); i++) {
        ifstream file(fileNames[i]);
        if (!file.is_open()) {
            cout << "Unable to open file: " << fileNames[i] << endl;
            continue;
        }

        string line;
        string word;
        while (getline(file, line)) {
            correctString(line);
            istringstream ss(line);
            while (ss >> word) {
                WordItem WordItemObject;
                WordItemObject.word = word;

                WordItem foundItemHash = findVec(vec, WordItemObject); //this is the find method, returns the wordItem itself
                updateVector(vec, WordItemObject, i, fileNames); //and this is the whole update logic,unlike hash table's 2 different updates, this time i wanted to combine all.

            }
        }
        file.close();
    }

    //NOW THE SORTING PART


    vector<WordItem> vecMergeSort(vec); //Creating the copy constructors
    vector<WordItem> vecHeapSort(vec);
    vector<WordItem> vecInsertSort(vec);

    vector<WordItem> vecMedianQuick(vec);
    vector<WordItem> vecRandomQuick(vec);
    vector<WordItem> vecFirstQuick(vec);

    //TIMINGS FOR THE SORTS

    //Since the given clock method gives out the results wrong, i used chrono, just like HW3

    int p = 1;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //MERGE SORT

        mergeSort(vecMergeSort);

    }
    auto mergeTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);


    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //HEAP SORT

        heapsort(vecHeapSort);

    }
    auto heapTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //INSERTION SORT

        insertionSort(vecInsertSort);

    }
    auto insertionTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //QUICK SORT MEDIAN

        quicksort(vecMedianQuick,((vecMedianQuick.size()-1)/2),vecMedianQuick.size()-1);

    }
    auto qMedianTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //QUICK SORT RANDOM
        RandGen r;
        int a = r.RandInt(0,vecRandomQuick.size()-1);
        quicksort(vecRandomQuick,a,vecRandomQuick.size()-1);

    }
    auto qRandomTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < p; i++){ //QUICK SORT FIRST

        quicksort(vecFirstQuick,0,vecFirstQuick.size()-1);

    }
    auto qFirstTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);


    //BELOW PART ARE FOR DEBUG, IT PRINTS OUT ALL THE VECTOR IN THE DESCENDING ORDER, ALSO PRINTS DOCUMENTS

    /*

    for (const WordItem& wordItem : vecFirstQuick) {
        cout << wordItem.word << ": ";
        for (const DocumentItem& documentItem : wordItem.documentList) {
            cout << documentItem.documentName << " ";
        }
        cout << endl;
    }

    */


    cout<<"After preprocessing, the unique word count is "<< myTable.wordCountHash()<<". Current load ratio is "<<myTable.loadRatioHash()<<endl;


    //Now the query part.

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


    int k = 1;
    start = std::chrono::high_resolution_clock::now(); //timer for bst
    for (int i = 0; i < k; i++){

        // QueryDocuments with Binary Search Tree Same as HW3
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



    start = std::chrono::high_resolution_clock::now(); // timer for hash
    for (int i = 0; i < k; i++) {

        // QueryDocuments with Hash Table Same as HW3

        for (int y = 0; y < fileNames.size(); ++y) { // hash query

            int countForCheck = 0; // a counter to check if all query words are in the same file we are currently on

            for (int q = 0; q < count; ++q) {

                WordItem WordItemObjectToFind; // forming a word item object
                WordItemObjectToFind.word = quarryWords[q];

                DocumentItem DocumentItemObjectToFind; // forming a document item object
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind); // finding the word without a file

                if (fileNames[y] == foundItemHashForQuery.documentList[y].documentName) { // if the files are the same
                    countForCheck++; // increment the count by one
                }
            }

            if ((count != countForCheck) && y == 0) { // no document contains the words
                if (i == 0) { // to print only once
                    cout << "No document contains the given query" << endl;
                }
            }
            else if (count == 1 && countForCheck == 1) { // we have only one word and the document contains the query
                WordItem WordItemObjectToFind; // creating a word item object
                WordItemObjectToFind.word = quarryWords[i];

                DocumentItem DocumentItemObjectToFind;
                DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind); // finding the word
                if (i == 0) {
                    cout << "in Document " << fileNames[y] << ", " << foundItemHashForQuery.word << " found " << foundItemHashForQuery.documentList[y].count << " times." << endl;
                }
            }
            else if (count > 1 && countForCheck > 1 && count == countForCheck) { // if there are more than 1 word and if the document contains all of them

                int ifInSameDocument = 0;

                if (i == 0) { // printing the document name
                    cout << "in Document " << fileNames[y] << ", ";
                }

                for (int j = 0; j < quarryWords.size(); ++j) {

                    WordItem WordItemObjectToFind; // creating a word item object
                    WordItemObjectToFind.word = quarryWords[j];

                    WordItem foundItemHashForQuery = myTable.find(WordItemObjectToFind);

                    for (size_t l = 0; l < foundItemHashForQuery.documentList.size(); ++l) { // this counter is created before but is not used, it holds the data of the words that are on the same document

                        if (foundItemHashForQuery.documentList[l].documentName == fileNames[y]) {
                            ifInSameDocument++;
                        }
                    }

                    if (i == 0) {
                        cout << foundItemHashForQuery.word << " found " << foundItemHashForQuery.documentList[y].count << " times"; // output formation
                    }

                    if (j < quarryWords.size() - 1) {
                        if (i == 0) {
                            cout << ", ";
                        }
                    }
                    else if (i == 0) {
                        cout << ".";
                    }
                }

                if (i == 0) {
                    cout << endl;
                }
            }
        }
    }

    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);

    //Binary Search part
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++){
        // QueryDocuments with Binary Search
        for (int y = 0; y < fileNames.size(); ++y) { //binary search query

            int countForCheck = 0; //a counter to check if the all query words are in the same file we are currently on

            for (int q = 0; q < count ; ++q) {

                WordItem WordItemObjectToFind; //forming a word item object
                WordItemObjectToFind.word = quarryWords[q];

                //DocumentItem DocumentItemObjectToFind; //forming a document item object
                //DocumentItemObjectToFind.documentName = fileNames[y];

                WordItem foundItemHashForQuery = binarySearch(vecInsertSort,WordItemObjectToFind); //finding the word without a file with the new function

                for (const DocumentItem& docItem : foundItemHashForQuery.documentList) {
                    if (fileNames[y] == docItem.documentName) { // if the files are the same
                        countForCheck++; // increment the count by one
                        break; // Break the loop once a match is found
                    }
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

                WordItem foundItemHashForQuery = binarySearch(vecInsertSort,WordItemObjectToFind); //finding the word
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

                    WordItem foundItemHashForQuery = binarySearch(vecInsertSort,WordItemObjectToFind);


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
    auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);



    cout << "\nBinary Search Tree Time: " << BSTTime.count() / k; //time of bst
    cout << "\nHash Table Time: " << HTTime.count() / k; //time of hash
    cout << "\nBinary Search Time: " << BSTime.count() / k; //time of hash
    cout<<endl;
    cout << "\nQuick Sort(median) Time: " << qMedianTime.count();
    cout << "\nQuick Sort(random) Time: " << qRandomTime.count();
    cout << "\nQuick Sort(first) Time: " << qFirstTime.count();
    cout << "\nMerge Sort Time: " << mergeTime.count();
    cout << "\nHeap Sort Time: " << heapTime.count();
    cout << "\nInsertion Sort Time: " << insertionTime.count();

    cout<<endl<<endl;

    //the speed up times
    cout<<"Speed Up BST/HST: "<<(BSTTime.count() * 1.0) / (HTTime.count() * 1.0)<<endl;

    cout<<"Speed Up Merge Sort/Quick Sort(Median):  "<<(mergeTime.count() * 1.0) / (qMedianTime.count() * 1.0)<<endl;
    cout<<"Speed Up Heap Sort/Quick Sort(Median): "<<(heapTime.count() * 1.0) / (qMedianTime.count() * 1.0)<<endl;
    cout<<"Speed Up Insertion Sort/Quick Sort(Median): "<<(insertionTime.count() * 1.0) / (qMedianTime.count() * 1.0)<<endl;
    cout<<endl;

    if ((BSTTime.count() * 1.0) > (BSTime.count() * 1.0)){
        cout<<"Speed Up Binary Search Tree Time / Binary Search: "<<(BSTTime.count() * 1.0) / (BSTime.count() * 1.0)<<endl;
    }
    else if((BSTTime.count() * 1.0) <= (BSTime.count() * 1.0)){
        cout<<"Speed Up Binary Search / Binary Search Tree Time: "<<(BSTime.count() * 1.0) / (BSTTime.count() * 1.0)<<endl;

    }

    if ((HTTime.count() * 1.0) > (BSTime.count() * 1.0)){
        cout<<"Speed Up Hash Table Time / Binary Search: "<<(HTTime.count() * 1.0) / (BSTime.count() * 1.0)<<endl;
    }
    else if((HTTime.count() * 1.0) <= (BSTime.count() * 1.0)){
        cout<<"Speed Up Binary Search / Hash Table Time: "<<(BSTime.count() * 1.0) / (HTTime.count() * 1.0)<<endl;

    }
    //cout<<"Speed Up Binary Search / Binary Search Tree Time: "<<(BSTTime.count() * 1.0) / (BSTime.count() * 1.0)<<endl;
    //cout<<"Speed Up Binary Search / Hash Table Time:  "<<(HTTime.count() * 1.0) / (BSTime.count() * 1.0)<<endl;

    return 0;
}
