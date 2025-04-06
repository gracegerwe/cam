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

  auto features = extractFeatures(shape);

  for (const auto& feat : features) {
    std::cout << feat.type << " at (" << feat.location.X() << ", "
              << feat.location.Y() << ", " << feat.location.Z() << ") -> "
              << feat.classification << std::endl;
  }

  return 0;
}
