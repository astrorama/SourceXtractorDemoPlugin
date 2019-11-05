/**
 * @file src/lib/PetrosianPhotometry/PetrosianPhotometryTaskFactory.cpp
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

#include "Petrosian/PetrosianConfig.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometry.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryTask.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryTaskFactory.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArray.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArrayTask.h"

#include <SEImplementation/Configuration/MagnitudeConfig.h>
#include <SEImplementation/Configuration/MeasurementImageConfig.h>
#include <SEImplementation/Configuration/WeightImageConfig.h>

namespace Petrosian {


std::shared_ptr<SourceXtractor::Task>
PetrosianPhotometryTaskFactory::createTask(const SourceXtractor::PropertyId& property_id) const {
  if (property_id.getTypeId() == typeid(PetrosianPhotometry)) {
    return std::make_shared<PetrosianPhotometryTask>(
      property_id.getIndex(), m_magnitude_zero_point, m_use_symmetry,
      m_checkimage
    );
  }
  else if (property_id.getTypeId() == typeid(PetrosianPhotometryArray)) {
    return std::make_shared<PetrosianPhotometryArrayTask>(m_images);
  }
  return nullptr;
}

void PetrosianPhotometryTaskFactory::reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
  manager.registerConfiguration<PetrosianConfig>();
  manager.registerConfiguration<SourceXtractor::MagnitudeConfig>();
  manager.registerConfiguration<SourceXtractor::MeasurementImageConfig>();
  manager.registerConfiguration<SourceXtractor::WeightImageConfig>();
}

void PetrosianPhotometryTaskFactory::configure(Euclid::Configuration::ConfigManager& manager) {
  m_magnitude_zero_point = manager.getConfiguration<SourceXtractor::MagnitudeConfig>().getMagnitudeZeroPoint();
  m_use_symmetry = manager.getConfiguration<SourceXtractor::WeightImageConfig>().symmetryUsage();
  m_checkimage = manager.getConfiguration<PetrosianConfig>().getCheckImagePath();

  const auto& measurement_config = manager.getConfiguration<SourceXtractor::MeasurementImageConfig>();
  const auto& image_infos = measurement_config.getImageInfos();

  for (unsigned i = 0; i < image_infos.size(); ++i) {
    m_petrosian_names.emplace_back(std::make_pair(std::to_string(i), image_infos[i].m_id));
    m_images.push_back(image_infos[i].m_id);
  }
}

void PetrosianPhotometryTaskFactory::registerPropertyInstances(SourceXtractor::OutputRegistry& registry) {
  registry.registerPropertyInstances<PetrosianPhotometry>(m_petrosian_names);
}

}  // namespace Petrosian
