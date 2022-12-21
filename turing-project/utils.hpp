#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <tuple>
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::tuple;
using std::vector;
using std::make_tuple;
namespace fs = std::filesystem;

tuple<bool, string> read_file (string dirname) {
	fs::path url(dirname);
	if (!fs::exists(url)) {
		return make_tuple(false, "");
	} else {
		std::ifstream turing_file(url);
		std::ostringstream sstr;
		sstr << turing_file.rdbuf();
		return make_tuple(true, sstr.str());
	}
}

vector<string> split (const string &str, const string &pattern) {
    vector<string> res;
    if(str == "") {
        return res;
    }
    string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos) {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos + pattern.length(), strs.size());
        pos = strs.find(pattern);
    }

    return res;
}

void print_vector (const vector<string>& v, string header = string("placeholder")){
	header += " = ";
  	for (auto item: v){
    	header += item + string(", ");
  	}
  	cout << header << endl;
}

tuple<int, vector<string>> chop (string sentence, vector<string> words) {
	vector<string> choped_sentence = vector<string>();
	int pos = 0;
	int flag = 0;

	while (sentence.length() > 0) {
		flag = 0;
		for (auto word: words) {
			if (sentence.find(word) == 0) {
				choped_sentence.push_back(word);
				sentence = sentence.substr(0 + word.length(), sentence.length());
				pos += word.length();
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			return make_tuple(pos, choped_sentence);
		}
	}

	return make_tuple(-1, choped_sentence);
}

string create_space (int length) {
	string str = "";
	for (int i = 0; i < length; i++) {
		str += " ";
	}
	return str;
}

#endif