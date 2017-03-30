#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<sys/ioctl.h>
using namespace std;

const int w=10000;
const int h=5000;
const int nfill=2000;

int board[h][w];
bool explored[h][w];

double golden_ratio=1.6180339887498948482;

int getRandInt(int,int);
bool getPath(int,int,int,int);
void printBoard();
void printBoard2();
double dist(double,double,double,double);
struct Node{
	Node* left;
	Node* right;
	double distance;
	double distcovered;
	int x,y;
	//Parent on board not tree
	Node* parent;
	Node(int x,int y, double w){
		this->x=x;
		this->y=y;
		this->distance=w;
		this->parent=NULL;
		this->distcovered=0;
	}

	Node(int x,int y, double w, Node* p){
                this->x=x;
                this->y=y;
                this->distance=w;
                this->parent=p;
		this->distcovered=p->distcovered+1;
        }
};

class Bst{
public:
	Node* head;

	Bst(){
		head=NULL;
	}

	void insert(int x,int y, double w){

		if(head==NULL){
			head=new Node(x,y,w);
			return;
		}
		Node *curr=head;
		Node *prev=NULL;
		while(curr!=NULL){
			prev=curr;
			if(w<= curr->distance)
				curr=curr->left;
			else if(w>curr->distance)
				curr=curr->right;
		}

		if(w<= prev->distance)
			prev->left=new Node(x,y,w);
		else
			prev->right=new Node(x,y,w);

	}

	void insert(int x,int y, double w, Node* p){

                if(head==NULL){
                        head=new Node(x,y,w,p);
                        return;
                }
                Node *curr=head;
                Node *prev=NULL;
                while(curr!=NULL){
                        prev=curr;
                        if(w<= curr->distance)
                                curr=curr->left;
                        else if(w>curr->distance)
                                curr=curr->right;
                }

                if(w<= prev->distance)
                        prev->left=new Node(x,y,w,p);
                else
                        prev->right=new Node(x,y,w,p);

        }

	void print(Node *curr){
		if(curr!=NULL){
			if(curr->left!=NULL)
				print(curr->left);
			cout<<curr->distance<<" ";
			if(curr->right!=NULL)
				print(curr->right);
		}
	}
	
	bool isEmpty(){
		if(head==NULL)
		return true;
	}

	Node* popLow(){
		Node* curr=head;
		Node* prev=NULL;
		if(head==NULL)
			return head;
		while(curr->left!=NULL){
			prev=curr;
			curr=curr->left;
		}
		if(prev!=NULL)
			prev->left=curr->right;
		else
			head=head->right;;
		return curr;
	}
};

Bst bst;
int startc=0;
int startr=0;
int endc=w-1;
int endr=h-1;
//int endr=32;
int main(){
	srandom(time(NULL));
	//struct winsize ws;
	//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	//w=ws.ws_col;
	//h=ws.ws_row;
	//startc=0;
	//startr=0;
	//endc=w-1;
	//endr=h-1;
	//board=new int[h][w]
	//explored=new bool[h][w];
	//printBoard();
	for(int i=0;i<nfill;i++){
		board[getRandInt(0,h-1)][getRandInt(0,w-1)]++;
	}

	//Empty start and end
	board[startr][startc]=0;
	board[endr][endc]=0;
	bst.insert(0,0,dist(startc,startr,endr,endc));
	getPath(startc,startr,endr,endc);
	printBoard2();
	return 0;
}

void printBoard(){
	cout << "\033[2J\033[1;1H";	
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(board[i][j]<0)
				cout<<"\033[32m* ";
			else if(board[i][j]==0)
				cout<<"  ";
			else
				cout<<"# ";
		}
		cout<<"\n";

	}
	sleep(1);
}

void printBoard2(){
	cout << "\033[2J\033[1;1H";	
	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(board[i][j]==0){
				if(explored[i][j])
					cout<<"\033[32m* ";
				else
					cout<<"\033[39m  ";	
			}else if(board[i][j]>0)
				cout<<"\033[31m# ";
			else
				cout<<"\033[34m@ ";
		}
		cout<<"\n";

	}
	usleep(200000);
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

double dist(double xs,double ys,double xe,double ye){
	return abs(xs-xe) + abs(ys-ye);
}

//Path sr->start row, sc->start coloumn
bool getPath(int sr,int sc,int er,int ec){
	while(!bst.isEmpty()){
		Node* curr=bst.popLow();
		if(curr->y==er && curr->x==ec){
			while(curr!=NULL){
				board[curr->y][curr->x]=-1;
				curr=curr->parent;
			}
			return true;
		}
		//cout<<curr->y<<" "<<curr->x<<" "<<curr->distance<<"\n";
		if(curr->x>=0 && curr->x<w && curr->y>=0 && curr->y<h && !explored[curr->y][curr->x] && board[curr->y][curr->x]==0){
			explored[curr->y][curr->x]=true;
			//printBoard2(); //Uncomment to print progress
			bst.insert(curr->x+1,curr->y,golden_ratio*dist(curr->x+1,curr->y,ec,er)+curr->distcovered,curr);
			bst.insert(curr->x,curr->y+1,golden_ratio*dist(curr->x,curr->y+1,ec,er)+curr->distcovered,curr);
			bst.insert(curr->x-1,curr->y,golden_ratio*dist(curr->x-1,curr->y,ec,er)+curr->distcovered,curr);
			bst.insert(curr->x,curr->y-1,golden_ratio*dist(curr->x,curr->y-1,ec,er)+curr->distcovered,curr);
			/*
			if(getPath(curr->y+1,curr->x,er,ec)){
				board[curr->y][curr->x]=-1;
				return true;
			}
			if(getPath(curr->y-1,curr->x,er,ec)){
                                board[curr->y][curr->x]=-1;
                                return true;
                        }
			if(getPath(curr->y,curr->x+1,er,ec)){
                                board[curr->y][curr->x]=-1;
                                return true;
                        }
			if(getPath(curr->y+1,curr->x-1,er,ec)){
                                board[curr->y][curr->x]=-1;
                                return true;
                        }
			*/
		}
	}
	return false;
}



