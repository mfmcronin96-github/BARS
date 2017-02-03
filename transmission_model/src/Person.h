/*
 * Person.h
 *
 *  Created on: Oct 8, 2015
 *      Author: nick
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_

#include "Rcpp.h"
#include "DiseaseParameters.h"
#include "Diagnoser.h"
#include "GeometricDistribution.h"
#include "AdherenceCategory.h"
#include "PrepParameters.h"

namespace TransModel {

class PersonCreator;

class Person {

private:
	friend PersonCreator;

	int id_, role_;
	float age_;
	bool circum_status_;
	InfectionParameters infection_parameters_;
	float infectivity_;
	PrepParameters prep_;
	bool dead_, diagnosed_, testable_;
	Diagnoser<GeometricDistribution> diagnoser_;
	AdherenceData adherence_;

public:
	Person(int id, float age, bool circum_status, int role, Diagnoser<GeometricDistribution>& diagnoser);

	virtual ~Person();

	/**
	 * Gets the id of this person.
	 */

	int id() const {
		return id_;
	}

	int role() const {
		return role_;
	}

	/**
	 * Gets the age of this Person.
	 */
	float age() const {
		return age_;
	}

	bool isOnPrep() const {
		return prep_.status() == PrepStatus::ON;
	}

	const PrepStatus prepStatus() const {
		return prep_.status();
	}

	const InfectionParameters& infectionParameters() const {
		return infection_parameters_;
	}

	bool isCircumcised() const {
		return circum_status_;
	}

	bool isOnART() const {
		return infection_parameters_.art_status;
	}

	bool isInfected() const {
		return infection_parameters_.infection_status;
	}

	float infectivity() const {
		return infectivity_;
	}

	float timeSinceInfection() const {
		return infection_parameters_.time_since_infection;
	}

	const Diagnoser<GeometricDistribution> diagnoser() const {
		return diagnoser_;
	}

	void setAdherence(AdherenceData data) {
		adherence_ = data;
	}

	const AdherenceData adherence() const {
		return adherence_;
	}

	void setViralLoad(float viral_load);

	void setCD4Count(float cd4_count);

	void setViralLoadARTSlope(float slope);

	void setInfectivity(float infectivity);

	void setAge(float age);

	/**
	 * Puts this Person on ART with the specified time stamp,
	 * setting the art status to true.
	 */
	void goOnART(float time_stamp);

	/**
	 * Takes this person off ART, setting the art status to false.
	 */
	void goOffART();

	/**
	 * Takes this person off of PreP
	 */
	void goOffPrep();

	/**
	 * Puts this person on Prep
	 */
	void goOnPrep(double start_time, double end_time);

	/**
	 * Infects this Person and sets the duration of the infection,
	 * and the time of infection.
	 */
	void infect(float duration_of_infection, float time);

	/**
	 * Updates age, etc. of person., to be called each iteration of the model.
	 */
	void step(float size_of_timestep);

	/**
	 * Checks if person is dead of old age. This doesn't kill
	 * the person, it just checks.
	 */
	bool deadOfAge(int max_age);

	/**
	 * Checks if person is dead of AIDS. This doesn't kill
	 * the person, it just checks.
	 */
	bool deadOfInfection();

	void setDead(bool isDead) {
		dead_ = isDead;
	}

	bool isDead() const {
		return dead_;
	}

	bool isDiagnosed() const {
		return diagnosed_;
	}

	bool isTestable() const {
		return testable_;
	}

	const PrepParameters prepParameters() const {
		return prep_;
	}

	bool diagnose(double tick);

	double timeUntilNextTest(double tick) const;

};

} /* namespace TransModel */

#endif /* SRC_PERSON_H_ */
