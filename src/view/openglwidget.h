#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QImageWriter>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../controller/controller.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OpenGLWidget(s21::Controller *controller, QWidget *parent = nullptr);
  void RotateModel(double step, char xyz);
  void MoveModel(double step, char xyz);
  void EditIntervalLines(double scale_factor);
  void EditThicknessLines(double scale_factor);
  void EditSizeVer(double size_ver);
  void ClearContent();
  void SetProjectionType(int value);
  QColor GetLineColor() const;
  QColor GetVertexColor() const;

 public slots:
  void LoadModelFile(const QString &file_path);
  void ScaleModelToFit(double scale_factor);
  void SetBackgroundColor(const QColor &color);
  void SetColorLineVer(const QColor &color, bool type);
  void SaveImage(const QString &file_path);
  void VerStyle(int dottedLine);
  void SetLineStyle(bool line);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

 private:
  s21::Controller *controller;
  bool file_loaded;
  float scale;
  QColor point_color;
  QColor line_color;
  QColor back_color;
  QPoint last_mouse_pos;
  double rotation_speed = 0.01;
  bool use_dotted_line;
  int use_dotted_ver = 0;
  int win_height = 540, win_width = 650;

 signals:
  void CountVertexFacets(int count_vertex, int count_facets);
  void FileIncorrect(QString error_message);
};

#endif  // OPENGLWIDGET_H
