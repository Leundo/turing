#include <iostream>
#include "parser.hpp"
#include "turing_helper.hpp"
#include "turing_core.hpp"

int main(int argc, char *argv[]) {
	Parser parser = Parser(argc, argv);
	auto result = parser.parse();

	if (std::get<0>(result) == _pr_success) {
		auto turing_program = std::get<1>(result);
		TuringMachine tm = TuringMachine(turing_program);
		auto run_result = tm.run();
		if (std::get<0>(run_result) == _tm_success) {
			cout << std::get<1>(run_result) << endl;
		}
		return std::get<0>(run_result);
	} else if (std::get<0>(result) == _pr_help) {
		return _pr_success;
	} else {
		return std::get<0>(result);
	}
	
}