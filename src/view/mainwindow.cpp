#include "mainwindow.h"

#include "gif.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      controller_(controller),
      settings_(QSettings(QDir::currentPath() + "/settings.ini",
                          QSettings::IniFormat)),
      back_color(Qt::black) {
  ui->setupUi(this);

  glWidget = new OpenGLWidget(controller_, ui->display);

  connect(this, SIGNAL(fileSelected(QString)), glWidget,
          SLOT(LoadModelFile(QString)));

  // Для окрашивания
  connect(ui->colorVer, SIGNAL(clicked()), this,
          SLOT(onColorButtonVerClicked()));
  connect(ui->colorFon, SIGNAL(clicked()), this,
          SLOT(onColorButtonFoneClicked()));
  connect(ui->colorLine, SIGNAL(clicked()), this,
          SLOT(onColorButtonLineClicked()));

  // Для получения значений SpinBox
  connect(ui->doubleSpinBox_scal,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::ScaleModelFromSpinBox);
  connect(ui->doubleSpinBox_interval,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::IntervalLines);
  connect(ui->doubleSpinBox_thickness,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::ThicknessLines);
  connect(ui->doubleSpinBox_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::SizeVer);

  // Для допки с ребрами
  connect(ui->dotted_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));
  connect(ui->solid_line, SIGNAL(toggled(bool)), this,
          SLOT(onLineRadioButtonClicked(bool)));

  // Для допки с вершинами
  connect(ui->Circle, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->Square, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));
  connect(ui->No_ver, SIGNAL(toggled(bool)), this,
          SLOT(onVerRadioButtonClicked(bool)));

  // Для отображения вершин, граней и некоректного файла
  connect(glWidget, &OpenGLWidget::CountVertexFacets, this,
          &MainWindow::TransferVerticesFacets);
  connect(glWidget, &OpenGLWidget::FileIncorrect, this,
          &MainWindow::TransferFileIncorrect);

  // Для допки сохранения в форматах
  connect(ui->pushButton_bmp, SIGNAL(clicked()), this,
          SLOT(onSaveBMPButtonClicked()));
  connect(ui->pushButton_jpeg, SIGNAL(clicked()), this,
          SLOT(onSaveJPEGButtonClicked()));
  connect(ui->pushButton_gif, SIGNAL(clicked()), this,
          SLOT(onPushButtonGifClicked()));
  // Для переворота
  connect(ui->pushButton_xProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zProtat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_xMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_yMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);
  connect(ui->pushButton_zMrotat, &QPushButton::clicked, this,
          &MainWindow::onPushButtonRotateClicked);

  // Для перемещения
  connect(ui->pushButton_xPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zPmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_xMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_yMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);
  connect(ui->pushButton_zMmove, &QPushButton::clicked, this,
          &MainWindow::onPushButtonMoveClicked);

  // Для проекции
  connect(ui->typeCenter, SIGNAL(toggled(bool)), this,
          SLOT(TranslateProjectionType(bool)));
  connect(ui->typeParalel, SIGNAL(toggled(bool)), this,
          SLOT(TranslateProjectionType(bool)));

  // Очищение
  connect(ui->Clear, SIGNAL(clicked()), this, SLOT(ClearAllFunc()));
  QTimer::singleShot(10, this, &MainWindow::loadSettings);
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

void MainWindow::on_saveButton_clicked() { saveSettings(); }

void MainWindow::on_loadButton_clicked() {
  ClearAllFunc();
  loadSettings();
}

void MainWindow::on_selectionFile_clicked() {
  file_path = QFileDialog::getOpenFileName(this, "Выбрать файл", "",
                                           "Wavefront OBJ файлы (*.obj)");
  if (!file_path.isEmpty()) {
    if (open_file == 1 && (ui->label_file->text() != "File incorrect")) {
      glWidget->ClearContent();
    }
    open_file = 1;  // Для отчистки предыдущего объекта
    QFileInfo fileInfo(file_path);
    if (fileInfo.exists()) {
      obj_path = file_path;
      QString fileName = fileInfo.fileName();
      ui->label_file->setText(fileName);
      emit fileSelected(file_path);
    } else {
      ui->label_file->setText("File doesn't exist");
      open_file = 0;
    }
  } else {
    ui->label_file->setText("File selection canceled");
    open_file = 0;
  }
}

void MainWindow::onPushButtonRotateClicked() {
  if (file_path.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xProtat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yProtat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zProtat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMrotat) {
      labelValue = ui->lineEdit_Xrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMrotat) {
      labelValue = ui->lineEdit_Yrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMrotat) {
      labelValue = ui->lineEdit_Zrotat->text().toDouble();
      glWidget->RotateModel(labelValue, 'Z');
    }
  }
}

void MainWindow::onPushButtonMoveClicked() {
  if (file_path.isEmpty()) {
    return;
  }
  double labelValue = 0;
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton) {
    if (clickedButton == ui->pushButton_xPmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'x');
    } else if (clickedButton == ui->pushButton_yPmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'y');
    } else if (clickedButton == ui->pushButton_zPmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'z');
    } else if (clickedButton == ui->pushButton_xMmove) {
      labelValue = ui->lineEdit_xMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'X');
    } else if (clickedButton == ui->pushButton_yMmove) {
      labelValue = ui->lineEdit_yMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'Y');
    } else if (clickedButton == ui->pushButton_zMmove) {
      labelValue = ui->lineEdit_zMove->text().toDouble();
      glWidget->MoveModel(labelValue, 'Z');
    }
  }
}

void MainWindow::onColorButtonFoneClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    back_color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (back_color.isValid()) {
      glWidget->SetBackgroundColor(back_color);
      ui->colorFon_2->setStyleSheet("background-color: " + back_color.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonLineClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    color_line = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color_line.isValid()) {
      glWidget->SetColorLineVer(color_line, true);
      ui->colorLine_2->setStyleSheet("background-color: " + color_line.name() +
                                     ";border-radius: 10px");
    }
  }
}

void MainWindow::onColorButtonVerClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton && glWidget) {
    color_ver = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
    if (color_ver.isValid()) {
      glWidget->SetColorLineVer(color_ver, false);
      ui->colorVer_2->setStyleSheet("background-color: " + color_ver.name() +
                                    ";border-radius: 10px");
    }
  }
}

void MainWindow::TransferVerticesFacets(int count_vertex, int count_facets) {
  ui->label_top->setText(QString::number(count_vertex));
  ui->label_ribs->setText(QString::number(count_facets));
}

void MainWindow::TransferFileIncorrect(QString error_message) {
  ui->label_file->setText(error_message);
}

void MainWindow::ScaleModelFromSpinBox(double scale_factor) {
  try {
    glWidget->ScaleModelToFit(scale_factor);
  } catch (const std::runtime_error &e) {
    ui->doubleSpinBox_scal->setValue(1);
  }
  update();
}

void MainWindow::IntervalLines(double interval_value) {
  glWidget->EditIntervalLines(interval_value);
  update();
}

void MainWindow::ThicknessLines(double thickness_value) {
  glWidget->EditThicknessLines(thickness_value);
  update();
}

void MainWindow::SizeVer(double size_ver) {
  glWidget->EditSizeVer(size_ver);
  update();
}

void MainWindow::onLineRadioButtonClicked(bool is_checked_line) {
  if (is_checked_line) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->dotted_line) {
      glWidget->SetLineStyle(true);
    } else if (selectedRadioButton == ui->solid_line) {
      glWidget->SetLineStyle(false);
    }
    update();
  }
}

void MainWindow::onVerRadioButtonClicked(bool is_checked_ver) {
  if (is_checked_ver) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->Circle) {
      glWidget->VerStyle(1);
    } else if (selectedRadioButton == ui->Square) {
      glWidget->VerStyle(2);
    } else if (selectedRadioButton == ui->No_ver) {
      glWidget->VerStyle(0);
    }
    update();
  }
}

void MainWindow::onSaveBMPButtonClicked() {
  QString file_path =
      QFileDialog::getSaveFileName(this, "Save Image", "", "Bitmap (*.bmp)");
  if (!file_path.isEmpty()) {
    glWidget->SaveImage(file_path);
  }
}

void MainWindow::onSaveJPEGButtonClicked() {
  QString file_path =
      QFileDialog::getSaveFileName(this, "Save Image", "", "JPEG (*.jpeg)");
  if (!file_path.isEmpty()) {
    glWidget->SaveImage(file_path);
  }
}

void MainWindow::onPushButtonGifClicked() {
  QString gif_path =
      QFileDialog::getSaveFileName(this, "", "", "GIF Files (*.gif)");
  bool success = true;
  if (gif_path.isEmpty()) {
    success = false;
  }
  if (success == true) {
    const int frame_count = 60;  // Количество кадров в GIF
    const int delay = 10;  // Задержка между кадрами (в сотых долях секунды)
    const int gif_width = glWidget->width();
    const int gif_height = glWidget->height();
    GifWriter gif_writer;
    if (!GifBegin(&gif_writer, gif_path.toStdString().c_str(), gif_width,
                  gif_height, delay)) {
      return;
    }
    for (int i = 0; i < frame_count; ++i) {
      glWidget->RotateModel(360.0 / frame_count, 'y');
      QImage frame = glWidget->grabFramebuffer();
      frame = frame.scaled(gif_width, gif_height, Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
      std::vector<uint8_t> pixels(gif_width * gif_height * 4);
      for (int y = 0; y < gif_height; ++y) {
        for (int x = 0; x < gif_width; ++x) {
          QColor color = frame.pixelColor(x, y);
          size_t index = (y * gif_width + x) * 4;
          pixels[index + 0] = color.red();
          pixels[index + 1] = color.green();
          pixels[index + 2] = color.blue();
          pixels[index + 3] = 255;
        }
      }
      GifWriteFrame(&gif_writer, pixels.data(), gif_width, gif_height, delay);
    }
    GifEnd(&gif_writer);
  }
}

void MainWindow::TranslateProjectionType(bool is_check_projection) {
  if (is_check_projection) {
    QRadioButton *selectedRadioButton = qobject_cast<QRadioButton *>(sender());
    if (selectedRadioButton == ui->typeCenter) {
      glWidget->SetProjectionType(0);
    } else if (selectedRadioButton == ui->typeParalel) {
      glWidget->SetProjectionType(1);
    }
    update();
  }
}

void MainWindow::ClearAllFunc() {
  if (open_file != 2) {
    if (!file_path.isEmpty() && open_file == 1 &&
        (ui->label_file->text() != "File incorrect")) {
      glWidget->ClearContent();
      open_file = 0;
    }
    obj_path = "";
    ui->label_file->setText("");

    ui->lineEdit_Xrotat->setText("0.0175");
    ui->lineEdit_Yrotat->setText("0.0175");
    ui->lineEdit_Zrotat->setText("0.0175");

    ui->lineEdit_xMove->setText("0.25");
    ui->lineEdit_yMove->setText("0.25");
    ui->lineEdit_zMove->setText("0.25");

    ui->doubleSpinBox_interval->setValue(1.00);
    ui->doubleSpinBox_scal->setValue(1.00);
    ui->doubleSpinBox_size->setValue(1.00);
    ui->doubleSpinBox_thickness->setValue(1.00);

    ui->colorFon_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px");
    ui->colorLine_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");
    ui->colorVer_2->setStyleSheet(
        "background-color: rgb(231, 230, 232); border-radius: 10px; border: "
        "2px solid #8f8f91;");

    glWidget->SetBackgroundColor(Qt::black);
    glWidget->SetColorLineVer(Qt::red, false);
    glWidget->SetColorLineVer(Qt::gray, true);

    open_file = 2;  // Не дает использоваать очистку дважды

    ui->label_ribs->setText("");
    ui->label_top->setText("");

    back_color = Qt::black;
    color_line = QColor("#9a9996");
    color_ver = QColor("#9a9996");
    glWidget->update();
  }
}

void MainWindow::saveMatrix(const std::vector<std::vector<double>> &matrix) {
  QStringList serializedMatrix;
  for (const auto &row : matrix) {
    QStringList rowItems;
    for (const auto &value : row) {
      rowItems.append(QString::number(value));
    }
    serializedMatrix.append(rowItems.join(","));
  }
  settings_.setValue("matrix_3d", serializedMatrix.join(";"));
}

std::vector<std::vector<double>> MainWindow::loadMatrix() {
  std::vector<std::vector<double>> matrix;
  QString serializedData = settings_.value("matrix_3d").toString();
  QStringList rows = serializedData.split(";");

  for (const auto &rowStr : rows) {
    QStringList rowItems = rowStr.split(",");
    std::vector<double> row;
    for (const auto &item : rowItems) {
      row.push_back(item.toDouble());
    }
    matrix.push_back(row);
  }
  return matrix;
}

void MainWindow::SetSaivedBackColor() {
  back_color = settings_.value("back_color", QColor(Qt::white)).value<QColor>();
  if (back_color.isValid()) {
    glWidget->SetBackgroundColor(back_color);
    ui->colorFon_2->setStyleSheet("background-color: " + back_color.name() +
                                  ";border-radius: 10px");
  }
}

void MainWindow::SetSaivedLinVerColor() {
  color_line = settings_.value("color_line", QColor(Qt::white)).value<QColor>();
  if (color_line.isValid()) {
    glWidget->SetColorLineVer(color_line, true);
    ui->colorLine_2->setStyleSheet("background-color: " + color_line.name() +
                                   ";border-radius: 10px");
  }
  color_ver = settings_.value("color_ver", QColor(Qt::white)).value<QColor>();
  if (color_ver.isValid()) {
    glWidget->SetColorLineVer(color_ver, false);
    ui->colorVer_2->setStyleSheet("background-color: " + color_ver.name() +
                                  ";border-radius: 10px");
  }
}

void MainWindow::saveSettings() {
  settings_.beginGroup("MainWindow");
  settings_.setValue("geometry", saveGeometry());
  settings_.setValue("windowState", saveState());
  settings_.setValue("open file", open_file);
  settings_.setValue("filePath", obj_path);
  settings_.setValue("fileName", ui->label_file->text());

  settings_.setValue("scale", ui->doubleSpinBox_scal->value());
  settings_.setValue("size", ui->doubleSpinBox_size->value());
  settings_.setValue("thickness", ui->doubleSpinBox_thickness->value());
  settings_.setValue("interval", ui->doubleSpinBox_interval->value());

  settings_.setValue("back_color", back_color.name());
  settings_.setValue("color_line", color_line.name());
  settings_.setValue("color_ver", color_ver.name());
  settings_.setValue("Xrotat", ui->lineEdit_Xrotat->text());
  settings_.setValue("Yrotat", ui->lineEdit_Yrotat->text());
  settings_.setValue("Zrotat", ui->lineEdit_Zrotat->text());
  settings_.setValue("xMove", ui->lineEdit_xMove->text());
  settings_.setValue("yMove", ui->lineEdit_yMove->text());
  settings_.setValue("zMove", ui->lineEdit_zMove->text());

  ui->doubleSpinBox_interval->setValue(1.00);
  saveMatrix(controller_->GetMatrix3D());
  settings_.endGroup();
}

void MainWindow::loadSettings() {
  QString settingsPath = QDir::currentPath() + "/settings.ini";
  if (!QFile::exists(settingsPath)) {
    return;
  }
  settings_.beginGroup("MainWindow");
  restoreGeometry(settings_.value("geometry").toByteArray());
  restoreState(settings_.value("windowState").toByteArray());
  open_file = settings_.value("open file").toInt();
  if (open_file == 2) {
    open_file = 0;
  }
  file_path = settings_.value("filePath", "").toString();
  obj_path = file_path;
  emit fileSelected(file_path);
  controller_->SetMatrix3D(loadMatrix());
  ui->label_file->setText(settings_.value("fileName").toString());

  ui->doubleSpinBox_scal->setValue(settings_.value("scale").toInt());
  ui->doubleSpinBox_size->setValue(settings_.value("size").toInt());
  ui->doubleSpinBox_thickness->setValue(settings_.value("thickness").toInt());
  ui->doubleSpinBox_interval->setValue(settings_.value("interval").toInt());

  back_color = settings_.value("back_color", QColor(Qt::white)).value<QColor>();
  SetSaivedBackColor();
  SetSaivedLinVerColor();
  ui->lineEdit_Xrotat->setText(settings_.value("Xrotat").toString());
  ui->lineEdit_Yrotat->setText(settings_.value("Yrotat").toString());
  ui->lineEdit_Zrotat->setText(settings_.value("Zrotat").toString());
  ui->lineEdit_xMove->setText(settings_.value("xMove").toString());
  ui->lineEdit_yMove->setText(settings_.value("yMove").toString());
  ui->lineEdit_zMove->setText(settings_.value("zMove").toString());
  settings_.endGroup();
}
