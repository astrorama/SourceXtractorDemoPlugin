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

#ifndef _PETROSIAN_PETROSIANCONFIG_H
#define _PETROSIAN_PETROSIANCONFIG_H

#include <Configuration/Configuration.h>
#include <boost/filesystem/path.hpp>

namespace Petrosian {

/**
 * @class PetrosianConfig
 * @details
 *  This class handles the configuration of the Petrosian tasks. If you have
 *  multiple tasks that do conceptually different things, you may want to have
 *  separated configurations too.
 *
 *  The configuration procedure has three steps: pre-initialization, initialization and post-initialization.
 *  We only implement the initialization step here. For more details, look at the documentation of
 *  Euclid::Configuration::Configuration
 *
 * @see
 *  https://sextractor.readthedocs.io/en/latest/Photom.html#petrosian-aperture-flux-flux-petro
 */
class PetrosianConfig : public Euclid::Configuration::Configuration {
public:

  /**
   * Constructor
   * @param manager_id
   *    An identifier of the configuration manager that is creating the object.
   *    Just needed to be passed to Configuration
   */
  PetrosianConfig(long manager_id);

  /**
   * Default destructor
   */
  ~PetrosianConfig() = default;

  /**
   * @brief
   *    Lets the Manager know which options are handled by this class
   * @return
   *    A map, where the key is used to group sets of options, and the value is
   *    a vector of boost program option
   */
  std::map<std::string, OptionDescriptionList> getProgramOptions() override;

  /**
   * @brief
   *    This is the main configuration method: the processing of the expected options is to be done here
   * @param args
   *    The user parameters
   */
  void initialize(const UserValues& args) override;

  /**
   * Getter for the η parameter.
   */
  double getEta() const;

  /**
   * Getter for the Petrosian factor \f$N_{\rm P}\f$
   */
  double getFactor() const;

  /**
   * Getter for the minimum radius
   */
  double getMinRadius() const;

  /**
   * Getter for the configured check image
   */
  boost::filesystem::path getCheckImagePath() const;

private:
  double m_eta, m_factor, m_minrad;
  boost::filesystem::path m_checkimage;
};

} // namespace Petrosian

#endif
