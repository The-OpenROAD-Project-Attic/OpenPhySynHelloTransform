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

void HelloTransform::addWire(phy::Database *db_, std::string name) {

  phy::LibrarySet libs = db_->getLibs();
  phy::Library *lib;
  phy::LibraryTechnology *tech;

  if (!libs.size()) {
    tech = phy::LibraryTechnology::create(db_);
    libs = db_->getLibs();
  } else {
    lib = *(libs.begin());
    tech = lib->getTech();
  }

  if (!tech) {
    tech = phy::LibraryTechnology::create(db_);
  }

  phy::Chip *chip = db_->getChip();
  if (!chip) {
    chip = phy::Chip::create(db_);
  }

  phy::Block *block = chip->getBlock();
  if (!block) {
    block = phy::Block::create(chip, "top");
  }
  phy::Net *n1 = phy::Net::create(block, name.c_str());
}

int HelloTransform::run(phy::Phy *phy_, phy::Database *db_,
                        std::vector<std::string> args) {
  if (args.size() != 1) {
    phy::PhyLogger::instance().error(
        "Usage phy::transform hello_transform <net_name>");
    return -1;
  }
  phy::PhyLogger::instance().info("Hello Transform, arguments:");
  for (auto &arg : args) {
    phy::PhyLogger::instance().info("{}", arg);
  }
  std::string net_name = args[0];
  addWire(db_, net_name);
  return 1;
}
