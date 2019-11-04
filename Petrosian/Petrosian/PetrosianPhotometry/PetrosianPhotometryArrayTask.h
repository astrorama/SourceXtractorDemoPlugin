/**
 * @file Petrosian/PetrosianPhotometry/PetrosianPhotometryArrayTask.h
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

#ifndef _PETROSIAN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYARRAYTASK_H
#define _PETROSIAN_PETROSIANPHOTOMETRY_PETROSIANPHOTOMETRYARRAYTASK_H

#include <SEFramework/Task/SourceTask.h>

namespace Petrosian {

/**
 * @class PetrosianPhotometryArrayTask
 * @brief
 *
 */
class PetrosianPhotometryArrayTask: public SourceXtractor::SourceTask {

public:

  /**
   * @brief Destructor
   */
  virtual ~PetrosianPhotometryArrayTask() = default;

  PetrosianPhotometryArrayTask(const std::vector<unsigned> &images);

  void computeProperties(SourceXtractor::SourceInterface& source) const override;

private:
  std::vector<unsigned> m_images;

};  // End of PetrosianPhotometryArrayTask class

}  // namespace Petrosian


#endif
