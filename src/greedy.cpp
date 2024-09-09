#include "greedy.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <random>
#include <algorithm>

using namespace std;

// LISTA
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

// REVISARLA
int hammingDistance(const string &s1, const string &s2, int position)
{
    int distance = 0;
    for (size_t i = 0; i < s1.length(); ++i)
    {
        int pos = (position + i) % s1.length();
        if (s1[pos] == ' ')
        {
            break;
        }
        if (s1[pos] != s2[pos])
        {
            distance++;
        }
    }
    return distance;
}

// REVISARLA
int calculateCost(const string &currentSolution, const vector<string> &omega, double t, const vector<bool> &alreadySatisfied, int position)
{
    int count = 0;
    for (size_t i = 0; i < omega.size(); ++i)
    {
        int distance = hammingDistance(currentSolution, omega[i], position);
        count += distance;
    }
    return count;
}

char chooseNextCharacter(const string &currentSolution, int position, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t, const vector<bool> &alreadySatisfied, int count)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double randomValue = dis(gen);
    if (randomValue <= epsilon)
    {
        int bestCost = 0;
        char bestChar = alphabet[0];
        for (char c : alphabet)
        {
            string tempSolution = currentSolution;
            tempSolution[position] = c;
            int cost = calculateCost(tempSolution, omega, t, alreadySatisfied, count);
            if (cost > bestCost)
            {
                bestCost = cost;
                bestChar = c;
            }
        }
        if (bestCost == 0)
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
        return bestChar;
    }
    else
    {
        uniform_int_distribution<> dis(0, alphabet.size() - 1);
        int randomIndex = dis(gen);
        return alphabet[randomIndex];
    }
}

// REVISARLA
string constructGreedySolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t)
{
    string currentSolution(stringLength, ' ');
    vector<bool> alreadySatisfied(omega.size(), false);
    int bestStartPosition = findBestStartPosition(omega, stringLength);
    for (int i = 0; i < stringLength; ++i)
    {
        int position = (bestStartPosition + i) % stringLength;
        char nextChar = chooseNextCharacter(currentSolution, position, alphabet, omega, epsilon, t, alreadySatisfied, i);
        currentSolution[position] = nextChar;
        /*  for (size_t j = 0; j < omega.size(); ++j)
          {
              if (!alreadySatisfied[j])
              {
                  if (hammingDistance(currentSolution, omega[j]) >= t * stringLength)
                  {
                      alreadySatisfied[j] = true;
                  }
              }
          } */
    }
    return currentSolution;
}
