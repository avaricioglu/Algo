/***********************  
*   Assignment 2       *   
*                      *
*   @Author            *
*   Ahsen Varýcýoðlu   *
*   150150059          *
***********************/

#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include <time.h>   
#include <typeinfo>
#include <vector>
#include <math.h>
#include <iomanip> //setprecision
#include <ctime>   //
#include <chrono>
using namespace std;

auto time1 = chrono::high_resolution_clock::now(); // start time 

vector<long double> taxi; 
long double x ;
long double y;
long double hotelx = 33.40819;
long double hotely = 39.19001;
int  m ;
float p;
int size = -1 ;

int parentNode(int k){  //specify index k's parent index 
	k = ( k - 1 ) / 2;
	return k;
}
int leftNode(int k){   //specify index k's left child index
	k = ( 2 * k ) + 1 ;
	return k;
}
int rightNode(int k){  //specify index k's right child index
	k = ( 2 * k ) + 2 ;
	return k;
}
void swap(long double *x,long double *y){  // swap indexis 
	long double temp;
	temp = *x;
	*x = *y ;
	*y= temp;
}
void shiftup(int k){  // shift up index if it is smaller than it's parent index value
	
	while( k>0 && taxi[parentNode(k)] > taxi[k]){
		swap(&taxi[parentNode(k)],&taxi[k]) ;
		k = parentNode(k) ; // update k for controlling index num is it in top or not
	}
}
void insert(long double newElement){ // adding element
	taxi.push_back(newElement);  // push value to the end of vector 
	
}
void extractMin(){   // for extracting minimum distance from vector 
	cout << "The distance of the called taxi : " << setprecision(10) << taxi[0] << endl ;   
	swap(&taxi[0],&taxi[size]);  // change last index and first index value
	taxi.pop_back();  // remove last index because it was called taxi
	int i = 0;      
	
	bool flag = true ; 
	while(flag){
		
		long double isleftSmall = taxi[0] - taxi[leftNode(i)] ;
	    long double isrightSmall = taxi[0] - taxi[rightNode(i)] ;
		int sleft = size - leftNode(i) ; // is there left child
		int sright = size - rightNode(i) ;  // is there right child
		
		if( sleft > 0 && sright > 0 && ( isleftSmall >0 || isrightSmall > 0 )){  
		//if there are left and right child && index 0's value is bigger than one of the left or right child
			if(taxi[leftNode(i)] < taxi[rightNode(i)]){   
			//  if left child is smaller swap with left child
				swap(&taxi[leftNode(i)] , &taxi[i]  );
				i=leftNode(i);  // update k for controlling index num is it in top or not
			}
			else{
				//  if right child is smaller swap with right child
				swap(&taxi[rightNode(i)] ,&taxi[i]);
				i=rightNode(i);  // update k for controlling index num is it in top or not
			}
		}
		else if( sleft > 0 && isleftSmall >0 ){ // if there is just left child
			swap(&taxi[leftNode(i)] , &taxi[i]  );
			i = leftNode(i);  // update k for controlling index num is it in top or not
		}
		else if( sright > 0 && isrightSmall > 0 ){ // if there is just right child
			swap(&taxi[rightNode(i)] ,&taxi[i]);
			i=rightNode(i);  // update k for controlling index num is it in top or not
		}
		else{  // if there is not node under index end loop 
			flag = false ;
		}
	}
}

int main(int argc, char *argv[]){
	
	srand(time(0));  // generating different random numbers in every compiling
  	ifstream file;
  	file.open("locations.txt");  // open file read mode
	if (!file){                  // if file can not open
		cerr << "Read File cannot be opened!";
		exit(1);
	}
    m = atoi(argv[1]) ; // m is first argument - total iteration number
	p = atof(argv[2]) ; // p is second argument - probability
	string line;
	getline(file, line); //this is the header line
	
	
	int countAdd = 0 ;  // count add operations
	int count = 0 ; //  for specifying taxi calling time
	int countUpdate = 0 ; // count update operations
	for( int i=0 ; i < m ; i++){  // loop will be return until iteration number is finished
		if(count == 100){  //is iteration num will be 100, 101 th iteration will be taxi called
			extractMin() ;
			size--;
			count = 1 ;
		}
		else if( (rand() % 10) + 1 <= (p*10) && countUpdate < countAdd){ 
		//first condtion for probability and second condition for if there is no taxi do not use update function 
		    int randIndex = (rand() % (taxi.size())) ;
		    taxi[randIndex] -= 0.01 ;
	    	shiftup(randIndex) ;
			count++ ;
			countUpdate++ ;
	    }
		else {
		    file >> x ;  // read longitude 
			file >> y ;  // read latitude
			insert(hypot(x - hotelx, y -hotely)) ; // calculate hypotenuse
			size++;
			shiftup(size); // shift up according to distance 
			count++ ;
			countAdd++ ;
		}
		
	}
	cout << "The number of taxi additions : " << countAdd << endl ;
	cout << "The number of distance updates : " << countUpdate << endl ;
	auto time2 = chrono::high_resolution_clock::now();    // end time 
	chrono::duration<double,milli> time = time2 - time1; // end time - start time 
	cout << "Runtime : " << time.count() << " milliseconds." << endl ;
	file.close();
	return 0;
}
