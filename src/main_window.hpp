#pragma once

#include <QMainWindow>

class OCCTWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  OCCTWidget *viewer;
};
