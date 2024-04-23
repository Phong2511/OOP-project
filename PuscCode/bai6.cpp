#include <iostream>
#include <vector>
#include <cmath>
#include "newImplement/object/Pedestrian.cpp"

// Assume Pedestrian class definition

// Function to calculate emotional impacts
std::vector<std::vector<double>> calculateEmotionalImpacts(Pedestrian p, int timeHorizon) {
    // Assume these functions are defined in the Pedestrian class
    std::vector<std::vector<double>> events = p.getEvents(); // Assuming it returns a 2D array of size 6x20
    double lambda = p.getLambda();
    std::vector<double> times = p.getTimeDistances();
    std::vector<std::vector<double>> allEmotions = {
        {p.personality.getPleasure()},
        {p.getSurprise()},
        {p.getAnger()},
        {p.getFear()},
        {p.getHate()},
        {p.getSad()}
    };

    double p, su, a, f, h, sa;
    p = events[0][0] + allEmotions[0][0] * exp(-lambda) + allEmotions[0][0];
    su = events[1][0] + allEmotions[1][0] * exp(-lambda) + allEmotions[1][0];
    a = events[2][0] + allEmotions[2][0] * exp(-lambda) + allEmotions[2][0];
    f = events[3][0] + allEmotions[3][0] * exp(-lambda) + allEmotions[3][0];
    h = events[4][0] + allEmotions[4][0] * exp(-lambda) + allEmotions[4][0];
    sa = events[5][0] + allEmotions[5][0] * exp(-lambda) + allEmotions[5][0];
    int lastTime = 0;
    int index = 0;
    allEmotions[0].push_back(p);
    allEmotions[1].push_back(su);
    allEmotions[2].push_back(a);
    allEmotions[3].push_back(f);
    allEmotions[4].push_back(h);
    allEmotions[5].push_back(sa);

    for (int i = 1; i < timeHorizon; ++i) {
        p = allEmotions[0][i];
        su = allEmotions[1][i];
        a = allEmotions[2][i];
        f = allEmotions[3][i];
        h = allEmotions[4][i];
        sa = allEmotions[5][i];

        if (i - lastTime == times[index]) {
            index += 1;
            lastTime = i;
            p += allEmotions[0][i] * exp(-lambda) + events[0][index];
            su += allEmotions[1][i] * exp(-lambda) + events[1][index];
            a += allEmotions[2][i] * exp(-lambda) + events[2][index];
            f += allEmotions[3][i] * exp(-lambda) + events[3][index];
            h += allEmotions[4][i] * exp(-lambda) + events[4][index];
            sa += allEmotions[5][i] * exp(-lambda) + events[5][index];
        }

        allEmotions[0].push_back(p);
        allEmotions[1].push_back(su);
        allEmotions[2].push_back(a);
        allEmotions[3].push_back(f);
        allEmotions[4].push_back(h);
        allEmotions[5].push_back(sa);
    }

    return allEmotions;
}
