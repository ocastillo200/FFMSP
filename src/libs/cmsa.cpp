#include "cmsa.h"
#include "greedy.h"
#include <chrono>
#include <random>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <algorithm>

Solution CMSA(const vector<string> &inputStrings, const vector<char> &alphabet, int na, int agemax, int tsolver, double epsilon, double t, double timelimit)
{
    int m = inputStrings.size();
    Solution Sbsf;
    unordered_set<SolutionComponent> C_prime;
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    while (chrono::duration<double>(chrono::high_resolution_clock::now() - start).count() < timelimit)
    {
        for (int i = 0; i < na; ++i)
        {
            Solution S = constructSolution(m, alphabet, inputStrings, epsilon, t);
            for (const auto &comp : S.components)
            {
                if (C_prime.find(comp) == C_prime.end())
                {
                    C_prime.insert(comp);
                }
            }
        }
        Solution S_opt = applyExactSolver(C_prime, inputStrings, inputStrings[0].size(), t, tsolver);
        if (S_opt.fitness > Sbsf.fitness)
        {
            Sbsf = S_opt;
        }
        adapt(C_prime, S_opt, agemax);
    }
    return Sbsf;
}

Solution constructSolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t)
{
    Solution newSolution;
    pair<int, string> greedySolution = constructGreedySolution(stringLength, alphabet, omega, epsilon, t);
    for (int i = 0; i < stringLength; ++i)
    {
        SolutionComponent component;
        component.position = i;
        component.letter = greedySolution.second[i];
        component.age = 0;
        newSolution.components.push_back(component);
    }
    newSolution.fitness = greedySolution.first;
    return newSolution;
}

Solution applyExactSolver(const unordered_set<SolutionComponent> &subInstance, const vector<string> &omega, int m, double t, int tsolver)
{
    IloEnv env;
    Solution optimalSolution;

    try
    {
        IloModel model(env);
        IloInt n = omega.size();
        IloInt stringLength = m;
        IloNumVarArray vars(env);
        vector<char> alphabet = {'A', 'C', 'G', 'T'};
        std::vector<IloBoolVarArray> x(stringLength, IloBoolVarArray(env, alphabet.size()));
        IloBoolVarArray y(n, env);

        for (int j = 0; j < stringLength; ++j)
        {
            for (int a = 0; a < alphabet.size(); ++a)
            {
                vars.add(IloBoolVar(env));
                x[j][a] = vars[vars.getSize() - 1];
            }
        }

        for (int j = 0; j < stringLength; ++j)
        {
            IloExpr sum(env);
            for (int a = 0; a < alphabet.size(); ++a)
            {
                sum += x[j][a];
            }
            model.add(sum == 1);
            sum.end();
        }

        // FIX
        IloExpr objective(env);
        for (int i = 0; i < n; ++i)
        {
            IloExpr sum(env);
            for (int j = 0; j < stringLength; ++j)
            {
                auto it = find(omega.begin(), omega.end(), omega[i][j]);
                sum += x[j][it - omega.begin()];
            }
            model.add(sum <= stringLength - t * stringLength * objective);
            objective += sum;
        }
        model.add(IloMaximize(env, objective));

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());
        cplex.setParam(IloCplex::Param::TimeLimit, tsolver);
        if (cplex.solve())
        {
            std::cout << "Fitness: " << cplex.getObjValue() << "/ " << omega.size() << std::endl;
            optimalSolution.fitness = cplex.getObjValue();
            for (int j = 0; j < stringLength; ++j)
            {
                for (int a = 0; a < alphabet.size(); ++a)
                {
                    if (cplex.getValue(x[j][a]) > 0.5)
                    {
                        SolutionComponent comp = {j, alphabet[a], 0};
                        optimalSolution.components.push_back(comp);
                    }
                }
            }
        }
        else
        {
            std::cout << "No feasible solution found." << std::endl;
        }
    }
    catch (IloException &e)
    {
        std::cerr << "CPLEX Exception: " << e.getMessage() << std::endl;
    }
    catch (...)
    {
        std::cerr << "An unexpected error occurred." << std::endl;
    }

    env.end();
    return optimalSolution;
}

void adapt(unordered_set<SolutionComponent> &C_prime, Solution &optimalSolution, int agemax)
{
    for (auto comp : C_prime)
    {
        comp.age++;
    }
    for (SolutionComponent &comp : optimalSolution.components)
    {
        if (C_prime.find(comp) != C_prime.end())
        {
            comp.age = 0;
        }
    }
    for (auto it = C_prime.begin(); it != C_prime.end();)
    {
        if (it->age > agemax)
        {
            it = C_prime.erase(it);
        }
        else
        {
            ++it;
        }
    }
}