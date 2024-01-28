//
//  stack.h
//  berke.ayyildizli_ayyildizli_berke_hw1
//
//  Created by Berke Ayyıldızlı on 8.11.2023.
//

#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdexcept>


using namespace std;


template <typename T>
class Stack{

public:


    Stack( ){
        topOfStack = nullptr;
    }

    bool isEmpty() const{

        /* if(topOfStack == nullptr){
            return true;
        }
        else if((topOfStack =! nullptr)){
            return false;
        }
       */

        return topOfStack == nullptr;
    }

    void pop( ){

        if (isEmpty()){
            cout<<"the stack is empty"<<endl;
        }
            //throw underflow_error("empty stack");
        else{
            ListNode *oldTop = topOfStack;
            topOfStack = topOfStack->next;
            delete oldTop;
        }
    }


    void push(const T& x){
        topOfStack = new ListNode( x, topOfStack );


    }



    const T & top( ) const{

        if (isEmpty()){
            cout<<"the stack is empty"<<endl;
            throw underflow_error("empty stack");
        }

        else{
            return topOfStack->element;
        }

    }



private:
    struct ListNode
    {
        T element;
        ListNode *next;

        ListNode( const T & theElement, ListNode * n = NULL )
                : element( theElement ), next( n ) { }  // Constructor


    };

    ListNode *topOfStack;  // list itself is the stack

};

template <typename T>
struct ListNode
{

    T element;
    ListNode<T> *next;

};








#endif
