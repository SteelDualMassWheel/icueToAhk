
#include "icueToAhk.h"
///////////////////////////////////////////////////
bool getToBeginning( FILE *original ){
	char buffer[N]  = "";
	bool found = false;
	const char *actionsSizeDynamic = "<actions size=\"dynamic\">";
	while( fgets( buffer, N, original) != NULL ){
		removeTabs(buffer);
		if( strcmp( buffer, actionsSizeDynamic ) == 0 ){
			found = true;
			break;
		}	
	}
	return found;
}
///////////////////////////////////////////////////
void removeTabs( char *cstring ){
	int tabCounter = 0;
	//this counts amount of tabs
	while( cstring[tabCounter] == '\t' ){
		tabCounter++;
	}
	int i = 0;
	while( cstring[tabCounter + i] != '\0'){
		cstring[i] = cstring[tabCounter + i];
		i++;
	}
	cstring[i-2] = '\0'; 
}
///////////////////////////////////////////////////
bool findNameLine( char buffer[], FILE *original, char *result){
	bool found = false;
	const char *name = "<name>";
	while( fgets( buffer, N, original) != NULL){
		if( strstr( buffer, name) !=NULL ){
			found = true;
			strcpy(result,buffer);
			break;
		}
	}
	return found;
}

///////////////////////////////////////////////////
void extractAngleBrackets( const char *line, char *result){
	int i = 0;
	while( line[i] != '>' && line[i] != '\0'){
		i++;
	}
	int j = i;
	while( line[j] != '<' && line[j] != '\0'){
		j++;
	}
	i++;
	int n = j - i;
	for( int k = 0; k < n; k++){
		result[k] = line[k + i];
	}
	result[n] = '\0';
}

///////////////////////////////////////////////////
void ProcessAllMacros( FILE *original, FILE *output){
	std::cout << "start of processAllMacros" << std::endl;
	int events[K] = {0};
	char eventsNames[K][N];
	
	//while( processMacro( original, output, events, eventsNames) == true );
	processMacro( original, output, events, eventsNames);
	for( int i = 0; i < K; i++){
		std::cout << "events[" << i << "] = " << events[i] << std::endl;
		std::cout << "eventsNames[" << i << "] = " << eventsNames[i] << std::endl;
	}

	std::cout << "end   of processAllMacros" << std::endl;

}

///////////////////////////////////////////////////
bool processMacro( FILE *original, FILE *output, int events[], 
		char eventsNames[][N] ){
	std::cout << "start of processMacro" << std::endl;
	char buffer[N] = "";
	char nameLine[N] = "";
	
	if ( findNameLine(buffer, original, nameLine) == false ){
		return false;
	}
	
	char name[N] = "";
	//std::cout << "nameline = " << nameLine << std::endl;
	extractAngleBrackets(nameLine, name);
	
	fprintf( output, "; Macro name: %s\n", name );
	// this means that current macro has ended
	if( findEventSizeDynamic( original, output ) == false){
		return false;
	}

	while( getKey( original, output, events, eventsNames) == true );
	
	std::cout << "end   of processMacro" << std::endl;
	return true;
}
///////////////////////////////////////////////////
void printErrorCloseFStreams(const char *errorMsg,
		FILE *original, FILE *output){
	std::cout << "error " << errorMsg << std::endl;
	fprintf( stderr, errorMsg);
	exit(1);
}
void closeFStreams( FILE *original, FILE *output){
	if( original != NULL ) fclose(original);
	if( output != NULL ) fclose(output);
}

// this will returnFalse if encounters <name>
// and will close the program if encounters </actions>
bool findEventSizeDynamic( FILE *original, FILE *output){
	char buffer[N] = "";
	bool found = false;
	const char *eventsSizeDynamic = "<events size=\"dynamic\">";
	const char *name = "<name>";
	const char *endOfActions = "</actions>";
	while( fgets( buffer, N, original) != NULL ){
		removeTabs( buffer );
		if( strcmp( buffer, eventsSizeDynamic) == 0 ){
			std::cout << buffer << std::endl;
			found = true;
			break;
		}
		if( strstr(buffer, name ) != NULL ){
			std::cout << "name was encountered before events size dynamic" << std::endl;
			break;
		}
		if( strcmp(buffer, endOfActions) == 0 ){
			std::cout << "End of macros has been reached"
			       << "program executed successfully" << std::endl;	
			closeFStreams( original);
				exit(0);
		}
	}
	return found;
}

// saves found keystrokes to output, updates events and eventsNames
// k means amount of elements in events[]
// 28 is the first index of event from icue file
// each new event is assigned 28 + i, where i = amount of new events encountered
bool getKey( FILE *original, FILE *output, int events[],
	 	char eventsNames[][N] ){
	std::cout << "start of getKeys" << std::endl;
	char buffer[N];
	// following line always returns -1 as error
	int id = findGetPolymorphicId(original);
	if( id == -2 ){
		std::cout << "END OF MACRO " << std::endl;
		return false;
	}
	std::cout << "id = " << id << std::endl;

	
	if( fgets( buffer, N, original) == NULL){
		std::cout << " could not parse <polymorphic_name>"<< std::endl;
	}
	
	bool newEvent = isNewEvent( buffer );
	char eventName[N];
	if( newEvent ){
		extractAngleBrackets(buffer, eventName);
		int i = 0;
		while( i < K){
			if( events[i] == 0 ) break;
			i++;
		}	
		events[i] = 28 + i; 
		strcpy( eventsNames[i], eventName );	
		std::cout << eventsNames[i] << std::endl;	
	}else{
		int index = getIndexFromEventsArray( id, events);
		if( index == -1){
			std::cout << "invalid index" << std::endl;
			return false;
		}
		strcpy( eventName, eventsNames[index]);
		fprintf( output, "; %s\n", eventName );
		// TODO 
		if( strcmp( eventName, "KeyboardMacroActionEvent" ) == 0 ){
			
		}else if(  strcmp( eventName, "DelayMacroActionEvent" ) == 0  ) {
			
		}else if(  strcmp( eventName, "MouseButtonMacroActionEvent" ) == 0  ){
		
		}else if(  strcmp( eventName, "MouseMoveActionEvent" ) == 0  ){
			
		}else{
			std::cout << "!!!found action is different that assumed" << std::endl;
		}


	
	
	}	
	std::cout << "end of getKeys" << std::endl;
	return true;
}

int getIndexFromEventsArray( int id, int events[]){
	for( int i = 0; i < K; i++ ){
		if( id == events[i] ){
			return i;
		}
	}
	return -1; // means that the id could not been found in events array
}

///////////////////////////////////////////////////
int findGetPolymorphicId( FILE *original ){
	char buffer[N] = "";
	int id = -2; // means not found
	const char polymorphicId[] = "<polymorphic_id>";
	const char endEvent[] = "</events>";
	while( fgets( buffer, N, original) != NULL ){
		removeTabs(buffer);
		//std::cout << buffer << std::endl;
		if( strstr( buffer, polymorphicId) != NULL ){
			id = extractId(buffer);
			return id;
		}
		if( strcmp( buffer, endEvent) == 0 ){
			return -2;
		}
	}
	std::cout << "returning id =" << id << std::endl;
	return id;
}

///////////////////////////////////////////////////
int extractId( const char *line){
	char result[N] = "";
	extractAngleBrackets( line, result);
	return atoi(result);
}

///////////////////////////////////////////////////
bool isNewEvent( char buffer[N] ){
	char polymorphicName[] = "<polymorphic_name>";
	if( strstr( buffer, polymorphicName) != NULL){
		return true;
	}
	return false;
}

