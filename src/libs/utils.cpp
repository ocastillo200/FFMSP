
#include "utils.h"

using namespace std;

string readParam(int argc, const vector<string> &argv, const string &key) {
    for (int i = 1; i < argc; i++) {
        const string &arg = argv[i];
        if (arg[0] != '-')
            continue;
        if (arg.find("-" + key) != string::npos) {
            return i + 1 < argc ? argv[i + 1] : "";
        }
    }
    return "";
}

bool containsFlag(int argc, const vector<string> &argv, const string &key) {
    for (int i = 1; i < argc; i++) {
        const string &arg = argv[i];
        if (arg[0] != '-')
            continue;
        size_t argLen = arg.length();
        if (arg.find("-" + key) != string::npos && argLen == key.length() + 1) {
            return true;
        }
    }
    return false;
}

int getStringLengthFromFilename(const string &filename) {
    size_t firstDash = filename.find('-');
    size_t secondDash = filename.find('-', firstDash + 1);
    string stringLengthStr =
        filename.substr(firstDash + 1, secondDash - firstDash - 1);
    return stoi(stringLengthStr);
}
