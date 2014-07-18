#ifndef _MW_ARGS_H_
#define _MW_ARGS_H_


#include <iostream>
#include <string>
#include <algorithm>

#include <cctype>


namespace MW_args {
    inline int args_string(int & i, const int argc, std::string * argstr, std::string & args_string)
    {
        int Flag = 0;
    
        if(++i<argc)
        {
            args_string = argstr[i];
        }
        else
        {
            std::cout << "No argument specified for option " << argstr[i-1] << "!\n";
            
            Flag = 1;
        }
    
        return Flag;
    }
    
    inline int args_bool(int & i, const int argc, std::string * argstr, bool & args_bool)
    {
        int Flag = 0;
    
        if(++i<argc)
        {
            transform(argstr[i].begin(), argstr[i].end(), argstr[i].begin(), tolower);
            if(argstr[i]=="true")
            {
                args_bool = true;
            }
            else if(argstr[i]=="false")
            {
                args_bool = false;
            }
            else
            {
                std::cout << "Invalid argument specified for option " << argstr[i-1] << ", must be \"true\" or \"false\"!\n";
                
                Flag = 1;
            }
        }
        else
        {
            std::cout << "No argument specified for option " << argstr[i-1] << "!\n";
            
            Flag = 1;
        }
    
        return Flag;
    }
}


#endif