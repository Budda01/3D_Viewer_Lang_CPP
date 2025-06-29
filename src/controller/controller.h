#ifndef CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H
#define CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H

#include <stdexcept>
#include <string>

#include "../model/command.h"
#include "../model/model.h"

namespace s21 {

class Controller {
 public:
  static Controller& getInstance(Model* model = nullptr) {
    static Controller instance(model);
    return instance;
  }

  void LoadModel(const std::string& file_path);
  void RotateModel(double step, char xyz);
  void ApplyRotation();
  void MoveModel(double distance, char xyz);
  void CenterModel();
  void ScaleModelToFit(double scale_factor);
  void ClearModelData();
  int GetVertexCount() const { return model_->GetVertexCount(); }
  int GetFacetCount() const { return model_->GetFacetCount(); }
  const std::vector<std::vector<double>>& GetMatrix3D() const {
    return model_->GetMatrix3D();
  }
  void SetMatrix3D(const std::vector<std::vector<double>>& matrix) {
    model_->SetMatrix3D(matrix);
  }
  const std::vector<Facet>& GetPolygons() const {
    return model_->GetPolygons();
  }

 private:
  Controller(Model* model) : model_(model) {}
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

  Model* model_;
};

}  // namespace s21

#endif  // CPP4_S21_3DVIEWER_V2_SRC_CONTROLLER_CONTROLLER_H
