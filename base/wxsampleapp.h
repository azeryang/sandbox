#pragma once

#include "base/basictypes.h"
#include "azer/render/render.h"
#include "azer/util/util.h"

class AzerRSApp;
class wxSampleFrame;
class wxPoint;
class wxSize;
class wxString;

class wxSampleApp {
 public:
  class Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}
    virtual bool OnInit() = 0;
    virtual void OnUpdateScene(double time, float delta_time) = 0;
    virtual void OnRenderScene(double time, float delta_time) = 0;
    virtual void OnQuit() = 0;
    virtual wxSampleFrame* CreateFrame(const wxString& title,
                                       const wxPoint& pos, const wxSize& size);
   protected:
    wxSampleApp* app_;

    friend class wxSampleApp;
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  wxSampleApp(Delegate* delegate, int width = 800, int height = 600,
              const std::string& title = "Render");
  static void MainLoop(int argc, char* argv[], wxSampleApp* app);

  int width() const { return width_;}
  int height() const { return height_;}
  const std::string& title() const { return title_;}
  Delegate* delegate() { return delegate_;}
 private:
  std::string title_;
  int width_;
  int height_;

  AzerRSApp* app_;
  Delegate* delegate_;
  azer::RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(wxSampleApp);
};
