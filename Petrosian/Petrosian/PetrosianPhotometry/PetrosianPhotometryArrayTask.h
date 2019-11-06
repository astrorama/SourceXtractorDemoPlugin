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
 *  This class groups the photometries measured on different images into a
 *  single property. This avoids having one column per measurement frame.
 * @details
 *  It inherits from SourceXtractor::SourceTask because it is a per-source property.
 *  For measurements that work on source groups - like the Model Fitting does -,
 *  it would have to inherit from SourceXtractor::GroupTask
 * @see
 *  SourceXtractor::GroupTask
 */
class PetrosianPhotometryArrayTask: public SourceXtractor::SourceTask {

public:

  /**
   * Default destructor
   */
  virtual ~PetrosianPhotometryArrayTask() = default;

  /**
   * Constructor
   * @param images
   *    List of frame IDs. To be used to retrieve the individual photometries.
   */
  PetrosianPhotometryArrayTask(const std::vector<unsigned> &images);

  /**
   * @brief
   *    Compute the corresponding property. Multiple properties could be computed at once.
   * @param source
   *    The source for which to compute the property. If this were an implementation of
   *    SourceXtractor::GroupTask, it would receive an instance of
   *    SourceXtractor::SourceGroupInterface instead
   */
  void computeProperties(SourceXtractor::SourceInterface& source) const override;

private:
  std::vector<unsigned> m_images;

};  // End of PetrosianPhotometryArrayTask class

}  // namespace Petrosian


#endif
