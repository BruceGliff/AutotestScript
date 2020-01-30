// ./programm DataSet/tests/001.dat < DataSet/input/001.inp > DataSet/output/001.out

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

ErrorType Launch()
{
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
    {
        std::stringstream command;
        command << "./programm " << x.second << " < " << input[x.first] 
                << " > " << "DataSet/output/" << x.first << ".out";
        system(command.str().c_str());
    }

    return ErrorType::Ok;
}
 
ErrorType Test()
{
    std::map<std::string, std::string> check, output;
    std::filesystem::directory_entry const DataPath{"DataSet/"};
    if (!DataPath.exists())
        return ErrorType::NoDataSet;

    for (auto const & entry : std::filesystem::directory_iterator(DataPath))
    {
        if (entry == std::filesystem::directory_entry{"DataSet/check"})
        {
            for (auto const & insideDir : std::filesystem::directory_iterator(entry))
            {
                std::string const path = insideDir.path().string();
                size_t const len = path.rfind(".") - 1 - path.rfind("/");
                std::string name{path, path.rfind("/") + 1, len};
            
                check[name] = path;
            }
        }
            
        if (entry == std::filesystem::directory_entry{"DataSet/output"})
        {
            for (auto const & insideDir : std::filesystem::directory_iterator(entry))
            {
                std::string const path = insideDir.path().string();
                size_t const len = path.rfind(".") - 1 - path.rfind("/");
                std::string name{path, path.rfind("/") + 1, len};
            
                output[name] = path;
            }
        }
    }

    int const checkAmount = check.size();
    int const outputAmount = output.size();
    if (!checkAmount)
        return ErrorType::NoCheck;
    if (checkAmount != outputAmount)
        return ErrorType::ErData;

    std::list<std::string> WA;
    for (auto const & x : output)
    {
        std::ifstream outputFile{x.second};
        std::ifstream checkFile{check[x.first]};

        std::stringstream outStream, checkStream;
        outStream << outputFile.rdbuf();
        checkStream << checkFile.rdbuf();

        if (!outStream.str().compare(checkStream.str()))
        {
            std::cout << Color::Modifier{Color::FG_GREEN, x.first.c_str()} << std::endl;
        }
        else
        {
            WA.push_back(x.first);
            std::cout << Color::Modifier{Color::FG_RED, x.first.c_str()} 
                      << Color::Modifier{Color::FG_RED, " WA"} <<  std::endl;
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
    }
    std::cout << "Testing in process..." << std::endl;
    switch (Test())
    {
    case ErrorType::ErData:
        std::cerr << Color::Modifier{Color::FG_RED, "Wrong data set!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoDataSet:
        std::cerr << Color::Modifier{Color::FG_RED, "Data set does not exist!"} << std::endl;
        return 0;
        break;
    case ErrorType::NoCheck:
        std::cerr << Color::Modifier{Color::FG_RED, "Check do not exist!"} << std::endl;
        return 0;
        break;
    }
    return 0;
}