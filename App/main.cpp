#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include <Ranok/OpenCL/OpenclSystem.h>
#include <Ranok/LanguageCore/Parser.h>

#include "Ranok/OpenCL/OpenclCalculator.h"

using namespace std;


string CharToLower(char* rawSource)
{
    string source(rawSource);
    transform(source.begin(), source.end(), source.begin(),
                   [](unsigned char c) { return tolower(c); });
    return source;
}

int NextErrorCode(const string& error = "")
{
    static int code = 0;
    if (!error.empty())
        cout << error << endl;

    OpenclSystem::Get().Destroy();

    return --code;
}


int main(int argc, char** argv)
{
    if (!OpenclSystem::Get().Init())
        return NextErrorCode("Couldn't init opencl system");

    string codePath = "..\\..\\Examples\\NewFuncs\\sphere.txt";
    if (argc >= 2)
        codePath = argv[2];
    
    string outputFilepath = "output";
    if (argc >= 4)
        outputFilepath = argv[4];

    unsigned recursiveDepth = 4;
    if (argc >= 5)
        recursiveDepth = stoi(argv[5]);

    unsigned batchSize = 0;
    if (argc >= 6)
        recursiveDepth = stoi(argv[6]);


    fstream codeFile(codePath);
    if (!codeFile)
        return NextErrorCode("Couldn't open file " + codePath);

    stringstream codeStream;
    codeStream << codeFile.rdbuf();
    string code = codeStream.str();
    codeFile.close();
    
    Parser parser;
    Program program = parser.Parse(Lexer::CreateFrom(code));
    auto args = program.Table().Arguments();


    glm::vec3 spaceSize;
    if (dynamic_cast<ArrayExpression*>(args[0]->child.get()))
        spaceSize = {program.Table().Ranges()[0][0].max, program.Table().Ranges()[0][1].max, program.Table().Ranges()[0][2].max};
    else
        spaceSize = {program.Table().Ranges()[0][0].max, program.Table().Ranges()[1][0].max, program.Table().Ranges()[2][0].max};

    Space space({0, 0, 0}, {spaceSize.x, spaceSize.y, spaceSize.z}, recursiveDepth);
    OpenclCalculator calculator;


    if (batchSize == 0)
        batchSize = space.GetTotalPartition();

    ofstream output;
    output.open(outputFilepath + ".mbin");

    output << space << "\n";
    auto callback = [&output, &calculator, spaceSize = space.GetTotalPartition()](unsigned start, unsigned count)
    {
        cout << "Done " << 100*(start + count)/(float)spaceSize << "% (" << (start + count) << "/" << spaceSize << ")\n";
        output << calculator.GetImage();
    };

    if (!calculator.Calculate(program, space, callback, batchSize))
        return NextErrorCode("Calculate failure");

    output.close();
    cout << "Done\n";

    OpenclSystem::Get().Destroy();
    return 0;
}
