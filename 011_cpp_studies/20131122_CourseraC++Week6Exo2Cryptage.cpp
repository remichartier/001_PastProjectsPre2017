#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/************************
 * 						*
 * function decale()	*
 * 						*
 * 						*
 ***********************/
char decale( char c, char debut, int decalage ) {
	
	while( decalage < 0 ){
		decalage += 26 ;
	}
	
	return (char)(debut + (((c - debut) + decalage) % 26)) ;
	
} // end decale()


/************************
 * 						*
 * function code()		*
 * 						*
 * 						*
 ***********************/
char code(char c, int d) {
	
	if ( ( c >= 'a' ) &&  ( c <= 'z' ) ) {
		return decale(c, 'a', d  ) ;
	}
	else if ( ( c >= 'A' ) &&  ( c <= 'Z' ) ) {
		return decale(c, 'A', d  ) ;
	}
	else
		return c ;
	
} // end code


/************************
 * 						*
 * function code()		*
 * 						*
 * 						*
 ***********************/
string code(string st, int d) {
	
	string s ;
	for(size_t i=0 ; i < st.size(); i++ ){
		s.push_back(code( st[i], d ) );
	}
	return s ;
		
} // end code

/************************
 * 						*
 * function decode()	*
 * 						*
 * 						*
 ***********************/
string decode(string st, int d) {
	
	
	return code( st, -d ) ;
		
} // end code


/************************
 * 						*
 * function main()		*
 * 						*
 * 						*
 ***********************/
int main(){

	// test
	cout << "c est codé : " << code( 'c', 4 ) << endl ;
	cout << "a est codé : " << code( 'a',  4 ) << endl ;
	cout << "A est codé : " << code( 'A',  4 ) << endl ;
	cout << "Z est codé : " << code( 'Z',  4 ) << endl ;
	cout << "! est codé : " << code( '!',  4 ) << endl ;

	cout << "Fuyez manants 4 est codé : " << code( "Fuyez manants",  4 ) << " et décodé : " << decode( code( "Fuyez manants",  4 ),  4 ) << endl ;
	cout << "Fuyez manants 6 est codé : " << code( "Fuyez manants",  6 ) << " et décodé : " << decode( code( "Fuyez manants",  6 ),  6 ) << endl ;
	cout << "Fuyez manants -4 est codé : " << code( "Fuyez manants",  -4 ) << " et décodé : " << decode( code( "Fuyez manants",  -4 ),  -4 ) << endl ;
	cout << "Avez-vous vu mes 3 chats et mes 2 chiens ?  4 est codé : " << code( "Avez-vous vu mes 3 chats et mes 2 chiens ?",  4 ) << " et décodé : " << decode( code( "Avez-vous vu mes 3 chats et mes 2 chiens ?",  4 ),  4 ) << endl ;

	
	
	return 0;
}
