#include "Encin.h"

namespace GamEncin
{
    string getFileContents(const char* fileName)
    {
        std::ifstream in(fileName, std::ios::binary);

        if(in)
        {
            string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        Application::GetInstance().Stop(IOErr);
    }
}
