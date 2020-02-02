#include <iostream>
#include <fstream>
//  ./program DataSet/tests/001.dat < DataSet/input/001.inp > DataSet/output/001.out
int main(int argc, char * argv[])
{
    std::ifstream file{argv[1]};
    int a = 0, b = 0, c = 0;
    file >> a >> b;
    
    std::cout << a + b + c;
    return 0;
}