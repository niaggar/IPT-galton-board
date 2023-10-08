#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

#include "./modules/Program.h"

using namespace std;

int main() {

    MainProgram *program = new MainProgram();
    program->run(100, 1.0 / (60 * 1.0), 0.4, 0.4, "./data/test.dat");

    return 0;
}
