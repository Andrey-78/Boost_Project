
// hotel booking system

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <fstream>
#include <sstream>


class Customer {
public:
   std::string name;
   int room_number;
   std::vector<std::string> guests;
   int days_to_stay;
   std::string booking_time;
   std::string check_in_time;
   std::string check_out_time;
   int cost;

   Customer(const std::string& name, int room_number, const std::vector<std::string>& guests,
      int days_to_stay, const std::string& booking_time, const std::string& check_in_time,
      const std::string& check_out_time, int cost)
      : name(name), room_number(room_number), guests(guests), days_to_stay(days_to_stay),
      booking_time(booking_time), check_in_time(check_in_time), check_out_time(check_out_time),
      cost(cost) {}
};

class Hotel {
public:
   std::string name;
   int capacity;
   int max_guests_per_room;
   int cost_per_person;
   std::vector<Customer> customers;

   Hotel(const std::string& name, int capacity, int max_guests_per_room, int cost_per_person)
      : name(name), capacity(capacity), max_guests_per_room(max_guests_per_room), cost_per_person(cost_per_person) {}

   // Method for booking a room
   bool bookRoom(const std::string& customer_name, const std::vector<std::string>& guests,
      const std::string& check_in, const std::string& check_out) {
      if (customers.size() >= capacity) {
         std::cout << "Hotel is fully booked." << std::endl;
         return false;
      }

      int room_number = customers.size() + 1; // Simple algorithm for assigning a number
      int days_to_stay = boost::posix_time::time_period(
         boost::posix_time::time_from_string(check_in),
         boost::posix_time::time_from_string(check_out)).length().hours();

      if (guests.size() > max_guests_per_room) {
         std::cout << "Too many guests for the room." << std::endl;
         return false;
      }

      // Calculating the cost
      int total_cost = days_to_stay * cost_per_person * guests.size();

      Customer customer(customer_name, room_number, guests, days_to_stay,
         boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::universal_time()),
         check_in, check_out, total_cost);

      customers.push_back(customer);
      return true;
   }

   void printCustomers() {
      for (const auto& customer : customers) {
         std::cout << "Customer: " << customer.name
            << ", Room: " << customer.room_number
            << ", Cost: " << customer.cost << std::endl;
      }
   }
};
class ReservationSystem {
public:
   std::map<std::string, Hotel> hotels;

   void newHotel(const std::string& name, int capacity, int max_guests, int cost) {
      hotels[name] = Hotel(name, capacity, max_guests, cost);
   }

   void bookCMD(const std::string& hotel_name, const std::string& customer_name,
      const std::vector<std::string>& guests, const std::string& check_in, const std::string& check_out) {
      if (hotels.find(hotel_name) != hotels.end()) {
         if (hotels[hotel_name].bookRoom(customer_name, guests, check_in, check_out)) {
            std::cout << "Booking successful for " << customer_name << std::endl;
         }
      }
      else {
         std::cout << "Hotel not found." << std::endl;
      }
   }

   // The rest of the operations will be implemented here: Book-Email, Prolong, Get-Storage, Clear-Storage
   // For example, receiving storage or cleaning the system:
   void getStorage(bool detailed) {
      for (const auto& pair : hotels) {
         pair.second.printCustomers();
         if (detailed) {
            // Additional information can be added
         }
      }
   }

   void clearStorage() {
      hotels.clear();
      std::cout << "All hotels and bookings have been cleared." << std::endl;
   }
};
int main() {
   ReservationSystem system;

   system.newHotel("Seaside Hotel", 50, 5, 100); // Create a new hotel

   // Example of using commands
   system.bookCMD("Seaside Hotel", "John Doe", { "Alice", "Bob" },
      "2023-10-01 14:00:00", "2023-10-05 10:00:00");

   system.getStorage(true); // Show all clients

   return 0;
}