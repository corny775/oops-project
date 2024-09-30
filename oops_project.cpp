#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Ticket {
private:
    string cname;
    int tno;
    string destination;
    string filename;

public:
    Ticket() : tno(0) {}

    void createTicket() {
    cin.ignore();
    cout << "Enter customer name: ";
    getline(cin, cname);
    cout << "Enter ticket number: ";
    cin >> tno;
    cout << "Enter destination address: ";
    cin.ignore();
    getline(cin, destination);

    filename = to_string(tno) + ".txt";

    ifstream existingFile(filename);
    if (existingFile.is_open()) {
        cout << "Ticket with this number already exists." << endl;
        existingFile.close();
        return;
    }

    ofstream ticketFile(filename);
    if (ticketFile.is_open()) {
        ticketFile << "Customer Name: " << cname << endl;
        ticketFile << "Ticket Number: " << tno << endl;
        ticketFile << "Destination: " << destination << endl;
        ticketFile.close();
        cout << "Ticket created successfully!" << endl;
    } else {
        cout << "Unable to open file" << endl;
    }
}

   void editTicket() {
    int newTno;
    cout << "Enter the new ticket number: ";
    cin >> newTno;

    string newFilename = to_string(newTno) + ".txt";

    ifstream ticketFileIn(filename);
    string content;

    if (ticketFileIn.is_open()) {
        string line;
        while (getline(ticketFileIn, line)) {
            content += line + "\n";
        }
        ticketFileIn.close();
    } else {
        cout << "Unable to open file" << endl;
        return;
    }

    if (rename(filename.c_str(), newFilename.c_str()) == 0) {
        ofstream ticketFileOut(newFilename);
        if (ticketFileOut.is_open()) {
            ticketFileOut << content;
            ticketFileOut.close();
            tno = newTno;
            filename = newFilename;
            cout << "Ticket number updated successfully!" << endl;
        } else {
            cout << "Unable to open new file" << endl;
        }
    } else {
        cout << "Error updating ticket number" << endl;
    }
}

   void appendPrice() {
    double price;
    cout << "Enter the price: ";
    cin >> price;

    ifstream ticketFileIn(filename);
    string line;
    string content;
    bool priceFound = false;

    if (ticketFileIn.is_open()) {
        while (getline(ticketFileIn, line)) {
            if (line.find("Price: ") == string::npos) {
                content += line + "\n";
            } else {
                priceFound = true;
            }
        }
        ticketFileIn.close();
    } else {
        cout << "Unable to open file" << endl;
        return;
    }

    ofstream ticketFileOut(filename);
    if (ticketFileOut.is_open()) {
        ticketFileOut << content;
        ticketFileOut << "Price: " << price << endl;
        ticketFileOut.close();
        if (priceFound) {
            cout << "Price updated successfully!" << endl;
        } else {
            cout << "Price appended successfully!" << endl;
        }
    } else {
        cout << "Unable to open file" << endl;
    }
}

    void displayTicket() const {
        ifstream ticketFile(filename);
        string line;

        if (ticketFile.is_open()) {
            while (getline(ticketFile, line)) {
                cout << line << endl;
            }
            ticketFile.close();
        } else {
            cout << "Unable to open file" << endl;
        }
    }

    int getTicketNumber() const {
        return tno;
    }

    void setTicketNumber(int ticketNumber) {
        tno = ticketNumber;
        filename = to_string(tno) + ".txt";
    }
};

int main() {
    int choice;
    vector<Ticket> tickets;

    while (true) {
        cout << "\nMenu:" << endl;
        cout << "1. Enter ticket details" << endl;
        cout << "2. Edit ticket number" << endl;
        cout << "3. Append price to ticket" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            Ticket ticket;
            ticket.createTicket();
            tickets.push_back(ticket);
        } else if (choice == 2) {
            cout << "Enter the ticket number you want to edit: ";
            int ticketNumber;
            bool found = false;
            cin >> ticketNumber;
            for (auto &i : tickets) { // Pass by reference
                if (i.getTicketNumber() == ticketNumber) {
                    i.editTicket();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Ticket not found" << endl;
            }
        } else if (choice == 3) {
            cout << "Enter the ticket number you want to append price to: ";
            int ticketNumber;
            bool found = false;
            cin >> ticketNumber;
            for (auto &i : tickets) { 
                if (i.getTicketNumber() == ticketNumber) {
                    i.appendPrice();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Ticket not found" << endl;
            }
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
