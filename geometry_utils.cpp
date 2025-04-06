#include "geometry_utils.hpp"

#include <BRepAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <STEPControl_Reader.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Pln.hxx>
#include <iostream>

TopoDS_Shape loadStepFile(const std::string& filename) {
  STEPControl_Reader reader;
  IFSelect_ReturnStatus status = reader.ReadFile(filename.c_str());

  if (status != IFSelect_RetDone) {
    std::cerr << "Failed to read STEP file: " << filename << std::endl;
    return TopoDS_Shape();  // null shape
  }

  reader.TransferRoots();
  return reader.OneShape();
}

void printShapeStats(const TopoDS_Shape& shape) {
  if (shape.IsNull()) {
    std::cerr << "Shape is null!" << std::endl;
    return;
  }

  TopTools_IndexedMapOfShape solids, shells, faces, edges, vertices;
  TopExp::MapShapes(shape, TopAbs_SOLID, solids);
  TopExp::MapShapes(shape, TopAbs_SHELL, shells);
  TopExp::MapShapes(shape, TopAbs_FACE, faces);
  TopExp::MapShapes(shape, TopAbs_EDGE, edges);
  TopExp::MapShapes(shape, TopAbs_VERTEX, vertices);

  std::cout << "Shape analysis:\n"
            << "  Solids:   " << solids.Extent() << "\n"
            << "  Shells:   " << shells.Extent() << "\n"
            << "  Faces:    " << faces.Extent() << "\n"
            << "  Edges:    " << edges.Extent() << "\n"
            << "  Vertices: " << vertices.Extent() << std::endl;
}
