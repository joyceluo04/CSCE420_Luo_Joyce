#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<algorithm>
using namespace std;
//State class keeps track of each configuration's stacks, heuristic score, parent, depth, and fn score
class State {         
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
string functionchoice;
long unsigned int max_queue_size = 0;
string stack1;
string stack2;
string stack3;
string stack4;
string stack5;
vector<State> outsuccessors;
vector<State> parents;
vector<State> solution;
vector<State> visited;
string goalstack1;
string goalstack2;
string goalstack3;
string goalstack4;
string goalstack5; 
//allstacks stores the current configurations stacks in a string vector (subject to change)
//allgoalstacks stores the goal configuration stacks in a string vector (not subject to change)
vector<string> allstacks;
vector<string> allgoalstacks;
//Function heuristicWrongBlocks: calculates how many blocks in each stack are in right order and adds it together to get an hscore
int heuristicWrongBlocks(vector<string> curr, vector<string> goal) {
    int score = 0;  //score tracks our heuristic score
    for (int i = 0; i < num_stacks; i++) {
        long unsigned int correctnum = 0; 
        while (correctnum < curr[i].size() && correctnum < goal[i].size() && curr[i][correctnum] == goal[i][correctnum]){
            correctnum++;
        }
        //heuristic is added depending on how many blocks in the stack are in the wrong spot vs how many are right
        //the bigger the hscore, the more off we are from the solution
        score = score + (curr[i].size() - correctnum); 
    }
    return score;
}
int bfs(vector<string> curr, vector<string> goal){
    return 0; //bfs only cares about path depth, heuristic returns 0, queue sorts by depth
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
        //run through all the possible block moves with the grabbed block
        for (int to = 0; to < num_stacks; to++) {
            if (from == to){
                //we cannot move the same block onto the same column, thats redundant
                continue;
            }
            //creates and updates the details for the successor State
            State nextState;
            nextState.configuration = curr->configuration;
            //if we are at depth 0, then this is the parent node, there is no parent index so we set it to -1
            if(nextState.depth == 0){
                nextState.parentidx = -1;
            }
            //if the depth is 1 then the parent is the initial stack configuration, which was the first state pushed into the parents vector
            else if (nextState.depth == 1){
                nextState.parentidx = 0;
            }
            else{
                //push back the parent and assign the successors parentidx to the last idx of the vector which is size()-1
                parents.push_back(*curr);
                nextState.parentidx = parents.size()-1;          
            }
            //grab the top block by popping the end of the string
            nextState.configuration[from].pop_back(); 
            //add the grabbed block to another column using push which adds to end of the string         
            nextState.configuration[to].push_back(grabbedblock);    
            //calculate the new heuristic score for the successor
            if(functionchoice=="bfs"){
                //bfs "heuristic" score is just 0
                nextState.heuristicscore = bfs(nextState.configuration, allgoalstacks);
            }
            else{
                //set the heuristic score by running it through the function with the current stacks and the goal stacks for reference
                nextState.heuristicscore = heuristicWrongBlocks(nextState.configuration,allgoalstacks);
            }

            //add one layer of depth for the successor state
            nextState.depth = curr->depth + 1; 
            //fn score is depth(cost)+hscore = total score
            nextState.fn = nextState.depth + nextState.heuristicscore;
            //add the successor state into the priority queue
            outsuccessors.push_back(nextState);
            //track max queue size
            if(outsuccessors.size()>max_queue_size){
                max_queue_size = outsuccessors.size();
            }
        }
    }
}
//Function readfile: reads in the problem bwp file and initializes the stacks/goal
void readfile(const string &filename) {
    cout << "File: " << filename << endl;
    //error handling in case the problem file is wrong
    ifstream in(filename);
    if (!in) {
        cerr << "Error occured while reading " << filename << endl;
        return;
    }

    string line;
    int linecount = 0; 
    //while loop reads line by line
    while (getline(in, line)){
        //initializes num_stacks, num_blocks, and num_moves using the first line of information
        if (linecount == 0){
            std::istringstream iss(line);
            if (!(iss >> num_stacks >> num_blocks >> num_moves)) {
                cout<< "Invalid file format"<< endl;  //throws error if the numbers didn't populate correctly
                break;
            }
        }
        //reading in the initial stacks for 3 columns (this is done in a rudimentary way/hardcoded)
        if(linecount == 2 && num_stacks==3){
            stack1 = line;
        }
        if(linecount == 3 && num_stacks==3){
            stack2 = line;
        }
        if(linecount == 4 && num_stacks==3){
            stack3 = line;
            //initializes all the stacks after reading 3 lines
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
    //helper operator overload to check if two states configurations are equal to eachother 
    return a.configuration == b.configuration;                    
}
//Function isVisited takes in the successor state we want to search and returns whether or not it has been visited inside the visited vector
bool isVisited(const State& s, const vector<State>& visited) {
    //could improve by deleting from visited if we have seen the state before
    return find(visited.begin(), visited.end(), s) != visited.end();
}
int main(int argc, char *argv[]) {

    string filename = argv[1];
    functionchoice = argv[2];
    if(functionchoice=="h0"){
        functionchoice = "bfs";
    }
    else if(functionchoice=="h1"){
        functionchoice = "heuristic";
    }
    else{
        cout<< "Error: Choose either h0 BFS or h1 heuristic."<< endl;
    }
    readfile(filename);
    //visited vector
    vector<State> visited;
    //initialize our first state with the given problem stacks
    State current; 
    current.depth = 0;                                                           
    current.configuration = allstacks;
    current.parentidx = -1;
    //track our initial start stack as visited and push back the initial state as parents[0] which will be set as the parent of any node with a depth of 1
    visited.push_back(current);
    parents.push_back(current);

    int iters = 1;
    while (iters < 15000) {
        //generate successors into outsuccessors vector (which is our priority queue)
        successors(&current);  
        //now sort the priority queue by lowest heuristic score
        //todo sort differently based on heuristic or bfs
        if(functionchoice == "bfs"){
            sort(outsuccessors.begin(), outsuccessors.end(), [&](const State& a, const State& b) {
                return (bfs(a.configuration, allgoalstacks)+a.depth) < (bfs(b.configuration, allgoalstacks)+b.depth);
            });  
        }
        else{
            sort(outsuccessors.begin(), outsuccessors.end(), [&](const State& a, const State& b) {
                    return (heuristicWrongBlocks(a.configuration, allgoalstacks)) < (heuristicWrongBlocks(b.configuration, allgoalstacks));
            });
        }
        //loop through outsuccessors until we find a node that hasn't been visited
        for (size_t i = 0; i < outsuccessors.size(); ++i) {
            const State& newNode = outsuccessors[i];
            if (isVisited(newNode, visited) != true) {
                //we go forward with the chosen node
                current = newNode;
                //track the chosen node in current
                visited.push_back(current);

                // goal check
                if (checkGoalState(current.configuration)) {
                    int parentidx = current.parentidx;
                    vector <State> path;
                    path.push_back(current);
                    //loop through each parent of the goal state and push into a path vector
                    while(parentidx != -1){
                        path.push_back(parents[parentidx]);
                        parentidx = parents[parentidx].parentidx;
                    }
                    //we reverse the path because the path is initially tracing backwards
                    reverse(path.begin(), path.end());
                    int movenum = 0;
                    for(State x: path){
                        //my heuristic score is slightly odd sometimes because of negative + positive scores of each stack in the configuration, it may show as 0
                        cout<< "Move # " << movenum << "  Path cost: "<< movenum<< "  Heuristic: "<< x.heuristicscore<< "  f(n) = g(n)+h(n) = "<< x.fn << endl;
                            for (int i = 0; i<num_stacks; i++) {
                            cout<< x.configuration[i]<< endl;
                        }
                        cout<<endl;
                        movenum++;
                    }
                    //the cost is the path size and how many moves we made, the depth is how deep we searched into the successors to find the goal node
                    cout <<"Solution reached using "<< functionchoice<<  ".  # Iterations: " << iters << "  Cost:  "<< path.size()-1 << "  Depth: "<< current.depth<< "  Max queue size: "<< max_queue_size<< endl;
                    return 0;
                }
                //we break because we have found a good unvisted node and will now advance to searching for the new nodes successors in the next for loop
                break; 
            }
        }
        iters++;
    }
    return 0;
}
