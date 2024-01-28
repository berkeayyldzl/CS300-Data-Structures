//
// Created by Berke Ayyıldızlı on 07.01.2024.
//

#ifndef HW4_SORT_H
#define HW4_SORT_H

//THIS IS THE SORTING METHODS CLASS, IT JUST HAS THE FUNCTIONS
//ALL THE FUNCTIONS ARE COPIED FROM LECTURE SLIDES
//SOME MODIFICATIONS HAVE BEEN MADE DUE TO SWAP FUNCTION BEING USED FOR MULTIPLE CLASSES


//TODO: ***************************** INSERTION SORT *************************************

template <class Comparable>
void insertionSort (vector <Comparable> & a)
{
    int j;
    // loop over the passes
    for (int p = 1;  p < a.size(); p++)
    {
        Comparable tmp = a[p];
        // loop over the elements
        for (j = p; j > 0 &&  tmp < a[j - 1]; j --)
            a[j] = a[j -1];
        a[j] = tmp;
    }
}

//TODO: **************************** HEAP SORT ********************************

inline int leftChild( int i )
{
    return 2*i+1;
}

template <class Comparable>
void percDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;

    for (tmp=a[i] ; leftChild(i) < n; i = child )
    {
        child = leftChild(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( a[child ] > tmp )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}


template <class Comparable>
void heapsort(vector<Comparable> & a)
{
    // buildHeap
    for (int i = a.size()/2; i >=0; i--)
    percDown(a, i, a.size());

    // sort
    for (int j = a.size() -1; j >0; j--)
    {
        std::swap(a[0], a[j]);    // swap max to the last pos.
        percDown(a, 0, j); // re-form the heap
    }
}



//TODO: ********************** MERGE SORT ***************************

template <class Comparable>
void mergeSort( vector<Comparable> & a )
{
    vector<Comparable> tmpArray( a.size( ) );

    mergeSort( a, tmpArray, 0, a.size( ) - 1 );
}

template <class Comparable>
void mergeSort( vector<Comparable> & a, vector<Comparable> & tmpArray, int left, int right )
{
    if ( left < right )
    {
        int center = ( left + right ) / 2;
        mergeSort( a, tmpArray, left, center );
        mergeSort( a, tmpArray, center + 1, right );
        merge( a, tmpArray, left, center + 1, right );
    }
}

template <class Comparable>
void merge(  vector<Comparable> & a,vector<Comparable> & tmpArray,int leftPos, int rightPos, int rightEnd ){

    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while ( leftPos <= leftEnd && rightPos <= rightEnd ){ // Main loop
        if ( a[ leftPos ] <= a[ rightPos ] )
            tmpArray[ tmpPos++ ] = a[ leftPos++ ];
        else
            tmpArray[ tmpPos++ ] = a[ rightPos++ ];
    }

    while ( leftPos <= leftEnd ){ // Copy rest of first half
        tmpArray[ tmpPos++ ] = a[ leftPos++ ];
    }

    while ( rightPos <= rightEnd ){ // Copy rest of right half
        tmpArray[ tmpPos++ ] = a[ rightPos++ ];
    }

    for ( int i = 0; i < numElements; i++, rightEnd-- ){ // Copy tmpArray back
        a[ rightEnd ] = tmpArray[ rightEnd ];
    }

}

//TODO: ***************************** QUICK SORT ********************************

template <class Comparable>
void quicksort( vector<Comparable> & a )
{
    quicksort( a, 0, a.size( ) - 1 );
}

template <class Comparable>
inline void swapIt( Comparable & obj1,
                  Comparable & obj2 )
{
    Comparable tmp = obj1;
    obj1 = obj2;
    obj2 = tmp;
}

template <class Comparable>
const Comparable & median3( vector<Comparable> & a,
                            int left, int right )
{
    int center = ( left + right ) / 2;
    if ( a[ center ] < a[ left ] )
        swap( a[ left ], a[ center ] );
    if ( a[ right ] < a[ left ] )
        swap( a[ left ], a[ right ] );
    if ( a[ right ] < a[ center ] )
        swap( a[ center ], a[ right ] );

    // Place pivot at position right - 1
    swap( a[ center ], a[ right - 1 ] );
    return a[ right - 1 ];
}

template <class Comparable>
void quicksort( vector<Comparable> & a,
                int left, int right )
{
    if ( left + 10 <= right )
    {
        Comparable pivot = median3( a, left, right );
        int i = left, j = right - 1;
        for ( ; ; )
        {
            while ( a[ ++i ] < pivot ) { }

            while ( pivot < a[ --j ] ) { }

            if ( i < j )
                swapIt( a[ i ], a[ j ] );
            else
                break;
        }
        swapIt( a[ i ], a[ right - 1 ] );   // Restore pivot

        quicksort( a, left, i - 1 );       // Sort small elements
        quicksort( a, i + 1, right );    // Sort large elements
    }
    else  // Do an insertion sort on the subarray
        insertionSort( a, left, right );
}

template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right )
{
    for ( int p = left + 1; p <= right; p++ )
    {
        Comparable tmp = a[ p ];
        int j;

        for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
            a[ j ] = a[ j - 1 ];
        a[ j ] = tmp;
    }
}




#endif //HW4_SORT_H
