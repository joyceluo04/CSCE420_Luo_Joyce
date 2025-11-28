#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<algorithm>
using namespace std;
vector<vector<int>> mapColorKB; //this is our knowledge base that stores each clause in a vector
unordered_map<string, int> regionColorIndex = {
    {"WAR", 1}, {"WAG", 2}, {"WAB", 3},
    {"NTR", 4}, {"NTG", 5}, {"NTB", 6},
    {"SAR", 7}, {"SAG", 8}, {"SAB", 9},
    {"QR", 10}, {"QG", 11}, {"QB", 12},
    {"NSWR", 13}, {"NSWG", 14}, {"NSWB", 15},
    {"VR", 16}, {"VG", 17}, {"VB", 18},
    {"TR", 19}, {"TG", 20}, {"TB", 21}
};
vector<int> mapColorTruthValue(22, 0); //keep it with 1-based indexing to match the map, everything starts with 0(undefined truth value)
vector<int> backtrackOrder; //tracks the order we assigned variables in with unit clause heuristic
//Function readfile: reads in the problem CNF file and initializes the KB vector
void readMapColorFile(const string &filename) {
    cout << "File: " << filename << endl;
    //error handling in case the problem file is wrong
    ifstream in(filename);
    if (!in) {
        cerr << "Error occured while reading " << filename << endl;
        return;
    }

    string line;
    //while loop reads line by line
    while (getline(in, line)){
        //skip the line if empty
        if(line.empty() == true){
            continue;
        }
        //in CNF files, c is used to denote a comment instead of the # that was written in the PA instructions
        if(line[0] == 'c'){
            continue;
        }
        else{ //If there is no c then we are on a clause line
            std::istringstream iss(line);
            string regionColor;
            vector<int> clause;
            while (iss >> regionColor) {
                bool isNeg = false;

                //If the var is a negation then keep track of it in boolean and grab the rest of the string
                if (regionColor[0] == '-') {
                    isNeg = true;
                    regionColor = regionColor.substr(1);
                }
                //Look up the var match in the map
                int id = regionColorIndex[regionColor];
                //If we have negated literal then push the negative number
                if(isNeg){
                    clause.push_back(-id);
                }
                else{
                    clause.push_back(id);
                }
            }
            mapColorKB.push_back(clause);
        }
        
    }
    cout << "File read completed." << endl;

    cout << "\n=== CNF Knowledge Base ===\n";       //printing out the clauses
    for (size_t i = 0; i < KB.size(); i++) {
        cout << "Clause " << i+1 << ": ";
        for (int lit : KB[i]) {
            cout << lit << " ";
        }
        cout << "\n";  // DIMACS-style clause terminator
    }
    cout << "==========================\n\n";

}
//HELPER FUNCTIONS
bool checkClauseSatisfaction (vector<int>& clause){

}
//Func: checkUnitClause checks to see if the clause has 1 undefined while the rest are false
//TODO understand and fix this 
int findUnitVar(const vector<int>& clause) {
    int undefinedCount = 0;
    int unitVar = 0;  // we will store the undef var index here

    for (int lit : clause) {
        int var = abs(lit);        //lit and var are the index of the truth value
        int val = mapColorTruthValue[var];     //this is the truth value at the index

        // Check if literal is TRUE
        bool literalIsTrue =
            (lit > 0 && val == 1) ||     // positive literal AND var=TRUE
            (lit < 0 && val == -1);      // negative literal AND var=FALSE

        if (literalIsTrue) {
            // Clause is satisfied → NOT a unit clause
            return 0;     //returning 0 means there was no unit literal found and clause is satisfied
        }

        // Count undefined variables
        if (val == 0) {
            undefinedCount++;
            unitVar = var;  //set unitvar undef index here
        }
    }

    // Unit clause → exactly 1 undefined var, no true literals
    return (undefinedCount == 1 ? unitVar : 0); //if undefined count is 1 then return unitVar otherwise return 0
}
bool mapColordpll(string filename, string given){
        int currIndex = 1; //tracking what variable index we are at
        bool solutionFound = false; 
        bool conflictFound = false;
    if(filename.equals("mapcolor.cnf")){ //use the mapcolor KB and truth map/assignments
        //TODO PSEUDOCODE
        mapColorTruthValue[1] =true; //set up while loop
        backtrackOrder.push_back(1); //we start with WAR, which is the first value
        while (solutionFound == false){
            loop through all the clauses{
                if(check clause satisfaction with helper == true){
                    continue
                }else if (check clause conflict == true){
                    conflictFound = true;
                    break;
                }
            }
            if( conflictFound is true ){
                loop to check last index of backtrackOrder if its true then set it to false
                if its false then erase the last element and loop
            }
            else{ //if no conflicts found
                if findUnitVar is not zero then
                    nextVar = findUnitVar
                    set unit var index to true and append it to backtrackOrder
                else loop through mapColorTruthValue to find an index with 0 and then assign it true
                    backtrackOrder append the index
                    if there is no undefined value then solutionFound is true; continue;
            }
        }
    }
}
int main(int argc, char *argv[]) {
    string filename = argv[1];
    string given = ""; // default given is empty
    if (argc >= 3) { //if we have more than 3 elements, then we can assign a given
        given = argv[2]; 
    }
    readMapColorFile(filename);
    //dpll(filename, given);    //send over the file name so dpll knows what map and truth vectors to use!
    return 0;
}