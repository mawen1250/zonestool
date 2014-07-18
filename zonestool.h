#ifndef _MW_ZONESTOOL_H_
#define _MW_ZONESTOOL_H_


#include <string>


//#define Debug

const int MAX_CHAP_NUM = 1024;
const int LAST_CHAP_FRAME_COUNT = 5000;


namespace MW_zonestool {
	struct ARGS {
		// Input
		std::string chapter;
		std::string qpfile;
		std::string output;
		bool append;
		std::string op;
		std::string ed;
		// Output
		std::string zones;
	};
	
    int stdout_help();
	int args_get(int argc, char ** argv, ARGS * args);

	int zones_get(ARGS * args);
	int zones_write(ARGS * args);
}


#endif