#include <iostream>
#include <vector>

using namespace std;

// Notez également que si le tableau est vide ou ne contient que des 0, le résultat sera le même tableau que celui de départ (vide ou tout nul).

 /***************************************
 * 										*
 * function somme_consecutifs_max()		*
 * 										*
 * 										*
 ***************************************/
 int somme_consecutifs_max( vector<int> tab ) {
	 
	 int sum(0), max(0) ;
	 
	 for ( size_t i = 0 ; i < tab.size() ; i++ ) {
		 
		 if (tab[i] != 0 ){ sum += tab[i] ;}
		 else{
			 if (sum>max){
				 max=sum;
			 }
			 sum=0;
		 } 
	 }
	if (sum>max){
		max=sum;
	}
			 
	return max;
 } // somme_consecutifs_max()
 
 
  /***************************************
 * 										*
 * function lignes_max()		*
 * 										*
 * 										*
 ***************************************/
 vector<size_t> lignes_max( vector<vector<int>> tab ) {
	 vector<size_t> tresult(0) ;
	 
	 // use somme_consecutifs_max
	 int max(0) ;
	 
	 for (size_t i=0 ; i<tab.size(); i++){
		 
		 int sum = somme_consecutifs_max( tab[i] ) ;
		 if (sum>max){
			 tresult.clear();
			 tresult.push_back(i);
			 max=sum;
		 }
		 else if (sum==max){
			 tresult.push_back(i);
		 }
		 
	 }
	 
	 return tresult ;
	 
	 
 } // end lignes_max()
 
 
 
 /***************************************
 * 										*
 * function tranches_max()		*
 * 										*
 * 										*
 ***************************************/
 vector<vector<int>> tranches_max( vector<vector<int>> tab ) {
	 
	 vector<vector<int>> tresult(0) ;
	 
	 // use somme_consecutifs_max / lignes_max
	 
	 vector<size_t> lignesMax = lignes_max(tab) ;
	 
	 if (!lignesMax.empty())
	 {
		 for (size_t i = 0 ; i < lignesMax.size() ; i++){
			 
			 tresult.push_back(tab[lignesMax[i]]) ;
		 }
	 }
	 
	 return tresult ;
	 
 } // end tranches_max()
 
 
/************************
 * 						*
 * function main()		*
 * 						*
 * 						*
 ***********************/
int main(){
	
	// Notez également que si le tableau est vide ou ne contient que des 0, le résultat sera le même tableau que celui de départ (vide ou tout nul).	
	
	// test somme_consecutifs_max
	vector<int> tab1 = { 0, 2, 2, 0 } ;
	cout << "test somme_consecutifs_max {" ;
	for(auto element : tab1){
		cout << element << ", " ;
	} 
	cout <<  "} : " << (int)(somme_consecutifs_max(tab1)) << endl; 
	
	tab1.clear();
	tab1 = { 2, 3, 0, 0, 4 }; 
	cout << "test somme_consecutifs_max {" ;
	for(auto element : tab1){
		cout << element << ", " ;
	} 
	cout <<  "} : " << somme_consecutifs_max(tab1) << endl ; 
	
		
	tab1.clear();
	tab1 = { 4, 0, 2, 3 } ;
		cout << "test somme_consecutifs_max {" ;
	for(auto element : tab1){
		cout << element << ", " ;
	} 
	cout <<  "} : " << somme_consecutifs_max(tab1) << endl ; 
	
	
	// test lignes_max()
	cout << "Test lignes_max() function " ;
	//vector<vector<int>> tab2 = { {2,1,0,2},{0,1,0,3},{1,3,0,0},{0,2,2,0}};
	
	
	//vector<vector<int>> tab2 = {{2,1,0,2,0,3,2},	{0,1,0,7,0},	{1,0,1,3,2,0,3,0,4},	{5,0,5},	{1,1,1,1,1,1,1}};
	//vector<vector<int>> tab2 = {{2,1,0,2,0,3,2},	{},	{1,0,1,3,2,0,3,0,4},	{5,0,5},	{1,1,1,1,1,1,1}};
	//vector<vector<int>> tab2 = {{2,1,0,2,0,3,2},	{2,1,0,2,0,3,2},	{2,1,0,2,0,3,2},	{2,1,0,2,0,3,2},	{2,1,0,2,0,3,2}};
	//tab2.clear();
	vector<vector<int>> tab2 = {{2,1,3,2,3,3,2},	{3,1,3,7,3},	{1,3,1,3,2,3,3,3,4},	{5,4,5},	{1,1,1,1,1,1,1}};
	
	vector<size_t> tab11 = lignes_max(tab2) ;
	for(auto element : tab11){
		cout << element << ", " ;
	} 
	
	// test tranches_max
	cout << "Test tranches_max() function " ;
	vector<vector<int>> tab3 ;
	tab3 = tranches_max(tab2) ;
	cout << "{ " ;
	for(size_t j=0 ; j<tab3.size() ; j++){
		cout << "{ " ;
		for(size_t i=0 ; i<(tab3[j]).size() ; i++){
			cout << tab3[j][i] << ", " ;
		}
		cout << "}, " ;
	} 
	cout << " }" << endl ;
	
	return 0;
}
