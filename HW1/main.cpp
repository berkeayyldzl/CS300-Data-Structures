//
// CS300 Data Structures 2023-2024 Fall Term Homework 1
//
// berke.ayyildizli_ayyildizli_berke_hw1.cpp
//
// Created by Berke Ayyildizli 31018 on 27.10.2023.


#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "randgen.h" // Our random number selector is located here, taken directly from CS201 lecture codes.
#include "stack.h" // My user created Stack class. Most of the functions are taken from CS300 lecture slides, certain changes have been made.

using namespace std;

struct Maze { // The main maze struct to store the values of the walls and the coordinates.
    bool visited = false; //if the cell is visited before or not.
    int u = 1; // Top wall, if present 1, if not 0.
    int d = 1; // Bottom wall, if present 1, if not 0.
    int l = 1; // Left wall, if present 1, if not 0.
    int r = 1; // Right wall, if present 1, if not 0.
    int x; //X Coordinates
    int y; // Y Coordinates
};

bool CheckTop(vector<vector<Maze>> &maze , Maze m1 , int m, int n){

    if(m1.y+1 < m && !maze[m1.x][m1.y+1].visited){

//        maze[m1.x+1][m1.y].u = 0;
       // maze[m1.x][m1.y+1].visited = true;

        return true;
    }
    return false;
}

 /* void pathFinder (vector<vector<vector<Maze>>> &maze, int mazeID, int entryX, int entryY, int exitX, int exitY){

    vector<vector<Maze>> MySolvableMaze;

    stack<Maze> PathFinderStack;

    vector<vector<Maze>> MySolvableMaze(M,vector<Maze>(N));

    for(int j = 0; j < MySolvableMaze.size(); j++){

        for(int k = 0; k < MySolvableMaze[0].size(); k++){

            MySolvableMaze[j][k].x = j;
            MySolvableMaze[j][k].y = k;
        }
    }
    Stack<Maze> mazeStack;

    PathFinderStack.push(MySolvableMaze[entryX][entryY]);

    MySolvableMaze[entryX][entryY].visited = true;

    Maze m1 = MySolvableMaze[entryX][entryY]; //starting cell

    }
*/


















bool CheckRight(vector<vector<Maze>> &maze,Maze m1,int m, int n){

    if(m1.x+1 < n && !maze[m1.x+1][m1.y].visited){

//        maze[m1.x][m1.y+1].r = 0;
        //maze[m1.x][m1.y+1].visited = true;
        return true;
    }
    return false;
}

bool CheckDown(vector<vector<Maze>> &maze,Maze m1){

    if(m1.y != 0 && !maze[m1.x][m1.y-1].visited){
        //maze[m1.x-1][m1.y].d = 0;
        //maze[m1.x-1][m1.y].visited = true;
        return true;
    }
    return false;
}

bool CheckLeft(vector<vector<Maze>> &maze,Maze m1){

    if(m1.x != 0 && !maze[m1.x-1][m1.y].visited){

        //maze[m1.x][m1.y-1].l = 0;
        //maze[m1.x][m1.y-1].visited = false;
        return true;
    }

    return false;
}

int main() {

    int K,M,N,ID,mazeID,entryX,entryY,exitX,exitY;

    cout<<"Enter the number of mazes: ";
    cin>>K;
    cout<<"Enter the number of rows and columns (M and N): ";
    cin>>M>>N;

    vector<vector<vector<Maze>>> vec;

    for(int i = 0; i < K ; i++){

        vector<vector<Maze>> MyMaze(M,vector<Maze>(N));

        for(int j = 0; j < MyMaze.size(); j++){

            for(int k = 0; k < MyMaze[0].size(); k++){

                MyMaze[j][k].x = j;
                MyMaze[j][k].y = k;
            }
        }
        Stack<Maze> mazeStack;

        mazeStack.push(MyMaze[0][0]);

        MyMaze[0][0].visited = true;

        Maze m1 = MyMaze[0][0];

        char chosenPath;
        string possiblePath = "";

        while (!mazeStack.isEmpty()) {

            RandGen rand;

            //1 -> top
            //2 -> right
            //3 -> down
            //4 -> left

            if(CheckRight(MyMaze,m1,M,N)){
                possiblePath += 'r';
            }
            if(CheckLeft(MyMaze,m1)){
                possiblePath += 'l';
            }
            if(CheckTop(MyMaze,m1,M,N)){
                possiblePath += 't';
            }
            if(CheckDown(MyMaze,m1)){
                possiblePath += 'd';
            }

            int max = possiblePath.size()-1;

            cout<< "max is: "<<max<<endl;

            int number = rand.RandInt(0,max);

            chosenPath = possiblePath[number];
            cout<< "chosen indx: "<<number<<endl;
            cout << "possible path: "<< possiblePath << endl;
            cout << "chosen path: " << chosenPath << endl;
            possiblePath = "";
            if(chosenPath == 't'){

                //if(CheckTop(MyMaze,m1)){

                    MyMaze[m1.x][m1.y].u = 0;
                    m1 = MyMaze[m1.x][m1.y+1];
                    cout << m1.x << m1.y << endl;

                    MyMaze[m1.x][m1.y].visited = true;

                    MyMaze[m1.x][m1.y].d= 0;
                    mazeStack.push(MyMaze[m1.x][m1.y]);

                //}
            }

            else if (chosenPath == 'r'){

                //if(CheckRight(MyMaze,m1)){

                    MyMaze[m1.x][m1.y].r = 0;
                    m1 = MyMaze[m1.x+1][m1.y];
                    cout << m1.x << m1.y << endl;

                    MyMaze[m1.x][m1.y].visited = true;

                    MyMaze[m1.x][m1.y].l= 0;
                    mazeStack.push(MyMaze[m1.x][m1.y]);

                //}

            }

            else if (chosenPath == 'd'){

                //if(CheckDown(MyMaze,m1)){

                    MyMaze[m1.x][m1.y].d = 0;
                    m1 = MyMaze[m1.x][m1.y-1];

                    MyMaze[m1.x][m1.y].visited = true;

                    MyMaze[m1.x][m1.y].u= 0;
                    mazeStack.push(MyMaze[m1.x][m1.y]);

                //}

            }
            else if (chosenPath == 'l'){
                //if(CheckLeft(MyMaze,m1)){

                    MyMaze[m1.x][m1.y].l = 0;
                    m1 = MyMaze[m1.x-1][m1.y];

                    MyMaze[m1.x][m1.y].visited = true;

                    MyMaze[m1.x][m1.y].r= 0;
                    mazeStack.push(MyMaze[m1.x][m1.y]);

                //}

            }
            else{
                //if(!mazeStack.isEmpty()){

                    mazeStack.pop();
                    cout << "poping" << endl;

                    if(!mazeStack.isEmpty()){

                        m1.x = mazeStack.top().x;
                        m1.y = mazeStack.top().y;
                        cout << "backtracking to: " << m1.x << m1.y << endl;
                    }

                //}
                    //if(!(mazeStack.top().x == 0 && mazeStack.top().y == 0)){
            }

        }
        vec.push_back(MyMaze);

        ofstream off;

        string nameOfFile = "maze_" + to_string(i) + ".txt";

        off.open(nameOfFile.c_str());

        off << MyMaze.size() << " " << MyMaze[0].size() << endl;

        for (int s = 0; s < MyMaze.size(); s++){

            for (int f = 0; f < MyMaze[0].size(); f++) {


                cout << "x=" << s << " y= " << f << " l=" << MyMaze[s][f].l << " r=" << MyMaze[s][f].r << " u=" << MyMaze[s][f].u << " d=" << MyMaze[s][f].d <<endl;
                off << "x=" << s << " y= " << f << " l=" << MyMaze[s][f].l << " r=" << MyMaze[s][f].r << " u=" << MyMaze[s][f].u << " d=" << MyMaze[s][f].d <<endl;
            }

        }

        off.close();

    }

    cout<<"All mazes are generated."<<endl;

    cout<<"Enter a maze ID between 1 to "<< K <<" inclusive to find a path: ";
    cin>>mazeID;
    cout<<"Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin>>entryY>>entryX;
    cout<<"Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin>>exitY>>exitX;


    vector<vector<Maze>> MySolvableMaze;

    Maze m2;

    Stack<Maze> PathFinderStack;



    MySolvableMaze = vec[mazeID-1];

    for (int p = 0; p < MySolvableMaze.size(); ++p) {

        for (int u = 0; u < MySolvableMaze.size(); ++u) {

            MySolvableMaze[p][u].visited = false;


        }


    }


    m2 = MySolvableMaze[entryX][entryY]; //our starting point

    m2.visited = true;

    PathFinderStack.push(m2);


    while (!(m2.x == exitX && m2.y == exitY)){ //until our current node becomes the exit node, we continue our search.
        //here we need to check the 4 directions that we can go from our starting node, (here it is m2).
        //and if we can't find any we need to pop the top node from our stack and backtrack.
        //the path generated will be unique for each maze because we also generated all mazes using a random number generator.

        // m2.x =0 m2.y = 0

        bool way = false;

        if (m2.y+1 < MySolvableMaze[0].size() && !MySolvableMaze[m2.x][m2.y+1].visited){  //checks if the top one is a border or not

            if(MySolvableMaze[m2.x][m2.y].u == 0){

                m2 = MySolvableMaze[m2.x][m2.y+1];
                //cout << m2.x <<" "<< m2.y << endl;

                MySolvableMaze[m2.x][m2.y].visited = true;

                //MySolvableMaze[m2.x][m2.y].d= 0;
                PathFinderStack.push(MySolvableMaze[m2.x][m2.y]);

                way = true;


            }



        }

        if ((m2.x+1 < MySolvableMaze.size() && !MySolvableMaze[m2.x+1][m2.y].visited) && way == false){ //checks if we can go right

            if(MySolvableMaze[m2.x][m2.y].r == 0){

                m2 = MySolvableMaze[m2.x+1][m2.y];
                //cout << m2.x << m2.y << endl;

                MySolvableMaze[m2.x][m2.y].visited = true;

                //MySolvableMaze[m2.x][m2.y].d= 0;
                PathFinderStack.push(MySolvableMaze[m2.x][m2.y]);

                way = true;


            }




        }

        if ((m2.y != 0 && !MySolvableMaze[m2.x][m2.y-1].visited) && way == false){ //checks if we can go down

            if(MySolvableMaze[m2.x][m2.y].d == 0){

                m2 = MySolvableMaze[m2.x][m2.y-1];
                //cout << m2.x << m2.y << endl;

                MySolvableMaze[m2.x][m2.y].visited = true;

                //MySolvableMaze[m2.x][m2.y].d= 0;
                PathFinderStack.push(MySolvableMaze[m2.x][m2.y]);

                way = true;

            }



        }

        if ((m2.x != 0  && !MySolvableMaze[m2.x-1][m2.y].visited) && way == false){ //checks if we can go left

            if(MySolvableMaze[m2.x][m2.y].l == 0){

                m2 = MySolvableMaze[m2.x-1][m2.y];
                //cout << m2.x << m2.y << endl;

                MySolvableMaze[m2.x][m2.y].visited = true;

               // MySolvableMaze[m2.x][m2.y].d= 0;
                PathFinderStack.push(MySolvableMaze[m2.x][m2.y]);

                way = true;


            }



        }

        if (way == false && !PathFinderStack.isEmpty()){
            //if no way is available, pops the last node m2 from our stack, and makes the remaining top our new m2.

           // if(!PathFinderStack.isEmpty()){

                PathFinderStack.pop();
                cout << "poping" << endl;

                m2 = MySolvableMaze[PathFinderStack.top().x][PathFinderStack.top().y];

                //m2.x = PathFinderStack.top().x;
                //m2.y = PathFinderStack.top().y;
                cout << "backtracking to: " << m2.x << m2.y << endl;

               // if(!PathFinderStack.isEmpty()){


                //}

           // }




        }



    }

    ofstream theEnd;

    string nameOfSolution = "maze_" + to_string(mazeID-1) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" +
            to_string(exitX) + "_" + to_string(exitY) + "_" +  ".txt";

    theEnd.open(nameOfSolution.c_str());

   /* for (int s = 0; s < MySolvableMaze.size(); s++){

        for (int f = 0; f < MySolvableMaze[0].size(); f++) {


            cout << "x=" << s << " y= " << f << endl;
            theEnd << "x=" << s << " y= " << f << " l=" << MySolvableMaze[s][f].l << " r=" << MySolvableMaze[s][f].r << " u=" << MySolvableMaze[s][f].u << " d=" << MySolvableMaze[s][f].d <<endl;
        }

    }
   */

   Stack<Maze> ReverseStack;

    while (!PathFinderStack.isEmpty()){

        Maze m3;

        m3 = PathFinderStack.top();

        ReverseStack.push(m3);

        PathFinderStack.pop();

    }





    while (!ReverseStack.isEmpty()) {
        Maze currentCoord = ReverseStack.top();
        ReverseStack.pop();

        // Print the coordinates
        cout << currentCoord.x << " " << currentCoord.y << endl;
        theEnd << currentCoord.x << " " << currentCoord.y << endl;


    }





        theEnd.close();


    //pathFinder();



    return 0;
}
