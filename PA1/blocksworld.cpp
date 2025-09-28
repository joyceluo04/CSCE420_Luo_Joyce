#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<algorithm>
using namespace std;
class State {         //keeps track of the current state of stacks
  public:             
    vector <string> configuration;
    int heuristicscore;
};
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
void successors(const vector<string> curr){
    outsuccessors.clear();
    for (int from = 0; from < num_stacks; ++from) {
        if (curr[from].empty()) continue;     //current stack is empty
        const char topBlock = curr[from].back();     //grabs the top block

        for (int to = 0; to < num_stacks; ++to) {
            if (to == from) continue;                  //don't move the block onto the same column

            State nextState;
            nextState.configuration = curr;        // copy current config
            nextState.configuration[from].pop_back();               // remove from source top
            nextState.configuration[to].push_back(topBlock);        // push onto dest top
            int hscore = heuristicBottomPrefix(curr, allgoalstacks);
            nextState.heuristicscore = hscore;
            outsuccessors.push_back({nextState}); //added a state into the total amount of successors
        }
    }
}
void readfile(const string &filename) {
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
}
int heuristicBottomPrefix(const vector<string>& cur, const vector<string>& goal) {
    int h = 0;
    size_t n = min(cur.size(), goal.size());
    for (size_t i = 0; i < n; ++i) {
        const string& cs = cur[i];
        const string& gs = goal[i];
        size_t k = 0; // length of common bottom-aligned prefix
        while (k < cs.size() && k < gs.size() && cs[k] == gs[k]) ++k;
        h += int(cs.size() - k); // blocks above the locked-in prefix
    }
    return h;
}
bool checkGoalState(const vector<string>& curr){
    for(int i=0; i<num_stacks; i++){
        if(curr[i] != allgoalstacks[i]){
            return false;
        }
    }
    return true;
}
// --- Program entry point ---
int main(int argc, char *argv[]) {

    string filename = argv[1];
    readfile(filename);
    //find successors after initializing start/goal states
    successors(allstacks);
    int iters = 0;
    while(iters<15000){
        //sort successors from best to worst heuristic score
        sort(outsuccessors.begin(), outsuccessors.end(),
        [&](const State& a, const State& b){
         return heuristicBottomPrefix(a.configuration, allgoalstacks)
              < heuristicBottomPrefix(b.configuration, allgoalstacks);
        });
        //pick the best one at the front of the vector
        //run successors again
        //keep going until we find goal state
    }
}

