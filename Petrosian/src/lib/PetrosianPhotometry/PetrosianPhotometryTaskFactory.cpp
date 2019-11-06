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

  // ------------------------------------------------------------------------
  // This task factory takes care of the creation of tasks for two different
  // properties:
  //  PetrosianPhotometry and PetrosianPhotometryArray
  // We switch based on the getTypeId()
  // ------------------------------------------------------------------------

  // Photometry of a source on a single image
  if (property_id.getTypeId() == typeid(PetrosianPhotometry)) {
    // Note we use getIndex() to identify the unique frame
    // In effect, a property is a unique combination of type and measurement frame
    return std::make_shared<PetrosianPhotometryTask>(
      property_id.getIndex(), m_magnitude_zero_point, m_use_symmetry,
      m_checkimage
    );
  }
  // Group photometries
  else if (property_id.getTypeId() == typeid(PetrosianPhotometryArray)) {
    return std::make_shared<PetrosianPhotometryArrayTask>(m_images);
  }
  return nullptr;
}

void PetrosianPhotometryTaskFactory::reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
  // This task uses also configuration classes that come from the main application:
  // For instance, for the magnitude zeropoint, or to identify the set of unique measurement frames
  manager.registerConfiguration<PetrosianConfig>();
  manager.registerConfiguration<SourceXtractor::MagnitudeConfig>();
  manager.registerConfiguration<SourceXtractor::MeasurementImageConfig>();
  manager.registerConfiguration<SourceXtractor::WeightImageConfig>();
}

void PetrosianPhotometryTaskFactory::configure(Euclid::Configuration::ConfigManager& manager) {
  // We can rely on the configuration also from the main SourceXtractor code
  m_magnitude_zero_point = manager.getConfiguration<SourceXtractor::MagnitudeConfig>().getMagnitudeZeroPoint();
  m_use_symmetry = manager.getConfiguration<SourceXtractor::WeightImageConfig>().symmetryUsage();
  m_checkimage = manager.getConfiguration<PetrosianConfig>().getCheckImagePath();

  const auto& measurement_config = manager.getConfiguration<SourceXtractor::MeasurementImageConfig>();
  const auto& image_infos = measurement_config.getImageInfos();

  for (unsigned i = 0; i < image_infos.size(); ++i) {
    m_images.push_back(image_infos[i].m_id);
  }
}

}  // namespace Petrosian
