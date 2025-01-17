/*
 * Stats.cpp
 *
 *  Created on: Nov 6, 2015
 *      Author: nick
 */
#include <cmath>

#include "boost/filesystem.hpp"

#include "Stats.h"
#include "file_utils.h"

namespace TransModel {

const std::string ARTEvent::header("\"tick\",\"p_id\",\"event_type\"");

void ARTEvent::writeTo(FileOutput& out) {
    out << tick << "," << p_id << "," << (int)type << "\n";
}

const std::string PREPEvent::header("\"tick\",\"p_id\",\"event_type\"");

void PREPEvent::writeTo(FileOutput& out) {
    out << tick << "," << p_id << "," << type << "\n";
}

const std::string TestingEvent::header("\"tick\",\"p_id\",\"result\"");

void TestingEvent::writeTo(FileOutput& out) {
    out << tick << "," << p_id << "," << result << "\n";
}

const std::string DeathEvent::header("\"tick\",\"p_id\",\"age\",\"art_status\",\"cause\"");
const std::string DeathEvent::AGE("AGE");
const std::string DeathEvent::INFECTION("INFECTION");
const std::string DeathEvent::ASM("ASM");
const std::string DeathEvent::ASM_CD4("ASM+CD4");

void DeathEvent::writeTo(FileOutput& out) {
    out << tick << "," << p_id << "," << age << "," << art_status << "," << cause << "\n";
}

const std::string Biomarker::header("\"tick\",\"p_id\",\"viral_load\",\"cd4_count\",\"art_status\"");

void Biomarker::writeTo(FileOutput& out) {
    out << tick << "," << p_id << "," << viral_load << "," << cd4 << "," << on_art << "\n";
}

const std::string InfectionEvent::header(
        "\"tick\",\"infector\",\"p1_age\",\"p1_viral_load\",\"p1_cd4\",\"p1_art_status\",\"p1_on_prep\",\"p1_infectivity\","
                "\"condom_used\",\"infectee\",\"p2_age\",\"p2_viral_load\",\"p2_cd4\",\"p1_on_prep\",\"network_type\"");

void InfectionEvent::writeTo(FileOutput& out) {
    out << tick << "," << p1_id << "," << p1_age << "," << p1_viral_load << "," << p1_cd4 << "," << p1_art << ","
            << p1_on_prep << "," << p1_infectivity << "," << condom_used << "," << p2_id << "," << p2_age << ","
            << p2_viral_load << "," << p2_cd4 << "," << p2_on_prep << "," << network_type << "\n";
}

const std::string PartnershipEvent::header("\"tick\",\"edge_id\",\"p1\",\"p2\",\"type\",\"network_type\"");

PartnershipEvent::PartnershipEvent(double tick, unsigned int edge_id, int p1, int p2, PEventType type, int net_type) :
        tick_(tick), edge_id_(edge_id), p1_id(p1), p2_id(p2), type_ { type }, network_type { net_type } {
}

void PartnershipEvent::writeTo(FileOutput& out) {
    out << tick_ << "," << edge_id_ << "," << p1_id << "," << p2_id << "," << static_cast<int>(type_) << "," << network_type << "\n";
}

const std::string Counts::header(
        "tick,entries,max_age_exits,infection_deaths,asm_deaths,"

        "infected_via_transmission,infected_via_transmission_18,infected_via_transmission_19,infected_via_transmission_20,"
        "infected_via_transmission_21,infected_via_transmission_22,infected_via_transmission_23,infected_via_transmission_24,infected_via_transmission_25,"
        "infected_via_transmission_26,infected_via_transmission_27,infected_via_transmission_28,infected_via_transmission_29,infected_via_transmission_30,"
        "infected_via_transmission_31,infected_via_transmission_32,infected_via_transmission_33,infected_via_transmission_34,"

        "infected_externally,infected_external_18,infected_external_19,infected_external_20,"
        "infected_external_21,infected_external_22,infected_external_23,infected_external_24,infected_external_25,"
        "infected_external_26,infected_external_27,infected_external_28,infected_external_29,infected_external_30,"
        "infected_external_31,infected_external_32,infected_external_33,infected_external_34,"

        "infected_at_entry,infected_at_entry_18,infected_at_entry_19,infected_at_entry_20,"
        "infected_at_entry_21,infected_at_entry_22,infected_at_entry_23,infected_at_entry_24,infected_at_entry_25,"
        "infected_at_entry_26,infected_at_entry_27,infected_at_entry_28,infected_at_entry_29,infected_at_entry_30,"
        "infected_at_entry_31,infected_at_entry_32,infected_at_entry_33,infected_at_entry_34,"

        "uninfected,uninfected_18,uninfected_19,uninfected_20,"
        "uninfected_21,uninfected_22,uninfected_23,uninfected_24,uninfected_25,"
        "uninfected_26,uninfected_27,uninfected_28,uninfected_29,uninfected_30,"
        "uninfected_31,uninfected_32,uninfected_33,uninfected_34,"

        "steady_edge_count,casual_edge_count,"

        "vertex_count,vertex_count_18,vertex_count_19,vertex_count_20,"
        "vertex_count_21,vertex_count_22,vertex_count_23,vertex_count_24,vertex_count_25,"
        "vertex_count_26,vertex_count_27,vertex_count_28,vertex_count_29,vertex_count_30,"
        "vertex_count_31,vertex_count_32,vertex_count_33,vertex_count_34,"

        "overlaps,sex_acts,casual_sex_acts,"
        "sd_casual_sex_with_condom,""sd_casual_sex_without_condom,"
        "sc_casual_sex_with_condom,""sc_casual_sex_without_condom,"
        "steady_sex_acts,"
        "sd_steady_sex_with_condom,sd_steady_sex_without_condom,"
        "sc_steady_sex_with_condom,sc_steady_sex_without_condom,"
        "on_art,on_prep,vl_supp_per_positives,vl_supp_per_diagnosed,cd4m_deaths");

void write_vector_out(std::vector<unsigned int>& vec, FileOutput& out) {
    out << "," << std::accumulate(vec.begin(), vec.end(), 0);
    for (auto i : vec) {
        out << "," << i;
    }
}

void Counts::writeTo(FileOutput& out) {
    out << tick << "," << entries << "," << age_deaths << "," << infection_deaths << "," << asm_deaths;
    write_vector_out(internal_infected, out);
    write_vector_out(external_infected, out);
    write_vector_out(infected_at_entry, out);
    write_vector_out(uninfected, out);
    out << "," << main_edge_count << "," << casual_edge_count;
    write_vector_out(vertex_count, out);
    out << "," << overlaps << "," << sex_acts << "," << casual_sex_acts << ","
    << sd_casual_sex_with_condom << "," << sd_casual_sex_without_condom << ","
    << sc_casual_sex_with_condom << "," << sc_casual_sex_without_condom << ","
    << steady_sex_acts << ","
    << sd_steady_sex_with_condom << "," << sd_steady_sex_without_condom << ","
    << sc_steady_sex_with_condom << "," << sc_steady_sex_without_condom << ","
    << on_art << "," << on_prep << "," << vl_supp_per_positives << "," << vl_supp_per_diagnosis << "," << cd4m_deaths << "\n";
}

Counts::Counts(int min_age, int max_age) :
        tick { 0 }, main_edge_count { 0 }, casual_edge_count { 0 }, entries { 0 }, age_deaths { 0 }, infection_deaths { 0 }, asm_deaths{0}, overlaps { 0 }, sex_acts { 0 },
                casual_sex_acts{0}, steady_sex_acts{0}, sd_casual_sex_with_condom{0}, sd_casual_sex_without_condom{0},
                sd_steady_sex_with_condom{0}, sd_steady_sex_without_condom {0},
                sc_casual_sex_with_condom{0}, sc_casual_sex_without_condom{0},
                sc_steady_sex_with_condom{0}, sc_steady_sex_without_condom {0},
                on_art{0}, on_prep{0}, uninfected(1 + max_age - min_age, 0), internal_infected(1 + max_age - min_age, 0), external_infected(1 + max_age - min_age, 0),
                infected_at_entry(1 + max_age - min_age, 0), vertex_count(1 + max_age - min_age, 0), min_age_(min_age),
                vl_supp_per_positives{0}, vl_supp_per_diagnosis{0}, cd4m_deaths{0}

{
}

void Counts::reset() {
    tick = 0;
    main_edge_count = casual_edge_count = entries = age_deaths = infection_deaths = asm_deaths = cd4m_deaths = 0;
    sex_acts = 0, casual_sex_acts = 0, steady_sex_acts = 0;
    sd_casual_sex_with_condom = sd_casual_sex_without_condom = 0;
    sd_steady_sex_with_condom = sd_steady_sex_without_condom = 0;
    sc_casual_sex_with_condom = sc_casual_sex_without_condom = 0;
    sc_steady_sex_with_condom = sc_steady_sex_without_condom = 0;
    on_art = on_prep = 0;
    vl_supp_per_positives = vl_supp_per_diagnosis = 0;
    std::fill(uninfected.begin(), uninfected.end(), 0);
    std::fill(internal_infected.begin(), internal_infected.end(), 0);
    std::fill(external_infected.begin(), external_infected.end(), 0);
    std::fill(infected_at_entry.begin(), infected_at_entry.end(), 0);
    std::fill(vertex_count.begin(), vertex_count.end(), 0);
    overlaps = 0;
}


void Counts::incrementInfected(PersonPtr& p) {
    ++internal_infected[(size_t)(std::floor(p->age())) - min_age_];
}

void Counts::incrementInfectedAtEntry(PersonPtr& p) {
    ++infected_at_entry[(size_t)(std::floor(p->age())) - min_age_];
}

void Counts::incrementInfectedExternal(PersonPtr& p) {
    ++external_infected[(size_t)(std::floor(p->age())) - min_age_];
}

void Counts::incrementUninfected(PersonPtr& p) {
    ++uninfected[(size_t)(std::floor(p->age())) - min_age_];
}

void Counts::incrementVertexCount(PersonPtr p) {
    ++vertex_count[(size_t)(std::floor(p->age())) - min_age_];
}

Stats* Stats::instance_ = nullptr;

Stats::Stats(std::shared_ptr<StatsWriterI<Counts>> counts, std::shared_ptr<StatsWriterI<PartnershipEvent>> pevents,
        std::shared_ptr<StatsWriterI<InfectionEvent>> infection_event_writer, std::shared_ptr<StatsWriterI<Biomarker>> bio_writer,
        std::shared_ptr<StatsWriterI<DeathEvent>> death_event_writer, const std::string& person_data_fname,
        std::shared_ptr<StatsWriterI<TestingEvent>> testing_event_writer, std::shared_ptr<StatsWriterI<ARTEvent>> art_writer,
        std::shared_ptr<StatsWriterI<PREPEvent>> prep_writer, int min_age, int max_age) :
        counts_writer { counts }, current_counts {min_age, max_age}, pevent_writer { pevents }, ievent_writer { infection_event_writer }, biomarker_writer {
                bio_writer }, death_writer { death_event_writer }, tevent_writer{testing_event_writer}, art_event_writer {art_writer}, prep_event_writer{prep_writer},
                pd_recorder{nullptr} {

    if (person_data_fname == "") {
        pd_recorder = std::make_shared<NullPersonDataRecorder>();
    } else {
        pd_recorder = std::make_shared<PersonDataRecorder>(person_data_fname, 1000);
    }
}

Stats::~Stats() {
}

void Stats::resetForNextTimeStep() {
    counts_writer->addOutput(current_counts);
    current_counts.reset();
}

void Stats::recordARTEvent(double time, int p_id, bool onART) {
    art_event_writer->addOutput(ARTEvent{time, p_id, onART});
}

void Stats::recordPREPEvent(double time, int p_id, int type) {
    prep_event_writer->addOutput(PREPEvent{time, p_id, type});
}

void Stats::recordPartnershipEvent(double t, unsigned int edge_id, int p1, int p2, PartnershipEvent::PEventType event_type, int net_type) {
    pevent_writer->addOutput(PartnershipEvent { t, edge_id, p1, p2, event_type, net_type });
}

void Stats::recordTestingEvent(double time, int p_id, bool result) {
    tevent_writer->addOutput(TestingEvent{time, p_id, result});
}

void Stats::recordInfectionEvent(double time, const PersonPtr& p) {
    InfectionEvent evt;
    evt.tick = time;
    evt.p1_id = p->id();
    evt.p1_age = p->age();
    evt.p1_art = p->infectionParameters().art_status;
    evt.p1_cd4 = p->infectionParameters().cd4_count;
    evt.p1_infectivity = p->infectivity();
    evt.p1_viral_load = p->infectionParameters().viral_load;
    evt.p1_on_prep = p->isOnPrep();
    evt.p2_id = -1;
    evt.p2_age = 0;
    evt.p2_cd4 = 0;
    evt.p2_viral_load = 0;
    evt.p2_on_prep = false;
    evt.condom_used = false;
    evt.network_type = -1;
    ievent_writer->addOutput(evt);
}

void Stats::recordInfectionEvent(double time, const PersonPtr& p1, const PersonPtr& p2, bool condom, int net_type) {
    InfectionEvent evt;
    evt.tick = time;
    evt.p1_id = p1->id();
    evt.p1_age = p1->age();
    evt.p1_art = p1->infectionParameters().art_status;
    evt.p1_cd4 = p1->infectionParameters().cd4_count;
    evt.p1_infectivity = p1->infectivity();
    evt.p1_viral_load = p1->infectionParameters().viral_load;
    evt.p1_on_prep = p1->isOnPrep();
    evt.p2_id = p2->id();
    evt.p2_age = p2->age();
    evt.p2_cd4 = p2->infectionParameters().cd4_count;
    evt.p2_viral_load = p2->infectionParameters().viral_load;
    evt.p2_on_prep = p2->isOnPrep();
    evt.condom_used = condom;
    evt.network_type = net_type;
    ievent_writer->addOutput(evt);
}

void Stats::recordBiomarker(double time, const PersonPtr& person) {
    Biomarker marker;
    marker.tick = time;
    marker.cd4 = person->infectionParameters().cd4_count;
    marker.on_art = person->infectionParameters().art_status;
    marker.p_id = person->id();
    marker.viral_load = person->infectionParameters().viral_load;
    biomarker_writer->addOutput(marker);
}

void Stats::recordDeathEvent(double time, const PersonPtr& person, const std::string& cause) {
    DeathEvent event;
    event.tick = time;
    event.age = person->age();
    event.p_id = person->id();
    event.art_status = person->infectionParameters().art_status;
    event.cause = cause;
    death_writer->addOutput(event);
}

} /* namespace TransModel */
