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
    int parentidx;
    int heuristicscore;
    int depth;
    int fn;
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
vector<State> parents;
vector<State> solution;
vector<State> visited;
string goalstack1;
string goalstack2;
string goalstack3;
string goalstack4;
string goalstack5; 
vector<string> allgoalstacks;
int heuristicWrongBlocks(vector<string> curr, vector<string> goal) {
    int score = 0;
    //size_t n = min(cur.size(), goal.size());
    for (int i = 0; i < num_stacks; ++i) {
        string currstack = curr[i];
        string currgoal = goal[i];
        //number of correct blocks from table up
        int correctnum = 0; 
        while (correctnum < currstack.size() && correctnum < currgoal.size() && currstack[correctnum] == currgoal[correctnum]){
            correctnum++;
        }
        //heuristic is added depending on how many blocks in the row are in the wrong spot vs how many are right
        //the bigger the hscore, the more off we are from the solution
        score = score + int(currstack.size() - correctnum); 
    }
    return score;
}
void successors(State* curr){
    //from is what column were taking the block from
    //to is what column we moved it to
    for (int from = 0; from < num_stacks; from++) {
        if (curr->configuration[from]==""){
            continue; //nothing to grab because the column is empty
        }
        //grabs the last char of the string which is the "top" block
        char grabbedblock = curr->configuration[from].back();   

        for (int to = 0; to < num_stacks; to++) {
            if (from == to){
                //we cannot move the same block onto the same column, thats redundant
                continue;
            }
            State nextState;
            //set the current state in the State class object
            nextState.configuration = curr->configuration;
            if(nextState.depth == 0){
                nextState.parentidx = -1;
            }
            else if (nextState.depth == 1){
                nextState.parentidx = 0;
            }
            else{
                parents.push_back(*curr);
                nextState.parentidx = parents.size()-1;             //todo fix
            }
            //grab the top block by popping the end of the string
            nextState.configuration[from].pop_back(); 
            //add the grabbed block to another column using push which adds to end of the string         
            nextState.configuration[to].push_back(grabbedblock);    
            //calculate the new heuristic score for the successor
            nextState.heuristicscore = heuristicWrongBlocks(nextState.configuration,allgoalstacks);
            nextState.depth = curr->depth + 1; 
            nextState.fn = nextState.depth + nextState.heuristicscore;
            //add the state into the queue
            outsuccessors.push_back({nextState});
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
    return a.configuration == b.configuration;                      //TODO fix boolean isVisited and change the find function?
}
bool isVisited(const State& s, const vector<State>& visited) {
    return find(visited.begin(), visited.end(), s) != visited.end();
}
int main(int argc, char *argv[]) {

    string filename = argv[1];
    readfile(filename);
    //visited vector
    vector<State> visited;
    //initialize our first state with the given problem stacks
    State current; 
    current.depth = 0;                                                           //TODO fix the depth updating and make array of correct moves
    current.configuration = allstacks;
    current.parentidx = -1;
    visited.push_back(current);
    parents.push_back(current);

    int iters = 1;
    while (iters < 15000) {
        //generate successors into outsuccessors vector (which is our priority queue)
        successors(&current);  
        //now sort the priority queue by lowest heuristic score
        sort(outsuccessors.begin(), outsuccessors.end(),
            [&](const State& a, const State& b) {
                return heuristicWrongBlocks(a.configuration, allgoalstacks)
                     < heuristicWrongBlocks(b.configuration, allgoalstacks);
            });

        for (size_t i = 0; i < outsuccessors.size(); ++i) {
            const State& newNode = outsuccessors[i];
            if (!isVisited(newNode, visited)) {
                //we go forward with the chosen node
                current = newNode;
                //track the chosen node in current
                visited.push_back(current);
                //todo add the node into the solution vector and delete any other nodes from prior depths explored in the tree

                /*cout << "Move #" << iters << " Depth of "<< current.depth<< " F(n)=g(n)+h(n)= "<< current.fn <<endl;
                for (int s = 0; s < num_stacks; ++s) {
                    cout << current.configuration[s] << endl;            //TODO: fix the depth and print the solution out correctly
                }
                cout << endl;*/

                // goal check
                if (checkGoalState(current.configuration)) {
                    cout << "Goal reached in " << iters << " iterations.\n";
                    int parentidx = current.parentidx;
                    vector <State> path;
                    while(parentidx != -1){
                        path.push_back(parents[parentidx]);
                        parentidx = parents[parentidx].parentidx;
                    }
                    reverse(path.begin(), path.end());
                    for(State x: path){
                            for (int i = 0; i<num_stacks; i++) {
                            cout<< x.configuration[i]<< endl;
                        }
                        cout<<endl;
                    }
                    //TODO print out statistics, solution length, iterations, max queue size
                    /*vector<const State> path;
                    const State* q = &current;
                    while (q!= nullptr) {                // includes root; stops when parent == nullptr
                        path.push_back(*q);
                        q = q->parent;
                    }
                    reverse(path.begin(), path.end());

                    // Print
                    for (size_t i = 0; i < path.size(); ++i) {
                        cout << "Move #" << i
                            << "  Depth=" << path[i].depth
                            << "  f(n)=" << path[i].fn << "\n";
                        for (const auto& stack : path[i].configuration) {
                            cout << stack << "\n";
                        }
                        cout << "----\n";
                    }*/
                    return 0;
                }

                //todo add in the solution array
                //we break because we have found a good unvisted node and will now advance to searching for the new nodes successors
                break; 
            }
        }
        iters++;
    }
    return 0;
}
