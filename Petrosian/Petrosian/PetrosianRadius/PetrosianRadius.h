/**
 * @file Petrosian/PetrosianRadius/PetrosianRadius.h
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

#ifndef _PLUGIN_PETROSIANRADIUS_PETROSIANRADIUS_H
#define _PLUGIN_PETROSIANRADIUS_PETROSIANRADIUS_H

#include <SEFramework/Property/Property.h>

namespace Petrosian {

/**
 * @class AvgAperture
 * @brief
 *
 */
class PetrosianRadius : public SourceXtractor::Property {

public:

  virtual ~PetrosianRadius() = default;

  PetrosianRadius(double radius);

  double getRadius() const;

private:
  double m_radius;

};  // End of PetrosianRadius class

}  // namespace Petrosian


#endif
