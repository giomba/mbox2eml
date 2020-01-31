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

#include <csignal>
#include <unistd.h>

/* Commonly used const */
const std::string FILENAME_EXTENSION = ".eml";
const std::string FILENAME_PREFIX = "mbox2eml-out-";

const std::regex SEPARATOR_R = std::regex("^From .*@.*\\..*( ){1,}[A-Z][a-z]{2} [A-Z][a-z]{2} \\d{1,2} \\d{1,2}:\\d{1,2}:\\d{1,2} \\d{4}$");
const std::string SUBJECT_STRING = "Subject: ";

const int RESERVED_MAIL_SIZE = 32 * (1 << 20);  // 32 MiB

/* FSM States */
enum Status { ST_SEARCH, ST_SPONGE, ST_END };

/* Global variables */
static unsigned long readbytes = 0;

/* Functions declarations */
void save(const std::string, const std::string&);
void alarm_handler(int);

int main(int argc, char** argv) {
  /* variables declaration */
  Status status = ST_SEARCH;
  std::string line;
  std::string sponge;
    sponge.reserve(RESERVED_MAIL_SIZE);
  std::string subject;

  std::string filename;
  int index = 1;

  /**/
  signal(SIGALRM, alarm_handler);
  alarm(1);

  while (status != ST_END) {
    switch (status) {
      /* Status Search: search for starting point of next email... */
      case ST_SEARCH:
        std::getline(std::cin, line);
        if (! std::cin.good()) {
          status = ST_END;
          break;
        }

        /* if starting point is found, ... */
        if (std::regex_match(line, SEPARATOR_R)) {
          status = ST_SPONGE;
        }
        break;
      /* Status Sponge: suck the next email like a sponge, up to the next separator */
      case ST_SPONGE:
        readbytes += line.length();
        sponge.append(line);
        sponge.append("\n");
        std::getline(std::cin, line);
        /* detect mail subject, in order to name the .eml file after it */
        if (line.substr(0, SUBJECT_STRING.length()).compare(SUBJECT_STRING) == 0) {
            subject = line.substr(SUBJECT_STRING.length());
        }
        /* detect new separator, save, then pass to next email */
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

  file.open(name, std::ios::out);
  file << content << std::endl;
  file.close();
}

/* print current number of bytes parsed */
void alarm_handler(int signum) {
  unsigned long size = readbytes / (1 << 10);
  char unit = 'K';

  std::clog << "read " << size << unit << std::endl;

  alarm(2);
}
