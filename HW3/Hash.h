//
// Created by Berke Ayyıldızlı on 12.12.2023.
//

#ifndef HW2_HASH_H
#define HW2_HASH_H

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

//THIS IS THE MAIN HASH TABLE CLASS, NEARLY ALL CODES ARE COPIED FROM LECTURE SLIDES, SOME MODIFICATIONS HAVE BEEN MADE AND WRITTEN ADDITIONAL FUNCTIONS FOR MY PROGRAM TO WORK

struct DocumentItem { //DocumentItem struct to hold the each WordItem object's information data such as count and document names. //i created this here because some functions need to know what a document item is

    string documentName;
    int count;

};

using namespace std;

bool isPrime( int n ) //it is for the rehashing and inserting
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}

int nextPrime( int n ) //it is for the rehashing and inserting
{
    if ( n % 2 == 0 )
        n++;

    for ( ; ! isPrime( n ); n += 2 )
        ;

    return n;
}

int hashFunc (const string& key, int tableSize) //my hash function, copied from the lecture slides
{
    int hashVal = 0;

    for (int i = 0; i < key.length();i++)
        hashVal = 37 * hashVal + key[ i ];

    hashVal = hashVal % tableSize;

    if (hashVal < 0)
        hashVal = hashVal + tableSize;

    return(hashVal);
}

template <class HashedObj>
class HashTable
{
public:
    explicit HashTable( const HashedObj & notFound, int size = 53 ); //constructor, since on test cases the size is 53, i used it as well

    HashTable( const HashTable & rhs ) //copy constructor
            : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
              array( rhs.array ), currentSize( rhs.currentSize ) { }

    const HashedObj & find( const HashedObj & x ) const{ //public find method, gives the element on the wanted position
        int currentPos = findPos( x );

        if (isActive( currentPos ))
            return array[ currentPos ].element;

        return ITEM_NOT_FOUND;
    }

    HashedObj HashFound (const HashedObj& word){ //an unused find function to give the element on the wanted position
        int i = 0;
        while (i < array.size()){

            if (word == array[i].element){

                return array[i].element;
            }
            else

                i++;
        }
    }

    HashedObj & update(const HashedObj & x, int j){ //the first function that i wrote to update the count of a word on the wanted document, i used this because i couldn't modify the private data of the table by just assigning
        int position = findPos(x);

        if (array[position].element == x) {
            array[position].element.documentList[j].count++;
        }
        return array[position].element;

    }

    void update2(HashedObj& wordItem, const std::string& documentName) { //another function that i wrote, since i couldn't push back another document item onto a current word in the table, this function updates the documents
        int position = findPos(wordItem);

        if (array[position].element == wordItem) {

            DocumentItem newDocumentItem;
            newDocumentItem.documentName = documentName;
            newDocumentItem.count = 1;

            array[position].element.documentList.push_back(newDocumentItem);
        } else {

        }
    }



    void makeEmpty( ){ //makes all the cells empty

        for( int i = 0; i < array.size( ); i++ )
            array[ i ].info = EMPTY;

        currentSize = 0;

    }

    int wordCountHash(){ //returns the current size

        return currentSize;
    }

    double loadRatioHash(){ //returns the load ratio

        float loadFactor= 1.0 * currentSize/array.size();

        return loadFactor;
    }


    void insert( const HashedObj & x ){ //insert method

        // Insert x as active
        int currentPos = findPos( x );
        if ( isActive( currentPos ) )
            return;
        array[ currentPos ] = HashEntry( x, ACTIVE );

        // enlarge the hash table if necessary
        if ( ++currentSize >= array.size( ) * 0.6 ) {
            //cout << "current size is: " << currentSize << endl;
            rehash();
        }
    }

    void remove( const HashedObj & x ){ //unused remove method

        int currentPos = findPos( x );
        if ( isActive( currentPos ) )
            array[ currentPos ].info = DELETED;
    }

    const HashTable & operator=( const HashTable & rhs ){ //operator overload for the table to work correctly

        if (this != &rhs) {
            array = rhs.array;
            currentSize = rhs.currentSize;
        }
        return *this;

    }

    float returnLoadFactor(){ //unused load factor getter

        float loadFactor= 1.0 * currentSize/array.size();
        return loadFactor;

    }

    enum EntryType { ACTIVE, EMPTY, DELETED }; //enumarators

private:
    struct HashEntry //structu for hash entry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ),EntryType i = EMPTY ): element( e ), info( i ) {}


    };



    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    int findPos( const HashedObj & x ) const{ //find pos method

        int collisionNum = 0;
        int currentPos = hashFunc(x.word, array.size());
        //cout << array.size() << endl;
        while ( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
            currentPos += pow(++collisionNum, 2) ;  //add the difference
            /*if (currentPos >= array.size()){
                currentPos = currentPos %array.size();
            */
            if ( currentPos >= array.size( ) )              // perform the mod
                currentPos -= array.size( );                // if necessary
        }
        return currentPos;
    }

    bool isActive( int currentPos ) const{ //returns if the slot is occupied

        return array[ currentPos ].info == ACTIVE;

    }

    void rehash(){ //rehashed according to the hash function and outputs the information about the table

        {
            vector<HashEntry> oldArray = array;
            int sizeofarray = oldArray.size();

            // Create new double-sized, empty table
            array.resize( nextPrime( 2 * sizeofarray ) );
            for ( int j = 0; j < array.size( ); j++ )
                array[ j ].info = EMPTY;

            // Copy table over
            currentSize = 0;
            for ( int i = 0; i < sizeofarray; i++ ){

                if ( oldArray[ i ].info == ACTIVE ){
                    insert( oldArray[ i ].element );
                }

            }
            float loadFactor= 1.0 * currentSize/array.size();


            cout<<"rehashed..."<<endl;
            cout<<"previous table size:"<<sizeofarray<<", new table size: "<<array.size()<<", current unique word count "<<currentSize<<", current load factor: "<<loadFactor<<endl;


        }

    }





};

template<class HashedObj> //the constructor
HashTable<HashedObj>::HashTable(const HashedObj &notFound, int size): array(nextPrime(size)), ITEM_NOT_FOUND(notFound) {

    makeEmpty();

}


#endif //HW2_HASH_H
