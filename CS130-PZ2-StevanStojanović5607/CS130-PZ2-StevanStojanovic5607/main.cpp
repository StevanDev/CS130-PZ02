#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>

class Vehicle {
public:
	virtual ~Vehicle() = default;
	virtual void display() const = 0;
	virtual double getPrice() const = 0;
	virtual std::string getModel() const = 0;
	virtual std::string getType() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
	friend std::istream& operator>>(std::istream& is, Vehicle*& vehicle);
};

class Car :public Vehicle {
private:
	std::string model;
	double price;

public:
	Car(const std::string& m = "", double p = 0.0) : model(m), price(p) {}
	void display() const override {
		std::cout << "Car: " << model << ", Price: $" << price << std::endl;
	}
	double getPrice() const override {
		return price;
	}
	std::string getModel() const override {
		return model;
	}
	std::string getType() const override {
		return "Car";
	}

	friend std::ostream& operator<<(std::ostream& os, const Car& car);
	friend std::istream& operator>>(std::istream& is, Car& car);
};

class Motorcycle :public Vehicle {
private:
	std::string model;
	double price;

public:
	Motorcycle(const std::string& m = "", double p = 0.0) : model(m), price(p) {}
	void display() const override {
		std::cout << "Motorcycle: " << model << ", Price: $" << price << std::endl;
	}
	double getPrice() const override {
		return price;
	}
	std::string getModel() const override {
		return model;
	}
	std::string getType() const override {
		return "Motorcycle";
	}

	friend std::ostream& operator<<(std::ostream& os, const Motorcycle& motorcycle);
	friend std::istream& operator>>(std::istream& is, Motorcycle& motorcycle);
};

std::ostream& operator<<(std::ostream& os, const Car& car) {
	os << car.model << std::endl << car.price;
	return os;
}

std::istream& operator>>(std::istream& is, Car& car) {
	is >> car.model >> car.price;
	return is;
}

std::ostream& operator<<(std::ostream& os, const Motorcycle& motorcycle) {
	os << motorcycle.model << std::endl << motorcycle.price;
	return os;
}

std::istream& operator>>(std::istream& is, Motorcycle& motorcycle) {
	is >> motorcycle.model >> motorcycle.price;
	return is;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
	os << vehicle.getType() << std::endl;
	if (vehicle.getType() == "Car") {
		const Car* car = dynamic_cast<const Car*>(&vehicle);
		if (car) os << *car;
	}
	else if (vehicle.getType() == "Motorcycle") {
		const Motorcycle* motorcycle = dynamic_cast<const Motorcycle*>(&vehicle);
		if (motorcycle) os << *motorcycle;
	}
	return os;
}

std::istream& operator>>(std::istream& is, Vehicle*& vehicle) {
	std::string type;
	is >> type;
	if (type == "Car") {
		Car* car = new Car();
		is >> *car;
		vehicle = car;
	}
	else if (type == "Motorcycle") {
		Motorcycle* motorcycle = new Motorcycle();
		is >> *motorcycle;
		vehicle = motorcycle;
	}
	return is;
}

class FileNotFoundException :public std::exception {
private:
	std::string message;

public:
	FileNotFoundException(const char* filename) {
		message = "Failed to open file: ";
		message += filename;
	};

	const char* what() {
		return message.c_str();
	}
};

class OutOfMemoryException :public std::exception {
public:
	OutOfMemoryException() = default;
	OutOfMemoryException(const char* message) :std::exception(message) {};
};

class VehicleCollection {

public:
	~VehicleCollection() {
		clear();
	}
private:
	std::vector<Vehicle*> vehicles;

public:
	Vehicle* addMotorcycle(const std::string& model, double price) {
		Motorcycle* motorcycle = new Motorcycle(model, price);
		if (!motorcycle) {
			throw OutOfMemoryException();
		}
		vehicles.emplace_back(motorcycle);
		return motorcycle;
	}

	Vehicle* addCar(const std::string& model, double price) {
		Car* car = new Car(model, price);
		if (!car) {
			throw OutOfMemoryException();
		}
		vehicles.emplace_back(car);
		return car;
	}

	Vehicle* getVehicleByIndex(int index) {
		if (index < 0 || index >= vehicles.size()) {
			return nullptr;
		}
		return vehicles[index];
	}

	void displayVehicles() const {
		for (const auto& vehicle : vehicles) {
			vehicle->display();
		}
	}

	double getTotalPrice() const {
		double total = 0;
		for (const auto& vehicle : vehicles) {
			total += vehicle->getPrice();
		}
		return total;
	}

	void saveToFile(const std::string& filename)const {
		std::ofstream file(filename);
		if (!file) {
			throw FileNotFoundException(filename.c_str());
		}

		for (const auto& vehicle : vehicles) {
			file << *vehicle << std::endl;
		}
	}

	void loadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (!file) {
			throw FileNotFoundException(filename.c_str());
		}

		clear();
		Vehicle* vehicle = nullptr;
		while (file >> vehicle) {
			vehicles.emplace_back(vehicle);
		}
	}

	void clear() {
		for (int i = 0; i < vehicles.size(); i++) {
			delete vehicles[i];
		}
		vehicles.clear();
	}
};

int main() {
	VehicleCollection collection;

	while (true) {
		std::cout << "\nMenu:\n";
		std::cout << "1. Add Car\n";
		std::cout << "2. Add Motorcycle\n";
		std::cout << "3. Display All Vehicles\n";
		std::cout << "4. Display Total Price\n";
		std::cout << "5. Display Vehicle by Index\n";
		std::cout << "6. Save to File\n";
		std::cout << "7. Load from File\n";
		std::cout << "8. Exit\n";
		std::cout << "Enter your choice: ";

		int choice;
		std::cin >> choice;

		try {
			if (choice == 1) {
				std::string model;
				double price;
				std::cout << "Enter Car Model: ";
				std::cin >> model;
				std::cout << "Enter Car Price: ";
				std::cin >> price;
				try {
					collection.addCar(model, price);
				}
				catch (const OutOfMemoryException& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			else if (choice == 2) {
				std::string model;
				double price;
				std::cout << "Enter Motorcycle Model: ";
				std::cin >> model;
				std::cout << "Enter Motorcycle Price: ";
				std::cin >> price;
				try {
					collection.addMotorcycle(model, price);
				}
				catch (const OutOfMemoryException& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			else if (choice == 3) {
				collection.displayVehicles();
			}
			else if (choice == 4) {
				std::cout << "Total Price: $" << collection.getTotalPrice() << std::endl;
			}
			else if (choice == 5) {
				int index;
				std::cout << "Enter Vehicle Index: ";
				std::cin >> index;
				Vehicle* vehicle = collection.getVehicleByIndex(index);
				if (vehicle) {
					vehicle->display();
				}
				else {
					std::cout << "Invalid index!" << std::endl;
				}
			}
			else if (choice == 6) {
				std::string filename;
				std::cout << "Enter filename: ";
				std::cin >> filename;
				try {
					collection.saveToFile(filename);
				}
				catch (FileNotFoundException& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			else if (choice == 7) {
				std::string filename;
				std::cout << "Enter filename: ";
				std::cin >> filename;
				try {
					collection.loadFromFile(filename);
				}
				catch (FileNotFoundException& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			else if (choice == 8) {
				break;
			}
			else {
				std::cout << "Invalid choice! Please try again." << std::endl;
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}

	collection.clear();
	return 0;
}