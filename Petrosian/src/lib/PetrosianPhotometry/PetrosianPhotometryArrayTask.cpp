/**
 * @file src/lib/PetrosianPhotometry/PetrosianPhotometryArrayTask.cpp
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

#include "Petrosian/PetrosianPhotometry/PetrosianPhotometry.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArray.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArrayTask.h"

namespace Petrosian {

PetrosianPhotometryArrayTask::PetrosianPhotometryArrayTask(const std::vector<unsigned>& images)
  : m_images(images) {
}

void PetrosianPhotometryArrayTask::computeProperties(SourceXtractor::SourceInterface& source) const {
  std::vector<PetrosianPhotometry> photometries;
  for (auto img : m_images) {
    photometries.emplace_back(source.getProperty<PetrosianPhotometry>(img));
  }
  source.setProperty<PetrosianPhotometryArray>(photometries);
}

}  // namespace Petrosian



