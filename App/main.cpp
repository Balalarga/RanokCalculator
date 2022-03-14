#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#include <Ranok/Core/Utils/FlatArray.h>

using namespace std;


string CharToLower(char* rawSource)
{
    string source(rawSource);
    std::transform(source.begin(), source.end(), source.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return source;
}

int NextErrorCode()
{
    static int code = 0;
    return --code;
}


int main(int argc, char** argv)
{
    std::string codePath = "..\\..\\Examples\\NewFuncs\\sphere.txt";

    fstream codeFile(codePath);
    if (!codeFile)
    {
        cout << "Couldn't open file " << codePath << endl;
        return NextErrorCode();
    }
    stringstream codeStream;
    codeStream << codeFile.rdbuf();
    string code = codeStream.str();
    codeFile.close();

    FlatArray<double> array({2, 2}, 2);

    cout << array << endl;

    std::vector<unsigned> ids{0, 1};
    cout << array[ids] << endl;

    ofstream out("test.txt");
    out << array;
    out.close();

    return 0;
}
