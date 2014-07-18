#include <iostream>
#include <fstream>
#include <string>

#include "zonestool.h"
#include "include\args.h"
#include "include\chapter.h"
#include "include\qpfile.h"

using namespace std;
using namespace MW_zonestool;


int main(int argc, char ** argv)
{
	// Initialize main function
	ARGS * args = new ARGS;

	args->zones = "";

	// Convert input arguments to options, and output help information if needed
	if(args_get(argc, argv, args))
	{
#ifdef Debug
        system("pause");
#endif
	    return 0;
	}

	if(zones_get(args))
	{
#ifdef Debug
        system("pause");
#endif
	    return 0;
	}

	if(zones_write(args))
	{
#ifdef Debug
        system("pause");
#endif
	    return 0;
	}

	// End of main function
	delete args;
#ifdef Debug
	system("pause");
#endif
	return 0;
}


namespace MW_zonestool {
    int stdout_help()
    {
        cout <<
            "Instruction for zonestool\n"
            "\n"
            "Function:\n"
            "    Accept a chapter and a QP file as input files, then convert the frame numbers of OP and ED to --zones parameter for x264, and output it to a text file.\n"
			"    Chapter name should be OP/Opening or ED/Ending. (not case sensitive)\n"
            "\n"
            "Usage:\n"
			"    zonestool [options] <output>\n"
			"\n"
			"General options:\n"
			"    -h, --help                List help information.\n"
			"Input/Output options:\n"
			"    -c, --chapter <string>    Specify path for chapter file. (Essential option)\n"
			"    -q, --qpfile <string>     Specify path for QP file. (Essential option)\n"
			"    -o, --output <string>     Specify path for output file. (Essential option, -o/--output can be omitted)\n"
			"    -a, --append <bool>       If the output file exists, whether to append the file or overwrite it. [true]\n"
			"Optional options:\n"
			"    -O, --op <string>         Options to be used in zones for OP\n"
			"    -E, --ed <string>         Options to be used in zones for ED\n"
			"\n";
        
        return 0;
    }


	int args_get(int argc, char ** argv, ARGS * args)
	{
		// Initialize function
		using namespace MW_args;

		int Flag = 0, HelpFlag = 0;
	    int i;
	    string * argstr = new string [argc];

		// Initialize value for options
		args->chapter = "";
		args->qpfile = "";
		args->output = "";
		args->append = true;
		args->op = "";
		args->ed = "";
	    
		// Convert pointer to "char array" to pointer to object of C++ "string" class
        for(i=0; i<argc; ++i)
        {
            argstr[i] = argv[i];
        }
        
		// Output help information when no argument specified
        if(argc<2)
        {
            stdout_help();
        
        	HelpFlag = 1;
        }

		// Loop to deal with all input arguments
        for(i=1; i<argc; ++i)
        {
            if     (argstr[i]=="-h" || argstr[i]=="--help")
        	{
                stdout_help();

				HelpFlag = 1;
        	}
			else if(argstr[i]=="-c" || argstr[i]=="--chapter")
			{
				Flag |= args_string(i, argc, argstr, args->chapter);
			}
			else if(argstr[i]=="-q" || argstr[i]=="--qpfile")
			{
				Flag |= args_string(i, argc, argstr, args->qpfile);
			}
            else if(argstr[i]=="-o" || argstr[i]=="--output")
			{
				Flag |= args_string(i, argc, argstr, args->output);
			}
            else if(argstr[i]=="-a" || argstr[i]=="--append")
			{
				Flag |= args_bool(i, argc, argstr, args->append);
			}
            else if(argstr[i]=="-O" || argstr[i]=="--op")
			{
				Flag |= args_string(i, argc, argstr, args->op);
			}
            else if(argstr[i]=="-E" || argstr[i]=="--ed")
			{
				Flag |= args_string(i, argc, argstr, args->ed);
			}
			else if(argstr[i][0]=='-')
			{
			    cout << "Unrecognized option " << argstr[i] << " caught!\n";

				Flag = 1;
			}
			else
			{
			    args->output = argstr[i];
			}
        }

		// Terminate when only help information is needed
		if(HelpFlag && argc<3) return HelpFlag;

		// Check availability of essential options
		if(args->chapter=="")
		{
		    cout << "Please specify path for chapter file with option -c/--chapter!\n";

			Flag = 1;
		}
		if(args->qpfile=="")
		{
		    cout << "Please specify path for QP file with option -q/--qpfile!\n";

			Flag = 1;
		}
		if(args->output=="")
		{
		    cout << "Please specify path for output file with option -o/--output!\n";

			Flag = 1;
		}
        
		// End of function
		delete [] argstr;

        return Flag;
	}


	int zones_get(ARGS * args)
	{
		// Initialize function
		using namespace MW_chapter;
		using namespace MW_qpfile;

		ifstream qpfile;
		int * mark = (int *)calloc(MAX_CHAP_NUM, sizeof(int));
		int chapnum, chapnumMax;

		// Process chapter file and get chapter names
		string * chapname = new string [MAX_CHAP_NUM];

		chapnumMax = ChapterName_get(args->chapter, chapname);
		if(chapnumMax<1) return 1;

		// Judge OP/ED by chapter names
		for(chapnum=1; chapnum<=chapnumMax; ++chapnum)
		{
			if     (chapname[chapnum]=="OP" || chapname[chapnum]=="OPENING")
			{
			    mark[chapnum] = 1;
			}
			else if(chapname[chapnum]=="ED" || chapname[chapnum]=="ENDING")
			{
			    mark[chapnum] = 2;
			}
			else
			{
			    mark[chapnum] = 0;
			}
		}
		
		delete [] chapname;

		// Process QP file and get frame numbers
		int * framenum = new int [MAX_CHAP_NUM];
		int zonesCount = 0;

		if(FrameNum_get(args->qpfile, framenum) < 1) return 1;

		// Generate zones string
        for(chapnum=1; chapnum<=chapnumMax; ++chapnum)
        {
            if     (mark[chapnum]==1)
        	{
        		++zonesCount;
        
        	    if(args->zones=="")
        		{
        		    args->zones = " --zones ";
        		}
        		else if(zonesCount)
        		{
        		    args->zones += "/";
        		}
        
        		args->zones += to_string(framenum[chapnum]) + ",";
        
        		if(chapnum>=chapnumMax)
        		{
        		    args->zones += to_string(framenum[chapnum]+LAST_CHAP_FRAME_COUNT-1) + ",";
        		}
        		else
        		{
        		    args->zones += to_string(framenum[chapnum+1]-1) + ",";
        		}
        
        		args->zones += args->op;
        	}
        	else if(mark[chapnum]==2)
        	{
        		++zonesCount;
        		
        	    if(args->zones=="")
        		{
        		    args->zones = " --zones ";
        		}
        		else if(zonesCount)
        		{
        		    args->zones += "/";
        		}
        
        		args->zones += to_string(framenum[chapnum]) + ",";
        
        		if(chapnum>=chapnumMax)
        		{
        		    args->zones += to_string(framenum[chapnum]+LAST_CHAP_FRAME_COUNT-1) + ",";
        		}
        		else
        		{
        		    args->zones += to_string(framenum[chapnum+1]-1) + ",";
        		}
        
        		args->zones += args->ed;
        	}
        }
        
		delete [] framenum;

		// Print zones string
		cout << "zones string has been successfully generated:\n";
		cout << args->zones << endl << endl;

		// End of function
		free(mark);

		return 0;
	}


	int zones_write(ARGS * args)
	{
		// Initialize function
		ofstream output;

		// Write zones string to output file
		if(args->append)
		{
		    output.open(args->output, ios_base::out | ios_base::app);
		}
		else
		{
		    output.open(args->output, ios_base::out | ios_base::trunc);
		}

		if(!output.is_open())
		{
		    cout << "Could not open output file \"" << args->output << "\"\n";

			return 1;
		}

		output << args->zones;
		cout << "zones string has been successfully writen to output file \"" << args->output << "\"\n\n";

		output.clear();
		output.close();

		// End of function
		return 0;
	}
}