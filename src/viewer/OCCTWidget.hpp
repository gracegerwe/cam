#pragma once

// Qt includes
// #include <QWidget>

// OpenCascade includes
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <QOpenGLWidget>
// #include <Aspect_DisplayConnection.hxx>
// #include <Aspect_Window.hxx>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>

class OCCTWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  explicit OCCTWidget(QWidget *parent = nullptr);
  ~OCCTWidget() override;
  void loadSTEP(const std::string &path);
  void meshShape();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  // void paintEvent(QPaintEvent *) override;
  // void resizeEvent(QResizeEvent *) override;

 private:
  Handle(V3d_View) view;
  Handle(AIS_InteractiveContext) context;
  Handle(AIS_Shape) displayedShape;
  TopoDS_Shape currentShape;
};
