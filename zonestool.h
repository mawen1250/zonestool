#ifndef MW_ZONESTOOL_H_
#define MW_ZONESTOOL_H_


#include <string>


//#define Debug

const int MAX_CHAP_NUM = 1024;
const int LAST_CHAP_FRAME_COUNT = 5000;


namespace mw {
namespace zonestool {

struct ARG {
    // Input
    std::string chapter = "";
    std::string qpfile = "";
    std::string output = "";
    bool append = true;
    std::string op = "";
    std::string ed = "";
    // Output
    std::string zones = "";
};

int stdout_help();
int args_get(int argc, char ** argv, ARG & arg);

int zones_get(ARG & arg);
int zones_write(ARG & arg);

} // namespace zonestool
} // namespace mw


#endif