#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
    string file = "";

    if(argc > 1)
    {
        string arg = argv[1];
        file = arg.substr(6);
    }
    else
    {
        file = "input.txt";
    }

}
