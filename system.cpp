#include <iostream>
#include <list>
#include <vector>
#include<iterator>
#include <algorithm>
#include <queue>
#include <limits>
using namespace std;

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
        
        // constructors for assigning values
        City(){} // default constructor
        City(int id, string name){
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
    // function to find the shortest route  between two cities
    int shortestRoute(int id1, int id2){
        // Applying Dijkstra's Algorithm

        // Initialization
        vector<int> distance(cities.size(), INT_MAX);
        vector<bool> visited(cities.size(), false);
        distance[id1] = 0;

        // Dijkstra's Algorithm
        for (int i = 0; i < cities.size() - 1; ++i) {
            int u = -1;
            // Find the unvisited city with the shortest distance
            for (int j = 0; j < cities.size(); ++j) {
                if (!visited[j] && (u == -1 || distance[j] < distance[u])) {
                    u = j;
                }
            }

            // Mark the selected city as visited
            visited[u] = true;

            // Update the distance of the neighboring cities
            for (list<Link>::iterator it = cities[u].linksDeparture.begin(); it != cities[u].linksDeparture.end(); ++it) {
                int v = it->destinationID;
                if (!visited[v] && distance[u] != INT_MAX && distance[u] + it->distance < distance[v]) {
                    distance[v] = distance[u] + it->distance;
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
};
// driver code
int main(){
    AirlineNetwork* network = new AirlineNetwork;
    
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

//    //printing the network
//     cout << "Departure Schedules are as follows: " << endl;
//     network->printDepartures();
//     cout << endl;

//     cout << "Arrival Schedules are as follows: " << endl;
//     network->printArrivals();
//     cout << endl;

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
    cout << "\nShortest route betweeb Karachi and Gilgit is : " << network->shortestRoute(1, 3) << " kilometers" << endl;

    // Task 6 -- Find a route between two cities
    cout << "\nRoute between Karachi(1) and Gilgit(4) is as follows: " << endl;
    list <City> route = network->findRouteBetweenCities(1,4);
    list <City> :: iterator it;
    for(it = route.begin(); it != route.end(); ++it){
        cout << it->cityID <<  "->";
    }
    
    delete network;

    return 0;
}