#ifndef CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H
#define CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H

#include "model.h"

namespace s21 {

class Command {
 public:
  virtual void execute() = 0;
};

class RotateCommand : public Command {
 public:
  RotateCommand(Model* m, double s, char a) : model(m), step(s), axis(a) {}
  void execute() override { model->RotateModel(step, axis); }

 private:
  Model* model;
  double step;
  char axis;
};

class MoveCommand : public Command {
 public:
  MoveCommand(Model* m, double d, char a) : model(m), distance(d), axis(a) {}
  void execute() override { model->MoveModel(distance, axis); }

 private:
  Model* model;
  double distance;
  char axis;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_MODEL_COMAND_H
