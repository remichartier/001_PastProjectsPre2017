#include <iostream>
#include <vector>
#include <string>
using namespace std;

/************************
 * 						*
 * function convert()	*
 * 						*
 * 						*
 ***********************/
 
 char convert( int n ){
	 if ( n == 1 ) { return '1' ; }
	 else
		return '0' ;
	 
 } // end convert()

/************************
 * 						*
 * function Z0()	*
 * 						*
 * 						*
 ***********************/
 
 string Z0( int n ){
	 string s="";
	 
	do{
		s.insert( 0, "0" ) ;
		s[0] = convert( n%2 )  ;

		n /= 2;
	}while (n != 0) ;
	 
	 return s ;
	
	 
 } // end Z0()
 
 /************************
 * 						*
 * function Z0t()	*
 * 						*
 * 						*
 ***********************/
 
 string Z0t( int n ){

	 string s=Z0(n) ;

	 return ( s.erase(0, 1 ) ) ; 
 } // end Z0t()
 
 /************************
 * 						*
 * function Z1()	*
 * 						*
 * 						*
 ***********************/
 
 string Z1( int n ){

	string s=Z0(n); 
	size_t sz = s.size() -1 ;
	for(size_t i = 0 ; i < sz ; i++){
		s.insert( 0, "0" ) ;
	}
	 return s ; 
 } // end Z1()
 
 /************************
 * 						*
 * function Z2()	*
 * 						*
 * 						*
 ***********************/
 
 string Z2( int n ){

	string s=Z1((Z0(n)).size()); 

	s.append(Z0t(n ));
	
	
	
	 return s ; 
 } // end Z2()
 
 
 
/************************
 * 						*
 * function main()		*
 * 						*
 * 						*
 ***********************/
int main(){
	
	// Test Z0()
	cout << "Test Z0 () : " ;
	for ( int i = 255; i <= 255 ; i++ ) {
		cout << Z0(i) << "; " ;
	}
	cout << endl ;
	
	// Test Z0t()
	cout << "Test Z0t () : " ;
	for ( int i = 255; i <= 255 ; i++ ) {
		cout << Z0t(i) << "; " ;
	}
	cout << endl ;
	
	// Test Z1()
	cout << "Test Z1 () : " ;
	for ( int i = 255; i <= 255 ; i++ ) {
		cout << Z1(i) << "; " ;
	}
	cout << endl ;
	
	// Test Z2()
	cout << "Test Z2 () : " ;
	for ( int i = 255; i <= 255 ; i++ ) {
		cout << Z2(i) << "; " ;
	}
	cout << endl ;
	
	
	
	return 0;
}
