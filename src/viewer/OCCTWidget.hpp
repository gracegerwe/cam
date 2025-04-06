#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <QWidget>
#include <TopoDS_Shape.hxx>
#include <V3d_View.hxx>

class OCCTWidget : public QWidget {
  Q_OBJECT

 public:
  explicit OCCTWidget(QWidget *parent = nullptr);
  void loadSTEP(const std::string &path);
  void meshShape();

 protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;

 private:
  Handle(V3d_View) view;
  Handle(AIS_InteractiveContext) context;
  Handle(AIS_Shape) displayedShape;
  TopoDS_Shape currentShape;
};
