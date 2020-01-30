// ./programm DataSet/tests/001.dat < DataSet/input/001.inp > DataSet/output/001.out

#include <sstream>
#include <iostream>
#include <filesystem>
#include <list>
#include <string>
#include <map>

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
    std::map<std::string, std::string> tests, inputs;

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
            
                inputs[name] = path;
            }
        }
    }

    int const testsAmount = tests.size();
    int const inputsAmount = inputs.size();
    if (!testsAmount)
        return ErrorType::NoTests;
    if (!inputsAmount)
        return ErrorType::NoInputs;
    if (testsAmount != inputsAmount)
        return ErrorType::ErData;

    for (auto & x : tests)
    {
        std::cout << x.first << ' ' << x.second << std::endl;
    }
    for (auto & x : inputs)
    {
        std::cout << x.first << ' ' << x.second << std::endl;
    }

    for (auto const & x : tests)
    {
        std::stringstream command;
        command << "./programm " << x.second << " < " << inputs[x.first] << " > " << "DataSet/output/" << x.first << ".out";
        system(command.str().c_str());
    }

    return ErrorType::Ok;
}
 
int main()
{
    std::cout << "Testing in process" << std::endl;
    switch (Launch())
    {
    case ErrorType::ErData:
        std::cerr << "Wrong data set!" << std::endl;
        return 0;
        break;
    
    default:
        break;
    }
    return 0;
}