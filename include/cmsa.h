#ifndef CMSA_H
#define CMSA_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <ilcplex/ilocplex.h>

using namespace std;

struct SolutionComponent {
    int position;
    char letter;
    int age;
    bool operator==(const SolutionComponent& other) const {
        return position == other.position && letter == other.letter && age == other.age;
    }
};

namespace std {
template <>
struct hash<SolutionComponent> {
    size_t operator()(const SolutionComponent& sc) const {
        return hash<int>()(sc.position) ^ hash<char>()(sc.letter) ^ hash<int>()(sc.age);
    }
};
}

struct Solution {
    vector<SolutionComponent> components;
    int fitness;
    Solution() : fitness(0) {}
};

Solution CMSA(const vector<string> &inputStrings, const vector<char> &alphabet, int na, int agemax, int tsolver, double epsilon, double t, double timelimit);
Solution constructSolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t);
Solution applyExactSolver(const unordered_set<SolutionComponent> &subInstance, const vector<string> &omega, int m, double t);
void adapt(unordered_set<SolutionComponent> &C_prime, Solution &optimalSolution, int agemax);

#endif