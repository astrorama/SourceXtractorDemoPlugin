/**
 * @file Petrosian/PetrosianConfig.h
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

#ifndef _PLUGIN_PETROSIANCONFIG_H
#define _PLUGIN_PETROSIANCONFIG_H

#include <Configuration/Configuration.h>

namespace Petrosian {

class PetrosianConfig : public Euclid::Configuration::Configuration {
public:

  PetrosianConfig(long manager_id);

  ~PetrosianConfig() = default;

  std::map<std::string, OptionDescriptionList> getProgramOptions() override;

  void initialize(const UserValues& args) override;

  double getEta() const;

  double getFactor() const;

  double getMinRadius() const;

  std::string getCheckImagePath() const;

private:
  double m_eta, m_factor, m_minrad;
  std::string m_checkimage;
};

} // namespace Petrosian

#endif
