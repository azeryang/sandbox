#pragma once

#include "base/basictypes.h"
#include "azer/render/render.h"
#include "azer/render/util/tex_render_target.h"

class WaterSimulation {
 public:
  WaterSimulation() : index_(0) {}
  bool Init(azer::RenderSystem* rs);
  void Render(azer::Renderer* renderer, double time, float delta);
 private:
  void CalcSimulation(double time, float delta);
  void RenderPerturb(azer::Renderer* renderer, double time, float delta);
  bool InitVertex(azer::RenderSystem* rs);
  int index_;
  azer::TexRenderTargetPtr target_[3];
  azer::VertexBufferPtr vb_;
  azer::IndicesBufferPtr ib_;
  azer::EffectPtr effect_;
  azer::EffectPtr quad_effect_;
  azer::EffectPtr perturb_effect_;
  azer::TexturePtr dampening_tex_;
  azer::TexturePtr brush_tex_;
  DISALLOW_COPY_AND_ASSIGN(WaterSimulation);
};
