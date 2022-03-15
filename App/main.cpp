#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#include <Ranok/Core/OpenclCalculator.h>
#include <Ranok/LanguageCore/Parser.h>

using namespace std;


string CharToLower(char* rawSource)
{
    string source(rawSource);
    std::transform(source.begin(), source.end(), source.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return source;
}

int NextErrorCode(const std::string& error = "")
{
    static int code = 0;
    if (!error.empty())
        cout << error << endl;
    return --code;
}


int main(int argc, char** argv)
{
    if (!OpenclSystem::Get().Init())
        return NextErrorCode("Couldn't init opencl system");

    std::string codePath = "..\\..\\Examples\\NewFuncs\\sphere.txt";

    fstream codeFile(codePath);
    if (!codeFile)
        return NextErrorCode("Couldn't open file " + codePath);

    stringstream codeStream;
    codeStream << codeFile.rdbuf();
    string code = codeStream.str();
    codeFile.close();
    
    Parser parser;
    Program program = parser.Parse(Lexer::CreateFrom(code));

    Space<3> space({0, 0, 0}, {2, 2, 2}, 3);
    OpenclCalculator<3> calculator;

    if (!calculator.CalculateImage(program, space))
        return NextErrorCode("Calculate failure");

    ofstream output("output.txt");
    auto image = calculator.GetImage();
    for (size_t i = 0; i <  image.Size(); ++i)
    {
        for (auto& j :image[i])
            output << j << " ";
        output << '\n';
    }
    output.close();

    OpenclSystem::Get().Destroy();
    return 0;
}
