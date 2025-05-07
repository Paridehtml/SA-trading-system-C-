#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Structure to represent a stock purchase
struct Stock {
    string stock_name;
    int number_of_shares;
    double price_per_share;
};

// Class to manage the stock portfolio
class StockPortfolio {
private:
    static const int MAX_SIZE = 100; // Maximum portfolio size
    Stock portfolio[MAX_SIZE]; // Array-based portfolio
    int size; // Current number of stocks in portfolio

    // Helper method to validate stock existence
    bool stockExists(const string& name) {
        for (int i = 0; i < size; i++) {
            if (portfolio[i].stock_name == name) {
                return true;
            }
        }
        return false;
    }

    // Helper method to count total shares for a stock
    int countShares(const string& name) {
        int total = 0;
        for (int i = 0; i < size; i++) {
            if (portfolio[i].stock_name == name) {
                total += portfolio[i].number_of_shares;
            }
        }
        return total;
    }

    // Helper method to validate input
    bool isValidInput(int shares, double price, double percentage) {
        if (shares <= 0) {
            cout << "Error: Number of shares must be positive." << endl;
            return false;
        }
        if (price <= 0) {
            cout << "Error: Price per share must be positive." << endl;
            return false;
        }
        if (percentage < -100 || percentage > 100) {
            cout << "Error: Percentage change must be between -100% and +100%." << endl;
            return false;
        }
        return true;
    }

public:
    StockPortfolio() : size(0) {}

    // Add a new stock purchase to portfolio
    void buyStock(const string& name, int shares, double price) {
        if (size >= MAX_SIZE) {
            cout << "Error: Portfolio is full!" << endl;
            return;
        }
        if (!isValidInput(shares, price, 0)) return;

        portfolio[size] = {name, shares, price};
        size++;
        cout << "Bought " << shares << " shares of " << name << " at £" 
             << fixed << setprecision(2) << price << endl;
    }

    // Sell stocks using FIFO (earliest purchase)
    void sellFIFO(const string& name, int shares) {
        if (!stockExists(name)) {
            cout << "Error: Stock " << name << " not found in portfolio." << endl;
            return;
        }
        if (!isValidInput(shares, 1.0, 0)) return;
        if (countShares(name) < shares) {
            cout << "Error: Not enough shares of " << name << " to sell. Available: " 
                 << countShares(name) << endl;
            return;
        }

        int sharesToSell = shares;
        for (int i = 0; i < size && sharesToSell > 0;) {
            if (portfolio[i].stock_name == name) {
                if (portfolio[i].number_of_shares <= sharesToSell) {
                    cout << "Sold " << portfolio[i].number_of_shares << " shares of " << name 
                         << " at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
                    sharesToSell -= portfolio[i].number_of_shares;
                    // Shift remaining elements left
                    for (int j = i; j < size - 1; j++) {
                        portfolio[j] = portfolio[j + 1];
                    }
                    size--;
                } else {
                    cout << "Sold " << sharesToSell << " shares of " << name 
                         << " at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
                    portfolio[i].number_of_shares -= sharesToSell;
                    sharesToSell = 0;
                }
            } else {
                i++;
            }
        }
    }

    // Sell stocks using LIFO (most recent purchase)
    void sellLIFO(const string& name, int shares) {
        if (!stockExists(name)) {
            cout << "Error: Stock " << name << " not found in portfolio." << endl;
            return;
        }
        if (!isValidInput(shares, 1.0, 0)) return;
        if (countShares(name) < shares) {
            cout << "Error: Not enough shares of " << name << " to sell. Available: " 
                 << countShares(name) << endl;
            return;
        }

        int sharesToSell = shares;
        for (int i = size - 1; i >= 0 && sharesToSell > 0;) {
            if (portfolio[i].stock_name == name) {
                if (portfolio[i].number_of_shares <= sharesToSell) {
                    cout << "Sold " << portfolio[i].number_of_shares << " shares of " << name 
                         << " at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
                    sharesToSell -= portfolio[i].number_of_shares;
                    // Shift remaining elements left
                    for (int j = i; j < size - 1; j++) {
                        portfolio[j] = portfolio[j + 1];
                    }
                    size--;
                } else {
                    cout << "Sold " << sharesToSell << " shares of " << name 
                         << " at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
                    portfolio[i].number_of_shares -= sharesToSell;
                    sharesToSell = 0;
                }
            }
            i--;
        }
    }

    // Process stock performance based on percentage change
    void enterStockPerformance() {
        string name;
        double percentage;
        cout << "Enter stock name: ";
        cin >> name;
        cout << "Enter percentage change (e.g., 8.5 for +8.5%, -2.3 for -2.3%): ";
        while (!(cin >> percentage)) {
            cout << "Error: Invalid percentage. Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        if (!isValidInput(1, 1.0, percentage)) return;

        if (percentage >= 5.0 && percentage <= 20.0) {
            // Sell using FIFO
            int shares;
            cout << "Enter number of shares to sell: ";
            while (!(cin >> shares)) {
                cout << "Error: Invalid number of shares. Please enter a positive integer: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            sellFIFO(name, shares);
        } else if (percentage >= -1.0 && percentage <= -0.5) {
            // Buy more shares
            int shares;
            double price;
            cout << "Enter number of shares to buy: ";
            while (!(cin >> shares)) {
                cout << "Error: Invalid number of shares. Please enter a positive integer: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cout << "Enter price per share (£): ";
            while (!(cin >> price)) {
                cout << "Error: Invalid price. Please enter a positive number: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            buyStock(name, shares, price);
        } else if (percentage >= -3.0 && percentage <= -1.1) {
            // Sell using LIFO
            int shares;
            cout << "Enter number of shares to sell: ";
            while (!(cin >> shares)) {
                cout << "Error: Invalid number of shares. Please enter a positive integer: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            sellLIFO(name, shares);
        } else {
            cout << "No action taken (percentage change outside strategy bounds)." << endl;
        }
    }

    // Find and display all entries for a specific stock
    void findStock() {
        string name;
        cout << "Enter stock name to find: ";
        cin >> name;
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (portfolio[i].stock_name == name) {
                cout << portfolio[i].stock_name << ": " << portfolio[i].number_of_shares 
                     << " shares at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Error: No entries found for " << name << endl;
        }
    }

    // Set up test data
    void setupTestData() {
        size = 0;
        buyStock("MSFT", 100, 200.39);
        buyStock("GME", 500, 9.39);
        buyStock("MSFT", 250, 214.22);
        buyStock("MSFT", 300, 222.59);
        buyStock("AAPL", 200, 150.75);
        buyStock("AAPL", 150, 145.20);
        buyStock("TSLA", 80, 300.45);
        buyStock("TSLA", 120, 310.90);
        buyStock("AMZN", 60, 110.25);
        buyStock("AMZN", 90, 105.80);
        cout << "Test data set up successfully." << endl;
    }

    // Display the entire portfolio
    void showPortfolio() {
        if (size == 0) {
            cout << "Portfolio is empty." << endl;
            return;
        }
        cout << "Stock Portfolio:" << endl;
        for (int i = 0; i < size; i++) {
            cout << portfolio[i].stock_name << ": " << portfolio[i].number_of_shares 
                 << " shares at £" << fixed << setprecision(2) << portfolio[i].price_per_share << endl;
        }
    }
};

// Main function with menu
int main() {
    StockPortfolio portfolio;
    int choice;

    do {
        cout << "\nStock Portfolio Menu:" << endl;
        cout << "1. Enter stock performance" << endl;
        cout << "2. Find stock" << endl;
        cout << "3. Set up test data" << endl;
        cout << "4. Show stock portfolio" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        while (!(cin >> choice)) {
            cout << "Error: Invalid choice. Please enter a number: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
            case 1:
                portfolio.enterStockPerformance();
                break;
            case 2:
                portfolio.findStock();
                break;
            case 3:
                portfolio.setupTestData();
                break;
            case 4:
                portfolio.showPortfolio();
                break;
            case 5:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Error: Invalid choice. Try again." << endl;
        }
    } while (choice != 5);

    return 0;
}