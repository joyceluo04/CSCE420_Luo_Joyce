#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

void blocksworld(const string &filename) {
    cout << "Problem # file: " << filename << endl;

    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open " << filename << endl;
        return;
    }

    string line;
    int linecount = 0;
    int num_stacks = 0;
    int num_blocks = 0;
    int num_moves = 0;
    vector<char> stack1;
    vector<char> stack2;
    vector<char> stack3;
    vector<char> stack4;
    vector<char> stack5;
    vector<char> goalstack1;
    vector<char> goalstack2;
    vector<char> goalstack3;
    vector<char> goalstack4;
    vector<char> goalstack5;  
    while (getline(in, line)) { //simple parser to read
        if (linecount == 0){
            std::istringstream iss(line);
            if (!(iss >> num_stacks >> num_blocks >> num_moves)) {
                cout<< "Invalid file"<< endl;
                break;
            }
            cout<< num_stacks<< num_blocks<< num_moves;
        }
        //reading in the initial stacks for 3 columns
        if(linecount == 2 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack1.push_back(c);
        }
        if(linecount == 3 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack2.push_back(c);
        }
        if(linecount == 4 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack3.push_back(c);
        }
        //reading in the goal stacks for 3 columns
        if(linecount == 5 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack1.push_back(c);
        }
        if(linecount == 6 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack2.push_back(c);
        }
        if(linecount == 7 && num_stacks==3){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack3.push_back(c);
        }
        //reading in the stacks for 5 columns
        if(linecount == 2 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack1.push_back(c);
        }
        if(linecount == 3 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack2.push_back(c);
        }
        if(linecount == 4 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack3.push_back(c);
        }
        if(linecount == 5 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack4.push_back(c);
        }
        if(linecount == 6 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) stack5.push_back(c);
        }
        //reading in the goal stacks for 5 columns
        if(linecount == 8 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack1.push_back(c);
        }
        if(linecount == 9 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack2.push_back(c);
        }
        if(linecount == 10 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack3.push_back(c);
        }
        if(linecount == 11 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack4.push_back(c);
        }
        if(linecount == 12 && num_stacks==5){
            string s = line;
            for (char c : s) if (!isspace((unsigned char)c)) goalstack5.push_back(c);
        }
        linecount++;
    }
    for (char i: stack1)
    std::cout << i << ' ';
    cout<<endl;
    for (char i: stack2)
    std::cout << i << ' ';
    cout<<endl;
    for (char i: stack3)
    std::cout << i << ' ';
    cout << "File read completed." << endl;
}

// --- Program entry point ---
int main(int argc, char *argv[]) {

    string filename = argv[1];
    blocksworld(filename);
    return 0;
}
