/**
 * @file Petrosian/PetrosianRadius/PetrosianRadiusTaskFactory.h
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

#ifndef _PLUGIN_PETROSIANRADIUS_PETROSIANRADIUSTASKFACTORY_H
#define _PLUGIN_PETROSIANRADIUS_PETROSIANRADIUSTASKFACTORY_H

#include <SEFramework/Task/TaskFactory.h>

namespace Petrosian {

/**
 * @class AvgApertureTaskFactory
 * @brief
 *
 */
class PetrosianRadiusTaskFactory: public SourceXtractor::TaskFactory {

public:

  /**
   * @brief Destructor
   */
  virtual ~PetrosianRadiusTaskFactory() = default;

  std::shared_ptr<SourceXtractor::Task> createTask(const SourceXtractor::PropertyId& property_id) const override;

  void reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const override;

  void configure(Euclid::Configuration::ConfigManager& manager) override;

private:
  double m_eta, m_factor, m_minrad;

};  // End of PetrosianRadiusTaskFactory class

}  // namespace Petrosian


#endif
