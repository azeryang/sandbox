#pragma once

#include "sandbox/base/base.h"
#include "dispersion.afx.h"

class MyMesh : public Mesh {
 public:
  MyMesh() {}
  bool Init(MeshData* data, azer::RenderSystem* rs);
  virtual void UseEffect(const Group& group, azer::Renderer* r);

  virtual void UpdateVertex(void* desc, const MeshData::Vertex& vertex);
  virtual void UpdateAll(azer::VertexData* vdata, azer::IndicesData* idata);

  void OnUpdateScene(const azer::Camera& camera);
  void SetCubemap(azer::TexturePtr& ptr) { cubemap_ = ptr;}
 private:
  azer::TexturePtr cubemap_;
  std::unique_ptr<DispersionEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(MyMesh);
};

MyMesh* Load(const ::base::FilePath::StringType& path, azer::RenderSystem* rs);
