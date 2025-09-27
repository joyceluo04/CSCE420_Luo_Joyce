#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void blocksworld(const string &filename) {
    cout << "Running blocksworld with file: " << filename << endl;

    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open " << filename << endl;
        return;
    }

    // Example: read and print file line by line
    string line;
    while (getline(in, line)) {
        cout << "Read: " << line << endl;
    }

    cout << "Blocksworld completed." << endl;
}

// --- Program entry point ---
int main(int argc, char *argv[]) {

    string filename = argv[1];
    blocksworld(filename);
    return 0;
}
