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
    
    CalculateTarget target = CalculateTarget::Model;
    if (argc >= 3 && CharToLower(argv[3]) == "image")
        target = CalculateTarget::Image;

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

    Space<3> space({0, 0, 0}, {args[0]->range.max, args[1]->range.max, args[2]->range.max}, recursiveDepth);
    OpenclCalculator<3> calculator;


    if (batchSize == 0)
        batchSize = space.GetTotalPartition();

    ofstream output;
    if (target == CalculateTarget::Model)
        output.open(outputFilepath + ".mbin");
    else
        output.open(outputFilepath + ".ibin");

    output << space << "\n";
    auto callback = [&output, &calculator, target, spaceSize = space.GetTotalPartition()](unsigned start, unsigned count)
    {
        cout << "Done " << 100*(start + count)/(float)spaceSize << "% (" << (start + count) << "/" << spaceSize << ")\n";
        if (target == CalculateTarget::Model)
            output << calculator.GetModel();
        else
            output << calculator.GetImage();
    };

    if (!calculator.Calculate(target, program, space, batchSize, callback))
        return NextErrorCode("Calculate failure");

    output.close();
    cout << "Done\n";

    OpenclSystem::Get().Destroy();
    return 0;
}
