#include "greedy.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <random>
#include <algorithm>

using namespace std;

int findBestStartPosition(const vector<string> &omega, int stringLength)
{
    vector<unordered_map<char, int> > positionCharCount(stringLength);
    for (const auto &line : omega)
    {
        for (int i = 0; i < stringLength; ++i)
        {
            positionCharCount[i][line[i]]++;
        }
    }
    int bestPosition = 0;
    int minDiversity = positionCharCount[0].size();
    for (int i = 1; i < stringLength; ++i)
    {
        int diversity = positionCharCount[i].size();
        if (diversity < minDiversity)
        {
            minDiversity = diversity;
            bestPosition = i;
        }
    }
    return bestPosition;
}

int hammingDistance(const string &s1, const string &s2)
{
    int distance = 0;
    for (size_t i = 0; i < s1.length(); ++i)
    {
        if (s1[i] != s2[i])
        {
            distance++;
        }
    }
    return distance;
}

int calculateCost(const string &currentSolution, const vector<string> &omega, double t, const vector<bool> &alreadySatisfied)
{
    int stringLength = currentSolution.length();
    int requiredDistance = t * stringLength;
    int count = 0;
    for (size_t i = 0; i < omega.size(); ++i)
    {
        if (!alreadySatisfied[i])
        {
            int distance = hammingDistance(currentSolution, omega[i]);
            if (distance >= requiredDistance)
            {
                count++;
            }
        }
    }
    return count;
}

char chooseNextCharacter(const string &currentSolution, int position, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t, const vector<bool> &alreadySatisfied)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double randomValue = dis(gen);
    if (randomValue <= epsilon)
    {
        int bestCost = -1;
        char bestChar = alphabet[0];

        for (char c : alphabet)
        {
            string tempSolution = currentSolution;
            tempSolution[position] = c;

            int cost = calculateCost(tempSolution, omega, t, alreadySatisfied);
            if (cost > bestCost)
            {
                bestCost = cost;
                bestChar = c;
            }
        }
        return bestChar;
    }
    else
    {
        unordered_map<char, int> charFrequency;
        for (const auto &line : omega)
        {
            charFrequency[line[position]]++;
        }
        char leastFrequentChar = alphabet[0];
        int minFrequency = charFrequency[leastFrequentChar];

        for (char c : alphabet)
        {
            if (charFrequency[c] < minFrequency)
            {
                minFrequency = charFrequency[c];
                leastFrequentChar = c;
            }
        }
        return leastFrequentChar;
    }
}

string constructGreedySolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t)
{
    string currentSolution(stringLength, ' ');
    vector<bool> alreadySatisfied(omega.size(), false);
    int bestStartPosition = findBestStartPosition(omega, stringLength);
    for (int i = 0; i < stringLength; ++i)
    {
        int position = (bestStartPosition + i) % stringLength;
        char nextChar = chooseNextCharacter(currentSolution, position, alphabet, omega, epsilon, t, alreadySatisfied);
        currentSolution[position] = nextChar;
        for (size_t j = 0; j < omega.size(); ++j)
        {
            if (!alreadySatisfied[j])
            {
                if (hammingDistance(currentSolution, omega[j]) >= t * stringLength)
                {
                    alreadySatisfied[j] = true;
                }
            }
        }
    }
    return currentSolution;
}
