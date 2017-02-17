#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
using namespace std;

const int w=50;
const int h=50;

int board[h][w];
bool explored[h][w];


int getRandInt(int,int);
bool getPath(int,int,int,int);
void printBoard();
void printBoard2();

int main(){
	srandom(time(NULL));
	printBoard();
	for(int i=0;i<1000;i++){
		board[getRandInt(0,49)][getRandInt(0,49)]++;
	}

	//Reset start and end
	board[0][0]=0;
	board[49][49]=0;

	getPath(0,0,49,49);
	printBoard();

	return 0;
}

void printBoard(){
	cout << "\033[2J\033[1;1H";	
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(board[i][j]<0)
				cout<<"* ";
			else if(board[i][j]==0)
				cout<<"  ";
			else
				cout<<"# ";
		}
		cout<<"\n";

	}
	usleep(80000);
}

void printBoard2(){
	cout << "\033[2J\033[1;1H";	
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(board[i][j]==0){
				if(explored[i][j])
					cout<<"* ";
				else
					cout<<"  ";	
			}else if(board[i][j]>0)
				cout<<"# ";
		}
		cout<<"\n";

	}
	usleep(80000);
}
// Floor to int
int getRandInt(int start, int end){
	double s=start;
	double e=end;
	double r=random();
	double fraction=r/RAND_MAX;
	double randDouble = s + (e-s+1)*fraction;
	return randDouble;
}

//Path sr->start row, sc->start coloumn
bool getPath(int sr,int sc,int er,int ec){
	if(sr>=0 && sc>=0 && sr<h && sc<w && !explored[sr][sc] && board[sr][sc]<=0){
		explored[sr][sc]=true;
		printBoard2();
		if(sr==er && sc==ec){
			board[sr][sc]=-1;
			return true;
		}else{
			if(getPath(sr-1,sc,er,ec)){
				board[sr][sc]=-1;
				return true;
			}

			if(getPath(sr-1,sc+1,er,ec)){
				board[sr][sc]=-1;
				return true;
			}

                        if(getPath(sr,sc+1,er,ec)){
				board[sr][sc]=-1;
                                return true;
                        }

			if(getPath(sr+1,sc+1,er,ec)){
                                board[sr][sc]=-1;
                                return true;
                        }

                        if(getPath(sr+1,sc,er,ec)){
				board[sr][sc]=-1;
                                return true;
                        }

			if(getPath(sr+1,sc-1,er,ec)){
                                board[sr][sc]=-1;
                                return true;
                        }

                        if(getPath(sr,sc-1,er,ec)){
				board[sr][sc]=-1;
                                return true;
			}

			if(getPath(sr-1,sc-1,er,ec)){
                                board[sr][sc]=-1;
                                return true;
                        }
		}
	}
	return false;
}


