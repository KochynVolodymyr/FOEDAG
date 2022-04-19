/*
Copyright 2021 The Foedag team

GPL License

Copyright (c) 2021 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Compiler/Design.h"

#include "Compiler/Constraints.h"

using namespace FOEDAG;

std::string Design::toString(Language lang) {
  switch (lang) {
    case VHDL_1987:
      return "VHDL_1987";
    case VHDL_1993:
      return "VHDL_1993";
    case VHDL_2008:
      return "VHDL_2008";
    case VERILOG_1995:
      return "VERILOG_1995";
    case VERILOG_2001:
      return "VERILOG_2001";
    case SYSTEMVERILOG_2005:
      return "SV_2005";
    case SYSTEMVERILOG_2009:
      return "SV_2009";
    case SYSTEMVERILOG_2012:
      return "SV_2012";
    case SYSTEMVERILOG_2017:
      return "SV_2017";
    case VHDL_2000:
      return "VHDL_2000";
  }
  return std::string{};
}

Design::~Design() {}

void Design::AddConstraintFile(const std::string& fileName) {
  m_constraintFileList.push_back(fileName);
  if (m_constraints == nullptr) m_constraints = new Constraints();
  m_constraints->addConstraint(fileName);
}
