#pragma comment(lib, "dim")

#include <iostream>
#include <Windows.h>
#include <dic.hxx>
#include <chrono>
#include <ctime>   
#include <stdio.h>
#include <memory.h>
#include <string>

using namespace std;

int main()
{
    string serviceName;

    serviceName = "DIM_Meranie1";

    char nolink[] = "No RPC link";
    DimRpcInfo service(serviceName.c_str(), nolink);

    int size; // size of buffer
    int sec = 60; // time window
    int packs = 0; // ammount of sent packages
    string text = ""; // input text and send buffer
    string response; // receive buffer
    string restart = "666"; // restart command


    while (1) // always true
    {
        cout << "Enter size of buffer: ";
        cin >> text; // new buffer size

        service.setData((char*)text.c_str()); // send it to server
        size = stoi(text); // write it to "size"
        text = ""; // clear it
        for (int i = 0; i < size / 8; i++)
        {
            text += "0"; // making filled buffer
        }

        auto start = chrono::system_clock::now(); // start the timer
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        double delta = elapsed_seconds.count(); // making delta != NULL

        while (delta <= sec) // start testing
        {
            service.setData((char*)text.c_str()); // send buffer to server
            response = service.getString(); // receive the response

            auto end = chrono::system_clock::now(); // check now time
            chrono::duration<double> elapsed_seconds = end - start;
            delta = elapsed_seconds.count(); // make a new delta
            packs++; // increment number of succeed transactions by one
        }

        service.setData((char*)restart.c_str()); // send restart command
        float bytes = ((packs * size) / 1000) / sec; // calculate results 
        cout << bytes << " KB per second" << endl; // write it to the console
        cout << packs << endl;
        packs = 0; // clear variables
        text = "";
    }

    return 0;
}

