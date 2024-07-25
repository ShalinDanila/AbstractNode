#include <iostream>
#include <vector>
#include <random>

using namespace std;

class Node {
public:
    virtual void HandleEvent(int data) = 0;
};

class Network {
private:
    vector<Node*> nodes;

public:
    void AddNode(Node* newNode) {
        nodes.push_back(newNode);
    }

    void UpdateNetwork() {
        for (Node* node : nodes) {
            int eventData = GenerateRandomData(); // Generate random data
            node->HandleEvent(eventData); // Handle node event
        }
    }

private:
    int GenerateRandomData() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        return dis(gen);
    }
};

class CustomNode : public Node {
public:
    void HandleEvent(int data) override
    {
        cout << "Data event processing: " << data << "\n";
    }
};

int main()
{
    Network network;
    CustomNode node1, node2, node3, node4;
    network.AddNode(&node1);
    network.AddNode(&node2);
    network.AddNode(&node3);
    network.AddNode(&node4);
    network.UpdateNetwork();

    cout << "Press Enter to close...";
    cin.get();
    
    return 0;
}