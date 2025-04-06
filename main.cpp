#include <iostream>

#include "geometry_utils.hpp"

int main() {
  std::string stepPath = "/Users/gracegerwe/Downloads/Servo_Horn.step";

  TopoDS_Shape shape = loadStepFile(stepPath);

  if (shape.IsNull()) {
    std::cerr << "Failed to load STEP file." << std::endl;
    return 1;
  }

  printShapeStats(shape);

  // Extract geometric features
  auto features = extractFeatures(shape);

  // Print raw features with classification
  for (const auto& feat : features) {
    std::cout << feat.type << " at (" << feat.location.X() << ", "
              << feat.location.Y() << ", " << feat.location.Z() << ") -> "
              << feat.classification << std::endl;
  }

  // Generate CAM operations from those features
  auto camOps = generateCamOps(features);

  // Print out toolpath actions
  for (const auto& op : camOps) {
    std::cout << op.type << " using " << op.tool << " at (" << op.position.X()
              << ", " << op.position.Y() << ", " << op.position.Z()
              << "), depth: " << op.depth << "mm" << std::endl;
  }

  return 0;
}
