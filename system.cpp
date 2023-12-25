#include <iostream>
#include <list>
#include <vector>
#include<iterator>
#include <algorithm>
#include <queue>
#include <limits>
#include <cstdlib>
using namespace std;

// forward decalaration
class Reservation;
// class for a passenger
class Passenger{
    public:
    string name;
    int pID;
    list <Reservation> passenger_reservations;
};
// Class for a reservation
class Reservation {
public:
    int reservationID;
    int departureID;
    string departureName;
    int destinationID;
    string destinationName;
    float distance;
    int day;
    int month;
    int year;
    int departure_hrs;
    int departure_mins;
    string pass_name;

    // Constructors for assigning values
    Reservation() {} // Default constructor
    Reservation(int resID, int deptID, string deptName, int destID, const string& destName, float dist, int dd, int mm, int yyyy, int dh, int dm, string name)
        : reservationID(resID), departureID(deptID), departureName(deptName), destinationID(destID), destinationName(destName), distance(dist),
          day(dd), month(mm), year(yyyy), departure_hrs(dh), departure_mins(dm), pass_name(name) {}
};

// class for DlinksDe between cities
class Link{
    public:
        int destinationID;
        string destinationName;
        float distance;
        int day;
        int month;
        int year;
        int departure_hrs;
        int departure_mins;

        // constructors for assigning values 
        Link(){} // default constructor
        Link(int id, string name, float dist, int dd, int mm, int yyyy, int dh, int dm){ // parameterized constructor
            destinationID = id;
            destinationName = name;
            distance = dist;
            day = dd;
            month = mm;
            year = yyyy;
            departure_hrs = dh;
            departure_mins = dm;
        }
};
// class for a node i.e. city in our class
class City{
    public:
        int cityID;
        string cityName;
        list<Link> linksDeparture; // list of the departure to other cities
        list<Link> linksArrival; // list of the arrival from other cities
        list<Reservation> reservations; // reservations for departure flights
        int total_reservations;
        
        // constructors for assigning values
        City() : total_reservations(0){} // default constructor
        City(int id, string name){
            total_reservations = 0;
            cityID = id;
            cityName = name;
            // list of DlinksDepar will be updated separately
        }
        // printing the DlinksDepa
        void printDeptLinks(){
            cout << "[";
            list<Link> :: iterator it;
            for(it = linksDeparture.begin(); it != linksDeparture.end(); ++it){
                cout << "{" << it->destinationName << " (" << it->destinationID << ")_" << it->distance << "kms" << "_departing at " << it->day <<"/" << it->month <<"/" << it->year <<" _" << it->departure_hrs << ":" << it->departure_mins << "}";
            }
            cout << "]";
        }
        void printArrivalLinks(){
            cout << "[";
            list<Link> :: iterator it;
            for(it = linksArrival.begin(); it != linksArrival.end(); ++it){
                // here destinationID is for city that the flight arrived from-- typo
                cout << "{" << it->destinationName << " (" << it->destinationID << ")_" << it->distance << "kms" << "_arriving at " << it->day <<"/" << it->month <<"/" << it->year <<" _" << it->departure_hrs << ":" << it->departure_mins << "}";
            }
            cout << "]";
        }
        void printLinkedCities(){
            list <Link> :: iterator it;
            for(it = linksDeparture.begin(); it != linksDeparture.end(); ++it){
                cout << it->destinationName << " (" << it->destinationID << ")" << endl; 
            }
        }
};
// class for a graph that acts as an airline network
class AirlineNetwork{
    public:
        vector <City> cities; // vector of nodes i.e. cities

        // function to add a city to the network
        void addCity(int Id, string name){
            // checking if the city exists already
            if(checkIfCityExists(Id)){
                cout << "The city already exists in the network " << endl;
            }
            else{
                // making a new object of the city class
                City newCity(Id, name);
                // adding the city to the network
                cities.push_back(newCity);
            }
        }
        // function to check if a city exists in the network
        bool checkIfCityExists(int Id){
            // traversing the vector and comparing all the existing cities with the new ID
            for(int i = 0; i < cities.size(); i++){
                if(cities.at(i).cityID == Id)
                    return true;
            }
            return false;
        }
        // function to add a link between two cities
        // the link will suggest if there is a flight between the two cities
        // this function will make a directed connection between the two cities i.e. from city 1 to city 2
        void addFlight(int id1, int id2, float distance,int d, int m, int y, int dh, int dm, int hours){
            // checking if the two cities exist in the network
            bool check1 = checkIfCityExists(id1);
            bool check2 = checkIfCityExists(id2);
            if(check1 && check2){
                bool check3 = checkIfLinkExists(id1,id2); // --> from city 1 to city 2
                if(check3){
                    cout << "Flight  from " << id1 << "  to " << id2 << "  already exists " << endl;
                }
                else{
                    City c2 = getCityByID(id2);
                    for(int i = 0; i < cities.size(); i++){
                        if(cities.at(i).cityID == id1){
                            Link ld(id2,c2.cityName, distance,d, m, y, dh, dm); // departure from id1 to id2
                            cities.at(i).linksDeparture.push_back(ld);
                            addArrivalFlight(id1, id2, distance, d, m, y, dh+hours, dm);
                        }
                    }
                }
            }
        }
        // function to update the arrival list of a City
        void addArrivalFlight(int id1, int id2, int distance,int d, int m, int y, int ah, int amins){
            // flight will arrive from id1 to id2
            // fixing the time first
            while(amins>60){
                ah++;
                amins-=60;
            }
            for(int i = 0; i < cities.size(); i++){
                if(cities.at(i).cityID==id2){
                    City temp = getCityByID(id1);
                    Link l(id1,temp.cityName, distance,d, m, y, ah, amins); // arrival link from id1 to id2
                    cities.at(i).linksArrival.push_back(l);
                }
            }
        }
        // function to check if a link exists between two cities
        bool checkIfLinkExists(int id1, int id2){
            // this function will check if a link from id2 to id2 exists
            // i.e. we shall traverse the DlinksDepar list of id1 and find in it id2
            City c1 = getCityByID(id1);
            // traversinmg the DlinksDepar list of city 1
            list<Link> :: iterator it;
            for(it = c1.linksDeparture.begin(); it != c1.linksDeparture.end(); ++it){
                if(it->destinationID == id2){
                    return true;
                }
            }
            return false;
        }
        // function to get ID of a city and return the respected city from the network
        City getCityByID(int id){
            City temp;
            for(int i = 0; i < cities.size(); i++){
                if(cities.at(i).cityID == id)
                    temp = cities.at(i); // making temp equal the respecetd city
            }
            return temp;
        }
        // printing the netwrok
        void printDepartures(){
            for(int i = 0; i < cities.size(); i++){
                cout << cities.at(i).cityName << " (" << cities.at(i).cityID << ") --> ";
                cities.at(i).printDeptLinks();
                cout << endl;
            }
        }
        void printArrivals(){
            for(int i = 0; i < cities.size(); i++){
                cout << cities.at(i).cityName << " (" << cities.at(i).cityID << ") --> ";
                cities.at(i).printArrivalLinks();
                cout << endl;
            }
        }
        //  Function to show a list of all the cities serviced by airline in a tabular form
        void showCitiesInTabularForm(){
            for(int i = 0; i < cities.size(); i++){
                cout << cities.at(i).cityName << " (" << cities.at(i).cityID << ")" << endl;
            }
        }
        list<Link> sortInAscending(list<Link> l) {
            list<Link> sortedList;
            list<Link>::iterator it;
            list<Link>::iterator minElem;

            while (!l.empty()) {
                // Finding the earliest flight
                int min = l.front().departure_hrs;
                minElem = l.begin();

                for (it = l.begin(); it != l.end(); ++it) {
                    if (it->departure_hrs < min) {
                        minElem = it;
                        min = it->departure_hrs;
                    }
                }

                // Adding the earliest flight to the sorted list
                sortedList.push_back(*minElem);

                // Erasing the earliest flight from the original list
                l.erase(minElem);
            }

            return sortedList;
        }
    // function to show a list of flight departures for a city, sorted by the time of departure.
    void showListofdepartures(int id) {
        City city = getCityByID(id);
        city.linksDeparture = sortInAscending(city.linksDeparture);

        // Now printing the sorted list of flight departures
        list<Link>::iterator it;
        for (it = city.linksDeparture.begin(); it != city.linksDeparture.end(); ++it) {
            cout << it->destinationName << " (" << it->destinationID << ") -- " << "Departure date: " << it->day << "/" << it->month << "/" << it->year << "_" <<
                 "Departure Time: " << it->departure_hrs << ":" << it->departure_mins << endl;
        }
    }
    // function to show a list of flight arrivals for a city, sorted by the time of arrival. 
    void showListofarrivals(int id) {
        City city = getCityByID(id);
        city.linksDeparture = sortInAscending(city.linksDeparture);

        // Now printing the sorted list of flight departures
        list<Link>::iterator it;
        for (it = city.linksArrival.begin(); it != city.linksArrival.end(); ++it) {
            cout << it->destinationName << " (" << it->destinationID << ") -- " <<"Arrival date: " << it->day << "/" << it->month << "/" << it->year << "_" <<
                 "Arrival Time: " << it->departure_hrs << ":" << it->departure_mins << endl;
        }
    }
    // function to show a list of all the cities which can be reached from a particular city
    void listOfCitiesFromaCity(int id){
        // traversing the network to find the city (id) in the network
        for(int i = 0; i < cities.size(); i++){
            if(cities.at(i).cityID == id){
                cities.at(i).printLinkedCities();
            }
        }

    }
    // using dijkstra's algorithm for finding the shortest path
    int shortestRoute(int id1, int id2) {
        // Priority queue to store vertices with the minimum distance at the top
        priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;

        // Initialization
        vector<int> distance(cities.size()+1, INT_MAX);
        vector<bool> visited(cities.size()+1, false);
        distance[id1] = 0;

        // Enqueue the source vertex with distance 0
        pq.push(make_pair(0, id1));

        // Dijkstra's Algorithm
        while (!pq.empty()) {
            // Extract the vertex with the minimum distance
            int u = pq.top().second;
            pq.pop();

            // Skip if the vertex is already visited
            if (visited[u]) {
                continue;
            }

            // Mark the current vertex as visited
            visited[u] = true;

            for(int i = 0; i < cities.size(); i++){
                if(cities.at(i).cityID == u){
                    // Update the distance of the neighboring cities
                    for (list<Link>::iterator it = cities[i].linksDeparture.begin(); it != cities[i].linksDeparture.end(); ++it) {
                        int v = it->destinationID;
                        if (!visited[v] && distance[u] != INT_MAX && distance[u] + it->distance < distance[v]) {
                            distance[v] = distance[u] + it->distance;
                            pq.push(make_pair(distance[v], v));
                        }
                    }
            }
        }
        }

        // Return the shortest distance to the destination city
        return distance[id2];
    }


    // function to find a route between two cities
    list <City> findRouteBetweenCities(int id1, int id2){
        // implementing bfs for finding the route
        vector <bool> visited (cities.size(), false);
        list <City> queue;
        list<City> r; // route

        // pushing first element in the queue
        City first = getCityByID(id1);
        queue.push_back(first);

        bool breakpt = false;
        while(!queue.empty() && !breakpt){
            City temp = queue.front();
            queue.pop_front();
            // marking as visited
            visited[temp.cityID] = true;
            r.push_back(temp);

            list <Link> :: iterator it;
            for(it = temp.linksDeparture.begin(); it != temp.linksDeparture.end(); ++it){
                if(!visited[it->destinationID]){
                    queue.push_back(getCityByID(it->destinationID));
                }
                // checking if the destination node is found
                if(it->destinationID == id2){
                    r.push_back(getCityByID(it->destinationID));
                    breakpt = true;
                }
            }
        }
        return r;
    }
    // function to make reservation for a passenger
    void makeReservation(Passenger &p, int deptID,int destID, string pass_name){
        bool check1 = checkIfCityExists(deptID);
        bool check2 = checkIfCityExists(destID);
        if(check1 && check2){
            bool check3 = checkIfLinkExists(destID, deptID);
            if(check3){
                //iterating the edge list of departuring city to find the flight for which reservation is to be made
                for(int i = 0; i < cities.size(); i++){
                    if(cities[i].cityID == deptID){
                        list <Link> :: iterator it;
                        for(it = cities[i].linksDeparture.begin(); it != cities[i].linksDeparture.end(); ++it){
                            if(it->destinationID==destID){
                                // we have found the flight
                                Reservation r(++cities[i].total_reservations,cities[i].cityID, cities[i].cityName, it->destinationID, it->destinationName, it->distance, it->day, it->month, it->year, it->departure_hrs, it->departure_mins, pass_name);
                                cities[i].reservations.push_back(r); // reservation made
                                p.passenger_reservations.push_back(r);
                                cout << "Reservation for " << p.name << " is made in the flight from " << getCityByID(deptID).cityName << " to " << getCityByID(destID).cityName << endl;
                            }
                        }
                    }
                }
            }
            else{
                cout << " No flight available between city " << getCityByID(deptID).cityName << " and " << getCityByID(destID).cityName << endl;
            }
        }
        else{
            cout << " Invalid cities " << endl;
        }
    }

    // function to print a passenger's reservation schedule
    void printReservations(Passenger &p){
        list <Reservation> :: iterator it;
        int i = 0;
        for(it = p.passenger_reservations.begin(); it != p.passenger_reservations.end(); ++it){
            cout << "Reservation # " << ++i  << endl;
            cout << "Passenger Name: " << it->pass_name << endl;
            cout << "Departure City: " << it->departureName << " (" << it->departureID << ")" << endl;
            cout << "Arrival City: " << it->destinationName << " (" << it->destinationID << ")" << endl;
            cout << "Flight Date: " << it->day << "/" << it->month << "/" << it->year << endl;
            cout << "Flight Time: " << it->departure_hrs << ":" << it->departure_mins << endl;
        }
    }
    void deleteReservation(Passenger &p, int resID) {
    list<Reservation>::iterator it;
    // deleting from passenger's list of reservations
    for (it = p.passenger_reservations.begin(); it != p.passenger_reservations.end(); it++) {
        if (it->reservationID == resID) {
            p.passenger_reservations.erase(it);
            break;
        }
    }
    // deleting from the respective city's list of reservations
    for (int i = 0; i < cities.size(); i++) {
        for (it = cities.at(i).reservations.begin(); it != cities.at(i).reservations.end(); ++it) {
            if (it->reservationID == resID){
                cities.at(i).reservations.erase(it);
                cout << "Reservation " << resID << " deleted from " << cities.at(i).cityName << "'s flight schedule. " << endl;
                break;
            }
        }
    }
}

    // function to return last name
    string returnLastName(string fullName){
        int ind = 0;
        while(fullName[ind] != '\0'){
            if(fullName[ind] == ' '){
                ind++;
                break;
            }
            ind++;
        }
        // now storing the remaining name in a string
        string temp = "";
        while(fullName[ind] != '\0'){
            temp += fullName[ind++];
        }
        return temp;
    }
    // Function to print a list of passengers of a particular flight (in order of last name).
    void printListOfPassengersinFlight(int deptID) {
        list<Reservation> r;
        
        for (int i = 0; i < cities.size(); i++) {
            if (cities.at(i).cityID == deptID) {
                r = cities.at(i).reservations;
                break;
            }
        }
        
        // now making a vector of all the reservations
        vector<string> v;
        list<Reservation>::iterator it;
        
        for (it = r.begin(); it != r.end(); ++it) {
            v.push_back(it->pass_name);
        }
        // sorting the vector in order of the last name by bubble sort
        for (int i = 0; i < v.size() - 1; i++) {
            for (int j = 0; j < v.size() - i - 1; j++) {
                string lastName1 = returnLastName(v.at(j));
                string lastName2 = returnLastName(v.at(j + 1));
                
                if (lastName1[0] > lastName2[0]) {
                    swap(v.at(j), v.at(j + 1));
                }
            }
        }
        
        // printing the sorted vector
        for (int i = 0; i < v.size(); i++) {
            cout << v.at(i) << endl;
        }
    }


};
// driver code
int main(){
    AirlineNetwork* network = new AirlineNetwork;
    Passenger* passengers = new Passenger[100]; // assuming the flights can only accomodate 100 passengers
    int pass_id = 0;

    
    //adding cities to the network
    network->addCity(1, "Khi");
    network->addCity(2, "Isl");
    network->addCity(3, "Pesh");
    network->addCity(4, "Gilgit");
    network->addCity(5, "Quetta");

    // adding flights between the cities
    network->addFlight(1, 2, 200, 12, 3, 2023, 6, 30, 1);
    network->addFlight(4, 2, 100, 12, 3, 2023, 7, 30, 1);
    network->addFlight(5, 2, 50, 12, 3, 2023,12, 30, 1);
    network->addFlight(2, 1, 200, 12, 3, 2023, 16, 00, 4);
    network->addFlight(3, 4, 100, 12, 3, 2023, 11, 50, 2);
    network->addFlight(2, 5, 100, 12, 3, 2023,14, 20, 1);
    network->addFlight(2, 3, 55, 12, 3, 2023,13, 45, 2);
    network->addFlight(5, 4, 50, 12, 3, 2023,12, 30, 1);

       //printing the network
        cout << "Departure Schedules are as follows: " << endl;
        network->printDepartures();
        cout << endl;

        cout << "Arrival Schedules are as follows: " << endl;
        network->printArrivals();
        cout << endl;

    //  Task 1 -- Show a list of all the cities serviced by airline in a tabular form.
    cout << "\nFollowing is the list of all the cities in the airline system in tabular form: " << endl;
    network->showCitiesInTabularForm();

    // Task 2 -- Show a list of flight departures for a city, sorted by the time of departure.
    cout << "\nList of flight departures for Islamabad, sorted by the time of departure, is: " << endl;
    network->showListofdepartures(2);  

    // TAsk 3 --  Show a list of flight arrivals for a city, sorted by the time of arrival. 
    cout << "\nList of flight arrivals for Islamabad, sorted by the time of arrival, is: " << endl;
    network->showListofarrivals(2); 

    // Task 4 --  Show a list of all the cities which can be reached from a particular city
    cout << "\nList of all the cities that can be reached from Islamabad is as follows: " << endl;
    network->listOfCitiesFromaCity(2);

    // Task 5 -- Find the shortest route between two cities
    cout << "\nShortest route betweeb Karachi and Islamabad is : " << network->shortestRoute(1, 2) << " kilometers" << endl;

    // Task 6 -- Find a route between two cities
    cout << "\nRoute between Karachi(1) and Gilgit(4) is as follows: " << endl;
    list <City> route = network->findRouteBetweenCities(1,4);
    list <City> :: iterator it;
    for(it = route.begin(); it != route.end(); ++it){
        cout << it->cityID <<  "->";
    }
    
    // Task 7 -- Make an airline reservation for a passenger between two cities.
    cout << "\n\nMaking reservation for three passenger for a flight from Karachi to Islamabad: " << endl;
    network->makeReservation(passengers[++pass_id], 1, 2, "Alishba Ramzan");
    network->makeReservation(passengers[++pass_id], 1, 2, "Noor Fatima");
    network->makeReservation(passengers[++pass_id], 1, 2, "Ayesha Khan");

    // Task 8 -- Print a passenger's reservation schedule.
    cout << "\nPrinting the passengers' reservation schedule: " << endl;
    network->printReservations(passengers[pass_id]); cout << endl;
    network->printReservations(passengers[pass_id-1]); cout << endl;
    network->printReservations(passengers[pass_id-2]); cout << endl;

    // Task 9 -- Delete a passenger's reservation
    cout << "\nDeleting the passenger reservation " << endl;
    network->deleteReservation(passengers[pass_id], 1);

    // Task 10 --  Print a list of passengers of a particular flight (in order of last name). 
    cout << "\nList of passengers in the flight from Karachi (in the order of their last name) is as follows: " << endl;
    network->printListOfPassengersinFlight(1);


    delete network;

    return 0;
}