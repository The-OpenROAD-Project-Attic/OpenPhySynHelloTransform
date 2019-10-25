// BSD 3-Clause License

// Copyright (c) 2019, SCALE Lab, Brown University
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "HelloTransform.hpp"
#include <PhyKnight/PhyLogger/PhyLogger.hpp>
#include <algorithm>
#include <cmath>

using namespace phy;

int HelloTransform::addWire(Database *db_, std::string name) {

  LibrarySet libs = db_->getLibs();
  Library *lib;
  LibraryTechnology *tech;

  if (!libs.size()) {
    tech = LibraryTechnology::create(db_);
    libs = db_->getLibs();
  } else {
    lib = *(libs.begin());
    tech = lib->getTech();
  }

  if (!tech) {
    tech = LibraryTechnology::create(db_);
  }

  Chip *chip = db_->getChip();
  if (!chip) {
    chip = Chip::create(db_);
  }

  Block *block = chip->getBlock();
  if (!block) {
    block = Block::create(chip, "top");
  }
  Net *n1 = Net::create(block, name.c_str());
  return (n1 != nullptr);
}

int HelloTransform::buffer(Phy *phy_inst, Database *db_, int max_fanout,
                           std::string buffer_cell, std::string buffer_in_port,
                           std::string buffer_out_port,
                           std::string clock_port_name) {
  PhyLogger &logger = PhyLogger::instance();
  DatabaseHelper helper = *(phy_inst->helper());
  LibraryCell *cell = helper.libraryCell(buffer_cell.c_str());
  if (!cell) {
    logger.error("Buffer {} not found!", buffer_cell);
    return -1;
  }
  LibraryTerm *cell_in_pin = helper.libraryPin(cell, buffer_in_port.c_str());
  LibraryTerm *cell_out_pin = helper.libraryPin(cell, buffer_out_port.c_str());
  if (!cell_in_pin) {
    logger.error("Pin {} not found!", buffer_in_port);
    return -1;
  }
  if (!cell_out_pin) {
    logger.error("Pin {} not found!", buffer_out_port);
    return -1;
  }
  auto nets = helper.nets();
  std::vector<Net *> high_fanout_nets;
  for (auto &net : nets) {
    if (!helper.isPrimary(net) &&
        helper.fanoutCount(net) > (unsigned int)max_fanout) {
      high_fanout_nets.push_back(net);
    }
  }
  logger.info("High fanout nets [{}]: ", high_fanout_nets.size());
  for (auto &net : high_fanout_nets) {
    logger.info("Net: {} {}", net->getConstName(), helper.fanoutCount(net));
  }
  // To-Do: Remove clock net..
  int buffer_index = 1;
  int net_in_index = 1;
  int net_out_index = 1;
  int added_buffer_count = 0;
  for (auto &net : high_fanout_nets) {
    InstanceTerm *source_pin = helper.faninPin(net);
    if (source_pin) {
      logger.info("Buffering: {}", net->getConstName());
      auto fanout_pins = helper.fanoutPins(net);
      logger.info("Sink count: {}", fanout_pins.size());

      int buffer_count = ceil(fanout_pins.size() / max_fanout);
      helper.disconnectAll(net);
      helper.del(net);
      Net *new_net_in = helper.createNet(
          std::string(std::string("bufnetin_") + std::to_string(net_in_index++))
              .c_str());
      helper.connect(new_net_in, source_pin);

      logger.info("Adding {} buffer", buffer_count);
      for (int i = 0; i < (int)fanout_pins.size(); i += max_fanout) {
        Instance *new_buffer = helper.createInstance(
            std::string(std::string("buf_") + std::to_string(buffer_index++))
                .c_str(),
            cell);
        Net *new_net_out =
            helper.createNet(std::string(std::string("bufnetout_") +
                                         std::to_string(net_out_index++))
                                 .c_str());
        helper.connect(new_net_in, new_buffer, cell_in_pin);
        helper.connect(new_net_out, new_buffer, cell_out_pin);
        added_buffer_count++;
        for (int j = i; j < i + max_fanout; j++) {
          if (j < (int)fanout_pins.size()) {
            helper.connect(new_net_out, fanout_pins[j]);
          }
        }
      }
    }
  }
  logger.info("Added {} buffer", added_buffer_count);

  return 1;
}

std::string HelloTransform::bufferName(int index) {
  return std::string("buf_" + std::to_string(index) + "_");
}

std::string HelloTransform::bufferNetName(int index) {
  return std::string("bufnet_" + std::to_string(index) + "_");
}

bool HelloTransform::isNumber(const std::string &s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) {
                         return !std::isdigit(c);
                       }) == s.end();
}

int HelloTransform::run(Phy *phy_inst, Database *db,
                        std::vector<std::string> args) {

  PhyLogger::instance().debug("Passed arguments:");
  for (auto &arg : args) {
    PhyLogger::instance().debug("{}", arg);
  }

  if (args.size() == 1) {
    std::string net_name = args[0];
    PhyLogger::instance().info("Adding random wire {}", net_name);
    return addWire(db, net_name);
  } else if (args.size() == 6 && args[0] == "buffer" && isNumber(args[1])) {
    return buffer(phy_inst, db, stoi(args[1]), args[2], args[3], args[4],
                  args[5]);
  } else {
    PhyLogger::instance().error("Usage:\n transform hello_transform "
                                "<net_name>\n transform hello_transform "
                                "buffer "
                                "<max_fanout> <buffer_cell> <buffer_in_port> "
                                "<buffer_out_port> <clock_port>");
  }

  return -1;
}
