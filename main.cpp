#include "SymbolTable.h"
#include"SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable *st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    delete st;
}

int main(int argc, char **argv)
{
   
    test("test5.txt");
    return 0;
}