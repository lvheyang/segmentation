/**
 * test FullBitStringTable class
 *
 */

#include<iostream>
#include<fstream>
#include<string>

#include"FullBitStringTable.h"
#include"FullInverseTable.h"
#include"FullCompositionTable.h"
#include"LanguageModel.h"
#include"Constriant.h"

using namespace std;


void examine1( const int, BitStringTable *, CompositionTable * );
void examine2( const int, BitStringTable *, InverseTable *, CompositionTable * );
bool loopR( int *, int );
bool loopr( int *, int );

/**
 * the set of relations occured in fma.csp
 */
int relation_set[] = { 2, 3, 24, 29, 96, 101, 128, 130, 156, 228, };
int relation_size = 10;

int
main() {


	FullBitStringTable fbst( 8,"/home/cwz/Desktop/segmentation/rcc8/rcc8.comp"  ); 
	FullInverseTable fit( &fbst, "/home/cwz/Desktop/segmentation/rcc8/rcc8.conv");
	FullCompositionTable fct ( &fbst, "/home/cwz/Desktop/segmentation/rcc8/rcc8.comp" );
	

	//LanguageModel lm( &fct, &fit, &fbst);

	examine1(3, &fbst ,&fct);
	examine2(3, &fbst, &fit, &fct);

	return 0;
}

void
exm_isolate_vex( BitStringTable *bt, InverseTable *it, CompositionTable *ct ) {
	ofstream outfile("d_i_v");
    
	for( int i = 1 ; i < 256 ; i++) {
		for(int j = 1; j< 256 ; j++ ) {
			if(ct->composition(i,j)==255)
				outfile << i << " " << j << " : " << bt->getString(i) << " " << getString(j) << endl;
		}
	}
	
}

void
examine2( const int r_num, BitStringTable *bt, InverseTable *it, CompositionTable *ct ) {
	ofstream outfile("3_2.txt");
	int *p_R = new int[r_num];

	for( int i = 0; i < r_num; i++)
		*(p_R+i) = relation_set[0];

	do {
		bool good = true;
		for( int i = 0; i < r_num ; i++ ) {
			if(!good)
				break;
			for( int j = i + 1; j < r_num; j++ ) {
				if(!good)
					break;
				for( int r = 1; r <= 128; r <<= 1) {
					if( !((  ct->composite(ct->composite(*(p_R+i), r ) , it->getInverse( *(p_R+j)) ) == 255 )\
								&& (ct->composite(ct->composite(it->getInverse(*(p_R+i)), r) , *(p_R+j) )) == 255)) {
						good = false;
						break;
					}
				}
			}
		}
		if(good) {
	       		for(int i = 0; i < r_num ; i++) {
		       		outfile<< *(p_R+i) << "(" << bt->getString(*(p_R+i)) << ") ";
	       		}
	       		outfile << endl;
		}
	}while(loopR(p_R, r_num));


	delete [] p_R;
	outfile.close();
}

void
examine1( const int r_num , BitStringTable *bt  ,CompositionTable *ct ) {
	int num2 = r_num * 2;
    ofstream outfile1("3_1.txt");

	int *p_R = new int[r_num];
	int *p_r = new int[num2];
	
	for( int i = 0; i < r_num ; i++ ) 
		*(p_R+i) = relation_set[0];
	for( int i = 0; i < num2; i++ )
		*(p_r+i) = 1;

	do {
		bool good = true;

		do{
			int result = 255;	
			for( int i = 0 ; i < r_num; i++ ) {
				result &= ct->composite(ct->composite(*(p_r+(2*i)), *(p_R+i)), *(p_r+(2*i)+1));
				if( !result ) {
					good = false;
					break;
				}
			}	
		}while(loopr( p_r, num2));

		if(good) {
		       for(int i = 0; i < r_num ; i++) {
			       outfile1<< *(p_R+i) << "(" << bt->getString(*(p_R+i)) << ") ";
		       }
		       outfile1 << endl;
		}	       
	}while(loopR(p_R, r_num));

	delete [] p_R;
	delete [] p_r;

	outfile1.close();
}


bool
loopR(int *p , int num ) {
	int i = num - 1;
	int last = relation_set[relation_size - 1];
	int first = relation_set[0];
	
	while( i >= 0 ) {
		if( *(p+i) == last ) { 
			*(p+i) = first;
			i--;
		} else {
			int k = 0;
			int r = *(p+i);
			while( relation_set[k] != r ) {
				k++;
			}
			k++;
			*(p+i) = relation_set[k];

			return true;
		}
	}

	return false;
}


bool
loopr( int *p , int num ) {
	int i = num - 1;

	while( i >= 0) {
		if( *(p+i) == 128 ) {
			*(p+i) = 1;
			i--;
		} else {
			(*(p+i)) <<= 1;
			return true;
		}
	}
	return false;
}
