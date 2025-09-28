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
    int depth;
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
vector<State> visited;
string goalstack1;
string goalstack2;
string goalstack3;
string goalstack4;
string goalstack5; 
vector<string> allgoalstacks;
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
void successors(const vector<string> curr){
    //outsuccessors.clear();
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

bool checkGoalState(const vector<string>& curr){
    for(int i=0; i<num_stacks; i++){
        if(curr[i] != allgoalstacks[i]){
            return false;
            break;
        }
    }
    return true;
}
bool operator==(const State& a, const State& b) {
    return a.configuration == b.configuration;
}
bool isVisited(const State& s, const vector<State>& visited) {
    return find(visited.begin(), visited.end(), s) != visited.end();
}
// --- Program entry point ---
int main(int argc, char *argv[]) {

    string filename = argv[1];
    readfile(filename);
    //visited vector
    vector<State> visited;
    State current; 
    current.depth = 1;                                                           //TODO fix the depth updating and make array of correct moves
    current.configuration = allstacks;   // start state
    visited.push_back(current);

    int iters = 1;
    while (iters < 15000) {
        // 1) expand current
        successors(current.configuration);   // -> fills outsuccessors (ensure it clears first!)

        // 2) sort by heuristic (lowest first)
        sort(outsuccessors.begin(), outsuccessors.end(),
            [&](const State& a, const State& b) {
                return heuristicBottomPrefix(a.configuration, allgoalstacks)
                     < heuristicBottomPrefix(b.configuration, allgoalstacks);
            });

        // 3) pick the best **unvisited** successor
        bool advanced = false;
        for (size_t i = 0; i < outsuccessors.size(); ++i) {
            const State& cand = outsuccessors[i];
            if (!isVisited(cand, visited)) {
                // Update current to the chosen child
                current = cand;
                visited.push_back(current);

                cout << "Move #" << iters << "\n";
                for (int s = 0; s < num_stacks; ++s) {
                    cout << current.configuration[s] << "\n";
                }
                cout << "\n";

                // goal check
                if (checkGoalState(current.configuration)) {
                    cout << "Goal reached in " << iters << " iterations.\n";
                    return 0;
                }

                advanced = true;
                break;  // IMPORTANT: break so we don’t keep looking this layer
            }
        }

        if (!advanced) {
            cout << "All successors already visited at iteration " << iters << ".\n";
            break;
        }

        ++iters;
    }
    return 0;
}

