/*
 * Jail.h
 *
 *  Created on: 1 Mar, 2019
 *      Author: Babak
 */

#ifndef SRC_JAIL_H_
#define SRC_JAIL_H_

#include "Rcpp.h"
#include "Person.h"

namespace TransModel
{

class Jail
{

  private:
    friend Person;
    //unsigned int capacity_;
    //std::vector<Person> jailed_pop;
    std::vector<PersonPtr> jailed_pop;

  public:
    Jail();
    Jail(int capacity);
    virtual ~Jail();

    void addPerson(PersonPtr person, double time, double serving_time);
    void releasePerson(PersonPtr person, double time);
    bool isServingTimeCompleted(PersonPtr person, double time);
    void checkAndReleaseTimeServedPopulation(double time);

    int populationSize();
    float avgPopulationAge();
    float avgPopulationServingTime();

    void printPopulationInfo(double time);
    void printPopulationID();
    void printPopulationServingTime();
};

} /* namespace TransModel */

#endif /* SRC_JAIL_H_ */
