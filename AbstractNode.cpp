#include <iostream>
#include <map>
#include <vector>
#include <random>

using namespace std;

class Node {
public:
    virtual void handleEvent(int data) = 0;
};

class Network {
private:
    vector<Node*> nodes;

public:
    void addNode(Node* newNode) {
        nodes.push_back(newNode);
    }

    void updateNetwork() {
        for (Node* node : nodes) {
            int eventData = generateRandomData(); // Генерация случайных данных
            node->handleEvent(eventData); // Обработка события узлом
        }
    }

private:
    int generateRandomData() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        return dis(gen);
    }
};

class CustomNode : public Node {
public:
    void handleEvent(int data) override
    {
        cout << "Обработка события с данными: " << data << endl;
    }
};

int main() {
    Network network;
    CustomNode node1, node2;
    network.addNode(&node1);
    network.addNode(&node2);
    network.updateNetwork();
    
    return 0;
}