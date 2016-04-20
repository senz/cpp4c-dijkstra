/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: senz
 *
 * Created on April 1, 2016, 1:44 PM
 */

#include <cstdlib>
#include <array>
#include <iterator>
#include <iostream>
#include <random>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits>

using namespace std;

template <unsigned int N> class MatrixTree {
public:
    const unsigned int size = N;

    MatrixTree() {
        nodes2 = new array<array<bool, N>*, N>;
        for (auto& row: *nodes2) {
            row = new array<bool, N>();
        }       
        
        nodesWeights = new array<array<unsigned int, N>*, N>;
        for (auto& row: *nodesWeights) {
            row = new array<unsigned int, N>();
        }
    }
    
    ~MatrixTree() {
        for (auto& row: *nodes2) {
            delete row;
        }
        delete nodes2;
        
        for (auto& row: *nodesWeights) {
            delete row;
        }
        delete nodesWeights;
    }
    
    bool hasConnection(int n1, int n2) const {
        if (n1 > size || n2 > size) throw out_of_range("node position is out of range");
        if (n1 == n2) return false;
        else return nodes2->at(n1)->at(n2);
    }
    
    void setConnection(int n1, int n2, unsigned int weight, bool connected = true, bool symmetrical = false) {
        if (n1 > size || n2 > size) throw out_of_range("node position is out of range");
        if (weight <= 0) throw new out_of_range("weight <= 0");

        nodes2->at(n1)->at(n2) = connected;
        nodesWeights->at(n1)->at(n2) = weight;
        if (symmetrical) {
            nodes2->at(n2)->at(n1) = connected;
            nodesWeights->at(n2)->at(n1) = weight;
        }
    }
    
    void setWeight(int node1, int node2, unsigned int weight) {
        nodesWeights->at(node1)->at(node2) = weight;
    }
    
    int getWeight(int node1, int node2) const {
        return nodesWeights->at(node1)->at(node2);
    }
    
    template<unsigned int T> friend std::ostream& operator<<(std::ostream& os, const MatrixTree<T>& obj) {
        os << "MatrixTree(";
        bool empty = true;
        for (int n1 = 0; n1 < obj.size; n1++) {
            for (int n2 = 0; n2 < obj.size; n2++) {
                if (obj.hasConnection(n1, n2)) {
                    os << n1 << "-(" << obj.getWeight(n1, n2) << ")->" << n2 << ", ";
                    empty = false;
                }
            }
        }
        if (empty) os << "EMPTY";
        os << ")";
        return os;
    }
    
    template <unsigned int T> static void randomizeSymmetrical(MatrixTree<T>& tree, float density, unsigned int maxWeight = 10) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis;
        uniform_int_distribution<unsigned int> idis(1, maxWeight);
        
        
        // TODO do half of iterations
        for (int r = 0; r < T; r++) 
            for (int c = 0; c < r; c++) {
                auto prob = dis(gen);
                auto weight = idis(gen);
                if (prob <= density) {
                    tree.setConnection(r, c, weight, true, true);
                }
            }
    }

    int minWeight(array<int,N>& tentWeights, array<bool, N>& visited) {
        int minW = INT_MAX;
        int minNode = -1;
        for (int i = 0; i < N; i++) {
            int w = tentWeights[i];
            if (!visited[i] && w < minW) {
                minW = w;
                minNode = i;
            }
        }
        return minNode;
    }

    int f(int start, int dest) {
        array<int, N> tentWeights{};
        array<bool, N> visited{};
        visited.fill(false);
        tentWeights.fill(INT_MAX);
        tentWeights[start] = 0;

        for (int n = 0; n < N - 1; n++) {
            int u = minWeight(tentWeights, visited);
            cout << "u is " << u << endl;
            if (u == dest) { break; }
            if (u == -1) {
                cout << "no more connections" << endl;
                break;
            }
            visited[u] = true;

            for (int v = 0; v < N; v++) {
                if (!visited[v] && hasConnection(u, v) && tentWeights[u] != INT_MAX && tentWeights[v] > tentWeights[u] + getWeight(u, v)) {
                    tentWeights[v] = tentWeights[u] + getWeight(u, v);
                }
            }
        }
        return tentWeights[dest];
    }
    
private:
    array<array<bool, N>*, N>* nodes2;
    array<array<unsigned int, N>*, N>* nodesWeights;
    
    vector<int> closest(int node) {
        vector<int> res;
        const auto conns = nodes2->at(node);
        for (int i = 0; i < N; i++) {
            auto c = conns->at(i);
            if (c && i != node) {
                res.push_back(i);
            }
        }
        return res;
    }
};

int main(int argc, char** argv) {
    const unsigned int treeSize = 100;
    float treeDensity = 1;

    int optChar;
    while ((optChar = getopt(argc, argv, "d:s:")) != -1) {
        switch (optChar) {
            case 'd': {
                string denstr = optarg;
                cout << "Density: " << denstr << endl;
                float d = stof(denstr);
                
                if (d > 1 || d < 0) {
                    cout << "density must be between 0 and 1" << endl;
                    exit(1);
                }
                treeDensity = d;
                break;
            }
            
            default:
                cout << "oc: " << optChar << endl;

                cout << "-h help, -d <density>" << endl;
                exit(1);
        }
    }
    MatrixTree<treeSize> tree;
    tree.randomizeSymmetrical(tree, treeDensity);
    int path = tree.f(0, 40);
    
//    cout << tree << endl;
    cout << "shortest path weight " << path << endl;
    return 0;
}

