#include "../include/Restaurant.h"
#include <iostream>
#include <vector>
#include <utility>
using namespace std;

Restaurant* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: rest <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Restaurant rest(configurationFile);
    rest.start();
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    //delete[] argv;
    return 0;
}