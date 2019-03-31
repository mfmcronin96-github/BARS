/*
 * Jail.cpp
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#include "Jail.h"

namespace TransModel {

Jail::Jail() {
}

Jail::Jail(int capacity) {
    jailed_pop.reserve(capacity);
}

Jail::~Jail() {
}

/**
* Add a person to the jail (list) 
*/ 
void Jail::addPerson(PersonPtr person, double time, double serving_time) {
    //std::cout << "+Jail:JailPerson, time: " << time;
    //std::cout << ", serving_time: " << serving_time << std::endl;
    if(std::find(jailed_pop.begin(), jailed_pop.end(), person) != jailed_pop.end()) {
        std::cout << "*****ERROR in Jail::addPerson:***** Person is already in jail" << std::endl;
    } else {
        person->getInJail(time, serving_time);
        jailed_pop.push_back(person);
        /* @TODO: perhaps removing the network of a person should be done by here by jail*/
        //net.addVertex(person);
    }
}

/**
* Release (remove) a person from the jail (list) 
*/ 
void Jail::releasePerson(PersonPtr person, double time) {
    //std::cout << "-Jail:ReleasePerson(), time: " << time << std::endl;
    person->getOutJail(time);
    jailed_pop.erase(std::remove(jailed_pop.begin(), jailed_pop.end(), person), jailed_pop.end());
    /* @TODO: if removing network of a person previously done by jial, this sould be reinstated here */
    //net.addVertex(person);
}

/**
* Check (iterate over) the jail (list) and release (remove) a person from the jail 
*/ 
void Jail::checkAndReleaseTimeServedPopulation(double time) {
    //std::cout << "-Jail:checkAndReleaseIfTimeServed(), time: " << time << std::endl;
    for (auto& p : jailed_pop) {
        if (isServingTimeCompleted(p, time)) {
            releasePerson(p, time);
        }
    }
}

/**
* Boolean function to verify whether the jail serving time of a person is completed 
*/ 
bool Jail::isServingTimeCompleted(PersonPtr person, double current_time) {
    //std::cout << "-Jail:isServingTimeCompleted, time: " << current_time << std::endl;
    if ((current_time - person->timeOfJail()) < person->jailServingTime()) {
        return false;
    }
    else return true;
}

/**
* Gets the jail population size  
*/ 
int  Jail::populationSize() {
    return jailed_pop.size();
}

/**
* Gets the average of ages of the jailed population  
*/ 
float Jail::avgPopulationAge() {
    //std::cout << "Jail:avgPopulationAge()" << std::endl;
    float sumOfAges=0;
    for (auto& p : jailed_pop) {
        sumOfAges+= p->age();
    }
    float avgAges = sumOfAges/jailed_pop.size();
    return avgAges;
}

/**
* Gets the average of serving times of the jailed population  
*/ 
float Jail::avgPopulationServingTime() {
    //std::cout << "Jail:avgPopulationServingTime()" << std::endl;
    float sumOfSevingTimes=0;
    for (auto& p : jailed_pop) {
        sumOfSevingTimes+= p->jailServingTime();
    }
    return sumOfSevingTimes/jailed_pop.size();
}

/**
* Debugging helper function to print the key informaton of the jail population  
*/ 
void Jail::printPopulationInfo(double current_time) {
    std::cout << "======Jail::printPopulationInfo: ========" << std::endl;
    std::cout << "---IDs:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    
    std::cout << std::endl << "---Time of jail:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->timeOfJail() << ", ";
    
    std::cout << std::endl << "---Age at time of jail:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << p->ageAtJail() << ", ";

    std::cout << std::endl << "---Age:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->age() << ", ";
    
    std::cout << std::endl << "---Serving time:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->jailServingTime() << ", ";

    std::cout << std::endl << "---Time since jailed:" << std::endl;
    for (auto& p : jailed_pop)
        std::cout << (current_time - p->timeOfJail()) << ", ";

    std::cout << std::endl << "---isServingTimeCompleted:" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << isServingTimeCompleted(p,current_time) << ", ";

    std::cout << std::endl << "===========================================" << std::endl;
}

/**
* Debugging helper function to print the IDs of the jail population  
*/ 
void Jail::printPopulationID() {
    std::cout << "======Jail::printPopulationID: ========" << std::endl;
    //for(std::vector<T>::iterator it = jailed_pop.begin(); it != jailed_pop.end(); ++it) {
    for (auto& p : jailed_pop) 
        std::cout << p->id() << ", ";
    std::cout << std::endl << "===========================================" << std::endl;
}

/**
* Debugging helper function to print the jail serving times  of the jail population  
*/ 
void Jail::printPopulationServingTime() {
    std::cout << "======Jail::printPopulationServingTimes: =======" << std::endl;
    for (auto& p : jailed_pop) 
        std::cout << p->jailServingTime() << ", ";
    std::cout << std::endl << "==========================================" << std::endl;
}


} /* namespace TransModel */

