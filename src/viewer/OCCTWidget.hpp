#pragma once
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <QOpenGLWidget>
#include <QTimer>
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
  Handle(V3d_Viewer) viewer;
  TopoDS_Shape currentShape;
  bool isInitialized = false;
  QTimer *frameTimer;
};
