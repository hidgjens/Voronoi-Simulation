#include "CSV.h"
#include "csv/Row.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

// constructors
CSV::CSV(){}
CSV::CSV(std::string fn) : filename(fn) {
    loadFile();
}

// copy constructor
CSV::CSV(CSV& csv) : filename(csv.filename) , rowCount(csv.rowCount) {
    rows = std::make_unique<Row[]>(rowCount);
    for (int i{0}; i < rowCount; i++) {
        rows[i] = csv.rows[i];
    }
}

// copy and move assignments
// deep copy assignment
CSV& CSV::operator=(CSV& csv) {
    if (&csv == this) { // no self assignment
        return *this;
    }

    filename = csv.filename;
    rowCount = csv.rowCount;

    rows = std::make_unique<Row[]>(rowCount);
    for (int i{0}; i < rowCount; i++){
        rows[i] = csv.rows[i];
    }

    return *this;
}

// move assignment
CSV& CSV::operator=(CSV&& csv) {
    if (&csv == this) {
        return *this;
    }

    filename = csv.filename;
    rowCount = csv.rowCount;

    rows = std::move(csv.rows);

    csv.rowCount = 0;

    return *this;

}

// load data
int header_lines = 1; // just to note

void CSV::loadFile() {
    // open file
    std::ifstream CSVFile(filename); 

    // count rows in file
    rowCount = std::count(std::istreambuf_iterator<char>(CSVFile), 
        std::istreambuf_iterator<char>(), '\n') - header_lines;

    // reset to beginning of file
    CSVFile.clear();
    CSVFile.seekg(0, std::ios::beg);

    // make array
    rows = std::make_unique<Row[]>(rowCount); // first line is header
    
    // count line number
    int line_num {-1 * header_lines}; // start at -1 because first line is header
    std::string line; // store line temporarily
    
    // iterate over lines
    while(std::getline(CSVFile, line)){
        
        // if first lines, skip
        if (line_num >= 0) {

            // need to split lines by tabs
            std::vector<std::string> fields;

            std::istringstream iss(line);
            std::string field;
            while(std::getline(iss, field, '\t')){
                fields.push_back(field);
            }

            // format fields - 
                /* 
                int IDX, int PID, double TIME, int FID, 
                std::string TEAM, int NUM, double X, double Y,
                double CTRL, double DCTRL, bool SMART
                */

            int IDX = std::stoi(fields.at(0));
            int PID = std::stoi(fields.at(1));
            double TIME = std::stod(fields.at(2));
            int FID = std::stoi(fields.at(3));
            std::string TEAM = fields.at(4);
            int NUM = std::stoi(fields.at(5));
            double X = std::stod(fields.at(6));
            double Y = std::stod(fields.at(7));
            double DX = std::stod(fields.at(8));
            double DY = std::stod(fields.at(9));
            double CTRL = std::stod(fields.at(10));
            double DCTRL = std::stod(fields.at(11));
            double CTRL2 = std::stod(fields.at(12));
            double DCTRL2 = std::stod(fields.at(13));
            bool SMART;
            if (fields.at(14) == "True") {
                SMART = true;
            } else
            if (fields.at(14) == "False") {
                SMART = false;
            } else {
                std::cout << "Incorrect smart value: " << fields.at(14) << "\n";
            }

            // create row to store data
            rows[line_num] = Row(IDX, PID, TIME, FID, TEAM, NUM, X, Y, DX, DY, CTRL, DCTRL, CTRL2, DCTRL2, SMART);
        line_num++;
    }

    // close file
    CSVFile.close();

}
void CSV::loadFile(std::string fn) {
    // change filename
    filename = fn;
    loadFile();
}

// accessors 
std::string CSV::get_filename() const { return filename; }
int CSV::get_rowCount() const { return rowCount; }
Row& CSV::get_row(int i) { return rows[i]; }

void CSV::print_rows() const {
    std::cout << "idx\tpid\ttime\tfid\tteam\tnum\tx\ty\tdx\tdy\tctrl\tdCtrl\tctrl2\tdCtrl2\tsmart\n";
    for (int i{0}; i < rowCount; i++){
        rows[i].print_row();
    }
}

// mutators
void CSV::set_filename(std::string fn) { filename = fn; }

