#include <gp_Pnt.hxx>
#include <iostream>

int main() {
  gp_Pnt p(1.0, 2.0, 3.0);
  std::cout << "3D Point: (" << p.X() << ", " << p.Y() << ", " << p.Z()
            << ")\n";
  return 0;
}
