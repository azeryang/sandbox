#pragma once

#include "base/basictypes.h"
#include "azer/render/render.h"

class WaterSimulation {
 public:
  WaterSimulation();
  bool Init(azer::RenderSystem* rs);
  void Render(azer::Renderer* renderer);
 private:
  bool InitVertex(azer::RenderSystem* rs);
  azer::TexturePtr prev_;
  azer::TexturePtr curr_;
  azer::VertexBufferPtr vb_;
  azer::IndicesBuffer ib_;
  azer::EffectPtr effect_;
  DISALLOW_COPY_AND_ASSIGN(WaterSimulation);
};
