# Algorithmics and Programming III

>Exhaustive search, greedy and metaheuristic algorithms performed on Comunio® databases to generate an optimum football team fulfilling several constraints.

This project consists of the implementation of the main algorithms seen during the 2019 Algorithmics and Programming III course given on the Degree in Data Science and Engineering at [UPC](https://www.upc.edu/ca).

The aim of this project consists in analyzing and evaluating the difference of performance between the three types of algorithms, in terms of computation speed and the correctness of the achieved solution.

<p align="center">
  <img src='README Images/comunio.jpg'/ width = 500>
</p>

## Introduction and basics

All algorithms are intended to solve the same problem, a football variant of the traditional [Knapsack problem](https://en.wikipedia.org/wiki/Knapsack_problem).
The goal of these algorithms is to generate the football team that will have the best punctuation but fulfilling the constraints provided by the users. These constraints are:

* Satisfy the provided lineup (*4-3-3 e.g.*).
* Do not exceed maximum team price.
* Do not exceed maximum player price.

*Note: Other constraints are internally imposed and not demanded to the user (a team can not have duplicate players e.g.)*

To execute this Knapsack variation, the Comunio® databases were used in order to obtain all the necessary information for the team creation process.
These databases have the following structure:

<p align="center">
  <img src='README Images/database_structure.png'/>
</p>

## Main differences between algorithms

* [`Exhasutive Search`](./exhaustive_search.cc) ensures that the optimal solution will be found, but for large databases and difficult queries may not finalize.
* [`Greedy`](./greedy.cc) ensures obtaining a relatively good solution in a short amount of time, but we can not assure that the created team is optimal.
* [`Metaheuristic`](./metaheuristic.cc) starts with the greedy result as an initial solution and improves them applying [simulated annealing](https://en.wikipedia.org/wiki/Simulated_annealing). Like greedy, it is a fast algorithm that does not provide any guarantee about the optimality of the result. However, it ensures that the obtained solution is, at least, better or equal than the found by the greedy algorithm.

## Benchmarks and performance analysis

The algorithms were executed using the Comunio® databases and the queries present on this repository. The maximum time of execution for each combination of database - query was:
* 2 min for Exhaustive Search.
* 5 s for Greedy.
* 10 s for Metaheuristic.
In case of not achieving a final solution, the lastest partial solution stored in the output file was selected as a result.

*Note: In order to pass this project, exhaustive search should be capable of achieving a solution in easy and medium queries, and always obtain a solution if greedy or metaheuristic is used. Our project achieved satisfactorily this requirement*

## Architecture

The files contained on this repository are:
* The [`Comunio® Databases`](./Databases).
* [`Exhaustive Search`](./exhaustive_search.cc),[`Greedy`](./greedy.cc) and [`Metaheuristic`](./metaheuristic.cc) algorithm implementations.
* [`Easy`](./Easy_Queries),[`Medium`](./Medium_Queries) and [`Hard`](./Hard_Queries) Queries.
* [`Additional Queries`](./Additional_Queries)to test the limits of the Metaheuristic Algorithm.

*Note: The rest of the files in the master branch are auxiliary or license related*

## Team

This project was developed by:
| [![Vinomo4](https://avatars2.githubusercontent.com/u/49389601?s=60&v=4)](https://github.com/Vinomo4) | [<img src="https://avatars2.githubusercontent.com/u/43076234?s=10&v=4" width="60" height="60" />](https://github.com/Naxel100) |
| --- | --- |
| [Victor Novelle Moriano](https://github.com/Vinomo4) | [Alex Ferrando de las Morenas](https://github.com/Naxel100) |


Students of Data Science and Engineering at [UPC](https://www.upc.edu/ca).

## License

[MIT License](./LICENSE)
