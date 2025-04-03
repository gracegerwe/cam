#pragma once

#include <TopoDS_Shape.hxx>

TopoDS_Shape loadStepFile(const std::string& filename);
void printShapeStats(const TopoDS_Shape& shape);
