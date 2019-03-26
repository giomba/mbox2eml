/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Basic usage:
 *  ./this-bin < SomeMboxFile
 * creates one eml file for every email contained in SomeMboxFile
 */

/* Include files for external symbols in STL */
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <regex>

/* Commonly used const strings */
const std::string FILENAME_EXTENSION = ".eml";
const std::string FILENAME_PREFIX = "mbox2eml-out-";

const std::regex SEPARATOR_R = std::regex("^From .*@.*\\..*( ){1,}[A-Z][a-z]{2} [A-Z][a-z]{2} \\d{1,2} \\d{1,2}:\\d{1,2}:\\d{1,2} \\d{4}$");
const std::string SUBJECT_STRING = "Subject: ";

/* FSM States */
enum Status { ST_SEARCH, ST_SPONGE, ST_END };

/* Functions declarations */
void save(const std::string, const std::string&);

int main(int argc, char** argv) {
  Status status = ST_SEARCH;
  std::string line;
  std::string sponge;
  std::string subject;

  std::string filename;
  int index = 1;

  while (status != ST_END) {
    switch (status) {
      case ST_SEARCH:
        std::getline(std::cin, line);
        if (! std::cin.good()) {
          status = ST_END;
          break;
        }
        // if (line.substr(0, FROM_STRING.length()).compare(FROM_STRING) == 0) {
        if (std::regex_match(line, SEPARATOR_R)) {
          status = ST_SPONGE;
        }
        break;
      case ST_SPONGE:
        sponge = sponge + line + '\n';
        std::getline(std::cin, line);
        if (line.substr(0, SUBJECT_STRING.length()).compare(SUBJECT_STRING) == 0) {
            subject = line.substr(SUBJECT_STRING.length());
        }
        if (std::regex_match(line, SEPARATOR_R) || ! std::cin.good()) {
          filename = FILENAME_PREFIX + std::string(4 - (int)log10(index), '0') + std::to_string(index) + '-' + subject + FILENAME_EXTENSION;
          save(filename, sponge);
          ++index;
          subject.clear();
          sponge.clear();
          if (! std::cin.good()) status = ST_END;
        }
        break;
      case ST_END:
        break;
    }
  }

  return 0;
}

void save(const std::string filename, const std::string& content) {
  std::fstream file;
  std::string name = filename;

  std::replace(name.begin(), name.end(), '/', '-');

  // std::clog << "Writing " << name << std::endl;

  file.open(name, std::ios::out);
  file << content << std::endl;
  file.close();
}
