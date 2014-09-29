#pragma once

#include "sandbox/base/base.h"
#include "sandbox/base/afx/light.afx.h"
#include "azer/render/render.h"
#include "base/files/file_path.h"

class EnvMap {
 public:
  EnvMap(const ::base::FilePath& path)
      : path_(path) {
  }

  bool Init(azer::RenderSystem* rs);
  void OnUpdate(const azer::Camera& camera);
  void Render(azer::Renderer* renderer);

  class Skydome : public Mesh {
   public:
    Skydome(EnvMap* env) : envmap_(env) {}
    bool Init(MeshData* data, azer::RenderSystem* rs);

    virtual void UseEffect(const Group& group, azer::Renderer* r);
    virtual void UpdateVertex(void* desc, const MeshData::Vertex& vertex);
    virtual void UpdateAll(azer::VertexData* vdata, azer::IndicesData* idata);

    void OnUpdateScene(const azer::Camera& camera);
   private:
    azer::EffectPtr effect_;
    
    EnvMap* envmap_;
    friend class Skydome;
    DISALLOW_COPY_AND_ASSIGN(Skydome);
  };

  const ::base::FilePath& path() const { return path_; }
  azer::TexturePtr& cubemap() { return cubemap_;};
 private:
  azer::TexturePtr cubemap_;
  ::base::FilePath path_;
  std::unique_ptr<Skydome> skydome_;

  friend class Skydome;
  DISALLOW_COPY_AND_ASSIGN(EnvMap);
};
