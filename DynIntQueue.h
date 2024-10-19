

#ifndef DYNINTQUEUE_H
#define DYNINTQUEUE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

// Define a meaningful default ID for uninitialized routers
const int NULL_ROUTER_ID = 0;

struct Packet {
    string id;
    vector<string> routerHistory;
};

struct QueueNode {
    Packet value;
    QueueNode* next;

    QueueNode(const Packet& p, QueueNode* n = nullptr) : value(p), next(n) {}
};

class DynIntQueue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    DynIntQueue();
    ~DynIntQueue();
    DynIntQueue(const DynIntQueue& copy);
    DynIntQueue& operator=(const DynIntQueue& copy);
    DynIntQueue& operator<<(const Packet& p);
    DynIntQueue& operator>>(Packet& p);
    friend ostream& operator<<(ostream& os, const DynIntQueue& q);
    size_t size() const;
    bool isEmpty() const; // Check if the queue is empty
};

// Router class declaration
class Router {
private:
    int id;  // Unique identifier for the router
    DynIntQueue packetQueue;  // Queue to hold packets at this router
    vector<Router*> connections;  // Connected routers to forward packets to

public:
    Router();          // Default constructor
    Router(int id);    // Constructor with ID
    void addConnection(Router* router);
    void enqueuePacket(const Packet& packet);
    void processPackets();
    size_t queueSize() const;
    int getId() const;

    bool dequeuePacket(Packet& packet);  // Method to dequeue a packet
};

#endif // DYNINTQUEUE_H


