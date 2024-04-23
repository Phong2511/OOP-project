#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "newImplement/object/Pedestrian.cpp"

using namespace std;

int countYoungerThan12(const vector<Pedestrian>& pedestrians) {
    int count = 0;
    for (const auto& p : pedestrians) {
        if (p.age < 12) {
            count++;
        }
    }
    return count;
}

int countOlderThan60(const vector<Pedestrian>& pedestrians) {
    int count = 0;
    for (const auto& p : pedestrians) {
        if (p.age > 60) {
            count++;
        }
    }
    return count;
}

int countALKW(const vector<Pedestrian>& pedestrians) {
    int count = 0;
    for (const auto& p : pedestrians) {
        if (p.department == "A" || p.department == "L" || p.department == "K" || p.department == "W") {
            count++;
        }
    }
    return count;
}

int countBFGMEN(const vector<Pedestrian>& pedestrians) {
    int count = 0;
    for (const auto& p : pedestrians) {
        if (p.department == "B" || p.department == "F" || p.department == "G" || p.department == "M" || p.department == "E" || p.department == "N") {
            count++;
        }
    }
    return count;
}

vector<double> getImpact(int numOfSamples, double minValue, double maxValue) {
    vector<double> impact(numOfSamples);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(minValue, maxValue);
    for (int i = 0; i < numOfSamples; ++i) {
        impact[i] = dis(gen);
    }
    return impact;
}

vector<Pedestrian> assignImpact(vector<Pedestrian>& allPedestrians) {
    int nChildren = countYoungerThan12(allPedestrians);
    auto impactToChildren = getImpact(nChildren, 0.01, 0.99);

    int nPersonnelOfALKW = countALKW(allPedestrians);
    auto impactToALKW = getImpact(nPersonnelOfALKW, -0.29, 0.99);

    int nPersonnelOfBFGMEN = countBFGMEN(allPedestrians);
    auto impactToBFGMEN = getImpact(nPersonnelOfBFGMEN, -0.99, 0.29);
    for (auto& val : impactToBFGMEN) {
        val *= -1; // Reverse sign for negative impact
    }

    int nElder = countOlderThan60(allPedestrians);
    auto impactToElder = getImpact(nElder, -0.99, 0.29);
    for (auto& val : impactToElder) {
        val *= -1; // Reverse sign for negative impact
    }

    int nBlinder = 0; // Assuming we have a function to count blind pedestrians
    auto impactToBlinder = getImpact(nBlinder, -0.99, 0.29);
    for (auto& val : impactToBlinder) {
        val *= -1; // Reverse sign for negative impact
    }

    int nOthers = allPedestrians.size() - nBlinder - nElder - nPersonnelOfBFGMEN - nPersonnelOfALKW - nChildren;
    auto impactToOthers = getImpact(nOthers, -0.99, 0.99);

    for (auto& person : allPedestrians) {
        if (person.age < 12) {
            person.impactOfAGV = impactToChildren;
        } else if (person.department == "A" || person.department == "L" || person.department == "K" || person.department == "W") {
            person.impactOfAGV = impactToALKW;
        } else if (person.department == "B" || person.department == "F" || person.department == "G" || person.department == "M" || person.department == "E" || person.department == "N") {
            person.impactOfAGV = impactToBFGMEN;
        } else if (person.age > 60) {
            person.impactOfAGV = impactToElder;
        } else if (person.is_blind) {
            person.impactOfAGV = impactToBlinder;
        } else {
            person.impactOfAGV = impactToOthers;
        }
    }

    return allPedestrians;
}

// Function to check if majority of given group has positive impact from AGV
bool majorityPositiveImpact(const vector<Pedestrian>& allPedestrians) {
    pedestrians = assignImpact(allPedestrians);
    int childrenPositiveCount = 0;
    int ALKWPositiveCount = 0;

    for (const auto& p : pedestrians) {
        if (p.getAge() < 12) {
            for (const auto& val : p.getImpactOfAGV().getValues()) {
                if (val > 0) {
                    childrenPositiveCount++;
                }
            }
        } else if (p.getStart().getDepartment() == "A" || p.getStart().getDepartment() == "L" || p.getStart().getDepartment() == "K" || p.getStart().getDepartment() == "W") {
            for (const auto& val : p.getImpactOfAGV().getValues()) {
                if (val > 0) {
                    ALKWPositiveCount++;
                }
            }
        }
    }

    double childrenPositivePercentage = static_cast<double>(childrenPositiveCount) / countYoungerThan12(pedestrians);
    double ALKWPositivePercentage = static_cast<double>(ALKWPositiveCount) / countALKW(pedestrians);

    return (childrenPositivePercentage > 0.5) || (ALKWPositivePercentage > 0.5);
}

// Function to check if majority of given group has negative impact from AGV
bool majorityNegativeImpact(const vector<Pedestrian>& allPedestrians) {
    pedestrians = assignImpact(allPedestrians);
    int elderNegativeCount = 0;
    int blinderNegativeCount = 0;
    int BFGMENNegativeCount = 0;

    for (const auto& p : pedestrians) {
        if (p.getAge() > 60) {
            for (const auto& val : p.getImpactOfAGV().getValues()) {
                if (val < 0) {
                    elderNegativeCount++;
                }
            }
        } else if (p.getPersonality().getWalkability() == blind) {
            for (const auto& val : p.getImpactOfAGV().getValues()) {
                if (val < 0) {
                    blinderNegativeCount++;
                }
            }
        } else if (p.getStart().getDepartment() == "B" || p.getStart().getDepartment() == "F" || p.getStart().getDepartment() == "G" || p.getStart().getDepartment() == "M" || p.getStart().getDepartment() == "E" || p.getStart().getDepartment() == "N") {
            for (const auto& val : p.getImpactOfAGV().getValues()) {
                if (val < 0) {
                    BFGMENNegativeCount++;
                }
            }
        }
    }

    double elderNegativePercentage = static_cast<double>(elderNegativeCount) / countOlderThan60(pedestrians);
    double blinderNegativePercentage = static_cast<double>(blinderNegativeCount) / countBlinder(pedestrians);
    double BFGMENNegativePercentage = static_cast<double>(BFGMENNegativeCount) / countBFGMEN(pedestrians);

    return (elderNegativePercentage > 0.5) || (blinderNegativePercentage > 0.5) || (BFGMENNegativePercentage > 0.5);
}