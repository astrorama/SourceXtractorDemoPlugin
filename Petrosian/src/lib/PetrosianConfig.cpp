/**
 * @file src/lib/PetrosianConfig.cpp
 * @date 11/04/19
 * @author aalvarez
 *
 * @copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "Petrosian/PetrosianConfig.h"

using namespace Euclid::Configuration;
namespace po = boost::program_options;

namespace Petrosian {

static const char PETROSIAN_ETA[]{"petrosian-eta"};
static const char PETROSIAN_FACTOR[]{"pretrosian-factor"};
static const char PETROSIAN_MINRAD[]{"petrosian-minimum-radius"};
static const char PETROSIAN_CHECKIMAGE[]{"check-image-petrosian"};

PetrosianConfig::PetrosianConfig(long manager_id) : Configuration(manager_id) {}

std::map<std::string, Configuration::OptionDescriptionList> PetrosianConfig::getProgramOptions() {
  return {{"Petrosian photometry options",
            {
              {
                PETROSIAN_ETA, po::value<double>()->default_value(0.2),
                "Fraction of the isophote over the surface brightness For the Petrosian radius"
              },
              {
                PETROSIAN_FACTOR, po::value<double>()->default_value(2.0),
                "Scale factor for Petrosian photometry"
              },
              {
                PETROSIAN_MINRAD, po::value<double>()->default_value(3.5),
                "Minimum radius for Petrosian photometry"
              },
              {
                PETROSIAN_CHECKIMAGE, po::value<std::string>(),
                  "Check image for Petrosian apertures"
              }
            }
          }};
}

void PetrosianConfig::initialize(const UserValues& args) {
  m_eta = args.at(PETROSIAN_ETA).as<double>();
  m_factor = args.at(PETROSIAN_FACTOR).as<double>();
  m_minrad = args.at(PETROSIAN_MINRAD).as<double>();
  if (args.count(PETROSIAN_CHECKIMAGE)) {
    m_checkimage = args.at(PETROSIAN_CHECKIMAGE).as<std::string>();
  }
}

double PetrosianConfig::getEta() const {
  return m_eta;
}

double PetrosianConfig::getFactor() const {
  return m_factor;
}

double PetrosianConfig::getMinRadius() const {
  return m_minrad;
}

std::string PetrosianConfig::getCheckImagePath() const {
  return m_checkimage;
}

} // namespace Petrosian
