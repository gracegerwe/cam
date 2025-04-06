#include "OCCTWidget.hpp"

#include <AppKit/NSView.h>
#include <Cocoa/Cocoa.h>

#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Cocoa_Window.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <QResizeEvent>
#include <STEPControl_Reader.hxx>

OCCTWidget::OCCTWidget(QWidget *parent) : QWidget(parent) {
  Handle(Aspect_DisplayConnection) disp = new Aspect_DisplayConnection();
  Handle(Graphic3d_GraphicDriver) driver = new OpenGl_GraphicDriver(disp);
  Handle(V3d_Viewer) viewer = new V3d_Viewer(driver);
  context = new AIS_InteractiveContext(viewer);

  view = viewer->CreateView();
  Handle(Cocoa_Window) window = new Cocoa_Window(disp, (NSView *)this->winId());
  view->SetWindow(window);
  view->SetBackgroundColor(Quantity_NOC_WHITE);
  view->MustBeResized();
  view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_GRAY, 0.1,
                        V3d_ZBUFFER);
}

void OCCTWidget::loadSTEP(const std::string &path) {
  STEPControl_Reader reader;
  if (reader.ReadFile(path.c_str()) != IFSelect_RetDone) return;
  reader.TransferRoots();
  currentShape = reader.OneShape();

  displayedShape = new AIS_Shape(currentShape);
  context->Display(displayedShape, Standard_True);
  view->FitAll();
}

void OCCTWidget::meshShape() {
  if (currentShape.IsNull()) return;
  BRepMesh_IncrementalMesh mesh(currentShape, 0.1);
  if (!displayedShape.IsNull()) context->Redisplay(displayedShape, true);
  view->Redraw();
}

void OCCTWidget::paintEvent(QPaintEvent *) {
  if (!view.IsNull()) view->Redraw();
}

void OCCTWidget::resizeEvent(QResizeEvent *) {
  if (!view.IsNull()) view->MustBeResized();
}
