#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;


string CharToLower(char* rawSource)
{
    string source(rawSource);
    std::transform(source.begin(), source.end(), source.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return source;
}



int main(int argc, char** argv)
{




    return 0;
}
