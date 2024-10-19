


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "DynIntQueue.h"
using namespace std;


int main() {
    // Input file names
    string routersFile, packetsFile;
    cout << "Please enter the name of the file for routers:\n";
    cin >> routersFile;
    cout << "Please enter the name of the file for packets:\n";
    cin >> packetsFile;

    // Open routers file
    ifstream routerStream(routersFile);
    ifstream packetStream(packetsFile);

    // Read the number of routers
    int numRouters;
    routerStream >> numRouters;

    // Create a vector to hold routers by their ID (1-based indexing)
    vector<Router> routers(numRouters ); // +1 to handle 1-based indexing

    // Initialize routers with their IDs
    for (int i = 0; i < numRouters; ++i) {
        routers[i] = Router(i);
    }

    // Read connections from the routers file
    string line;
    getline(routerStream, line);  // Skip the first line
    getline(routerStream, line);  // Read the second line for the entry router connections
    Router entryRouter(0);  // Create the entry router with ID 0
    istringstream entryStream(line);
    int connectedRouterId;
    while (entryStream >> connectedRouterId) {
        entryRouter.addConnection(&routers[connectedRouterId]);
    }

    // Read the connections for other routers
    while (getline(routerStream, line)) {
        istringstream iss(line);
        int routerId;
        iss >> routerId;
        while (iss >> connectedRouterId) {
            routers[routerId].addConnection(&routers[connectedRouterId]);
        }
    }

    // Read packets and enqueue them to the entry router
    string packetName;
    while (getline(packetStream, packetName)) {
        Packet packet = {packetName, {"E"}};
        entryRouter.enqueuePacket(packet);
    }

    // Process packets through the network starting from the entry router
    entryRouter.processPackets();
    for (int i = 1; i <= numRouters; ++i) {
        routers[i].processPackets();
    }

    // Collect packets that reached the terminal router
    vector<Packet> terminalRouterPackets;
    for (int i = 0; i < numRouters; ++i) {
        Packet packet;
        while (routers[i].dequeuePacket(packet)) {  // Dequeue the packet from the router's queue
            if (!packet.routerHistory.empty() && packet.routerHistory.back() == "T") {
                terminalRouterPackets.push_back(packet);
            }
        }
    }

    // Output the final state of packets that reached the terminal router
    cout << "Packets are displayed in the order they arrive in the terminal router, along with their router visit history:\n";
    for (const auto& packet : terminalRouterPackets) {
        cout << packet.id << ": ";
        for (size_t i = 0; i < packet.routerHistory.size(); ++i) {
            cout << packet.routerHistory[i];
            if (i < packet.routerHistory.size() - 1) {
                cout << ", ";
            }
        }
        cout << ", T\n";  // Ensure this exactly matches the expected output format
    }

    return 0;
}

