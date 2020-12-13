#include<iostream>
#include<string.h>
#include<stdlib.h>

#include"icueToAhk.h"

int main( int argc, char *argv[]){
	if( argc < 2){
		printErrorCloseFStreams(   "enter icue filename as argument" );
		
	}

	char *filename = argv[1];
	FILE *original = fopen(filename, "r");
	if( original == NULL ){
		printErrorCloseFStreams( "file could not be opened"  );
	}

	FILE *output = fopen("output.txt", "w");
	if( output == NULL ){
		printErrorCloseFStreams(  "output file could not be created/opened ", original );
	}

	if( getToBeginning(original) == false ){
		printErrorCloseFStreams(  "could not find beginning ", original, output );
	}
	ProcessAllMacros(original, output);
	
	


	std::cout << "end of program, closing fstreams"<< std::endl;
	closeFStreams( original, output);
	return 0;
}
