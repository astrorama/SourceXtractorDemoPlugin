/**
 * @file src/lib/PetrosianPhotometry/PetrosianPhotometryTask.cpp
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

#include "Petrosian/PetrosianPhotometry/PetrosianPhotometryTask.h"
#include "Petrosian/PetrosianRadius/PetrosianRadius.h"
#include "Petrosian/PetrosianPhotometry/PetrosianPhotometry.h"

#include <limits>

#include <SEFramework/Aperture/FluxMeasurement.h>
#include <SEFramework/Aperture/EllipticalAperture.h>
#include <SEFramework/Aperture/TransformedAperture.h>

#include <SEImplementation/Measurement/MultithreadedMeasurement.h>
#include <SEImplementation/Plugin/MeasurementFrame/MeasurementFrame.h>
#include <SEImplementation/Plugin/MeasurementFramePixelCentroid/MeasurementFramePixelCentroid.h>
#include <SEImplementation/Plugin/ShapeParameters/ShapeParameters.h>
#include <SEImplementation/Plugin/Jacobian/Jacobian.h>


namespace Petrosian {

PetrosianPhotometryTask::PetrosianPhotometryTask(unsigned instance, double mag_zeropoint, bool use_symmetry)
  : m_instance(instance), m_mag_zeropoint(mag_zeropoint), m_use_symmetry(use_symmetry) {

}

void PetrosianPhotometryTask::computeProperties(SourceXtractor::SourceInterface& source) const {
  std::lock_guard<std::recursive_mutex> lock(SourceXtractor::MultithreadedMeasurement::g_global_mutex);

  // get the measurement frame
  const auto& measurement_frame = source.getProperty<SourceXtractor::MeasurementFrame>(m_instance).getFrame();

  // get the images and image information from the frame
  const auto& measurement_image = measurement_frame->getSubtractedImage();
  const auto& variance_map = measurement_frame->getVarianceMap();
  const auto& variance_threshold = measurement_frame->getVarianceThreshold();
  const auto& threshold_image = measurement_frame->getThresholdedImage();
  double gain = measurement_frame->getGain();

  // get the object center
  auto& centroid = source.getProperty<SourceXtractor::MeasurementFramePixelCentroid>(m_instance);
  const auto& centroid_x = centroid.getCentroidX();
  const auto& centroid_y = centroid.getCentroidY();

  // get the shape parameters
  auto& shape = source.getProperty<SourceXtractor::ShapeParameters>();
  const auto& cxx = shape.getEllipseCxx();
  const auto& cyy = shape.getEllipseCyy();
  const auto& cxy = shape.getEllipseCxy();

  // get the jacobian
  const auto& jacobian = source.getProperty<SourceXtractor::JacobianSource>(m_instance);

  // get the kron-radius
  double petrosian_radius = source.getProperty<PetrosianRadius>().getRadius();

  // create the elliptical aperture
  auto ell_aper = std::make_shared<SourceXtractor::TransformedAperture>(
    std::make_shared<SourceXtractor::EllipticalAperture>(cxx, cyy, cxy, petrosian_radius),
    jacobian.asTuple());

  auto measurement = SourceXtractor::measureFlux(
    ell_aper, centroid_x, centroid_y, measurement_image, variance_map, variance_threshold, m_use_symmetry);

  // compute the derived quantities
  auto flux_error = sqrt(measurement.m_variance + measurement.m_flux / gain);
  auto mag = measurement.m_flux > 0.0 ? -2.5 * log10(measurement.m_flux) + m_mag_zeropoint
                                      : std::numeric_limits<double>::quiet_NaN();
  auto mag_error = 1.0857 * flux_error / measurement.m_flux;

  // set the source properties
  source.setIndexedProperty<PetrosianPhotometry>(
    m_instance, measurement.m_flux, flux_error, mag, mag_error, measurement.m_flags);
}

}  // namespace Petrosian



