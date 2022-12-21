#ifndef TURING_CORE_HPP
#define TURING_CORE_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <regex>
#include "turing_helper.hpp"
#include "utils.hpp"

using std::tuple;
using std::get;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::endl;
using std::regex;
using std::smatch;
using std::regex_match;

class TuringMachine {
public:
	TuringMachine (TuringProgram turing_program) {
		description = turing_program.description;
		input = turing_program.input;
		is_verbose = turing_program.is_verbose;
	}

    TuringMachineExitCode run () {
        vector<string> tape1 = vector<string>();
        vector<string> tape2 = vector<string>();
        tape1.push_back("_");
        tape1.push_back("apple");
        tape1.push_back("orangle");
        tape1.push_back("cat");

        tape2.push_back("egg");
        tape2.push_back("e");
        tape2.push_back("longlonglong");
        tape2.push_back("_");
        
        vector<vector<string>> tapes = vector<vector<string>>();
        tapes.push_back(tape1);
        tapes.push_back(tape2);

        turing_process.first_number = -1;
        turing_process.tape_ptr = 1;
        turing_process.tapes = tapes;

        cout << convert_turing_process_to_string(turing_process, "_") << endl;

        return _tm_success;
    }

    ~TuringMachine () {}

private:
    string description = "";
    string input = "";
    bool is_verbose = false;

    TuringProcess turing_process;
};


#endif