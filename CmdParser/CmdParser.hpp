
/* Command Line Parser */

#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

using namespace std;

class CmdParser {

	private:
	    int opt;
	    string input;
	    bool flagB;

    public:
	    bool flagW;	
    	CmdParser(int argc, char **argv);

};


#endif
