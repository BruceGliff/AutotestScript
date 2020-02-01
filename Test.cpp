// ./program DataSet/tests/001.dat < DataSet/input/001.inp > DataSet/output/001.out

#include "Color.hpp"
#include <sstream>
#include <iostream>
#include <filesystem>
#include <list>
#include <string>
#include <map>
#include <fstream>

enum class ErrorType
{
    NoDataSet,
    NoTests,
    NoInputs,
    NoCheck,
    ErData,
    Ok
};

ErrorType Test(std::string const & x, std::list<std::string> & WA);
ErrorType Launch()
{
    std::list<std::string> WA;
    std::map<std::string, std::string> tests, input;

    std::filesystem::directory_entry const DataPath{"DataSet/"};
    if (!DataPath.exists())
        return ErrorType::NoDataSet;

    for (auto const & entry : std::filesystem::directory_iterator(DataPath))
    {
        if (entry == std::filesystem::directory_entry{"DataSet/tests"})
        {
            for (auto const & insideDir : std::filesystem::directory_iterator(entry))
            {
                std::string const path = insideDir.path().string();
                size_t const len = path.rfind(".") - 1 - path.rfind("/");
                std::string name{path, path.rfind("/") + 1, len};
            
                tests[name] = path;
            }
        }
            
        if (entry == std::filesystem::directory_entry{"DataSet/input"})
        {
            for (auto const & insideDir : std::filesystem::directory_iterator(entry))
            {
                std::string const path = insideDir.path().string();
                size_t const len = path.rfind(".") - 1 - path.rfind("/");
                std::string name{path, path.rfind("/") + 1, len};
            
                input[name] = path;
            }
        }
    }

    int const testsAmount = tests.size();
    int const inputAmount = input.size();
    if (!testsAmount)
        return ErrorType::NoTests;
    if (!inputAmount)
        return ErrorType::NoInputs;
    if (testsAmount != inputAmount)
        return ErrorType::ErData;
    for (auto const & x : tests)
        if (input.find(x.first) == input.end())
        {
            std::stringstream errout;
            errout << '"' << x.first << '"' << " doesn't found in input directory!";
            std::cerr << Color::Modifier{Color::FG_RED, errout.str().c_str()} << std::endl;
            return ErrorType::ErData;
        }

    system("rm DataSet/output/*");

    for (auto const & x : tests)
    {
        std::stringstream command;
        command << "./program " << x.second << " < " << input[x.first] 
                << " > " << "DataSet/output/" << x.first << ".out";
        system(command.str().c_str());

        const auto & ret = Test(x.first, WA);
        if(ret != ErrorType::Ok)
        {
            return ret;
        }
    }
    if (WA.size() != 0)
    {
        std::cout << "Errors in: ";
        for (auto const & x : WA)
            std::cout << Color::Modifier{Color::FG_RED, x.c_str()} << ' ';
        std::cout << std::endl;
    }
    return ErrorType::Ok;
}
 
ErrorType Test(std::string const & x, std::list<std::string> & WA)
{
    std::cout << std::setfill('-') << std::setw(40) << std::left << x;
    
        std::stringstream out, check;
        out << "DataSet/output/" << x << ".out";
        std::ifstream outputFile{out.str()};
        check << "DataSet/check/" << x;
        
        std::ifstream checkFile; 
        checkFile.open(check.str(), std::ios_base::in);
        if (!checkFile.is_open())
            return ErrorType::NoCheck;

        std::stringstream outStream, checkStream;
        outStream << outputFile.rdbuf();
        checkStream << checkFile.rdbuf();

        if (!outStream.str().compare(checkStream.str()))
        {
            std::cout << Color::Modifier{Color::FG_GREEN, x.c_str()} << std::endl;
        }
        else
        {
            WA.push_back(x);
            std::cout << Color::Modifier{Color::FG_RED, x.c_str()} 
                      << Color::Modifier{Color::FG_RED, " WA"} <<  std::endl;
        }

    return ErrorType::Ok;    
}


int main()
{
    std::cout << "Launching in process..." << std::endl;
    switch (Launch())
    {
    case ErrorType::ErData:
        std::cerr << Color::Modifier{Color::FG_RED, "Wrong data set!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoDataSet:
        std::cerr << Color::Modifier{Color::FG_RED, "Data set does not exist!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoTests:
        std::cerr << Color::Modifier{Color::FG_RED, "Tests do not exist!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoInputs:
        std::cerr << Color::Modifier{Color::FG_RED, "Input data does not exist!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoCheck:
        std::cerr << Color::Modifier{Color::FG_RED, "Check data does not exist!"} << std::endl;
        return 0;
        break;
    }
    return 0;
    
}