#ifndef TURING_CORE_HPP
#define TURING_CORE_HPP

#include <iostream>
#include <string>
#include <tuple>
#include <regex>
#include <map>
#include "turing_helper.hpp"
#include "utils.hpp"

using std::tuple;
using std::map;
using std::pair;
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
        // vector<string> tape1 = vector<string>();
        // vector<string> tape2 = vector<string>();
        // tape1.push_back("_");
        // tape1.push_back("apple");
        // tape1.push_back("orangle");
        // tape1.push_back("cat");

        // tape2.push_back("egg");
        // tape2.push_back("e");
        // tape2.push_back("longlonglong");
        // tape2.push_back("_");
        
        // vector<vector<string>> tapes = vector<vector<string>>();
        // tapes.push_back(tape1);
        // tapes.push_back(tape2);

        // turing_process.first_number = -1;
        // turing_process.tape_ptrs.push_back(1);
        // turing_process.tape_ptrs.push_back(1);
        // turing_process.tapes = tapes;

        // cout << convert_turing_process_to_string(turing_process, "_") << endl;

        // auto a = make_tuple("qwer", "abcd");
        // auto b = make_tuple("qwera", "abcd");
        // cout << (a == b) << endl;

        if (parse() == _tm_error) {
			return _tm_error;
		}

        // auto action = find_action("cmp", "0_");
        // cout << get<0>(action) << endl;
        // cout << get<1>(action) << endl;
        // print_vector(get<2>(action));

        return _tm_success;
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
        tape_symbols_and_star = tape_symbols;
        tape_symbols_and_star.push_back("*");
		return _tm_success;
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

        regex transfer_func_regex("(.+)[ ]+(.+)[ ]+(.+)[ ]+(.+)[ ]+(.+)");


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
		} else if (regex_match(line, match, transfer_func_regex)) {
            auto input = make_tuple(match[1].str(), match[2].str());
            auto output = make_tuple(match[3].str(), match[4].str(), match[5].str());
            transition_func.insert(pair<tuple<string, string>, tuple<string, string, string>>(input, output));
        }
	}

    tuple<TuringMachineExitCode, bool, vector<string>, vector<Move>, string> find_action (string state, string inputs) {
        bool isHalt = true;
        auto choped = chop(inputs, tape_symbols);
        auto mask = create_init_mask(n_tape, 0);

        if (get<0>(choped) != -1 || get<1>(choped).size() != n_tape) {
            throw_error("error: fail in chopping tape symbols");
            return make_tuple(_tm_error, !isHalt, vector<string>(), vector<Move>(), "");
        }
        

        do {
            mask_word(get<1>(choped), mask);
            string input_symbols = concat(mask_word(get<1>(choped), mask));

            auto it = transition_func.find(make_tuple(state, input_symbols));
            if (it != transition_func.end()) {
                auto choped_outputs = chop(get<0>(it->second), tape_symbols_and_star);
                auto choped_outputs_symbol = get<1>(choped_outputs);

                if (get<0>(choped_outputs) != -1 || choped_outputs_symbol.size() != n_tape) {
                    throw_error("error: fail in chopping tape symbols");
                    return make_tuple(_tm_error, !isHalt, vector<string>(), vector<Move>(), "");
                }

                for (int i = 0; i < choped_outputs_symbol.size(); i++) {
                    if (choped_outputs_symbol[i] == "*") {
                        choped_outputs_symbol[i] = get<1>(choped)[i];
                    }
                }

                auto moves = vector<Move>();
                for (auto move: get<1>(it->second)) {
                    if (move == 'l') {
                        moves.push_back(_left);
                    } else if (move == 'r') {
                        moves.push_back(_right);
                    } else if (move == '*') {
                        moves.push_back(_suspend);
                    } else {
                        throw_error("error: unknown move: " + to_string(move));
                        return make_tuple(_tm_error, !isHalt, vector<string>(), vector<Move>(), "");
                    }
                }
                return make_tuple(_tm_success, !isHalt, choped_outputs_symbol, moves, get<2>(it->second));
            }
            mask = get_next_mask(mask);
        } while ( mask.size() > 0 );
        
        return make_tuple(_tm_success, isHalt, vector<string>(), vector<Move>(), "");
    }

    void remove_note (string &line) {
		size_t pos = line.find(NOTE_SYMBOL);
		if (pos != line.npos) {
			line = line.substr(0, pos);
		}
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

    void print_setting () {
		print_vector(states, string("states"));
		print_vector(input_symbols, string("input_symbols"));
		print_vector(tape_symbols, string("tape_symbols"));
		cout << "start_state = " << start_state << endl;
		cout << "blank_symbol = " <<  blank_symbol << endl;
		print_vector(final_states, string("final_states"));
		cout << "n_tape = " <<  n_tape << endl;
        print_transition_func();
	}

    void print_transition_func () {
        cout << "n_tape = " << endl;
        for (auto it = transition_func.begin(); it != transition_func.end(); it++) {
            cout << get<0>(it->first) << ", " << get<1>(it->first) << ": " << get<0>(it->second) << ", " << get<1>(it->second) << ", " << get<2>(it->second) << endl;
        }
    }

    void throw_error (string message) {
        is_verbose = true;
		if (!is_verbose) {
			cerr << "illegal input" << endl;
		} else {
			cerr << "==================== ERR ====================" << endl;
			cerr << message << endl;
			cerr << "==================== END ====================" << endl;
		}
	}

    ~TuringMachine () {}

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

    vector<string> tape_symbols_and_star;
    int n_tape = 0;
    map<tuple<string, string>, tuple<string, string, string>> transition_func;

    TuringProcess turing_process;
};


#endif