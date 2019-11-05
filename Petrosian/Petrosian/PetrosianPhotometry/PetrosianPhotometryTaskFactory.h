/**
 * @file Petrosian/PetrosianPhotometry/PetrosianPhotometryTaskFactory.h
 * @date 11/05/19
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

#ifndef _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYTASKFACTORY_H
#define _PLUGIN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYTASKFACTORY_H

#include <SEFramework/Task/TaskFactory.h>

namespace Petrosian {

/**
 * @class PetrosianPhotometryTaskFactory
 * @brief
 *
 */
class PetrosianPhotometryTaskFactory : public SourceXtractor::TaskFactory {

public:

  /**
   * @brief Destructor
   */
  virtual ~PetrosianPhotometryTaskFactory() = default;

  std::shared_ptr<SourceXtractor::Task> createTask(const SourceXtractor::PropertyId& property_id) const override;

  void reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const override;

  void configure(Euclid::Configuration::ConfigManager& manager) override;

  void registerPropertyInstances(SourceXtractor::OutputRegistry& registry) override;

private:
  double m_magnitude_zero_point;
  bool m_use_symmetry;
  std::string m_checkimage;

  std::vector<std::pair<std::string, unsigned>> m_petrosian_names;
  std::vector<unsigned> m_images;

};  // End of PetrosianPhotometryTaskFactory class

}  // namespace Petrosian


#endif
