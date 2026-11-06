#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_POINTS = 3;
const int CAMPUS_X = 0;
const int CAMPUS_Y = 0;
const int GRID_SIZE = 12;

struct UniversityPoint {
    int pointNumber;
    char routeName[50];
    int currentX;
    int currentY;
    int status;
};

UniversityPoint points[MAX_POINTS];

// Set default route information
void setDefaultRoutes() {
    points[0] = {1, "Gulshan-Point", 8, 12, 1};
    points[1] = {2, "Johar-Point", 6, 8, 1};
    points[2] = {3, "Nazimabad", 4, 11, 1};
}

// Calculate distance from campus
double calculateDistance(int x, int y) {
    return sqrt(pow(x - CAMPUS_X, 2) +
                pow(y - CAMPUS_Y, 2));
}

// Save routes to a file
void saveRoutesToFile() {
    ofstream file("routes.txt");

    if (!file) {
        cout << "Error: Could not save routes.\n";
        return;
    }

    for (int i = 0; i < MAX_POINTS; i++) {
        file << points[i].pointNumber << " "
             << points[i].routeName << " "
             << points[i].currentX << " "
             << points[i].currentY << " "
             << points[i].status << "\n";
    }

    file.close();
    cout << "Routes saved successfully.\n";
}

// Load routes from a file
void loadRoutesFromFile() {
    ifstream file("routes.txt");

    if (!file) {
        cout << "Routes file not found. Loading default routes.\n";
        setDefaultRoutes();
        return;
    }

    UniversityPoint loaded[MAX_POINTS];
    int count = 0;

    while (count < MAX_POINTS &&
           file >> loaded[count].pointNumber
                >> loaded[count].routeName
                >> loaded[count].currentX
                >> loaded[count].currentY
                >> loaded[count].status) {
        count++;
    }

    file.close();

    if (count == MAX_POINTS) {
        for (int i = 0; i < MAX_POINTS; i++) {
            points[i] = loaded[i];
        }
        cout << "Routes loaded successfully.\n";
    } else {
        cout << "Invalid route file. Loading default routes.\n";
        setDefaultRoutes();
    }
}

// Display live tracking information
void displayLiveTracking() {
    cout << "\n========== LIVE POINT TRACKING ==========\n";

    cout << left
         << setw(8) << "Point"
         << setw(20) << "Route"
         << setw(12) << "Position"
         << setw(12) << "Distance"
         << "Status\n";

    cout << "---------------------------------------------------------------\n";

    for (int i = 0; i < MAX_POINTS; i++) {
        double distance = calculateDistance(
            points[i].currentX,
            points[i].currentY
        );

        cout << left
             << setw(8) << points[i].pointNumber
             << setw(20) << points[i].routeName;

        cout << "(" << points[i].currentX
             << "," << points[i].currentY << ")";

        cout << setw(5) << " ";

        cout << fixed << setprecision(2)
             << setw(12) << distance;

        if (points[i].status == 1)
            cout << "Active\n";
        else
            cout << "Inactive\n";
    }

    cout << "==========================================\n";
}

// Display a console map
void renderConsoleMap() {
    cout << "\n========== UNIVERSITY ROUTE MAP ==========\n";

    for (int y = GRID_SIZE; y >= 0; y--) {
        cout << setw(2) << y << " |";

        for (int x = 0; x <= GRID_SIZE; x++) {
            char symbol = '.';

            if (x == CAMPUS_X && y == CAMPUS_Y)
                symbol = 'C';

            for (int i = 0; i < MAX_POINTS; i++) {
                if (points[i].currentX == x &&
                    points[i].currentY == y) {
                    symbol = '0' + points[i].pointNumber;
                }
            }

            cout << " " << symbol;
        }

        cout << "\n";
    }

    cout << "   +";
    for (int x = 0; x <= GRID_SIZE; x++)
        cout << "--";
    cout << "\n     ";

    for (int x = 0; x <= GRID_SIZE; x++)
        cout << x << " ";

    cout << "\n\nC = Campus | 1, 2, 3 = University Points\n";
}

// Simulate movement of university points
void simulateFleetMovement() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "\n========== MOVEMENT SIMULATION ==========\n";

    for (int i = 0; i < MAX_POINTS; i++) {
        int oldX = points[i].currentX;
        int oldY = points[i].currentY;

        if (points[i].status == 1) {
            int direction = rand() % 4;

            switch (direction) {
                case 0:
                    points[i].currentX++;
                    break;
                case 1:
                    points[i].currentX--;
                    break;
                case 2:
                    points[i].currentY++;
                    break;
                case 3:
                    points[i].currentY--;
                    break;
            }

            // Keep coordinates inside the map
            if (points[i].currentX < 0)
                points[i].currentX = 0;
            if (points[i].currentX > GRID_SIZE)
                points[i].currentX = GRID_SIZE;
            if (points[i].currentY < 0)
                points[i].currentY = 0;
            if (points[i].currentY > GRID_SIZE)
                points[i].currentY = GRID_SIZE;

            cout << points[i].routeName
                 << ": (" << oldX << "," << oldY << ")"
                 << " -> (" << points[i].currentX
                 << "," << points[i].currentY << ")\n";
        } else {
            cout << points[i].routeName
                 << ": Inactive; no movement.\n";
        }
    }

    saveRoutesToFile();
    cout << "Simulation completed.\n";
}

// Main program
int main() {
    setDefaultRoutes();
    loadRoutesFromFile();

    int choice;

    do {
        cout << "\n====================================\n";
        cout << " UNIVERSITY POINT ROUTE SIMULATOR\n";
        cout << "====================================\n";
        cout << "1. View Live Tracking\n";
        cout << "2. Simulate Point Movement\n";
        cout << "3. Display Console Map\n";
        cout << "4. Reload Routes From File\n";
        cout << "5. Save Routes To File\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Enter a number from 1 to 6.\n";
            continue;
        }

        switch (choice) {
            case 1:
                displayLiveTracking();
                break;

            case 2:
                simulateFleetMovement();
                break;

            case 3:
                renderConsoleMap();
                break;

            case 4:
                loadRoutesFromFile();
                break;

            case 5:
                saveRoutesToFile();
                break;

            case 6:
                cout << "Exiting University Point Route Simulator.\n";
                break;

            default:
                cout << "Invalid choice. Please select 1-6.\n";
        }

    } while (choice != 6);

    return 0;
}
