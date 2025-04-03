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
  return 0;
}
