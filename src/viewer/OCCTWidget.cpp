#include "OCCTWidget.hpp"

// #include <AppKit/NSView.h>
// #include <Cocoa/Cocoa.h>
// #include <Cocoa_Window.hxx>

#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
// #include <Aspect_Window.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <QResizeEvent>
#include <STEPControl_Reader.hxx>

OCCTWidget::OCCTWidget(QWidget *parent) : QOpenGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  setUpdateBehavior(QOpenGLWidget::PartialUpdate);
  setAttribute(Qt::WA_NativeWindow);
  setAttribute(Qt::WA_PaintOnScreen, false);
  setAutoFillBackground(false);
}

OCCTWidget::~OCCTWidget() = default;

// this->setAttribute(
// Qt::WA_NativeWindow);  // Ensure native window ID is created
void OCCTWidget::initializeGL() {
  Handle(Aspect_DisplayConnection) disp = new Aspect_DisplayConnection();
  Handle(Graphic3d_GraphicDriver) driver = new OpenGl_GraphicDriver(disp);
  Handle(V3d_Viewer) viewer = new V3d_Viewer(driver);
  context = new AIS_InteractiveContext(viewer);

  view = viewer->CreateView();
  // view->SetWindow(new Aspect_WindowNative((Aspect_Handle)this->winId()));
  //  Handle(Cocoa_Window) window = new Cocoa_Window(disp, (NSView
  //  *)this->winId()); hi grace try this
  //  Handle(Aspect_DisplayConnection) disp;
  //  Handle(Cocoa_Window) window = new Cocoa_Window((NSView *)this->winId());
  //  view->SetWindow(window);
  // view->SetWindow(Handle(Aspect_Window)());

  Handle(Aspect_NeutralWindow) window = new Aspect_NeutralWindow();
  window->SetSize(this->width(), this->height());
  window->SetNativeHandle((Aspect_Drawable)this->winId());
  view->SetWindow(window);

  view->SetBackgroundColor(Quantity_NOC_WHITE);
  view->MustBeResized();
  view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_GRAY, 0.1,
                        V3d_ZBUFFER);
}

void OCCTWidget::resizeGL(int w, int h) {
  if (!view.IsNull()) {
    view->Window()->DoResize();
    view->MustBeResized();
  }
}

void OCCTWidget::paintGL() {
  if (!view.IsNull()) {
    view->Redraw();
  }
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

/*void OCCTWidget::paintEvent(QPaintEvent *) {
  if (!view.IsNull()) view->Redraw();
}

void OCCTWidget::resizeEvent(QResizeEvent *) {
  if (!view.IsNull()) view->MustBeResized();
}*/
