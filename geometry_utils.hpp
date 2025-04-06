#pragma once
#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <vector>

struct Feature {
  std::string type;
  std::string classification;
  gp_Pnt location;
};

TopoDS_Shape loadStepFile(const std::string& path);
void printShapeStats(const TopoDS_Shape& shape);
std::vector<Feature> extractFeatures(const TopoDS_Shape& shape);
