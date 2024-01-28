//
// Created by Berke Ayyildizli on 15.11.2023.
//

#ifndef HW2_TREE_H
#define HW2_TREE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

//NEARLY ALL OF THIS CLASS IS COPIED FROM LECTURE SLIDES, FUNCTIONS ARE TAKEN FROM THE BST PART, NO ISSUES SO FAR SINCE I GOT HELP FROM THE OFFICE HOURS MAINLY

//since i didn't want to change all the names of the classes and such, i left it avlnode and avltree, but deleted some rotation parts, since a binary search tree is also an avl tree

template <typename T> //i am using the typename approach rather than class, same but i learned it like that before. Also not using key, value approach to work better with slide codes.
class AvlTree;

template <typename T>
class AvlNode //avlNode class
{
    T 	element;
    AvlNode *left = NULL;
    AvlNode *right = NULL;
    int height;

    AvlNode( const T & theElement, AvlNode *lt, AvlNode *rt, int h = 0 ) : element( theElement ), left( lt ), right( rt ), height( h ) { } //constructor
    AvlNode() : element(NULL),  left( NULL ), right( NULL ), height( 0 ){} //again got help from office hours here as i got a constructor error
    friend class AvlTree<T>;
};

template <typename T>
class AvlTree { //our main class
public:


    explicit AvlTree(const T &notFound) : ITEM_NOT_FOUND(notFound), root(NULL){} //explicit constructor from the lecture slides

    AvlTree(): root(NULL) {} //i added this


    AvlTree(const AvlTree<T> & rhs): root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND){ //coppy constructor

        *this = rhs;

    }

    ~AvlTree(){
        makeEmpty(root); //destructor

    }

    const AvlTree & operator=( const AvlTree & rhs ){ // = operator overload, i also copied them to my main, since i got some errors

        if (this != &rhs)
        {
            makeEmpty(); //clear old one
            root = rhs.clone(rhs.root);
        }
        return *this;

    }

    bool operator < (const AvlTree<T> &rhs) const { //for the find and insert, again got help from office hours
        return root->element < rhs.root->element;
    }


    const T &findMin() const{ //public find min (not used)

        return elementAt( findMin( root ) );

    }

    const T &findMax() const{ //public find max (not used)

        return elementAt( findMax( root ) );

    }

    const T &find(const T &x) const{ //public find

        return elementAt( find( x, root ) );


    }

    bool isEmpty() const{ //public is empty

        if (root == NULL)
            return true;

        return false;

    }

    void printTree() const{ //i didn't used this,

        if( isEmpty( ) )
            cout << "Empty tree" << endl;
        else
            printTree( root );


    }

    void makeEmpty(){ //just copied from the lecture slides

        makeEmpty( root );


    }

    void insert(const T &x){ //just copied from the lecture slides

        insert( x, root );

    }

    void remove(const T &x){ //just copied from the lecture slides BST part

        remove( x, root );

    }



private:

    string title;
    AvlNode<T> *root;
    const T ITEM_NOT_FOUND;

    const T &elementAt(AvlNode<T> *t) const{ //copied from the lecture slides

        return t == NULL ? ITEM_NOT_FOUND : t->element;

    }

    void insert(const T &x, AvlNode<T> *&t) const{ //copied from the lecture slides, since till the end of my homework, i copied it by shifting an else statement wrongly, this part caused many many errors :(


        if ( t == NULL )
            t = new AvlNode<T>( x, NULL, NULL );

        else if ( x < t->element ) {
            // X should be inserted to the left tree!
            insert( x, t->left );

            // Check if the left tree is out of balance (left subtree grew in height!)
            if ( height( t->left ) - height( t->right ) == 2 )
                if ( x < t->left->element )  // X was inserted to the left-left subtree!
                    rotateWithLeftChild( t );
                else			     // X was inserted to the left-right subtree!
                    doubleWithLeftChild( t );
        }
        else if( t->element < x )
        {    // Otherwise X is inserted to the right subtree
            insert( x, t->right );
            if ( height( t->right ) - height( t->left ) == 2 )
                // height of the right subtree increased
                if ( t->right->element < x )
                    // X was inserted to right-right subtree
                    rotateWithRightChild( t );
                else // X was inserted to right-left subtree
                    doubleWithRightChild( t );
        }
        else
            ;  // Duplicate; do nothing

        // update the height the node
        t->height = max( height( t->left ), height( t->right ) ) + 1;
    }


    void remove(const T &x, AvlNode<T> *&t) { //the first part of this function is copied directly from the BST tree function of lecture slides
        if (t == NULL)
            return; // Item not found; do nothing

        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != NULL && t->right != NULL) // Two children
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else // One or no children
        {
            AvlNode<T> *oldNode = t;
            t = (t->left != NULL) ? t->left : t->right;
            delete oldNode;
        }

    }

    AvlNode<T> *findMin(AvlNode<T> *t) const{ //copied from the slides

        if( t == NULL )
            return NULL;
        if( t->left == NULL )
            return t;
        return findMin( t->left );


    }

    AvlNode<T> *findMax(AvlNode<T> *t) const{ //copied from the slides

        if( t != NULL )
            while( t->right != NULL )
                t = t->right;
        return t;


    }

    AvlNode<T> *find(const T &x, AvlNode<T> *t) //copied from the slides, again caused me many errors
    const{

        if ( t == NULL )
            return NULL;
        else if( x < t->element )
            return find( x, t->left );
        else if( t->element < x )
            return find( x, t->right );
        else
            return t;    // Match


    }


    void makeEmpty(AvlNode<T> *&t) const{ //copied from the slides

        if( t != NULL )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = NULL;


    }

    void printTree(AvlNode<T> *t) const{ //copied from the slides

        if ( t != NULL )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }


    }

    AvlNode<T> *clone(AvlNode<T> *t) const{ //copied from the slides

        if ( t == NULL )
            return NULL;
        else
            return new AvlNode<T>
                    ( t->element,
                      clone( t->left ),
                      clone( t->right ) );


    }

    // Avl manipulations
    int height(AvlNode<T> *t) const{ //copied from the slides

        if (t == NULL)
            return -1;

        return t->height;


    }

    int max(int lhs, int rhs) const{ //copied from the slides

        if (lhs > rhs)
            return lhs;

        return rhs;


    }

    void rotateWithLeftChild(AvlNode<T> *&k2) const{ //copied from the slides

        AvlNode<T> *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
        k1->height = max( height( k1->left ), k2->height ) + 1;
        k2 = k1;


    }

    void rotateWithRightChild(AvlNode<T> *&k1) const{ //copied from the slides

        AvlNode<T> *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
        k2->height = max( height( k2->right ), k1->height ) + 1;
        k1 = k2;


    }

    void doubleWithLeftChild(AvlNode<T> *&k3) const{ //copied from the slides

        rotateWithRightChild( k3->left );
        rotateWithLeftChild( k3 );


    }

    void doubleWithRightChild(AvlNode<T> *&k1) const{ //copied from the slides

        rotateWithLeftChild( k1->right );
        rotateWithRightChild( k1 );


    }


};

#endif //HW2_TREE_H

