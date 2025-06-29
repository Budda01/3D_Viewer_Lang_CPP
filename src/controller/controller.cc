#include "controller.h"

namespace s21 {

void Controller::LoadModel(const std::string& file_path) {
  model_->CountVerticesAndFacets(file_path);
  model_->ParseModelData(file_path);
}

void Controller::RotateModel(double step, char xyz) {
  RotateCommand cmd(model_, step, xyz);
  cmd.execute();
}

void Controller::ApplyRotation() { model_->ApplyRotation(); }

void Controller::MoveModel(double distance, char xyz) {
  MoveCommand cmd(model_, distance, xyz);
  cmd.execute();
}

void Controller::CenterModel() { model_->CenterModel(); }

void Controller::ScaleModelToFit(double scale_factor) {
  model_->ScaleModelToFit(scale_factor);
}

void Controller::ClearModelData() { model_->ClearData(); }

}  // namespace s21
