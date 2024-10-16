#include <filesystem>

#include "utils.h"
#include <dirent.h>
#include <iostream>

namespace fs = std::__fs::filesystem;

using namespace std;

string readParam(int argc, const vector<string> &argv, const string &key) {
    string fullKey = "-" + key;
    for (int i = 1; i < argc; i++) {
        const string &arg = argv[i];
        if (arg == fullKey) {
            return (i + 1 < argc) ? argv[i + 1] : "";
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
    struct dirent *entry;
    DIR *dir = opendir(folder.c_str());

    if (dir == nullptr) {
        cout << "No se pudo abrir el directorio: " << folder << endl;
        return files;
    }
    while ((entry = readdir(dir)) != nullptr) {
        string filename = entry->d_name;
        if (filename == "." || filename == "..") {
            continue;
        }

        pair<string, string> parsed = parseFilename(filename);
        if (parsed.first == amountOfFiles) {
            files.push_back(make_pair(parsed.second, folder + "/" + filename));
        }
    }

    closedir(dir);
    return files;
}

double calculateStandardDeviation(const vector<double> &qualities, double mean) {
    double sum = 0.0;
    for (double quality : qualities) {
        sum += pow(quality - mean, 2);
    }
    return sqrt(sum / qualities.size());
}
