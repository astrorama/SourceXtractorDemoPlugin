/**
 * @file src/lib/PetrosianPhotometry/PetrosianPhotometryArray.cpp
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

#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryArray.h"

namespace Petrosian {


PetrosianPhotometryArray::PetrosianPhotometryArray(const std::vector<PetrosianPhotometry>& photometries) {
  for (auto &p : photometries) {
    m_fluxes.emplace_back(p.getFlux());
    m_flux_errors.emplace_back(p.getFluxError());
    m_mags.emplace_back(p.getMag());
    m_mag_errors.emplace_back(p.getMagError());
    m_flags.emplace_back(p.getFlags());
  }
}

std::vector<double> PetrosianPhotometryArray::getFluxes() const {
  return m_fluxes;
}

std::vector<double> PetrosianPhotometryArray::getFluxErrors() const {
  return m_flux_errors;
}

std::vector<double> PetrosianPhotometryArray::getMags() const {
  return m_mags;
}

std::vector<double> PetrosianPhotometryArray::getMagErrors() const {
  return m_mag_errors;
}

std::vector<SourceXtractor::Flags> PetrosianPhotometryArray::getFlags() const {
  return m_flags;
}
}  // namespace Petrosian
