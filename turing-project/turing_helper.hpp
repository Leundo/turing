#ifndef TURING_HELPER_HPP
#define TURING_HELPER_HPP

#include <iostream>
#include <algorithm>
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
using std::make_tuple;
using std::regex_match;

const string NOTE_SYMBOL = ";";

struct TuringProgram {
	string description = "";
	string input = "";
	bool is_verbose = false;
};

struct TuringProcess {
	vector<vector<string>> tapes = vector<vector<string>>();
	// vector<int> tape_ptrs = vector<int>();
	int tape_ptr = 0;
	int first_number = 0;
	int n_step = 0;
	string state = "";
};

enum TuringMachineExitCode {
	_tm_success = 0,
	_tm_error = -1
};

// 第 i 个编号的长度
int get_i_number_length (int first_number, int i) {
	return to_string(abs(first_number + i)).length();
}

vector<vector<int>> create_space_list (vector<vector<string>> tapes, int first_number) {
	vector<vector<int>> space_list = vector<vector<int>>();
	for (auto tape: tapes) {
		vector<int> space = vector<int>();
		for (int i = 0; i < tape.size(); i++) {
			space.push_back(std::max(get_i_number_length(first_number, i), int(tape[i].length())));
		}
		space_list.push_back(space);
	}
	return space_list;
}

tuple<int, int> find_non_blank_begin_and_end (vector<string> tape, int tape_ptr, string blank) {
	int begin = -1;
	int end = -1;
	for (int i = 0; i < tape.size(); i++) {
		if (tape[i] != blank) {
			begin = i;
			break;
		}
	}
	for (int i = tape.size() - 1; i >= 0; i--) {
		if (tape[i] != blank) {
			end = i;
			break;
		}
	}
	if (begin == -1) {
		begin = tape_ptr;
		end = tape_ptr;
	} else {
		if (tape_ptr < begin) {
			begin = tape_ptr;
		} else if (tape_ptr > end) {
			end = tape_ptr;
		}
	}

	return make_tuple(begin, end);
}

string convert_turing_process_to_string (TuringProcess turing_process, string blank) {
	vector<vector<int>> space_list = create_space_list(turing_process.tapes, turing_process.first_number);
	int tip_length = 7;
	string str = "";
	string tip = "Step";
	str += tip + create_space(tip_length - tip.length()) + ": " + to_string(turing_process.n_step);
	tip = "State";
	str += "\n" + tip + create_space(tip_length - tip.length()) + ": " + turing_process.state;

	for (int i = 0; i < turing_process.tapes.size(); i++) {
		vector<int> space = space_list[i];

		int begin = get<0>(find_non_blank_begin_and_end(turing_process.tapes[i], turing_process.tape_ptr, blank));
		int end = get<1>(find_non_blank_begin_and_end(turing_process.tapes[i], turing_process.tape_ptr, blank));

		// cout << begin << " " << end << endl;
		
		tip = "Index" + to_string(i);
		str += "\n" + tip + create_space(tip_length - tip.length()) + ":";
		for (int j = begin; j <= end; j++) {
			string index = to_string(abs(turing_process.first_number + j));
			str += " " + index + create_space(space[j] - index.length());
		}
		tip = "Tape" + to_string(i);
		str += "\n" + tip + create_space(tip_length - tip.length()) + ":";
		for (int j = begin; j <= end; j++) {
			string symbol = turing_process.tapes[i][j];
			str += " " + symbol + create_space(space[j] - symbol.length());
		}
		tip = "Head" + to_string(i);
		str += "\n" + tip + create_space(tip_length - tip.length()) + ":";
		for (int j = begin; j <= end; j++) {
			if (j + turing_process.first_number == turing_process.tape_ptr) {
				str += " ^" + create_space(space[j] - 1);
			} else {
				str += " " + create_space(space[j]);
			}
		}

	}

	return str;
}

#endif