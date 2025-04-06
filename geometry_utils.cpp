#include "geometry_utils.hpp"

#include <BRepAdaptor_Surface.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
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

std::vector<Feature> extractFeatures(const TopoDS_Shape& shape) {
  std::vector<Feature>
      features;  // creates empty list where we'll store all features we find

  TopExp_Explorer explorer;  // helper that lets us walk through sub-shapes
  for (explorer.Init(shape, TopAbs_FACE); explorer.More();
       explorer.Next()) {  // starts look over all faces in shape, TopAbs -
                           // "only faces"
    TopoDS_Face face =
        TopoDS::Face(explorer.Current());  // convert from explorer to TopoDS
    BRepAdaptor_Surface surface(
        face, Standard_True);  // wrap in helper for analyzing geometry of a
                               // face; Standard_True means apply any geometric
                               // transformation, like rotation
    GeomAbs_SurfaceType surfType =
        surface.GetType();  // get type of surface, is it a plane? cylinder?
                            // sphere? returns an enum

    Feature feat;  //
                   // Get the geometric surface from the face
    Handle(Geom_Surface) geomSurf = BRep_Tool::Surface(face);

    // Get UV bounds
    Standard_Real u1, u2, v1, v2;
    BRepTools::UVBounds(face, u1, u2, v1, v2);

    // Compute point at center of UV domain
    gp_Pnt center = geomSurf->Value((u1 + u2) / 2.0, (v1 + v2) / 2.0);
    feat.location = center;

    switch (surfType) {
      case GeomAbs_Plane:
        feat.type = "flat_face";
        feat.classification = "millable";
        break;
      case GeomAbs_Cylinder:
        feat.type = "cylindrical_hole";
        feat.classification = "drillable";
        break;
      default:
        feat.type = "unknown";
        feat.classification = "unmachinable";
        break;
    }

    features.push_back(feat);
  }

  return features;
}
