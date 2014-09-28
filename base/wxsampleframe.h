#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "azer/render/render.h"
#include "azer/util/util.h"
#include "base/time/time.h"
#include "sandbox/base/wxsampleapp.h"

class wxSampleApp;
class RenderPanel : public wxPanel {
 public:
  RenderPanel(wxFrame* frame) : wxPanel(frame) {}
 private:
  DISALLOW_COPY_AND_ASSIGN(RenderPanel);
};

class wxSampleFrame: public wxFrame {
 public:
  wxSampleFrame(wxSampleApp::Delegate* delegate, const wxString& title,
              const wxPoint& pos, const wxSize& size);

  virtual void CreateUI() {};
  virtual bool Init();
 private:
  void onIdle(wxIdleEvent& evt);

  bool CreateRender();

  RenderPanel* panel_;
  wxSampleApp::Delegate* delegate_;
  bool render_loop_on_;

  // render time
  azer::RenderSystem* render_system_;
  ::base::TimeDelta total_ticking_;
  base::TimeTicks prev_frame_;
  double prev_time_;
  float prev_delta_;
  azer::WindowHost host_;
  wxDECLARE_EVENT_TABLE();
};

class AzerRSApp: public wxApp {
 public:
  AzerRSApp(wxSampleApp* sample)
      : sample_(sample) {
  }

  virtual bool OnInit();
 private:
  wxSampleFrame* frame_;
  wxSampleApp* sample_;
  DISALLOW_COPY_AND_ASSIGN(AzerRSApp);
};

