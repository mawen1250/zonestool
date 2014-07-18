#ifndef _MW_QPFILE_H_
#define _MW_QPFILE_H_


#include <iostream>
#include <fstream>
#include <string>


namespace MW_qpfile {
    inline int FrameNum_get(const std::string QP_file, int * framenum)
    {
        // Initialize function
        int i;
        std::ifstream qpfile;
        std::string line = "";
        char ch;
        int chapnum = 0;
        
        // Open QP file
        qpfile.open(QP_file);
        
        if(!qpfile.is_open())
        {
            std::cout << "Could not open QP file \"" << QP_file << "\"\n";

            return 0;
        }
        
        // Get frame numbers
        while(getline(qpfile, line))
        {
            if(line.size()==0) continue;

            std::string framenum_str = "";

            i = 0;
            while((ch=line[i]) != '\0')
            {
                ++i;
                if(isdigit(ch))
                {
                    framenum_str += ch;

                    while(((ch=line[i]) != '\0') && isdigit(ch))
                    {
                        ++i;
                        framenum_str += ch;
                    }
                    framenum[++chapnum] = stoi(framenum_str);
                    
                    break;
                }
            }
        }
        
        qpfile.clear();
        qpfile.close();

        return chapnum;
    }
}


#endif