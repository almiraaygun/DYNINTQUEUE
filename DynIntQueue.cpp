
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "DynIntQueue.h"
using namespace std;


// Default constructor for DynIntQueue
DynIntQueue::DynIntQueue() : front(nullptr), rear(nullptr) {}

// Destructor for DynIntQueue
DynIntQueue::~DynIntQueue() {
    while (front != nullptr) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}

// Copy constructor (if needed)
DynIntQueue::DynIntQueue(const DynIntQueue& copy) : front(nullptr), rear(nullptr) {
    QueueNode* current = copy.front;
    while (current != nullptr) {
        *this << current->value;  // Use the overloaded << operator to enqueue
        current = current->next;
    }
}

// Assignment operator (if needed)
DynIntQueue& DynIntQueue::operator=(const DynIntQueue& copy) {
    if (this != &copy) {
        // Clear the current queue
        while (front != nullptr) {
            Packet temp;
            *this >> temp;  // Use the overloaded >> operator to dequeue
        }
        // Copy the new queue
        QueueNode* current = copy.front;
        while (current != nullptr) {
            *this << current->value;  // Enqueue each packet
            current = current->next;
        }
    }
    return *this;
}

// Constructor to initialize the router with an ID
Router::Router(int id) : id(id) {}

// Default constructor for Router
Router::Router() : id(NULL_ROUTER_ID) {}

// Other Router methods remain the same...

// Method to add a connection to another router
void Router::addConnection(Router* router) {
    connections.push_back(router);
}

// Method to enqueue a packet to this router's queue
void Router::enqueuePacket(const Packet& packet) {
    packetQueue << packet;  // Using the overloaded << operator to enqueue
}

void Router::processPackets() {
    while (!packetQueue.isEmpty()) {
        Packet packet;
        packetQueue >> packet;  // Dequeue the packet

        if (!connections.empty()) {
            // Find the connected router with the smallest queue
            Router* nextRouter = connections[0];  // Start with the first connection

            for (Router* router : connections) {
                if (router->queueSize() < nextRouter->queueSize()) {
                    nextRouter = router;
                }
            }

            // Add the current router ID to the packet's router history
            packet.routerHistory.push_back(std::to_string(id));

            // Forward the packet to the next router
            nextRouter->enqueuePacket(packet);
        } else {
            // No forward connection, so packet is lost (considered as lost if needed).
        }
    }
}

// Method to get the size of the queue at this router
size_t Router::queueSize() const {
    return packetQueue.size();
}

// Method to get the router's ID
int Router::getId() const {
    return id;
}

// Method to dequeue a packet from the router's queue
bool Router::dequeuePacket(Packet& packet) {
    if (packetQueue.size() > 0) {
        packetQueue >> packet;
        return true;
    }
    return false;
}


