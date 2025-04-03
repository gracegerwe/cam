
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <iostream>

int main() {
  const char* filename = "/Users/gracegerwe/Downloads/Servo_Horn.step";

  STEPControl_Reader reader;
  IFSelect_ReturnStatus status = reader.ReadFile(filename);

  if (status != IFSelect_RetDone) {
    std::cerr << "STEP read failed with status: " << status << std::endl;
    return 1;
  }

  reader.TransferRoots();
  TopoDS_Shape shape = reader.OneShape();

  if (shape.IsNull()) {
    std::cerr << "Shape is null after loading." << std::endl;
    return 1;
  }

  std::cout << "✅ STEP file loaded successfully." << std::endl;
  return 0;
}
