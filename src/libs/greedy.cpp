#include "greedy.h"
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_map>

using namespace std;

int findBestStartPosition(const vector<string> &omega, int stringLength) {
    vector<unordered_map<char, int>> positionCharCount(stringLength);
    for (const auto &line : omega) {
        for (int i = 0; i < stringLength; ++i) {
            positionCharCount[i][line[i]]++;
        }
    }
    int bestPosition = 0;
    int minDiversity = positionCharCount[0].size();
    for (int i = 1; i < stringLength; ++i) {
        int diversity = positionCharCount[i].size();
        if (diversity < minDiversity) {
            minDiversity = diversity;
            bestPosition = i;
        }
    }
    return bestPosition;
}

int hammingDistance(const string &s1, const string &s2, int position) {
    int distance = 0;
    for (int i = position; i < s1.length(); ++i) {
        if (s1[i] == ' ') {
            break;
        }
        if (s1[i] != s2[i]) {
            distance++;
        }
    }
    return distance;
}

int calculateCost(const string &currentSolution, const vector<string> &omega, double t, int position, int lenght) {
    int count = 0;
    int threshold = currentSolution.size() * t;
    for (size_t i = 0; i < omega.size(); ++i) {
        int distance = hammingDistance(currentSolution, omega[i], position);
        if (distance >= threshold) {
            count++;
        }
    }
    return count;
}

char chooseNextCharacter(const string &currentSolution, int position, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t, int lenght, int bestStartPosition) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double randomValue = dis(gen);
    if (randomValue <= epsilon) {
        int bestCost = calculateCost(currentSolution, omega, t,
                                     bestStartPosition, lenght);
        char bestChar = alphabet[0];
        bool didcostchange = false;
        for (char c : alphabet) {
            string tempSolution = currentSolution;
            tempSolution[position] = c;
            int cost = calculateCost(tempSolution, omega, t,
                                     bestStartPosition, lenght);
            if (cost > bestCost) {
                bestCost = cost;
                bestChar = c;
                didcostchange = true;
            }
        }
        if (!didcostchange) {
            unordered_map<char, int> charFrequency;
            for (const auto &line : omega) {
                charFrequency[line[position]]++;
            }
            char leastFrequentChar = alphabet[0];
            int minFrequency = charFrequency[leastFrequentChar];

            for (char c : alphabet) {
                if (charFrequency[c] < minFrequency) {
                    minFrequency = charFrequency[c];
                    leastFrequentChar = c;
                }
            }
            return leastFrequentChar;
        }
        return bestChar;
    } else {
        uniform_int_distribution<> dis(0, alphabet.size() - 1);
        int randomIndex = dis(gen);
        return alphabet[randomIndex];
    }
}

pair<int, string> constructGreedySolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega,double epsilon, double t) {
    string currentSolution(stringLength, ' ');
    int bestStartPosition = findBestStartPosition(omega, stringLength);
    for (int i = 0; i < stringLength; ++i) {
        int position = (bestStartPosition + i) % stringLength;
        char nextChar =
            chooseNextCharacter(currentSolution, position, alphabet, omega, epsilon, t, i, bestStartPosition);
        currentSolution[position] = nextChar;
    }
    int quality = calculateCost(currentSolution, omega, t, bestStartPosition, stringLength);
    pair<int, string> result = make_pair(quality, currentSolution);
    return result;
}
