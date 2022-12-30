#include <iostream>
#include <string>
#include <chrono>
#include "chap8_nouveautesStd.h"

void nouveautesstd() {
  // p77_conversion();
  p78_chronos();
}

// conversion d'un type à l'autre
void p77_conversion() {
  double d = 2.34;
  std::string s = std::to_string(d);
  s = std::stod(s); // string to double
  std::cout << d << std::endl;
}

// date et heure
void p78_chronos() {
  using namespace std::chrono;
  time_point<system_clock> maintenant = system_clock::now();
  time_point<system_clock> demain_soir = maintenant + hours(26) + minutes(5);
  time_point<system_clock> demain_soir2 = maintenant + 26h + 5min;  // pareil qu'au dessus

  duration<double, std::milli> attend = demain_soir - maintenant;
  std::cout << attend.count() << std::endl;

  // bilan:
  // timepoint - timepoint = duration
  // timepoint - duration = timepoint
  // timepoint + duration = timepoint
  // duration - duration = duration
  // duration + duration = duration
  // timepoint + timepoint = !! impossible !!
}