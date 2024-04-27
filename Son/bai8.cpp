#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath> 
#include "json.hpp"

using json = nlohmann::json;

class LeavingDistribution {
public:
    LeavingDistribution(const std::string& filename) { //Constructer
        readDataFromFile(filename);
        calculateTotals();
    }

    int getTotal(char locationKey, int time) { //Hàm lấy giá trị 
        if (totals.count(locationKey) == 0 || totals[locationKey].count(time) == 0) {
            std::cerr << "Location or time not found" << std::endl;
            return 0;
        }
        return totals[locationKey][time];
    }

private:
    json data;
    std::map<char, std::map<int, int>> totals; //Tạo map để lưu kết quả

    void readDataFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Can't open file" << std::endl;
            return;
        }

        file >> data;
        file.close();
    }

    //Đọc các thông số từ file json
    void calculateStatistics(const json& distribution, int& numSamples, int& standardDeviation, int& start, int& end, int& timeStep) const {
        start = distribution["start"];
        end = distribution["end"];
        timeStep = distribution["timeStep"];
        int mean = (start + end) / 2;

        numSamples = (end - start) / timeStep + 1;

        standardDeviation = distribution["std_dev"];
    }

    double normalDistribution(double x, double mean, double std_dev) const { //Trả về xác suất theo pp chuẩn
        return (1 / (std_dev * sqrt(2 * M_PI))) * exp(-0.5 * pow((x - mean) / std_dev, 2));
    }

    void calculateTotals() { //Tính tổng người ra ở 1 khoa tại 1 thời điểm
        for (const auto& location : data["leavingDistribution"]["distribution"].items()) {
            char locationKey = location.key();
            std::map<int, int> locationTotals;
            for (int time = 0; time <= 1000; ++time) { // Cho time chạy từ 0 đến 1000
                int numSamples, standardDeviation, start, end, timeStep;
                calculateStatistics(location.value()["normal"], numSamples, standardDeviation, start, end, timeStep);

                int mean = (start + end) / 2;
                int total = calculateNumberOfPeople(mean, standardDeviation, numSamples, start, end, timeStep);
                locationTotals[time] = total;
            }
            totals[locationKey] = locationTotals;
        }
    }
    //lấy xác suất  nhân số mẫu đển tìm số người ra
    int calculateNumberOfPeople(double mean, double std_dev, int num_samples, int start, int end, int timeStep) const { 
        int total = 0;
        for (int i = start; i <= end; i += timeStep) {
            double probability = normalDistribution(i, mean, std_dev);
            total += static_cast<int>(probability * num_samples);
        }
        return total;
    }
};

<<<<<<< HEAD

=======

