#ifndef TURING_HPP
#define TURING_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <regex>
#include "utils.hpp"

using std::tuple;
using std::get;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::regex;
using std::smatch;
using std::regex_match;

const string NOTE_SYMBOL = ";";

struct TuringProgram {
	string description = "";
	string input = "";
	bool is_verbose = false;
};

struct TuringProcess {
	vector<vector<string>> tapes = vector<vector<string>>();
	vector<int> tape_ptrs = vector<int>();
	int n_step = 0;
	string state = "";
};

string convert_turing_process_to_string (TuringProcess turing_process);

enum TuringMachineExitCode {
	_tm_success = 0,
	_tm_error = -1
};

enum TuringMachineStepState {
	_tm_st_running = 0,
	_tm_st_halt = 1
};

class TuringMachine  {
public:
	TuringMachine (TuringProgram turing_program) {
		description = turing_program.description;
		input = turing_program.input;
		is_verbose = turing_program.is_verbose;
	}

	~TuringMachine () {}

	TuringMachineExitCode run () {

		if (parse() == _tm_error) {
			return _tm_error;
		}

		return simulate();
	}

	TuringMachineExitCode parse () {
		auto lines = split(description, string("\n"));
		for (auto line: lines) {
			remove_note(line);
			save(line);
		}

		// print_setting();
		if (!check_setting()) {
			throw_error("error: definition of turing machine is imcomplete");
			return _tm_error;
		}
		return _tm_success;
	}

	TuringMachineExitCode simulate () {
		if (init_turing_process() == _tm_error) {
			return _tm_error;
		}

		log("==================== RUN ====================");
		log(convert_turing_process_to_string(turing_process));



		return _tm_success;
	}

	TuringMachineStepState step () {

		return _tm_st_running;
	} 

	void remove_note (string &line) {
		size_t pos = line.find(NOTE_SYMBOL);
		if (pos != line.npos) {
			line = line.substr(0, pos);
		}
	}

	void save (string &line) {
		smatch match;
		regex state_regex("#Q = \\{(.*)\\}");
		regex input_symbol_regex("#S = \\{(.*)\\}");
		regex tape_symbol_regex("#G = \\{(.*)\\}");
		regex start_state_regex("#q0 = (.*)");
		regex blank_symbol_regex("#B = (.*)");
		regex final_state_regex("#F = \\{(.*)\\}");
		regex n_tape_regex("#N = (.*)");


		if (regex_match(line, match, state_regex)) {
			string capture = match[1].str();
			states = split(capture, string(","));

		} else if (regex_match(line, match, input_symbol_regex)) {
			string capture = match[1].str();
			input_symbols = split(capture, string(","));

		} else if (regex_match(line, match, tape_symbol_regex)) {
			string capture = match[1].str();
			tape_symbols = split(capture, string(","));

		} else if (regex_match(line, match, start_state_regex)) {
			string capture = match[1].str();
			start_state = capture;

		} else if (regex_match(line, match, blank_symbol_regex)) {
			string capture = match[1].str();
			blank_symbol = capture;

		} else if (regex_match(line, match, final_state_regex)) {
			string capture = match[1].str();
			final_states = split(capture, string(","));

		} else if (regex_match(line, match, n_tape_regex)) {
			string capture = match[1].str();
			n_tape = stoi(capture);

		}
	}

	void print_setting () {
		print_vector(states, string("states"));
		print_vector(input_symbols, string("input_symbols"));
		print_vector(tape_symbols, string("tape_symbols"));
		cout << "start_state = " << start_state << endl;
		cout << "blank_symbol = " <<  blank_symbol << endl;
		print_vector(final_states, string("final_states"));
		cout << "n_tape = " <<  n_tape << endl;
	}

	bool check_setting () {
		if (states.size() == 0) {
			return false;

		} else if (input_symbols.size() == 0) {
			return false;

		} else if (tape_symbols.size() == 0) {
			return false;

		} else if (final_states.size() == 0) {
			return false;

		} else if (start_state.length() == 0) {
			return false;

		} else if (blank_symbol.length() == 0) {
			return false;
			
		} else if (n_tape == 0) {
			return false;
			
		}

		return true;
	}

	vector<vector<string>> create_empty_tapes () {
		vector<vector<string>> tapes = vector<vector<string>>();
		for (int i = 0; i < n_tape; i++) {
			tapes.push_back(vector<string>());
		}
		return tapes;
	}

	TuringMachineExitCode init_turing_process () {
		turing_process.n_step = 0;
		turing_process.state = start_state;

		turing_process.tapes = create_empty_tapes();
		for (int i = 0; i < n_tape; i++) {
			turing_process.tape_ptrs.push_back(0);
		}


		auto result = chop(input, input_symbols);

		int pos = get<0>(result);
		input_units = get<1>(result);
		if (pos != -1) {
			// ------ BEGIN ------
			string error_message = string("error: '") + input[pos] + "' was not declared in the set of input symbols";
			error_message += "\nInput: " + input + "\n";
			for (int i = 0; i < 7 + pos; i++) {
				error_message += " ";
			}
			error_message += "^";
			// ------ END ------
			throw_error(error_message);
			return _tm_error;
		} else {
			turing_process.tapes[0] = input_units;
		}

		return _tm_success;
	}

	void throw_error (string message) {
		if (!is_verbose) {
			cerr << "illegal input" << endl;
		} else {
			cerr << "==================== ERR ====================" << endl;
			cerr << message << endl;
			cerr << "==================== END ====================" << endl;
		}
	}

	void log (string message) {
		if (is_verbose) {
			cout << message << endl;
		}
	}



private:
	string description = "";
	string input = "";
	bool is_verbose = false;
	vector<string> states;
	vector<string> input_symbols;
	vector<string> tape_symbols;
	string start_state = "";
	string blank_symbol = "";
	vector<string> final_states;

	vector<string> input_units;
	int n_tape = 0;

	vector<vector<string>> tapes;
	TuringProcess turing_process;
};

string convert_turing_process_to_string (TuringProcess turing_process) {
	int tip_length = 7;
	string str = "";

	string tip = "Step";
	str += tip + create_space(tip_length - tip.length()) + ": " + to_string(turing_process.n_step);

	tip = "State";
	str += "\n" + tip + create_space(tip_length - tip.length()) + ": " + turing_process.state;

	for (int i = 0; i < turing_process.tapes.size(); i++) {

		tip = "Index" + to_string(i);
		str += "\n" + tip + create_space(tip_length - tip.length()) + ":";
		for (int j = 0; j < turing_process.tapes[i].size(); j++) {
			str += " " + to_string(j);
		}

		tip = "Tape" + to_string(i);
		str += "\n" + tip + create_space(tip_length - tip.length()) + ":";
		for (int j = 0; j < turing_process.tapes[i].size(); j++) {
			str += " " + turing_process.tapes[i][j];
		}

	}

	return str;
} 

#endif