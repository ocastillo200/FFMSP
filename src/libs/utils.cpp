#include <filesystem>

#include "utils.h"

namespace fs = std::__fs::filesystem;

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

pair<string, string> parseFilename(const string &filename) {
    string n = filename.substr(0, filename.find('-'));
    string stringLengths = filename.substr(filename.find('-') + 1, filename.rfind('-') - filename.find('-') - 1);
    return make_pair(n, stringLengths);
}

vector<pair<string, string>> getFilesFromFolder(const string &folder, const string &amountOfFiles) {
    vector<pair<string, string>> files;
    for (const auto &entry : fs::directory_iterator(folder)) {
        pair<string, string> parsed = parseFilename(entry.path().filename().string());
        if (parsed.first == amountOfFiles) {
            files.push_back(make_pair(parsed.second, entry.path().string()));
        }
    }
    return files;
}

double calculateStandardDeviation(const vector<double> &qualities, double mean) {
    double sum = 0.0;
    for (double quality : qualities) {
        sum += pow(quality - mean, 2);
    }
    return sqrt(sum / qualities.size());
}