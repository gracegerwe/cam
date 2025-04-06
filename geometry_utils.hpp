#pragma once
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <string>
#include <vector>

struct Feature {
  std::string type;
  std::string classification;
  gp_Pnt location;
};

struct CamOperation {
  std::string type;     // "drill", "mill", etc.
  std::string tool;     // e.g. "twist_drill_5mm"
  gp_Pnt position;      // starting point
  double depth;         // depth of cut
  double feedRate;      // mm/min
  double spindleSpeed;  // rpm
};

TopoDS_Shape loadStepFile(const std::string& path);
void printShapeStats(const TopoDS_Shape& shape);
std::vector<Feature> extractFeatures(const TopoDS_Shape& shape);

/*^
define function that takes a constant TopoDS_Shape
aka receives a reference to a shape and won't modify it
returns list of all detected features (holes, faces, etc)
*/

std::vector<CamOperation> generateCamOps(const std::vector<Feature>& features);
