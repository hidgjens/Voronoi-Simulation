#ifndef CSV_H
#define CSV_H

#include<string>
#include<memory>
#include<iostream>
#include "csv/Row.h"
#include "vect/Cart.h"

class CSV{
    // Frame in dataframe
private:
    std::string filename;
    int rowCount;
    std::unique_ptr<Row[]> rows;

public:
    // constructors
    CSV();
    CSV(std::string); // filename, shortcut to loadFile(std::string);

    // copy and move constructors
    CSV(CSV&);
    CSV(CSV&& csv) { *this = std::move(csv);}

    // copy and move assignments
    CSV& operator=(CSV&);
    CSV& operator=(CSV&&);

    // addition
    CSV& operator+=(const CSV& rhs){
        //std::cout << "sponge\n"; // sponge
        CSV tmp_this(*this); // copy this
        //std::cout << "sponge\n"; // sponge
        rowCount = tmp_this.rowCount + rhs.rowCount; // calcuate new row count
        //std::cout << "sponge\n"; // sponge
        rows = std::make_unique<Row[]>(rowCount);
        //std::cout << "sponge\n"; // sponge
        // loop through original rows, then add new rows
        for (int i{0}; i < rowCount; i++) {
            //std::cout << "here1\n"; // sponge
            if (i < tmp_this.rowCount){
                //std::cout << "here2\n"; // sponge
                rows[i] = tmp_this.rows[i];
                
            }else{
                //std::cout<<"here\n:"; // sponge
                rows[i] = rhs.rows[i - tmp_this.rowCount];
            }
            rows[i].set_idx(i);
        }
        return *this;
    }

    friend CSV operator+(CSV lhs, const CSV& rhs){
        lhs += rhs;
        return lhs;
    }

    // load data
    void loadFile();
    void loadFile(std::string);
    
    // accessors
    std::string get_filename() const;
    int get_rowCount() const;
    Row& get_row(int);

    void print_rows() const;

    // mutators
    void set_filename(std::string);

};

#endif