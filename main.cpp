#include <iostream>
#include <fstream>
using namespace std;
class Ship {
	private:
		string _name;
		string _class;
		short _length;
		int _shieldCap;
		float _maxWarp;
		int _armamentCount;
		int _firepower;

	public:
		class Armament {
			private:
				string _name;
				int _power;
				float _powerUse;
			public:
				void SetArmName(string name) {
					_name = name;
				}
				void SetArmPower(int power) {
					_power = power;
				}
				void SetArmPowerUse(float powerUse) {
					_powerUse = powerUse;
				}
				string GetArmName() {
					return _name;
				}
				int GetArmPower() {
					return _power;
				}
				float GetArmPowerUse() {
					return _powerUse;
				}
		};
		Armament* armaments;
		void DisplayShip() {
			cout << "Name: " << GetName() << endl;
			cout << "Class: " << GetClass() << endl;
			cout << "Length: " << GetLength() << endl;
			cout << "Shield capacity: " << GetShieldCap() << endl;
			cout << "Maximum Warp: " << GetMaxWarp() << endl;
			cout << "Armaments: " << endl;
			if (_armamentCount == 0) {
				cout << "Unarmed" << endl;
			}
			else {
				for (int i = 0; i < _armamentCount; i++) {
					cout << armaments[i].GetArmName() << ", " << armaments[i].GetArmPower() << ", " << armaments[i].GetArmPowerUse() << endl;
				}
				cout << "Total firepower: " << GetTotalPower() << endl;
			}
		}
		void SetName(string name) {
			_name = name;
		}
		void SetClass(string className) {
			_class = className;
		}
		void SetLength(int length) {
			_length = length;
		}
		void SetShieldCap(int shieldCap) {
			_shieldCap = shieldCap;
		}
		void SetMaxWarp(double maxWarp) {
			_maxWarp = maxWarp;
		}
		void SetArmamentCount(int armamentCount) {
			_armamentCount = armamentCount;
		}
		string GetName() {
			return _name;
		}	
		string GetClass() {
			return _class;
		}
		short GetLength() {
			return _length;
		}
		int GetShieldCap() {
			return _shieldCap;
		}
		float GetMaxWarp() {
			return _maxWarp;
		}
		int GetArmamentCount() {
			return _armamentCount;
		}
		int GetTotalPower() {
			_firepower = 0;
			for (int i = 0; i < _armamentCount; i++) {
				_firepower += armaments[i].GetArmPower();
			}
			return _firepower;
		}
};
unsigned int shipCount = 0;
void LoadFile(string fileName, Ship* &ships) {
	ifstream shipFile(fileName, ios_base::binary);
	unsigned int count;
	shipFile.read((char*) &count, 4);
	unsigned int origShipCount = shipCount;
	shipCount += count;
	Ship* shipsToAdd = new Ship[shipCount];
	for (unsigned int i = 0; i < origShipCount; i++) {
		shipsToAdd[i] = ships[i];
	}
	unsigned int propSize;
	short length;
	int shieldCap;
	float maxWarp;
	int armamentCount;
	int power;
	float powerUse;
	for (unsigned int i = origShipCount; i < shipCount; i++) {
		shipFile.read((char*) &propSize, 4);
		char* nameStr = new char[propSize + 1];
		shipFile.read(nameStr, propSize);
		nameStr[propSize] = '\0';
		shipsToAdd[i].SetName(nameStr);
		delete[] nameStr;
		shipFile.read((char*) &propSize, 4);
		char* classStr = new char[propSize + 1];
		shipFile.read(classStr, propSize);
		classStr[propSize] = '\0';
		shipsToAdd[i].SetClass(classStr);
		delete[] classStr;
		shipFile.read((char*) &length, 2);
		shipsToAdd[i].SetLength(length);
		shipFile.read((char*) &shieldCap, 4);
		shipsToAdd[i].SetShieldCap(shieldCap);
		shipFile.read((char*) &maxWarp, 4);
		shipsToAdd[i].SetMaxWarp(maxWarp);
		shipFile.read((char*) &armamentCount, 4);
		shipsToAdd[i].SetArmamentCount(armamentCount);
		if (armamentCount != 0) {
			shipsToAdd[i].armaments = new Ship::Armament[armamentCount];
			for (int j = 0; j < armamentCount; j++) {
				shipFile.read((char*) &propSize, 4);
				char* nameStr = new char[propSize + 1];
				shipFile.read(nameStr, propSize);
				nameStr[propSize] = '\0';
				shipsToAdd[i].armaments[j].SetArmName(nameStr);
				delete[] nameStr;
				shipFile.read((char*) &power, 4);
				shipsToAdd[i].armaments[j].SetArmPower(power);
				shipFile.read((char*) &powerUse, 4);
				shipsToAdd[i].armaments[j].SetArmPowerUse(powerUse);
			}
		}
	}
	delete[] ships;
	ships = shipsToAdd;
	shipFile.close();
}
void ShipWithStrongestWeapon(Ship* ships) {
	int strongestWeapon = 0;
	int holdingShip = 0;
	for (unsigned int i = 0; i < shipCount; i++) {
		for (int j = 0; j < ships[i].GetArmamentCount(); j++) {
			if (ships[i].armaments[j].GetArmPower() > ships[holdingShip].armaments[strongestWeapon].GetArmPower()) {
				holdingShip = i;
				strongestWeapon = j;
			}
		}
	}
	ships[holdingShip].DisplayShip();
}
void StrongestShip(Ship* ships) {
	int strongestShip = 0;
	for (unsigned int i = 0; i < shipCount; i++) {
		if (ships[i].GetTotalPower() > ships[strongestShip].GetTotalPower() && ships[i].GetTotalPower() != 0) {
			strongestShip = i;
		}
	}
	ships[strongestShip].DisplayShip();
}
void WeakestShip(Ship* ships) {
	int weakestShip = 0;
	for (unsigned int i = 0; i < shipCount; i++ ) {
		if (ships[i].GetTotalPower() < ships[weakestShip].GetTotalPower() && ships[i].GetTotalPower() != 0) {
			weakestShip = i;
		}
	}
	ships[weakestShip].DisplayShip();
}
int main()
{
	cout << "Which file(s) to open?\n";
	cout << "1. friendlyships.shp" << endl;
	cout << "2. enemyships.shp" << endl;
	cout << "3. Both files" << endl;
	int option;
	cin >> option;

   /* Load files here */
	Ship* ships = nullptr;
	switch (option) {
		case 1:
			LoadFile("friendlyships.shp", ships);
			break;
		case 2:
			LoadFile("enemyships.shp", ships);
			break;
		case 3:
			LoadFile("friendlyships.shp", ships);
			LoadFile("enemyships.shp", ships);
			break;
		default:
			cout << "Invalid Selection" << endl;
			break;
	}

	cout << "1. Print all ships" << endl;
	cout << "2. Starship with the strongest weapon" << endl;
	cout << "3. Strongest starship overall" << endl;
	cout << "4. Weakest ship (ignoring unarmed)" << endl;
	cout << "5. Unarmed ships" << endl;
	
	cin >> option;
	
	/* Work your magic here */
	switch (option) {
		case 1:
			for (unsigned int i = 0; i < shipCount; i++) {
				ships[i].DisplayShip();
			}
			break;
		case 2:
			ShipWithStrongestWeapon(ships);
			break;
		case 3:
			StrongestShip(ships);
			break;
		case 4:
			WeakestShip(ships);
			break;
		case 5:
			for (unsigned int i = 0; i < shipCount; i++) {
				if (ships[i].GetArmamentCount() == 0) {
					ships[i].DisplayShip();
				}
			}
			break;
		default:
			cout << "Invalid Selection" << endl;
			break;
	}
	
   return 0;
}
