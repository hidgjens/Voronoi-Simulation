#include<iostream>
#include<string>
#include "lib/CSV.h"

using namespace std;

int main(int argc, char *argv[]) {

    int args_required = 1;

    if (argc != args_required + 1) {
        cout << "Wrong number of arguments (" << argc << "/" << args_required << ")\n"
        << "AnalyseRun.cpp | args:\n"
        << "\t[1] Filename (data_files/csvs/Filename.csv)\n";
        return 1;
    }

    // filename is first argument
    string filename{argv[1]};

    // directory and extension are usually the same
    string dir{"data_files/csvs/"};
    string ext{".csv"};

    
}