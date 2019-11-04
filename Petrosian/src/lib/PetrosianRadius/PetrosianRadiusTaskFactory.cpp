/**
 * @file src/lib/PetrosianRadius/PetrosianRadiusTaskFactory.cpp
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
#include "Petrosian/PetrosianRadius/PetrosianRadius.h"
#include "Petrosian/PetrosianRadius/PetrosianRadiusTask.h"
#include "Petrosian/PetrosianRadius/PetrosianRadiusTaskFactory.h"

namespace Petrosian {


std::shared_ptr<SourceXtractor::Task>
PetrosianRadiusTaskFactory::createTask(const SourceXtractor::PropertyId& property_id) const {
  if (property_id.getTypeId() == typeid(PetrosianRadius)) {
    return std::make_shared<PetrosianRadiusTask>(m_eta, m_factor, m_minrad);
  }
  return nullptr;
}

void PetrosianRadiusTaskFactory::reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
  manager.registerConfiguration<PetrosianConfig>();
}

void PetrosianRadiusTaskFactory::configure(Euclid::Configuration::ConfigManager& manager) {
  const auto& petrosian_config = manager.getConfiguration<PetrosianConfig>();
  m_eta = petrosian_config.getEta();
  m_factor = petrosian_config.getFactor();
  m_minrad = petrosian_config.getMinRadius();
}


}  // namespace Petrosian
