// The MIT License (MIT)

// Copyright (c) 2024 Chinmay Girish Gadgil

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

//  This is a simple ticket booking system that allows the user to create, edit, append price to, and display tickets.


#include <iostream>
#include <thread>  
#include <chrono>  
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

void showSpinner(int duration) {
    const char spinner[] = {'|', '/', '-', '\\'};
    int spinnerIndex = 0;
    auto start = chrono::high_resolution_clock::now();
    while (chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start).count() < duration) {
        cout << "\r" << spinner[spinnerIndex++] << flush;
        spinnerIndex %= 4;
    }
    cout << "\r" << " " << "\r"; // Clear the spinner
}

class Ticket
{
private:
    string cname;
    int tno;
    string destination;
    string filename;

public:
    Ticket() : tno(0) {}

    void createTicket()
    {
        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, cname);
    
        cout << "Enter ticket number: ";
        cin >> tno;
        
        cout << "Enter destination address: ";
        cin.ignore();
        getline(cin, destination);
        showSpinner(2); 

        filename = to_string(tno) + ".txt";

        ifstream existingFile(filename);
        if (existingFile.is_open())
        {
            cout << "\nTicket with this number already exists." << endl;
            existingFile.close();
            return;
        }

        ofstream ticketFile(filename);
        if (ticketFile.is_open())
        {
            ticketFile << "------------------------------------------\n";
            ticketFile << "|                  TICKET                |\n";
            ticketFile << "------------------------------------------\n";
            time_t now = time(0);
            tm *ltm = localtime(&now);
            char date[11];
            char time[9];

            strftime(date, sizeof(date), "%d/%m/%Y", ltm);
            strftime(time, sizeof(time), "%H:%M:%S", ltm);

            ticketFile << "| Date: " << date;
            ticketFile << "        Time: " << time << " |\n";
            ticketFile << "| Customer Name: " << cname;
            for (int i = cname.length(); i < 24; i++)
            {
                ticketFile << " ";
            }
            ticketFile << "|\n";

            ticketFile << "| Ticket Number: " << tno;
            for (int i = to_string(tno).length(); i < 24; i++)
            {
                ticketFile << " ";
            }
            ticketFile << "|\n";
            ticketFile << "| Destination: " << destination;
            for (int i = destination.length(); i < 26; i++)
            {
                ticketFile << " ";
            }
            ticketFile << "|\n";
            ticketFile << "------------------------------------------\n";
            ticketFile.close();
            cout << "\nTicket created successfully!" << endl;
        }
        else
        {
            cout << "\nUnable to open file" << endl;
        }
    }

    void editTicket()
    {
        int newTno;
        cout << "Enter the new ticket number: ";
        cin >> newTno;
        showSpinner(2); 
        string newFilename = to_string(newTno) + ".txt";

        ifstream ticketFileIn(filename);
        string content;

        if (ticketFileIn.is_open())
        {
            string line;
            int count = 1;
            while (getline(ticketFileIn, line))
            {
                if (count == 5)
                {
                    content += "Ticket Number: " + to_string(newTno) + "\n";
                    count++;
                    continue;
                }
                content += line + "\n";
                count++;
            }
            ticketFileIn.close();
        }
        else
        {
            cout << "Unable to open file" << endl;
            return;
        }

        if (rename(filename.c_str(), newFilename.c_str()) == 0)
        {
            ofstream ticketFileOut(newFilename);
            if (ticketFileOut.is_open())
            {
                ticketFileOut << content;
                ticketFileOut.close();
                tno = newTno;
                filename = newFilename;
                cout << "Ticket number updated successfully!" << endl;
            }
            else
            {
                cout << "Unable to open new file" << endl;
            }
        }
        else
        {
            cout << "Error updating ticket number" << endl;
        }
    }

    void appendPrice()
    {
        double price;
        cout << "Enter the price: ";
        cin >> price;
        showSpinner(2); 

        ifstream ticketFileIn(filename);
        string line;
        string content;
        bool priceFound = false;

        if (ticketFileIn.is_open())
        {
            for (int i = 0; i < 7; i++)
            {
                getline(ticketFileIn, line);
                content += line + "\n";
            }
            stringstream stream;
            stream << fixed << setprecision(2) << price;
            content += "| Price: " + stream.str();
            for (int i = to_string(price).length(); i < 36; i++)
            {
                content += " ";
            }
            content += "|\n";
            getline(ticketFileIn, line);
            if (line.find("---") == string::npos)
            {
                getline(ticketFileIn, line);
            }
            content += line + "\n";
        }
        else
        {
            cout << "Unable to open file" << endl;
            return;
        }

        ofstream ticketFileOut(filename);
        if (ticketFileOut.is_open())
        {
            ticketFileOut << content;
        }
        else
        {
            cout << "Unable to open file" << endl;
        }
    }

    void displayTicket() const
    {
        ifstream ticketFile(filename);
        string line;

        if (ticketFile.is_open())
        {
            while (getline(ticketFile, line))
            {
                cout << CYAN << line << endl;
            }
            ticketFile.close();
        }
        else
        {
            cout << "Unable to open file" << endl;
        }
    }

    int getTicketNumber() const
    {
        return tno;
    }

    void setTicketNumber(int ticketNumber)
    {
        tno = ticketNumber;
        filename = to_string(tno) + ".txt";
    }
};

int main()
{
    int choice;
    vector<Ticket> tickets;

    while (true)
    {
        showSpinner(1); 
        cout << "\nMenu:" << endl;
        cout << "1. Enter ticket details" << endl;
        cout << "2. Edit ticket number" << endl;
        cout << "3. Append price to ticket" << endl;
        cout << "4. Display ticket" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            Ticket ticket;
            ticket.createTicket();
            tickets.push_back(ticket);
        }
        else if (choice == 2)
        {
            cout << "Enter the ticket number you want to edit: ";
            int ticketNumber;
            bool found = false;
            cin >> ticketNumber;
            for (auto &i : tickets)
            {
                if (i.getTicketNumber() == ticketNumber)
                {
                    i.editTicket();
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "Ticket not found" << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "Enter the ticket number you want to append price to: ";
            int ticketNumber;
            bool found = false;
            cin >> ticketNumber;
            for (auto &i : tickets)
            {
                if (i.getTicketNumber() == ticketNumber)
                {
                    i.appendPrice();
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "Ticket not found" << endl;
            }
        }
        else if (choice == 4)
        {
            cout << "Enter the ticket number you want to display: ";
            int ticketNumber;
            cin >> ticketNumber;
            string filename = to_string(ticketNumber) + ".txt";
            ifstream ticketFile(filename);
            if (ticketFile.is_open())
            {
                string line;
                vector<string> colors = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
                string color = colors[rand() % colors.size()];
                while (getline(ticketFile, line))
                {
                    cout << color << line << endl;
                }
                cout << RESET;
                ticketFile.close();
            }
            else
            {
                cout << "Ticket not found" << endl;
            }
        }
        else if (choice == 5)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
