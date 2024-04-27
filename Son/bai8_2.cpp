#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath> 
#include "lib/nlohmann/json.hpp"
#include <random>
using json = nlohmann::json;

class LeavingDistribution {
public:
    LeavingDistribution(const std::string& filename, std::map<char,int>& total_value) { //Constructer
        readDataFromFile(filename);
        this->total_value = total_value;
        distributionWalking();
    }

    int getWalkingAtTime(char locationKey, int time) { //Hàm lấy giá trị 
        if (walkingAtTime.count(locationKey) == 0 || totals[locationKey].count(time) == 0) {
            std::cerr << "Location or time not found" << std::endl;
            return 0;
        }
        return walkingAtTime[locationKey][time];
    }

private:
    json data;
    
    std::map<char,int> total_value; //so nguoi xuat phat tu khoa (key)
    std::map <char,std::map<int,int>> walkingAtTime;    //so nguoi di ra trong time tuong ung (key,time,numWalk) 
                                                        //time la khoang thoi gian (time,time+timestep)
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
        numSamples = (end - start +1) / timeStep;
        standardDeviation = distribution["std_dev"];
    }

    void distributionWalking() { 
        for (const auto& location : data["leavingDistribution"]["distribution"].items()) {
            char locationKey = location.key();
            int numSamples, standardDeviation, start, end, timeStep;
            calculateStatistics(location.value()["normal"], numSamples, standardDeviation, start, end, timeStep);
            double mean = (start + end) / 2.0;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<double> distribution(mean, standardDeviation);
            for (int i=0;i<total_value[locationKey];++i) { // duyet so nguoi den khoa 
                double random_value = distribution(gen);
                random_value = max(min(random_value,end),start);    // dam bao random trong gia tri
                if (int(random_value-start)%2 == 0) walkingAtTime[locationKey][int(random_value)]++;
                else walkingAtTime[locationKey][int(random_value)-1]++;
            }
        }
    }
};