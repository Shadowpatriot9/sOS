//
/*
 
 */
//

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

char command_list{};

//
vector<string> split(const string &input, char delimiter) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

//
char ** convert_to_c_array(const vector<string> &args) {
    char** c_args = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); i++) {
        c_args[i] = const_cast<char*>(args[i].c_str());
    }
    c_args[args.size()] = nullptr;
    return c_args;
}

int main() {
    string input;
    
    while (true) {
        //
        cout << "sOS >> ";
        getline(cin, input);
        
        //
        if (input == "exit") {
            break;
        }
        if (input == "help") {
            cout << "Here are all of your available commands:" << "\n\n";
            cout << command_list;
        }
        
        //
        vector<string> args = split(input, ' ');
        if (args.empty()) {
            continue;
        }
        
        //
        char** c_args = convert_to_c_array(args);
        
        //
        pid_t pid = fork();
        if (pid < 0) {
            cerr << "Fork Failed" << endl;
        } else if (pid == 0 ) {
            //
            if (execvp(c_args[0], c_args) == -1) {
                cerr << "Command not found: " << args[0] << endl;
            }
            
            //
            delete[] c_args;
            exit(EXIT_FAILURE);
        } else {
            //
            wait(nullptr);
        }
        
        //
        delete[] c_args;
        
    }
    
    return 0;
}
