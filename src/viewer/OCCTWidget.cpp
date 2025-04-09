#include "OCCTWidget.hpp"

#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Font_FontAspect.hxx>
#include <Font_FontMgr.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_Vec2.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <QApplication>
#include <QElapsedTimer>
#include <QOpenGLContext>
#include <QResizeEvent>
#include <STEPControl_Reader.hxx>

OCCTWidget::OCCTWidget(QWidget *parent) : QOpenGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
  // setAttribute(Qt::WA_NativeWindow);
  // setAttribute(Qt::WA_PaintOnScreen, false);
  setAutoFillBackground(false);
  setMinimumSize(400, 400);

  frameTimer = new QTimer(this);
  connect(frameTimer, &QTimer::timeout, this,
          QOverload<>::of(&OCCTWidget::update));
  frameTimer->start(16);
}

OCCTWidget::~OCCTWidget() = default;

void OCCTWidget::initializeGL() {
  qDebug() << "🌱 initializeGL()";
  makeCurrent();

  qDebug() << "✅ OpenGL context is valid:"
           << QOpenGLContext::currentContext()->isValid();

  Handle(Font_FontMgr) fontMgr = Font_FontMgr::GetInstance();
  qDebug() << "🖋 Font manager acquired:" << !fontMgr.IsNull();
  Handle(Aspect_DisplayConnection) disp = new Aspect_DisplayConnection();
  Handle(Graphic3d_GraphicDriver) driver = new OpenGl_GraphicDriver(disp);

  Handle(OpenGl_GraphicDriver) glDriver =
      Handle(OpenGl_GraphicDriver)::DownCast(driver);
  if (!glDriver.IsNull()) {
    glDriver->ChangeOptions().contextDebug = Standard_True;
    qDebug() << "🧠 OpenGL debug context enabled";
  }

  viewer = new V3d_Viewer(driver);
  viewer->SetDefaultLights();
  viewer->SetLightOn();
  context = new AIS_InteractiveContext(viewer);
  view = viewer->CreateView();
  qDebug() << "🎥 View and context created";

  Handle(Aspect_NeutralWindow) window = new Aspect_NeutralWindow();
  window->SetSize(this->width(), this->height());
  window->SetNativeHandle((Aspect_Drawable)this->winId());

  if (!view->Window().IsNull()) {
    view->Window()->DoResize();
  }
  view->SetWindow(window);

  qDebug() << "🪟 SetNativeHandle success";

  view->SetBackgroundColor(Quantity_NOC_WHITE);
  view->SetImmediateUpdate(Standard_True);
  view->MustBeResized();
  view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_GRAY, 0.1,
                        V3d_ZBUFFER);

  qDebug() << "✅ OpenGL context is valid:"
           << QOpenGLContext::currentContext()->isValid();
  qDebug() << "🧽 Finished initializeGL";
  update();
  repaint();
  QApplication::processEvents();
}

void OCCTWidget::paintGL() {
  QElapsedTimer timer;
  timer.start();
  qDebug() << "🎨 paintGL()";
  // makeCurrent();

  if (view.IsNull()) {
    qDebug() << "❌ View is null in paintGL";
    return;
  }

  if (!isInitialized) {
    view->SetSize(Graphic3d::Vec2i(w, h));
    view->MustBeResized();
    // context->UpdateCurrentViewer();  //where is the def of this function
    isInitialized = true;
  }
  qDebug() << "🖼 Redrawing view";
  // view->Invalidate();
  view->Redraw();
  update();
  qint64 elapsed = timer.elapsed();
  qDebug() << "⏱️ paintGL() took" << elapsed << "ms";
}

void OCCTWidget::resizeGL(int w, int h) {
  // double chck that this isnt firing over and over
  qDebug() << "resizeGL function fires";
  if (!view.IsNull()) {
    // view->SetSize({w, h});
    Handle(Aspect_NeutralWindow) neutralWindow =
        Handle(Aspect_NeutralWindow)::DownCast(view->Window());
    if (!neutralWindow.IsNull()) {
      neutralWindow->SetSize(w, h);
    }
  }
  view->Window()->DoResize();
  view->MustBeResized();
  update();
}

void OCCTWidget::loadSTEP(const std::string &path) {
  STEPControl_Reader reader;
  if (reader.ReadFile(path.c_str()) != IFSelect_RetDone) return;
  reader.TransferRoots();
  currentShape = reader.OneShape();

  displayedShape = new AIS_Shape(currentShape);
  qDebug() << "📐 STEP shape is null?" << currentShape.IsNull();
  context->Display(displayedShape, Standard_True);
  view->FitAll();
  view->Redraw();
  update();
  repaint();
}

void OCCTWidget::meshShape() {
  if (currentShape.IsNull()) return;
  BRepMesh_IncrementalMesh mesh(currentShape, 0.1);
  if (!displayedShape.IsNull()) context->Redisplay(displayedShape, true);
  view->Redraw();
  update();
  repaint();
}
