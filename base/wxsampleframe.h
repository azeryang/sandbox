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
class AzerCanvas : public wxWindow {
 public:
  AzerCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize, long style = 0,
             const wxString& name = wxT("TestGLCanvas"))
      :  wxWindow(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name) {
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(AzerCanvas);
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

  AzerCanvas* panel_;
  wxSampleApp::Delegate* delegate_;
  bool render_loop_on_;

  // render time
  azer::RenderSystem* render_system_;
  ::base::TimeDelta total_ticking_;
  base::TimeTicks prev_frame_;
  double prev_time_;
  float prev_delta_;
  azer::WindowHost host_;
  int64 frame_cnt_;
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

