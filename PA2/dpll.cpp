#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;
//GLOBAL VARIABLES 
vector<vector<int>> mapColorKB; //This will store our map color clauses
unordered_map<string,int> regionColorIndex = //essentially a hashmap returning the index of the truth value
{{"WAR", 1},{"WAG", 2},{"WAB", 3},
{"NTR", 4},{"NTG", 5},{"NTB", 6},
{"SAR", 7},{"SAG", 8},{"SAB", 9},
{"QR", 10},{"QG", 11},{"QB", 12},
{"NSWR", 13},{"NSWG", 14},{"NSWB", 15},
{"VR", 16},{"VG", 17},{"VB", 18},
{"TR", 19},{"TG", 20},{"TB", 21}};
vector<int> mapColorTruthValue(22,0); //intialize all truth values to 0(undefined)
vector<int> backtrackOrder; //stores the order in which we assigned variables using unit clause heuristic
int iters = 0; //keeps track of how many times we have looped in dpll
//Function: readMapColorFile 
//Purpose: Take in a cnf file and convert it into a vector of numbered clauses using the regionColorIndex map
void readMapColorFile(string &filename){
    ifstream in(filename); //initialize ifstream
    string line;
    int index;
    while(getline(in,line)){ //while loop to grab all lines
        if(line[0] == '#'){ //ignore comment lines
            continue;
        }
        istringstream iss(line);
        string regionVar;
        vector<int> clause; //this will hold the entire clause of numbers
        while(iss>> regionVar){
            bool isNegation = false; //tracking whether the stored index number will be pos or neg
            if(regionVar[0] =='-'){
                isNegation =true; //index saved must be negative to denote a negation
                index = regionColorIndex[regionVar.substr(1)];
            }
            else{
                index = regionColorIndex[regionVar];
            }
            if(isNegation== true){
                clause.push_back(-index); //use neg index to track negation
            }
            else{
                clause.push_back(index); //use pos index to track normal var
            }
        }
        mapColorKB.push_back(clause); //add the finished clause to the KB
    }
    cout<< "File read. Clauses saved."<< endl;
}
//Helper Function checkSat
bool checkSat(const vector<int> &clause){
    bool truthFound = false;
    for(int var: clause){
        if(var<0){ //negation so looking for a -1(F)
            int truthVal = mapColorTruthValue[abs(var)];
            if(truthVal == -1){
                truthFound = true;
            }
        }
        else if(var>0){ //not a negation so looking for a 1(T)
            int truthVal = mapColorTruthValue[var];
            if(truthVal == 1){
                truthFound = true;
            }    
        }
    }
    if(truthFound){
        return true;
    }
    else{
        return false;
    }
}
//Helper function checkConflict
bool checkConflict (const vector<int> &clause){
    int numFalse = 0;
    for(int var: clause){
        if(var<0){ //negation so looking for a 1(T)
            int truthVal = mapColorTruthValue[abs(var)];
            if(truthVal == 1){
                numFalse++;
            }
        }
        else if(var>0){ //not a negation so looking for a -1(F)
            int truthVal = mapColorTruthValue[var];
            if(truthVal == -1){
                numFalse++;
            }    
        }
    }
    if(numFalse == clause.size()){
        return true;
    }
    else{
        return false;
    }  
}
//Helper Function findUnitClause: loops through KB and finds a clause where 1 var is undefined and the rest are false and returns the index
int findUnitClause(const vector<int> &clause){
    int undefinedCount = 0;
    int unitVar = 0;
    for (int index : clause){
        //get the index
        int absindex = abs(index);
        //get the truth value
        int truthVal = mapColorTruthValue[absindex];
        if(index>0 && truthVal==1 || index<0 && truthVal==-1){
            return 0; //found a truth
        }
        if(truthVal == 0){
            undefinedCount++;
            unitVar = absindex;
        }
    }
    if(undefinedCount==1){
        return unitVar;
    }else{
        return 0;
    }
}
//This implementation is slightly different/jank, it does not call recursively because I use a loop and search all combination branches greedily with the unit clause heuristic
//Function dpll for map coloring: returns true if we get a solution and false if not
bool mapColordpll(const string& given){
    int currIndex = 1;
    bool solutionFound = false;
    bool conflictFound = false;
    //initialize the first truth assignment
    mapColorTruthValue[1] = 1;
    backtrackOrder.push_back(1);
    if(given != ""){
        int index = regionColorIndex.at(given);
        vector<int> clause; //make a clause with a single index to force variable positive
        clause.push_back(index);
        mapColorKB.push_back(clause);
    }
    while(solutionFound == false){
        iters++;
        conflictFound = false; //reset conflict found for every loop
        //loop through all clauses, if conflict found then break
        for(auto clause : mapColorKB){
            if(checkSat(clause)){
                continue;
            }
            else if(checkConflict(clause)){
                conflictFound = true;
                break;
            }
        }
        //handle the conflict and backtrack
        if(conflictFound){
            while (backtrackOrder.empty()==false){
                int last = backtrackOrder.back(); //grab the last assigned var
                if(mapColorTruthValue[last] == 1){ //if its true then try false
                    mapColorTruthValue[last] = -1;
                    break;
                }
                else if(mapColorTruthValue[last] == -1){ //if its false then pop and check the next backtracked var
                    mapColorTruthValue[last] = 0;
                    backtrackOrder.pop_back();
                }
            }
            if(backtrackOrder.empty()){ //if theres nothing left to backtrack then no solution is found
                return false;
            }
        }
        else if(conflictFound == false){ //no conflict found
            //find the unit clause
            bool unitClauseFound = false;
            for(auto clause : mapColorKB){
                int unitIndex = findUnitClause(clause);
                if(unitIndex != 0){ //make sure the we found a unit index and that its not 0
                    mapColorTruthValue[unitIndex] = 1; //assign the index true to go ahead and satisfy the clause
                    backtrackOrder.push_back(unitIndex);
                    unitClauseFound = true;
                    break;
                }
            }
            if(unitClauseFound==false){ //no unit clauses so we just pick the first unassigned variable to assign true
                int index = 0;
                for(auto i = 1; i < mapColorTruthValue.size(); i++){
                    if(mapColorTruthValue[i] == 0){
                        index = i;
                        break;
                    }
                }
                if(index == 0){
                    solutionFound = true; //no more variables left to assign so solution is found
                }
                else{
                    mapColorTruthValue[index] = 1; //assign the unit clause index to 1 for True
                    backtrackOrder.push_back(index);
                }
            }
        }
        if(conflictFound==false){ //last but not least check of we have found a solution
            int numSatisfied=0;
            for(auto clause : mapColorKB){
                if(checkSat(clause)==true){
                    numSatisfied++;
                }
            }
            if(numSatisfied==mapColorKB.size()){
                solutionFound = true;
            }
        }
    }
    return true; //if we exited the while loop that means solution was found
}

//Main Function that calls dpll
int main(int argc, char *argv[]){
    string filename = argv[1];
    string given = "";
    if (argc >= 3){
        given = argv[2];
    }
    if(filename=="mapcolor.cnf"){
        readMapColorFile(filename);
        bool solutionFound = mapColordpll(given);
        if(solutionFound){
            cout << "Number of Iterations: " << iters<< endl;
            cout << "Final Solution Assignments"<< endl;
            for(auto x: regionColorIndex){
                string color = x.first;
                int index = x.second;
                cout<< color<< ": "<< mapColorTruthValue[index]<< endl;
            }
        }
        else{
        cout <<"No solution found."<< endl;
        }
    }
    //TODO 6 queens here
    return 0;
}