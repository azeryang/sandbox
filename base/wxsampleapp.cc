#include "sandbox/base/wxsampleapp.h"
#include "sandbox/base/wxsampleframe.h"

wxSampleApp::wxSampleApp(wxSampleApp::Delegate* delegate, int width, int height,
                         const std::string& title)
    : delegate_(delegate)
    , width_(width)
    , height_(height)
    , title_(title) {
  app_ = new AzerRSApp(this);
}

void wxSampleApp::MainLoop(int argc, char* argv[], wxSampleApp* app) {
  wxApp::SetInstance(app->app_);
  wxEntryStart(argc, argv);
  app->app_->CallOnInit();
  app->app_->OnRun();
  wxEntryCleanup();
}

wxSampleFrame* wxSampleApp::Delegate::CreateFrame(const wxString& title,
                                                const wxPoint& pos,
                                                const wxSize& size) {
  return new wxSampleFrame(this, title, pos, size);
}
