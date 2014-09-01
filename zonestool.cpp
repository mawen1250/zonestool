#include <iostream>
#include <fstream>
#include <string>

#include "zonestool.h"
#include "include\args.h"
#include "include\chapter.h"
#include "include\qpfile.h"


using namespace std;


int main(int argc, char ** argv)
{
    using namespace mw::zonestool;
	
    // Initialize main function
    ARG arg;

    // Convert input arguments to options, and output help information if needed
    if(args_get(argc, argv, arg))
    {
#ifdef Debug
        system("pause");
#endif
        return 0;
    }

    if(zones_get(arg))
    {
#ifdef Debug
        system("pause");
#endif
        return 0;
    }

    if(zones_write(arg))
    {
#ifdef Debug
        system("pause");
#endif
        return 0;
    }

    // End of main function
#ifdef Debug
    system("pause");
#endif
    return 0;
}


namespace mw {
namespace zonestool {

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


int args_get(int argc, char ** argv, ARG & arg)
{
    // Initialize function
    int Flag = 0, HelpFlag = 0;
    int i;
    string * args = new string[argc];

    // Convert pointer to C-style string to pointer to object of C++ "string" class
    for(i=0; i<argc; ++i)
    {
        args[i] = argv[i];
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
        if     (args[i]=="-h" || args[i]=="--help")
        {
            stdout_help();

            HelpFlag = 1;
        }
        else if(args[i]=="-c" || args[i]=="--chapter")
        {
            Flag |= args2arg(i, argc, args, arg.chapter);
        }
        else if(args[i]=="-q" || args[i]=="--qpfile")
        {
            Flag |= args2arg(i, argc, args, arg.qpfile);
        }
        else if(args[i]=="-o" || args[i]=="--output")
        {
            Flag |= args2arg(i, argc, args, arg.output);
        }
        else if(args[i]=="-a" || args[i]=="--append")
        {
            Flag |= args2arg(i, argc, args, arg.append);
        }
        else if(args[i]=="-O" || args[i]=="--op")
        {
            Flag |= args2arg(i, argc, args, arg.op);
        }
        else if(args[i]=="-E" || args[i]=="--ed")
        {
            Flag |= args2arg(i, argc, args, arg.ed);
        }
        else if(args[i][0]=='-')
        {
            cout << "Unrecognized option " << args[i] << " caught!\n";

            Flag = 1;
        }
        else
        {
            arg.output = args[i];
        }
    }

    // Terminate when only help information is needed
    if(HelpFlag && argc<3) return HelpFlag;

    // Check availability of essential options
    if(arg.chapter=="")
    {
        cout << "Please specify path for chapter file with option -c/--chapter!\n";

        Flag = 1;
    }
    if(arg.qpfile=="")
    {
        cout << "Please specify path for QP file with option -q/--qpfile!\n";

        Flag = 1;
    }
    if(arg.output=="")
    {
        cout << "Please specify path for output file with option -o/--output!\n";

        Flag = 1;
    }
    
    // End of function
    delete [] args;

    return Flag;
}


int zones_get(ARG & arg)
{
    // Initialize function
    using namespace chapter;
    using namespace qpfile;

    ifstream qpfile;
    int * mark = (int *)calloc(MAX_CHAP_NUM, sizeof(int));
    int chapnum, chapnumMax;

    // Process chapter file and get chapter names
    string * chapname = new string[MAX_CHAP_NUM];

    chapnumMax = ChapterName_get(arg.chapter, chapname);
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
    int * framenum = new int[MAX_CHAP_NUM];
    int zonesCount = 0;

    if(FrameNum_get(arg.qpfile, framenum) < 1) return 1;

    // Generate zones string
    for(chapnum=1; chapnum<=chapnumMax; ++chapnum)
    {
        if     (mark[chapnum]==1)
        {
            ++zonesCount;
    
            if(arg.zones=="")
            {
                arg.zones = " --zones ";
            }
            else if(zonesCount)
            {
                arg.zones += "/";
            }
    
            arg.zones += to_string(framenum[chapnum]) + ",";
    
            if(chapnum>=chapnumMax)
            {
                arg.zones += to_string(framenum[chapnum]+LAST_CHAP_FRAME_COUNT-1) + ",";
            }
            else
            {
                arg.zones += to_string(framenum[chapnum+1]-1) + ",";
            }
    
            arg.zones += arg.op;
        }
        else if(mark[chapnum]==2)
        {
            ++zonesCount;
            
            if(arg.zones=="")
            {
                arg.zones = " --zones ";
            }
            else if(zonesCount)
            {
                arg.zones += "/";
            }
    
            arg.zones += to_string(framenum[chapnum]) + ",";
    
            if(chapnum>=chapnumMax)
            {
                arg.zones += to_string(framenum[chapnum]+LAST_CHAP_FRAME_COUNT-1) + ",";
            }
            else
            {
                arg.zones += to_string(framenum[chapnum+1]-1) + ",";
            }
    
            arg.zones += arg.ed;
        }
    }
    
    delete [] framenum;

    // Print zones string
    cout << "zones string has been successfully generated:\n";
    cout << arg.zones << endl << endl;

    // End of function
    free(mark);

    return 0;
}


int zones_write(ARG & arg)
{
    // Initialize function
    ofstream output;

    // Write zones string to output file
    if(arg.append)
    {
        output.open(arg.output, ios_base::out | ios_base::app);
    }
    else
    {
        output.open(arg.output, ios_base::out | ios_base::trunc);
    }

    if(!output.is_open())
    {
        cout << "Could not open output file \"" << arg.output << "\"\n";

        return 1;
    }

    output << arg.zones;
    cout << "zones string has been successfully writen to output file \"" << arg.output << "\"\n\n";

    output.clear();
    output.close();

    // End of function
    return 0;
}

} // namespace zonestool
} // namespace mw
