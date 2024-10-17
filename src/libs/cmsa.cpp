#include "cmsa.h"
#include "greedy.h"
#include <chrono>
#include <random>
#include <functional>
#include <iostream>
#include <unordered_map>

Solution CMSA(const vector<string> &inputStrings, const vector<char> &alphabet, int na, int agemax, int tsolver, double epsilon, double t, double timelimit) {
    int m = inputStrings[0].size();
    Solution Sbsf;
    unordered_set<SolutionComponent> C_prime;
    unordered_map<SolutionComponent, int> age;
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    while (chrono::duration<double>(chrono::high_resolution_clock::now() - start).count() < timelimit) {
        for (int i = 0; i < na; ++i) {
            Solution S = constructSolution(m, alphabet, inputStrings, epsilon, t);
            for (const auto &comp : S.components) {
                if (C_prime.find(comp) == C_prime.end()) {
                    age[comp] = 0;
                    C_prime.insert(comp);
                }
            }
        }
        Solution S_opt = applyExactSolver(C_prime, inputStrings, m, t);
        if (S_opt.fitness > Sbsf.fitness) {
            Sbsf = S_opt;
        }
        adapt(C_prime, S_opt, agemax);
    }
    return Sbsf;
}

Solution constructSolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t) {
    Solution newSolution;
    pair<int, string> greedySolution = constructGreedySolution(stringLength, alphabet, omega, epsilon, t);
    for (int i = 0; i < stringLength; ++i) {
        SolutionComponent component;
        component.position = i;
        component.letter = greedySolution.second[i];
        component.age = 0;
        newSolution.components.push_back(component);
    }
    newSolution.fitness = greedySolution.first;
    return newSolution;
}

Solution applyExactSolver(const unordered_set<SolutionComponent> &subInstance, const vector<string> &omega, int m, double t) {
    IloEnv env;
    Solution optimalSolution;

    try {
        IloModel model(env);
        IloInt n = omega.size();
        IloInt stringLength = m;
        IloNumVarArray vars(env);

        std::vector<IloBoolVarArray> x(stringLength, IloBoolVarArray(env, omega[0].size()));

        for (int j = 0; j < stringLength; ++j) {
            for (int a = 0; a < omega[0].size(); ++a) {
                vars.add(IloBoolVar(env));
                x[j][a] = vars[vars.getSize() - 1];
            }
        }


        for (int j = 0; j < stringLength; ++j) {
            IloExpr sum(env);
            for (int a = 0; a < omega[0].size(); ++a) {
                sum += x[j][a];
            }
            model.add(sum == 1);
            sum.end();
        }


        IloExpr objective(env);
        for (int i = 0; i < n; ++i) {
            IloExpr hammingDistance(env);
            for (int j = 0; j < stringLength; ++j) {
                for (int a = 0; a < omega[0].size(); ++a) {
                    if (omega[i][j] != a + 'A') {
                        hammingDistance += x[j][a];
                    }
                }
            }
            model.add(hammingDistance >= t * stringLength);
            objective += hammingDistance;
        }
        model.add(IloMaximize(env, objective));


        IloCplex cplex(model);
        cplex.setParam(IloCplex::Param::TimeLimit, 300);
        if (cplex.solve()) {
            std::cout << "Fitness: " << cplex.getObjValue() << std::endl;
            optimalSolution.fitness = cplex.getObjValue();
            // Extract the optimal solution
            for (int j = 0; j < stringLength; ++j) {
                for (int a = 0; a < omega[0].size(); ++a) {
                    if (cplex.getValue(x[j][a]) > 0.5) {
                        SolutionComponent comp = {j, static_cast<char>(a + 'A'), 0};
                        optimalSolution.components.push_back(comp);
                    }
                }
            }
        } else {
            std::cout << "No feasible solution found." << std::endl;
        }
    } catch (IloException &e) {
        std::cerr << "CPLEX Exception: " << e.getMessage() << std::endl;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
    }

    env.end();
    return optimalSolution;
}

void adapt(unordered_set<SolutionComponent> &C_prime, Solution &optimalSolution, int agemax) {

    for (auto comp : C_prime) {
        comp.age++;
    }
    for (SolutionComponent &comp : optimalSolution.components) {
        C_prime.erase(comp);
    }

    for (auto it = C_prime.begin(); it != C_prime.end();) {
        if (it->age > agemax) {
            it = C_prime.erase(it);
        } else {
            ++it;
        }
    }
}