#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <stdexcept>
using namespace std;

void Create_transfer_map(const string &key_file, map<string, string> &out_m) {
    ifstream handle(key_file.c_str());
    
    if (!handle) {
	cout << "open " << key_file << " failed" << endl;
	throw runtime_error("open file failed");
    }
    
    istringstream iss;   
    string line;
    string word;
    string key;
    string value;
    
    int line_num = 1; 
    int word_num_of_line = 0;
    while (getline(handle, line)) {
	iss.str(line);
	word_num_of_line = 0;

	//check if line applys correct format   
	while (iss >> word) {
	    word_num_of_line++;
	}
	if (word_num_of_line!=0 && word_num_of_line!=2) {
	    cout << "wrong format in line " << line_num << " of key file" << endl;
	    throw runtime_error("format error");
	}
	iss.clear();

	//create map
	iss.str(line);
	while (iss >> key >> value) {
	    out_m[key] = value;
	}
	iss.clear();
	
	++line_num;
	line = "";
    }
    
    handle.close();
} 	

void Transfer(const string &src_file, const map<string, string> &transfer_m) {
    ifstream ifs;
    ofstream ofs;
    istringstream iss;
    string line;
    string word;
    string filename_after_transfer = src_file;

    int index = src_file.find_last_of('.');
    if (-1 != index) {
	filename_after_transfer.replace(index, 1, "_afterTransfer.");
    }
    else {
	filename_after_transfer +=  "_afterTransfer";
    }
    
    ifs.open(src_file.c_str());
    if(!ifs) {
	cout << "open " << src_file << " failed" << endl;
	throw runtime_error("open src file failed");
    }

    ofs.open(filename_after_transfer.c_str());
    if (!ofs) {
	cout << "open " << filename_after_transfer << " failed" << endl;
	throw runtime_error("open output file failed");
    }
    
    while (getline(ifs, line) ) {
	bool firstword = true;
	iss.str(line);
	while (iss >> word ) {
	    if (firstword) {
		firstword = false;
	    }
	    else {
		ofs << " ";
	    }
	    map<string, string>::const_iterator iter = transfer_m.find(word);
	    ofs << ((iter != transfer_m.end()) ? iter->second : word);
	}
	iss.clear();
	ofs << endl;
    }
    
    ifs.close();
    ofs.close();
}


int main(int argc, char **argv) {
    if (3 != argc) {
	throw runtime_error("wrong number of arguments");	    
    }

    map<string, string> m;
    string transfer_file(argv[1]);
    string src_file(argv[2]);

    Create_transfer_map(transfer_file, m);

    Transfer(src_file, m);

}
