/*
 Program:		Theatre Ticket Sales - Final Project
 Programmer:	Jacob Baynes
 Date:			4/26/2019
 Purpose:		This program simulates a theatre ticket purchasing system.
				The user is prompted with a menu and based on their choice
				they can see the seating chart for the theatre, purchase tickets,
				see the sales report for the theatre, and quit the program.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Create a structure SeatStructures
struct SeatStructures
{
	double cost;
	char valid;
};


// Create a class TicketManager
class TicketManager
{
private:
	SeatStructures aud[15][30];
	int seat;
	double price;
public:
	// Constructor
	TicketManager()
	{
		// Create input file stream objects
		ifstream fileprices("SeatPrices.dat");
		ifstream fileseats("SeatAvailability.dat");

		// Declare variables
		seat = 0; price = 0.0;
		double price;

		// For loop reads prices from file stream object
		// and writes them to the SeatStructures aud
		for (int row = 0; row < 15; row++)
		{
			fileprices >> price;
			for (int col = 0; col < 30; col++)
			{
				aud[row][col].cost = price;
				fileseats >> aud[row][col].valid;
			}
		}

		// Close file stream objects
		fileprices.close();
		fileseats.close();
	}

	// Destructor
	~TicketManager()
	{
		// Create output file steram object
		ofstream fileresults("SeatAvailability.dat");

		// Update the seats data
		for (int row = 0; row < 15; row++)
		{
			for (int col = 0; col < 30; col++)
			{
				fileresults << aud[row][col].valid;
			}
			fileresults << endl;
		}
		// Close output file stream object
		fileresults.close();
	}

	// seatArrangment member function displays theater seating
	string seatArrangement()
	{
		// Display seat numbers
		string seats = "Seats: ";
		seats += "";

		for (int i = 1; i <= 30; i++)
		{
			seats += (char)('0' + i % 10);
		}

		seats += "\n";

		// Display row numbers and seat availability
		for (int row = 0; row < 15; row++)
		{
			string rowMessage = "Row ";
			if (row < 9)
			{
				rowMessage = rowMessage + (char)('0' + (row + 1));
				rowMessage = rowMessage + " " + " ";
			}
			else
			{
				rowMessage = rowMessage + '1';
				rowMessage = rowMessage + (char)('0' + (row + 1) % 10);
				rowMessage = rowMessage + " ";
			}
			seats = seats + rowMessage;

			for (int col = 0; col < 30; col++)
			{
				seats = seats + aud[row][col].valid;
			}
			seats += "\n";
		}
		return seats;
	}

	// Order Member function
	void order(int number, int rowNum, int startNum, char& purchase)
	{
		string result = "";

		// Check if the number of seats can fit
		// in the given row
		if (number + startNum - 1 > 30)
		{
			purchase = 'N';
			cout << "Seats are not available\n";
			
		}

		// Check if seats are taken
		for (int x = 0; x < number; x++)
		{
			if (aud[rowNum - 1][startNum + x - 1].valid == '*')
			{
				purchase = 'N';
				cout <<  "Seats are not available\n";
				
			}
		}

		// Display final ticket order
		cout << setprecision(2) << fixed;
		cout << "The number of seats requested: " << number << endl;
		cout << "The price for each seat: $" << aud[rowNum - 1][0].cost << endl;
		cout << "The total cost is: $" << aud[rowNum - 1][0].cost*number << endl;
		
		purchase = 'Y';
		
	}

	// purchaseTickets member function updats seats
	// when tickets are ordered
	string purchaseTickets(double amount, int number, int rowNumber, int startNum)
	{
		string result = "";
		double seatPrice = aud[rowNumber - 1][0].cost;
		double totalPrice = seatPrice * number;

		// Check if amount paid is enough
		if (totalPrice - amount < 0.01)
		{
			// Update the seats as sold
			for (int x = 0; x < number; x++)
			{
				aud[rowNumber - 1][startNum + x - 1].valid = '*';
			}

			// Create the tickets
			result += "Tickets are as follows: \n";

			for (int x = 0; x < number; x++)
			{
				result += "Ticket number: " + to_string(x + 1);
				result += "; Row: " + to_string(rowNumber) + "; Seat: " +
					to_string(startNum + x) + "\n";
			}
		}
		else
		{
			// If amount isn't sufficient display error message
			result = "Insufficient Amount!\n";
		}
		return result;
	}

	// report member function displays the 
	// sales history report
	void report()
	{
		seat = 0;
		price = 0.0;
		for (int row = 0; row < 15; row++)
		{
			for (int col = 0; col < 30; col++)
			{
				if (aud[row][col].valid == '*')
				{
					seat++;
					price += aud[row][col].cost;
				}
			}
		}
		cout << fixed << setprecision(2);
		cout << "Seats Sold: " << seat << endl;
		cout << "Seats Available: " << (15 * 30) - seat << endl;
		cout << "Money Collected: $" << price << endl;
	}

};


int main()
{
	// Declare variables
	int op, row, amount, seat1;
	char purchase = 'Y';
	double moneyPaid;
	string result;
	TicketManager aud;
	
	// Print menu
	do
	{
		cout << "Menu" << endl;
		cout << "press 1 for seating chart" << endl;
		cout << "press 2 to request tickets" << endl;
		cout << "press 3 for print sales" << endl;
		cout << "press 4 for exit" << endl;
		cin >> op;

		// While the user enters a choice outside of 1 or 4
		while (op < 1 || op > 4)
		{
			cout << "Enter the input(1-4): ";
			cin >> op;
		}
		// If user enters correct choice
		switch (op)
		{
		// Display seats
		case 1:
			cout << aud.seatArrangement();
			break;
		// Purchase tickets
		case 2:
			cout << endl << "Enter number of seats required: ";
			cin >> amount;
			cout << "Enter the row number: ";
			cin >> row;
			cout << "Enter the column number: ";
			cin >> seat1;
			aud.order(amount, row, seat1, purchase);
			cout << result << endl;

			if (toupper(purchase) != 'N')
			{
				cout << "Do you want to purchase these seats? (Y/N): ";
				cin >> purchase;

				if (toupper(purchase) == 'Y')
				{
					cout << "Enter the amount for the tickets: ";
					cin >> moneyPaid;
					result = aud.purchaseTickets(moneyPaid, amount, row, seat1);
					cout << result << endl;
				}
			}
			break;
		
		// Display sales report
		case 3:
			cout << endl << "Sales Report: " << endl;
			aud.report();
			cout << result;
			break;

		// Quit
		case 4:
			break;
		}
		cout << endl;
	} while (op != 4); // While user choice is not 4

	system("pause");
	return 0;
}