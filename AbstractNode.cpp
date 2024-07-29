#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <random>
#include <thread>

using namespace std;

class Node
{
public:
    /*VARIABLES*/
    string Name;
    vector<Node*> Neighbors;
    int Value = 0;
    int AmountOfThreads = 0;

    /*FUNCTIONS*/
    Node(string nodeName, int nodeValue, int nodeThreads) : Name(std::move(nodeName)), Value(nodeValue), AmountOfThreads(nodeThreads) {}
    
    int GenerateRandomInt()
    {
        Value = rand() % 100;
        return Value;
    }

    string GenerateRandomNameForNode()
    {
        string NewNameInt = to_string(GenerateRandomInt());
        string NewName = "Node" + NewNameInt;
        return NewName;
    }

    void SumOfTheAllNumbersOfSender(Node* Sender, Node* Reciever, int TransferredValue, int TransferredAmountOfThreads) // Получаем сумму ВСЕХ (value + amountofthreads) чисел от отправителя. 
    { 
        int Sum = TransferredValue + TransferredAmountOfThreads;
        cout << Sender->Name << " ->" << Reciever->Name << " : " << Sum << "\n";
    }

    void SumOfTheThreadsOfSender(Node* Sender, Node* Reciever, int TransferredAmountOfThreads) // Получаем сумму событий (AmountOfThreads) от отправителя 
    { 
        cout << Sender->Name << " ->" << Reciever->Name << " : " << TransferredAmountOfThreads << "\n";
    }
    
    void SubscribeToNeighbor(Node* Sender, Node* Reciever, int TransferredValue, int TransferredAmountOfThreads) // Отправитель - узел, который вызвал функцию, получатель - узел, на который подписались
    {
        Neighbors.push_back(Reciever);
        int ProbabilityOfAction = rand() % 2;
        switch(ProbabilityOfAction) // использовал switch, потому в будущем узлы могут содержать больше информации и, следовательно, функций для сбора этой информации будет больше
        {
        case 0:
            SumOfTheAllNumbersOfSender(Sender, Reciever, TransferredValue, TransferredAmountOfThreads);
            break;
        case 1:
            SumOfTheThreadsOfSender(Sender, Reciever, TransferredAmountOfThreads);
            break;
        default: break;
        }
    }

    void UnsubscribeFromNeighbor()
    {
        if(!Neighbors.empty())
        {
            Node* NeighborToUnsubscribe = Neighbors[0];
            auto NeighborToUnsubscribePtr = find(Neighbors.begin(), Neighbors.end(), NeighborToUnsubscribe);
            if(NeighborToUnsubscribePtr != Neighbors.end())
            {
                Neighbors.erase(NeighborToUnsubscribePtr);
                cout << "Unsubscribed from neighbor: " << NeighborToUnsubscribe->Name << "\n";
            }
        }
        else //debug
        {
            cout << "No neighbors to unsubscribe from!" << "\n";
        }
    }

    Node* CreateNewNode()
    {
        int NewValue = GenerateRandomInt();
        string NewName = GenerateRandomNameForNode();
        Node* NewNode = new Node(NewName, NewValue, 0);
        return NewNode;
    }
};

class Network
{
public:
    /*VARIABLES*/
    int AmountOfNodes = 15; // Количество нод при инициализации сети 
    vector<Node*> AllNodes;
    vector<Node*> ArrayInstance;

    /*FUNCTIONS*/
    void Initialization()
    {
        cout << "Network started!" << "\n"; // Начинаем программу (сообщение)
        for(int i = 0; i < AmountOfNodes; i++)
        {
            int NewValue = rand() % 100;
            string NewName = "Node" + to_string(i);
            Node* NewNode = new Node(NewName, NewValue, 0);
            AllNodes.push_back(NewNode);
            cout << "INIT Node created" << " " << NewNode->Name << " " << NewNode->Value << "\n"; //DEBUG
        }
        ArrayInstance = AllNodes;
    }

    void ChooseTheAction(Node* ActiveNode)
    {
        int RandNumberOfAction = rand() % 5;
        switch(RandNumberOfAction)
        {
        case 0:
                // Генерация случайного числа
                ActiveNode->GenerateRandomInt();
                ActiveNode->AmountOfThreads += 1;
                cout << ActiveNode->Name << " generated value: " << ActiveNode->Value << "\n";
                break;
        case 1: // Подписка на рандомный узел
            {
                Node* RandomNode = ArrayInstance[rand() % ArrayInstance.size()];
                if (RandomNode != ActiveNode)
                {
                    cout << ActiveNode->Name << " subscribed to " << RandomNode->Name << "\n";
                    ActiveNode->AmountOfThreads += 1;
                    ActiveNode->SubscribeToNeighbor(ActiveNode, RandomNode, ActiveNode->Value, ActiveNode->AmountOfThreads);
                }
                break;
            }
        case 2: // Отписка от соседа
            ActiveNode->UnsubscribeFromNeighbor();
            ActiveNode->AmountOfThreads += 1;
            break;
        case 3: // Создание нового узла и последующая подписка на него
            {
                Node* GeneratedNode = ActiveNode->CreateNewNode();
                AllNodes.push_back(GeneratedNode);
                ActiveNode->SubscribeToNeighbor(ActiveNode, GeneratedNode, ActiveNode->Value, ActiveNode->AmountOfThreads);
                ActiveNode->AmountOfThreads += 1;
                cout << ActiveNode->Name << " created and subscribed to new node: " << GeneratedNode->Name << "\n";
                break;
            }
        case 4: // Не делать ничего
            ActiveNode->AmountOfThreads += 1;
            cout << ActiveNode->Name << " is being lazy!\n";
            break;
        default: break;
        }
    }
    
    void Update()
    {
        for(unsigned i = 0; i < ArrayInstance.size(); i++)
        {
            Node* NewActiveNode = ArrayInstance[i];
            ChooseTheAction(NewActiveNode);
            if(NewActiveNode->Neighbors.empty())
            {
                cout << NewActiveNode->Name << " was deleted" << "\n";
                auto GarbageNode = find(AllNodes.begin(), AllNodes.end(), NewActiveNode);
                AllNodes.erase(GarbageNode);
            }
        }
        ArrayInstance = AllNodes;
        this_thread::sleep_for(chrono::seconds(2));
        Update();
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    Network network;
    network.Initialization();
    network.Update();
    for (Node* Node : network.AllNodes)
    {
        delete Node;
    }
    return 0;
}