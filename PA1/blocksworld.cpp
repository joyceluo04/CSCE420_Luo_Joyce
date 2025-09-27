#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
int num_stacks = 0;
int num_blocks = 0;
int num_moves = 0;
string stack1;
string stack2;
string stack3;
string stack4;
string stack5;
vector<string> allstacks;
vector<State> outsuccessors;
string goalstack1;
string goalstack2;
string goalstack3;
string goalstack4;
string goalstack5; 
vector<string> allgoalstacks;
class State {         //
  public:             // Access specifier
    int myNum;        // Attribute (int variable)
    string myString;  // Attribute (string variable)
    vector <string> configuration;
};
void blocksworld(const string &filename) {
    cout << "Problem # file: " << filename << endl;

    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open " << filename << endl;
        return;
    }

    string line;
    int linecount = 0; 
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
            stack1 = line;
        }
        if(linecount == 3 && num_stacks==3){
            stack2 = line;
        }
        if(linecount == 4 && num_stacks==3){
            stack3 = line;
            allstacks = {stack1, stack2, stack3};
        }
        //reading in the goal stacks for 3 columns
        if(linecount == 6 && num_stacks==3){
            goalstack1 = line;
        }
        if(linecount == 7 && num_stacks==3){
            goalstack2 = line;
        }
        if(linecount == 8 && num_stacks==3){
            goalstack3 = line;
            allgoalstacks = {goalstack1, goalstack2, goalstack3};
        }
        //reading in the stacks for 5 columns
        if(linecount == 2 && num_stacks==5){
            stack1 = line;
        }
        if(linecount == 3 && num_stacks==5){
            stack2 = line;
        }
        if(linecount == 4 && num_stacks==5){
            stack3 = line;
        }
        if(linecount == 5 && num_stacks==5){
            stack4 = line;
        }
        if(linecount == 6 && num_stacks==5){
            stack5 = line;
            allstacks = {stack1, stack2, stack3, stack4, stack5};
        }
        //reading in the goal stacks for 5 columns
        if(linecount == 8 && num_stacks==5){
            goalstack1 = line;
        }
        if(linecount == 9 && num_stacks==5){
            goalstack2 = line;
        }
        if(linecount == 10 && num_stacks==5){
            goalstack3 = line;
        }
        if(linecount == 11 && num_stacks==5){
            goalstack4 = line;
        }
        if(linecount == 12 && num_stacks==5){
            goalstack5 = line;
            allgoalstacks = {goalstack1, goalstack2, goalstack3, goalstack4, goalstack5};
        }
        linecount++;
    }
    cout << "File read completed." << endl;
    //generate successors now
//successor here
}
void successors(const vector<vector<char>> curr){
    outsuccessors.clear();
    for (int from = 0; from < num_stacks; ++from) {
        if (curr[from].empty()) continue;            // nothing to move

        const char topBlock = curr[from].back();     // block on top of 'from'

        for (int to = 0; to < num_stacks; ++to) {
            if (to == from) continue;                  // no self-move

            State nextState;
            nextState.allstacks = curr;        // copy current config
            nextState.allstacks[from].pop_back();               // remove from source top
            nextState.allstacks[to].push_back(topBlock);        // push onto dest top

            outsuccessors.push_back({nextState});
        }
    }
}
// --- Program entry point ---
int main(int argc, char *argv[]) {

    string filename = argv[1];
    blocksworld(filename);
    return 0;
}
