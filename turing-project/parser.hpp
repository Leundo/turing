#ifndef PARSER_HPP
#define PARSER_HPP

#include <regex>
#include <string>
#include <iostream>
#include <tuple>
#include "turing_helper.hpp"
#include "utils.hpp"

using std::string;
using std::tuple;
using std::get;
using std::cout;
using std::endl;
using std::cerr;
using std::regex;
using std::smatch;
using std::regex_match;

enum ParserExitCode {
	_pr_success = 0,
	_pr_help = 1,
	_pr_error = -1
};

class Parser {
public:
	Parser (int argc, char *argv[]) {
		n_arg = argc;
		p_arg = argv;

	}

	~Parser () {}

	tuple<ParserExitCode, TuringProgram> parse () {
		smatch match;
		regex single_minus_regex("-([a-zA-Z0-9]*)");
		regex double_minus_regex("--([a-zA-Z0-9]*)");
		int flag = 0;

		for (int i = 1; i < n_arg; i++) {
			string word = p_arg[i];
			if (regex_match(word, match, single_minus_regex)) {
				string options = match[1].str();
				for (int j = 0; j < options.length(); j++) {
					if (options[j] == 'v') {
						turing_program.is_verbose = true;
					} else if (options[j] == 'h') {
						help();
						return make_tuple(_pr_help, turing_program);
					} else {
						throw_error(string("invalid option - ") + options[j] + ".");
						return make_tuple(_pr_error, turing_program);
					}
				}
			} else if (regex_match(word, match, double_minus_regex)) {
				string option = match[1].str();
				if (option == "verbose") {
					turing_program.is_verbose = true;
				} else if (option == "help") {
					help();
					return make_tuple(_pr_help, turing_program);
				} else {
					throw_error(string("invalid option -- ") + option + ".");
					return make_tuple(_pr_error, turing_program);
				}
			} else {
				if (flag == 0) {
					auto result = read_file(word);
					if (!get<0>(result)) {
						throw_error("turing file \"" + word + "\" does not exist.");
						return make_tuple(_pr_error, turing_program);
					} else {
						turing_program.description = get<1>(result);
					}
					flag += 1;
				} else if (flag == 1) {
					turing_program.input = word;
					flag += 1;
				} else {
					throw_error(string("invalid argument \"") + word  + "\".");
					return make_tuple(_pr_error, turing_program);
				}
			}
		}
		return make_tuple(_pr_success, turing_program);
	}

	void help () {
		cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << endl;
	}

	void throw_error (string error_message) {
		cerr << "error: " << error_message << endl;
	}

private:
	int n_arg = 0;
	char **p_arg = nullptr;
	TuringProgram turing_program;
};


#endif