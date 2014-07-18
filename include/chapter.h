#ifndef _MW_CHAPTER_H_
#define _MW_CHAPTER_H_


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <cctype>


namespace MW_chapter {
	inline int ChapterName_get(const std::string chapter_file, std::string * chapname)
	{
		// Initialize function
		int i;
		std::ifstream chapter;
		std::string line = "";
		char ch;
		int chapnum, chapnumMax = 0;
		unsigned int pos1, pos2;

		// Open chapter file
		chapter.open(chapter_file);
		
		if(!chapter.is_open())
		{
		    std::cout << "Could not open chapter file \"" << chapter_file << "\"\n";

			return 0;
		}

		// Get chapter names
		while(getline(chapter, line))
		{
			if(line.size()<=12) continue;

		    transform(line.begin(), line.end(), line.begin(), toupper);

			pos1 = line.find("CHAPTER", 0);
			pos2 = line.find("NAME", 0);
			
			if(pos1<line.size() && pos2<line.size())
			{
			    std::string chapnum_str(line, pos1+7, pos2-(pos1+7));
				chapnum = stoi(chapnum_str);

				i = 0;
				while((ch=line[i]) != '\0')
				{
				    ++i;
					if(ch == '=')
					{
						chapnumMax = std::max(chapnumMax, chapnum);
						chapname[chapnum] = "";
					    while((ch=line[i]) != '\0')
						{
							++i;
							chapname[chapnum] += ch;
						}
					}
				}
			}
		}

		chapter.clear();
		chapter.close();

		// return max number of chapter
		return chapnumMax;
	}
}


#endif