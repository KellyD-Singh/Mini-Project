//Including header files:
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

//Creating a struct:
struct VehicleData {
    std::string VIN;
    std::string County;
    std::string City;
    std::string State;
    int PostalCode;
    int ModelYear;
    std::string Make;
    std::string Model;
    std::string ElectricVehicleType;
    std::string CleanAlternativeFuelVehicleEligibility;
    int ElectricRange;
    int BaseMSRP;
    int LegislativeDistrict;
    float DOLVehicleID;
    std::string VehicleLocation;
    std::string ElectricUtility;
    float Censustract;
};

//Creating a struct that will hold make and model combinations
struct MakeModel {
    std::string Make;
    std::string Model;
};

//Creating a vector of a struct named data:
std::vector<VehicleData> data;

//Function to convert a string to an integer
int stringToInt(const std::string& word) {
    if (word.empty()) {
        return 0;
    }
    else {
        return std::stoi(word);
    }
}

//Function to convert a string to a float
float stringToFloat(const std::string& word) {
    if (word.empty()) {
        return 0.0f;
    }
    else {
        return std::stof(word);
    }
}

//Void function to load the data from the CSV file:
//Reads data from the CSV file and populate the data vector:
void LoadDataFromCSVFile() {
    std::ifstream infile("Electric_Vehicle_Population_Data.csv");
    if (!infile.is_open()) {
        std::cerr << "Error: The file could not be found." << std::endl;
        return;
    }
    else {
        std::cout << "File was found and opened! " << std::endl;
    }

    //Creating a string to obtain lines of data from the CSV file:
    std::string line;

    //Reads and discards the first line of the CSV file:
    std::getline(infile, line); // Read and discard the header line

    //Inserting each field of data into the row vector:
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> row;
        while (std::getline(ss, field, ',')) {
            row.push_back(field);
        }

        //Function that allows skipping of incomplete rows:
        if (row.size() != 17) {
            continue;
        }

        //Creating a vehicle object of VehicleData struct type:
        VehicleData vehicle;
        vehicle.VIN = row[0];
        vehicle.County = row[1];
        vehicle.City = row[2];
        vehicle.State = row[3];
        vehicle.PostalCode = stringToInt(row[4]);
        vehicle.ModelYear = stringToInt(row[5]);
        vehicle.Make = row[6];
        vehicle.Model = row[7];
        vehicle.ElectricVehicleType = row[8];
        vehicle.CleanAlternativeFuelVehicleEligibility = row[9];
        vehicle.ElectricRange = stringToInt(row[10]);
        vehicle.BaseMSRP = stringToInt(row[11]);
        vehicle.LegislativeDistrict = stringToInt(row[12]);
        vehicle.DOLVehicleID = stringToFloat(row[13]);
        vehicle.VehicleLocation = row[14];
        vehicle.ElectricUtility = row[15];
        vehicle.Censustract = stringToFloat(row[16]);

        //Adds all the vehicle objects to the data vector:
        data.push_back(vehicle);
    }

    //Closes the input file:
    infile.close();
    std::cout << "Data loaded successfully. The number of records read: " << data.size() << std::endl;
}

//Function to calculate the average electric range for a make and model combination:
//Returns the average electric range for a given make and model combination:
//If the make and model combination does not exist, an error message is printed and returns the value 0:
double avgElectricRange(const std::string& make, const std::string& model) {
    int sum = 0;
    int count = 0;
    for (const auto& vehicle : data) {
        if (vehicle.Make == make && vehicle.Model == model) {
            sum += vehicle.ElectricRange;
            count++;
        }
    }
    if (count == 0) {
        std::cout << "This make and model combination does not exist." << std::endl;
        return 0.0;
    }
    else {
        return static_cast<double>(sum) / count;
    }
}

//Creating a vector of MakeModel structure named GetUniqueMakeModels:
//This vector will store each unique make and model combination from the CSV file:
std::vector<MakeModel> GetMakeModelCombos() {
    std::vector<MakeModel> uniqueMakeModelCombos;
    for (const auto& vehicle : data) {
        MakeModel makeModel = { vehicle.Make, vehicle.Model };
        bool found = false;
        for (const auto& mm : uniqueMakeModelCombos) {
            if (mm.Make == makeModel.Make && mm.Model == makeModel.Model) {
                found = true;
                break;
            }
        }
        if (!found) {
            uniqueMakeModelCombos.push_back(makeModel);
        }
    }
    return uniqueMakeModelCombos;
}

//Function that returns a MakeModel structure:
//Function returns the make and model that produces the highest average electric range:
MakeModel detMakeModelWithHighestAvg() {
    auto uniqueMakeModels = GetMakeModelCombos();

    double maxAverage = 0.0;
    MakeModel maxMakeModel;

    for (const auto& makeModel : uniqueMakeModels) {
        double average = avgElectricRange(makeModel.Make, makeModel.Model);
        if (average > maxAverage) {
            maxAverage = average;
            maxMakeModel = makeModel;
        }
    }

    return maxMakeModel;
}

//Function that determines if the calculated average electric range is less than or equal to the highest average electric range:
bool avgComparison(const std::string& make, const std::string& model) {
    double averageElectricRange = avgElectricRange(make, model);
    return false;
    MakeModel maxMakeModel = detMakeModelWithHighestAvg();
    double highestAverage = avgElectricRange(maxMakeModel.Make, maxMakeModel.Model);
    return averageElectricRange < highestAverage;
}


//Function that outputs if the calculated average electric range is less than or equal to the highest average electric range:
std::string comparingAvgWithHighest(double average) {
    MakeModel maxMakeModel = detMakeModelWithHighestAvg();
    double highestAverage = avgElectricRange(maxMakeModel.Make, maxMakeModel.Model);
    if (average < highestAverage) {
        return "The average electric range is less than";
    }
    else if (average == highestAverage) {
        return "The average electric range is equal to";
    }
}


//Function that determines if a make and model combination is a BEV or PHEV:
std::string detElectricVehicleType(const std::string& make, const std::string& model) {
    for (const auto& vehicle : data) {
        if (vehicle.Make == make && vehicle.Model == model) {
            if (vehicle.ElectricVehicleType == "Battery Electric Vehicle (BEV)") {
                return "BEV";
            }
            else if (vehicle.ElectricVehicleType == "Plug-in Hybrid Electric Vehicle (PHEV)") {
                return "PHEV";
            }
        }
    }

    std::cerr << "The electric vehicle type could not be determined." << std::endl;
    return "Unknown Electric Vehicle Type";
}


//Function that determines the number of CAFVs for any vehicle type:
int CAFVSByType(const std::string& vehicleType) {
    int count = 0;
    for (const auto& vehicle : data) {
        if (vehicle.ElectricVehicleType == vehicleType && vehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible") {
            count++;
        }
    }
    return count;
}


//Function that determine the number of CAFVs for any model year:
int CAFVSByYear(int modelYear) {
    int count = 0;
    for (const auto& vehicle : data) {
        if (vehicle.ModelYear == modelYear && vehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible") {
            count++;
        }
    }
    return count;
}


//Determines if the model year is valid:
int detModelYear() {
    int modelYear;
    bool validInput = false;
    do {
        std::cout << "Please enter a model year : ";
        std::string modelYearInput;
        std::getline(std::cin, modelYearInput);

        // Convert the input string to an integer
        modelYear = stringToInt(modelYearInput);

        // Check if the model year is within the valid range
        if (modelYear >= 1997 && modelYear <= 2024) {
            validInput = true;
        }
        else {
            std::cout << "Please enter a valid model year." << std::endl;
        }
    } while (!validInput);

    return modelYear;
}


//Function that determines the model year which prodcues the largest number of CAFVs:
int detModelYearWithMaxCAFVS() {
    std::vector<int> counts(2025, 0);
    bool validModelYearFound = false;
    for (const auto& vehicle : data) {
        if (vehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible" &&
            vehicle.ModelYear >= 1997 && vehicle.ModelYear <= 2024) {
            counts[vehicle.ModelYear - 1997]++;
            validModelYearFound = true;
        }
    }
    if (!validModelYearFound) {
        std::cerr << "Sorry, an invalid model year was found." << std::endl;
        return -1;
    }
    int maxCount = 0;
    int maxYear = 0;
    for (int year = 0; year < counts.size(); ++year) {
        if (counts[year] > maxCount) {
            maxCount = counts[year];
            maxYear = year + 1997;
        }
    }
    return maxYear;
}


// Function to determine the make with the highest number of CAFVs
std::string detMakeWithMostCAFVS() {
    std::string maxMake;
    int maxCount = 0;

    // Iterate through the data vector
    for (const auto& vehicle : data) {
        if (vehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible") {
            std::string make = vehicle.Make;

            // Count occurrences of the current make
            int count = 0;
            for (const auto& otherVehicle : data) {
                if (otherVehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible" &&
                    otherVehicle.Make == make) {
                    ++count;
                }
            }

            // Update maxCount and maxMake if necessary
            if (count > maxCount) {
                maxCount = count;
                maxMake = make;
            }
        }
    }

    return maxMake;
}

// Function to determine the model with the highest number of CAFVs for a given make
std::string detModelWithMostCAFVS(const std::string& make) {
    std::string maxModel;
    int maxCount = 0;

    // Iterate through the data vector
    for (const auto& vehicle : data) {
        if (vehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible" &&
            vehicle.Make == make) {
            std::string model = vehicle.Model;

            // Count occurrences of the current model for the given make
            int count = 0;
            for (const auto& otherVehicle : data) {
                if (otherVehicle.CleanAlternativeFuelVehicleEligibility == "Clean Alternative Fuel Vehicle Eligible" &&
                    otherVehicle.Make == make && otherVehicle.Model == model) {
                    ++count;
                }
            }

            // Update maxCount and maxModel if necessary
            if (count > maxCount) {
                maxCount = count;
                maxModel = model;
            }
        }
    }

    return maxModel;
}

int main() {

    LoadDataFromCSVFile();

    //Prompting for user's input:
    std::string make, model;
    std::cout << "Please enter a make for a vehicle: ";
    std::getline(std::cin, make);
    std::cout << "Please enter a model for a vehicle: ";
    std::getline(std::cin, model);


    //Calculates the average electric range:
    double averageElectricRange = avgElectricRange(make, model);
    std::cout << "Average electric range for make: " << make << " and model: " << model << " is: " << averageElectricRange << std::endl;
    std::cout << comparingAvgWithHighest(averageElectricRange) << " the highest average." << std::endl;

    //Prints the make and model combination with the highest average electric range:
    MakeModel maxMakeModel = detMakeModelWithHighestAvg();
    double highestAverage = avgElectricRange(maxMakeModel.Make, maxMakeModel.Model);
    std::cout << "Make with highest average electric range: " << maxMakeModel.Make << ", Model: " << maxMakeModel.Model << ", and its average electric range is: " << highestAverage << std::endl;


    // Determines the electric vehicle type
    std::string electricVehicleType = detElectricVehicleType(make, model);
    std::cout << "The electric vehicle type is: " << electricVehicleType << std::endl;


    //Determines the number of CAFVS based on the electric vehicle type:
    int bevCount = CAFVSByType("Battery Electric Vehicle (BEV)");
    int phevCount = CAFVSByType("Plug-in Hybrid Electric Vehicle (PHEV)");

    //Prints the number of CAFVS based on the electric vehicle type:
    std::cout << "The number of Clean Alternative Fuel Vehicles for Battery Electric Vehicles (BEV): " << bevCount << std::endl;
    std::cout << "The number of Clean Alternative Fuel Vehicles for Plug-in Hybrid Electric Vehicles (PHEV): " << phevCount << std::endl;


    //Conditional to output which electric vehicle type prodcues more CAFVS:
    if (bevCount > phevCount) {
        std::cout << "Battery Electric Vehicles (BEV) produce more Clean Alternative Fuel Vehicles." << std::endl;
    }
    else if (bevCount < phevCount) {
        std::cout << "Plug-in Hybrid Electric Vehicles (PHEV) produce more Clean Alternative Fuel Vehicles." << std::endl;
    }
    else {
        std::cout << "Both Battery Electric Vehicles (BEV) and Plug-in Hybrid Electric Vehicles (PHEV) produce the same number of Clean Alternative Fuel Vehicles." << std::endl;
    }

    int modelYear = detModelYear();
    //Prints the number of CAFVS based on the model year:
    int cleanFuelVehiclesCountByYear = CAFVSByYear(modelYear);
    std::cout << "The number of clean alternative fuel vehicles for model year " << modelYear << " is: " << cleanFuelVehiclesCountByYear << std::endl;

    //Prints the model year with the most CAFVS:
    int maxYear = detModelYearWithMaxCAFVS();
    std::cout << "The model year with the largest production of clean alternative fuel vehicles is: " << maxYear << std::endl;

    //Prints the value of the CAFVS based on the model year with the most CAFVS:
    int maxYearCount = CAFVSByYear(maxYear);
    std::cout << "The number of clean alternative fuel vehicles for the maximum model year " << maxYear << " is: " << maxYearCount << std::endl;


    //Prints the make and model with the largest number of CAFVS:
    std::string makeWithMostCAFVS = detMakeWithMostCAFVS();
    std::string modelWithMostCAFVs = detModelWithMostCAFVS(makeWithMostCAFVS);
    std::cout << "The make with the highest number of Clean Alternative Fuel Vehicles is: " << makeWithMostCAFVS << std::endl;
    std::cout << "The model associated with this make is: " << modelWithMostCAFVs << std::endl;

    return 0;

}