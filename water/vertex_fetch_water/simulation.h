#pragma once

#include "base/basictypes.h"
#include "azer/render/render.h"
#include "azer/render/util/tex_render_target.h"

class WaterSimulation {
 public:
  WaterSimulation() : index_(0) {}
  bool Init(azer::RenderSystem* rs);
  void Render(azer::Renderer* renderer);
 private:
  void CalcSimulation();
  bool InitVertex(azer::RenderSystem* rs);
  int index_;
  azer::TexRenderTargetPtr target_[2];
  azer::VertexBufferPtr vb_;
  azer::IndicesBufferPtr ib_;
  azer::EffectPtr effect_;
  azer::EffectPtr quad_effect_;
  DISALLOW_COPY_AND_ASSIGN(WaterSimulation);
};
