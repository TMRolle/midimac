#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <stdlib.h>
using namespace std;

const string configFile = "devicemap.conf";

map<string,string> handlerList;

int values[5];


//function to parse list of paired controllers strings and handler programs. Handlers may optionally use $MIDIVAL[x] to get the actual values
void parseCalls(string conf) {
	string dev,handler;
	ifstream handlerFile (conf.c_str());
	if(handlerFile.is_open()){
		while(getline(handlerFile,dev)){
			if(!getline(handlerFile,handler)){
				cout << "no handler listed for " << dev << "\n";
				return;
			}
			cout << "Mapping device [" <<dev<<"]\nto call ["<<handler<<"]\n";
			handlerList[dev]=handler;
		}
	}
}

//function to find handler paired to input string
void callHandler(string event) {
	if(handlerList.count(event)){
		system((handlerList[event]+" "+to_string(values[0])+" "+to_string(values[1])+" "+to_string(values[2])+" "+to_string(values[3])+" "+to_string(values[4])).c_str());
		//cout << "EVENT: " << event <<"\n";
		//cout << "CALLED: " << handlerList[event] <<"\n";
	}
}

//function to pull all the integers out of the input line
void extractIntegerWords(string str)
{
    stringstream ss;    
 
    /* Storing the whole string into string stream */
    ss << str;
 
    /* Running loop till the end of the stream */
    string temp;
    int found;
    int count=0;
    while (!ss.eof()) {
 
        /* extracting word by word from stream */
        ss >> temp;
        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found){
        	values[count] = found;
            //cout << "Set Value " << count<< " to "<<found <<"\n";
            //cout << (("MIDIVAL"+to_string(count)+"="+to_string(found)).c_str())<<"\n";
            count++;
        }

 
        /* To save from space at the end of string */
        temp = "";
    }
}

int main()
{
	string midiIn;
	cout << "Reading device map from input file ["<<configFile<<"]\n";
	parseCalls(configFile);
	cout << "Listening for MIDI events..."<<"\n";
	while(true){
		getline(cin, midiIn);
		bool isValid = false;
		string match;
		for(auto& x : handlerList){
			regex pattern(".*"+(x.first)+".*");
		    if(regex_match(midiIn,pattern)){
		    	isValid = true;
		    	match = x.first;
		    }
		}
		if(isValid){
			extractIntegerWords(midiIn);
			callHandler(match);
		}
		
	}
	return 0;
}

