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
#include <SEImplementation/Property/SourceId.h>
#include <SEImplementation/Plugin/MeasurementFrame/MeasurementFrame.h>
#include <SEImplementation/Plugin/MeasurementFramePixelCentroid/MeasurementFramePixelCentroid.h>
#include <SEImplementation/Plugin/ShapeParameters/ShapeParameters.h>
#include <SEImplementation/Plugin/Jacobian/Jacobian.h>
#include <SEImplementation/CheckImages/CheckImages.h>


namespace Petrosian {

PetrosianPhotometryTask::PetrosianPhotometryTask(unsigned instance, double mag_zeropoint, bool use_symmetry,
                                                 const boost::filesystem::path& checkimage)
  : m_instance(instance), m_mag_zeropoint(mag_zeropoint), m_use_symmetry(use_symmetry),
    m_checkimage(checkimage) {
}

void PetrosianPhotometryTask::computeProperties(SourceXtractor::SourceInterface& source) const {
  // When accessing directly the underlying image, we need to make sure no one else is
  // If this plugin only used other properties - including stamps -, then it would not need to do this
  std::lock_guard<std::recursive_mutex> lock(SourceXtractor::MultithreadedMeasurement::g_global_mutex);

  // We compute the photometry on the measurement frames.
  // A frame comprises the image, but also its variance map, threshold, coordinate system...
  // Note that the measurement frame is, itself, a property
  const auto& measurement_frame = source.getProperty<SourceXtractor::MeasurementFrame>(m_instance).getFrame();

  // Get, from the frame, the detection image, variance map, threshold, and already thresholded image
  const auto& measurement_image = measurement_frame->getSubtractedImage();
  const auto& variance_map = measurement_frame->getVarianceMap();
  const auto& variance_threshold = measurement_frame->getVarianceThreshold();
  const auto& threshold_image = measurement_frame->getThresholdedImage();
  double gain = measurement_frame->getGain();

  // Get the pixel centroid for the source on this frame.
  auto& centroid = source.getProperty<SourceXtractor::MeasurementFramePixelCentroid>(m_instance);
  const auto& centroid_x = centroid.getCentroidX();
  const auto& centroid_y = centroid.getCentroidY();

  // Similarly, get the shape parameters.
  // This property is computed on the detection frame!
  auto& shape = source.getProperty<SourceXtractor::ShapeParameters>();
  const auto& cxx = shape.getEllipseCxx();
  const auto& cyy = shape.getEllipseCyy();
  const auto& cxy = shape.getEllipseCxy();

  // We get the Jacobian that allows to transform coordinates between the measurement and the
  // detection frames
  const auto& jacobian = source.getProperty<SourceXtractor::JacobianSource>(m_instance);

  // Get the Petrosian radius, also a detection-frame property
  double petrosian_radius = source.getProperty<PetrosianRadius>().getRadius();

  // Here we create a *transformed* elliptical aperture. This will project the aperture
  // computed on the detection frame into the measurement frame
  auto ell_aper = std::make_shared<SourceXtractor::TransformedAperture>(
    std::make_shared<SourceXtractor::EllipticalAperture>(cxx, cyy, cxy, petrosian_radius),
    jacobian.asTuple());

  // We do not need to iterate anymore pixel per pixel, and we can rely on this utility
  auto measurement = SourceXtractor::measureFlux(
    ell_aper, centroid_x, centroid_y, measurement_image, variance_map, variance_threshold,
    m_use_symmetry);

  // Compute the derived quantities, as error and magnitude
  auto flux_error = sqrt(measurement.m_variance + measurement.m_flux / gain);
  auto mag = measurement.m_flux > 0.0 ? -2.5 * log10(measurement.m_flux) + m_mag_zeropoint
                                      : std::numeric_limits<double>::quiet_NaN();
  auto mag_error = 1.0857 * flux_error / measurement.m_flux;

  // Set the source properties
  source.setIndexedProperty<PetrosianPhotometry>(
    m_instance, measurement.m_flux, flux_error, mag, mag_error, measurement.m_flags);

  // If configured, write the aperture into the check image for this frame
  if (!m_checkimage.empty()) {
    // We rebuild the final path, appending the instance number, and suppressing the extension, as
    // it is added back by getWriteableCheckImage
    auto path = m_checkimage.parent_path();
    auto filename = m_checkimage.stem();
    filename += "_" + std::to_string(m_instance);

    auto img = SourceXtractor::CheckImages::getInstance().getWriteableCheckImage(
      (path / filename).native(), measurement_image->getWidth(), measurement_image->getHeight()
    );
    auto source_id = source.getProperty<SourceXtractor::SourceId>().getSourceId();
    SourceXtractor::fillAperture(ell_aper, centroid_x, centroid_y, img, static_cast<float>(source_id));
  }
}

}  // namespace Petrosian
