#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <unordered_map>
#include <string>
#include "greedy.h"
#include <ilcplex/ilocplex.h>
#include <chrono>

using namespace std;

struct SolutionComponent
{
    int position;
    char letter;
    int age;
};

struct Solution
{
    vector<SolutionComponent> components;
    int fitness;
    Solution() : fitness(0) {}
};

Solution constructSolution(int stringLength, const vector<char> &alphabet, const vector<string> &omega, double epsilon, double t);
Solution applyExactSolver(const unordered_set<SolutionComponent> &subInstance);
void adapt(unordered_set<SolutionComponent> &C_prime, const Solution &optimalSolution, int agemax);

Solution CMSA(const vector<string> &inputStrings, const vector<char> &alphabet, int na, int agemax, int tsolver, double epsilon, double t, double timelimit)
{
    int m = inputStrings[0].size();            // Longitud de las cadenas
    Solution Sbsf;                             // Mejor solución hasta ahora
    unordered_set<SolutionComponent> C_prime;  // Subinstancia actual
    vector<SolutionComponent> C;               // Componentes de la solución
    unordered_map<SolutionComponent, int> age; // Edad de los componentes de C
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    // Inicializar la edad de todos los componentes a cero
    for (const auto &c : C)
    {
        age[c] = 0;
    }
    while (chrono::duration<double>(chrono::high_resolution_clock::now() - start).count() < timelimit)
    {
        for (int i = 0; i < na; ++i)
        {
            Solution S = constructSolution(m, alphabet, inputStrings, epsilon, t);
            for (const auto &comp : S.components)
            {
                if (C_prime.find(comp) == C_prime.end())
                {
                    age[comp] = 0;
                    C_prime.insert(comp);
                }
            }
        }
        Solution S_opt = applyExactSolver(C_prime);
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

Solution applyExactSolver(const std::unordered_set<SolutionComponent> &subInstance, const std::vector<std::string> &omega, int m, double t)
{
    IloEnv env;
    Solution optimalSolution;
    try
    {
        IloModel model(env);
        IloInt n = omega.size();  // Número de cadenas de entrada
        IloInt stringLength = m;  // Longitud de las cadenas
        IloNumVarArray vars(env); // Variables de decisión

        std::vector<IloBoolVarArray> x(stringLength, IloBoolVarArray(env, omega[0].size()));

        for (int j = 0; j < stringLength; ++j)
        {
            for (int a = 0; a < omega[0].size(); ++a)
            {
                vars.add(IloBoolVar(env));
                x[j][a] = vars[vars.getSize() - 1]; // Cada x_j,a es una variable booleana
            }
        }

        // Definir las restricciones de que en cada posición debe haber una única letra
        for (int j = 0; j < stringLength; ++j)
        {
            IloExpr sum(env);
            for (int a = 0; a < omega[0].size(); ++a)
            {
                sum += x[j][a];
            }
            model.add(sum == 1); // Solo una letra puede ocupar la posición j
            sum.end();
        }

        // Definir la función objetivo (maximizar el número de cadenas cuya distancia de Hamming es mayor o igual a t)
        IloExpr objective(env);
        for (int i = 0; i < n; ++i)
        {
            IloExpr hammingDistance(env);
            for (int j = 0; j < stringLength; ++j)
            {
                for (int a = 0; a < omega[0].size(); ++a)
                {
                    if (omega[i][j] != a)
                    {
                        hammingDistance += x[j][a]; // Sumar cuando no coincide la letra
                    }
                }
            }
            model.add(hammingDistance >= t * stringLength); // Hamming distance >= threshold
            objective += (hammingDistance >= t * stringLength);
        }
        model.add(IloMaximize(env, objective)); // Maximizar el número de cadenas con dH >= t

        // Resolver el modelo
        IloCplex cplex(model);
        cplex.setParam(IloCplex::TiLim, 300); // Limitar el tiempo de solución a 300 segundos
        if (cplex.solve())
        {
            std::cout << "Fitness: " << cplex.getObjValue() << std::endl;
            optimalSolution.fitness = cplex.getObjValue();
            // Extraer la solución óptima
            for (int j = 0; j < stringLength; ++j)
            {
                for (int a = 0; a < omega[0].size(); ++a)
                {
                    if (cplex.getValue(x[j][a]) > 0.5)
                    {
                        SolutionComponent comp = {j, a + 'A', 0}; // Asumiendo A=0, B=1, C=2...
                        optimalSolution.components.push_back(comp);
                    }
                }
            }
        }
        else
        {
            std::cout << "No se encontró una solución factible." << std::endl;
        }

        objective.end();
    }
    catch (IloException &e)
    {
        std::cerr << "Error: " << e << std::endl;
    }
    env.end(); // Finalizar el entorno de CPLEX
    return optimalSolution;
}

void adapt(unordered_set<SolutionComponent> &C_prime, const Solution &optimalSolution, int agemax)
{
    for (auto &comp : C_prime)
    {
        comp.age++;
    }
    for (const auto &comp : optimalSolution.components)
    {
        C_prime.erase(comp);
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
