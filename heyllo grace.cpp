#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Font_FontAspect.hxx>
#include <Font_FontMgr.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <QApplication>
#include <QElapsedTimer>
#include <QOpenGLContext>
#include <QResizeEvent>
#include <STEPControl_Reader.hxx>

#include "OCCTWidget.hpp"

OCCTWidget::OCCTWidget(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "🔧 OCCTWidget constructor called";
  setFocusPolicy(Qt::StrongFocus);
  setUpdateBehavior(QOpenGLWidget::PartialUpdate);
  setAttribute(Qt::WA_AlwaysStackOnTop);
  setAutoFillBackground(false);
  setMinimumSize(400, 400);

  // Enable OpenGL debugging
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setSamples(4);
  setFormat(format);

  qDebug() << "🔧 OCCTWidget constructor completed";
}

OCCTWidget::~OCCTWidget() = default;

void OCCTWidget::initializeGL() {
  qDebug() << "🌱 initializeGL()";
  makeCurrent();

  qDebug() << "✅ OpenGL context is valid:"
           << QOpenGLContext::currentContext()->isValid();

  try {
    Handle(Font_FontMgr) fontMgr = Font_FontMgr::GetInstance();
    qDebug() << "🖋 Font manager acquired:" << !fontMgr.IsNull();

    Handle(Aspect_DisplayConnection) disp = new Aspect_DisplayConnection();
    qDebug() << "🖥️ Display connection created:" << !disp.IsNull();

    Handle(Graphic3d_GraphicDriver) driver = new OpenGl_GraphicDriver(disp);
    qDebug() << "🚗 Graphic driver created:" << !driver.IsNull();

    Handle(OpenGl_GraphicDriver) glDriver =
        Handle(OpenGl_GraphicDriver)::DownCast(driver);
    if (!glDriver.IsNull()) {
      glDriver->ChangeOptions().contextDebug = Standard_True;
      qDebug() << "🧠 OpenGL debug context enabled";
    }

    viewer = new V3d_Viewer(driver);
    qDebug() << "👁️ Viewer created:" << !viewer.IsNull();

    viewer->SetDefaultLights();
    viewer->SetLightOn();
    viewer->SetDefaultComputedMode(Standard_True);
    viewer->SetDefaultShadingModel(Graphic3d_TypeOfShadingModel_Phong);

    context = new AIS_InteractiveContext(viewer);
    qDebug() << "🔍 Context created:" << !context.IsNull();

    view = viewer->CreateView();
    qDebug() << "🎥 View created:" << !view.IsNull();

    Handle(Aspect_NeutralWindow) window = new Aspect_NeutralWindow();
    qDebug() << "🪟 Window created:" << !window.IsNull();

    window->SetSize(this->width(), this->height());
    window->SetNativeHandle((Aspect_Drawable)this->winId());

    if (!view->Window().IsNull()) {
      view->Window()->DoResize();
    }
    view->SetWindow(window);

    qDebug() << "🪟 SetNativeHandle success";

    view->SetBackgroundColor(Quantity_NOC_WHITE);
    view->MustBeResized();
    view->TriedronDisplay(Aspect_TOTP_RIGHT_LOWER, Quantity_NOC_GRAY, 0.1,
                          V3d_ZBUFFER);

    qDebug() << "✅ OpenGL context is valid:"
             << QOpenGLContext::currentContext()->isValid();
    qDebug() << "🧽 Finished initializeGL";
  } catch (const Standard_Failure &e) {
    qDebug() << "❌ OpenCASCADE error in initializeGL:" << e.GetMessageString();
  } catch (const std::exception &e) {
    qDebug() << "❌ Standard exception in initializeGL:" << e.what();
  } catch (...) {
    qDebug() << "❌ Unknown exception in initializeGL";
  }

  doneCurrent();
  update();
}

void OCCTWidget::paintGL() {
  QElapsedTimer timer;
  timer.start();
  qDebug() << "🎨 paintGL()";

  if (!QOpenGLContext::currentContext()) {
    qDebug() << "❌ No OpenGL context in paintGL";
    return;
  }

  makeCurrent();

  try {
    if (view.IsNull()) {
      qDebug() << "❌ View is null in paintGL";
      doneCurrent();
      return;
    }

    if (!isInitialized) {
      qDebug() << "🔄 First paint, resizing view";
      view->Window()->DoResize();
      view->MustBeResized();
      isInitialized = true;
    }

    qDebug() << "🖼 Redrawing view";
    view->Redraw();

    qint64 elapsed = timer.elapsed();
    qDebug() << "⏱️ paintGL() took" << elapsed << "ms";
  } catch (const Standard_Failure &e) {
    qDebug() << "❌ OpenCASCADE error in paintGL:" << e.GetMessageString();
  } catch (const std::exception &e) {
    qDebug() << "❌ Standard exception in paintGL:" << e.what();
  } catch (...) {
    qDebug() << "❌ Unknown exception in paintGL";
  }

  doneCurrent();
  update();
}

void OCCTWidget::resizeGL(int w, int h) {
  if (!view.IsNull()) {
    makeCurrent();
    Handle(Aspect_NeutralWindow) neutralWindow =
        Handle(Aspect_NeutralWindow)::DownCast(view->Window());
    if (!neutralWindow.IsNull()) {
      neutralWindow->SetSize(w, h);
    }
    view->Window()->DoResize();
    view->MustBeResized();
    doneCurrent();
    update();
  }
}

void OCCTWidget::loadSTEP(const std::string &path) {
  qDebug() << "📂 Loading STEP file:" << QString::fromStdString(path);
  try {
    STEPControl_Reader reader;
    if (reader.ReadFile(path.c_str()) != IFSelect_RetDone) {
      qDebug() << "❌ Failed to read STEP file";
      return;
    }
    reader.TransferRoots();
    currentShape = reader.OneShape();

    displayedShape = new AIS_Shape(currentShape);
    qDebug() << "📐 STEP shape is null?" << currentShape.IsNull();
    context->Display(displayedShape, Standard_True);
    view->FitAll();
    view->Redraw();
    update();
    repaint();
    qDebug() << "✅ STEP file loaded successfully";
  } catch (const Standard_Failure &e) {
    qDebug() << "❌ OpenCASCADE error in loadSTEP:" << e.GetMessageString();
  } catch (const std::exception &e) {
    qDebug() << "❌ Standard exception in loadSTEP:" << e.what();
  } catch (...) {
    qDebug() << "❌ Unknown exception in loadSTEP";
  }
}

void OCCTWidget::meshShape() {
  if (currentShape.IsNull()) return;
  BRepMesh_IncrementalMesh mesh(currentShape, 0.1);
  if (!displayedShape.IsNull()) context->Redisplay(displayedShape, true);
  view->Redraw();
  update();
  repaint();
}
