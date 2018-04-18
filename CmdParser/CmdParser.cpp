


#include "CmdParser.hpp"

CmdParser::CmdParser(int argc, char** argv) {

	input = "";
	flagW = false;
	flagB = false;	

    printf("Command Line:\n");	

    // Retrieve the (non-option) argument:
    if ( (argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) {  // there is NO input...
    	printf("\tno input\n");
        //return 1;
    }
    else {  // there is an input...
        input = argv[argc-1];
	    // Debug:
	    cout << "\tinput: " << input << endl;        
    }


    // Shut GetOpt error messages down (return '?'): 
    opterr = 0;

    // Retrieve the options:
    while ( (opt = getopt(argc, argv, "bw")) != -1 ) {  // for each option...
        switch ( opt ) {
            case 'w': // GTK window
                    flagW = true;
                	break;
            case 'b':
                    flagB = true;
                	break;
            case '?':  // unknown option...
		    		//printf("\tunknown option: %c\n",optopt);
                    //cerr << "Unknown option: '" << char(optopt) << "'!" << endl;
                	break;
        }
    }

    // Debug:
    printf("\tflagW: %d\n",flagW);
    printf("\tflagB: %d\n",flagB);

}