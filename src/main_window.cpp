#include "main_window.hpp"

#include <QOpenGLWidget>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "viewer/OCCTWidget.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  auto *central = new QWidget(this);
  auto *layout = new QVBoxLayout(central);

  viewer = new OCCTWidget(this);
  layout->addWidget(viewer);

  auto *meshButton = new QPushButton("Mesh It!", this);
  connect(meshButton, &QPushButton::clicked, [this]() {
    viewer->meshShape();  // call your mesh function
  });

  layout->addWidget(meshButton);

  setCentralWidget(central);
  setWindowTitle("DFM Visualizer");

  // ✅ Delay loadSTEP until OpenGL context is ready
  QTimer::singleShot(0, this, [this]() {
    qDebug() << "📦 Triggering loadSTEP from MainWindow";
    viewer->loadSTEP("/Users/gracegerwe/Downloads/Servo_Horn.step");
  });

  // ✅ Delay additional redraw AFTER widget is shown on screen
  QTimer::singleShot(100, viewer, [this]() {
    qDebug() << "🔁 Forcing post-show repaint";
    viewer->update();   // schedule paintGL
    viewer->repaint();  // force immediate paintGL
  });
}

MainWindow::~MainWindow() = default;
