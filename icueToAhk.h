#pragma once

#include<iostream>
#include<stdlib.h>
#include<string.h>
#define N 200
#define K 4 // amount of events:
/*
 * keystroke
 * delay
 * mouseButton
 * mouseMove
 * */

// this gets to the beginning of macros
bool getToBeginning( FILE *original );

// this removes prefix tabs from cstring
void removeTabs( char *cstring );

// this finds line that contains <name>
bool findNameLine( char buffer[], FILE *original, char *result);

// line must be in format <>"stuff"<>
void extractAngleBrackets( const char *line, char *result);

// this uses processMacro
void ProcessAllMacros( FILE *original, FILE *output);

// this is used by processMacro
bool processMacro( FILE *original, FILE *output, int events[], 
		char eventsNames[][N] );


// prints errorMsg, closes FILE streams
void printErrorCloseFStreams(const char *errorMsg,
		FILE *original = NULL, FILE *output = NULL );

// simply closing both fstreams
void closeFStreams( FILE *original = NULL, FILE *output = NULL);


// this will stop if encounters <name>
bool findEventSizeDynamic( FILE *original, FILE *output);

// saves found keystrokes to output, updates events and eventsNames
// k means amount of elements in events[]
// 28 is the first index of event from icue file
// each new event is assigned 28 + i, where i = amount of new events encountered
bool getKey( FILE *original, FILE *output, int events[],
	 	char eventsNames[][N] );


// returns number from 0 ... K
int getIndexFromEventsArray( int id, int events[]);

// searches file for <polymorphic_id>
int findGetPolymorphicId( FILE *original );

// uses extractAngleBrackets, returns int as atoi(cstring)
int extractId( const char *line);

// this should be executed after function extractId, to check if the event is new( not encountered before )
bool isNewEvent( char buffer[N] );
