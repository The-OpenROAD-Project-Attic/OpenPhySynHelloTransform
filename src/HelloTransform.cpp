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

int
HelloTransform::addWire(Database* db_, std::string name)
{

    LibrarySet         libs = db_->getLibs();
    Library*           lib;
    LibraryTechnology* tech;

    if (!libs.size())
    {
        tech = LibraryTechnology::create(db_);
        libs = db_->getLibs();
    }
    else
    {
        lib  = *(libs.begin());
        tech = lib->getTech();
    }

    if (!tech)
    {
        tech = LibraryTechnology::create(db_);
    }

    Chip* chip = db_->getChip();
    if (!chip)
    {
        chip = Chip::create(db_);
    }

    Block* block = chip->getBlock();
    if (!block)
    {
        block = Block::create(chip, "top");
    }
    Net* n1 = Net::create(block, name.c_str());
    return (n1 != nullptr);
}

int
HelloTransform::run(Phy* phy_inst, Database* db, std::vector<std::string> args)
{

    PhyLogger::instance().debug("Passed arguments:");
    for (auto& arg : args)
    {
        PhyLogger::instance().debug("{}", arg);
    }

    if (args.size() == 1)
    {
        std::string net_name = args[0];
        PhyLogger::instance().info("Adding random wire {}", net_name);
        return addWire(db, net_name);
    }
    else
    {
        PhyLogger::instance().error("Usage:\n transform hello_transform "
                                    "<net_name>\n");
    }

    return -1;
}
