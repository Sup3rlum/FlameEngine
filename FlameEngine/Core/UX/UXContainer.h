#pragma once


#include "UXFRISurface.h"
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>

EXPORT(class, UXContainer) : public ultralight::ViewListener,
	public ultralight::LoadListener,
	public ultralight::Logger
{
public:

	UXContainer(FRIContext* context);

	UXFRISurface* GetSurface()
	{
		return static_cast<UltraLightFRISurface*>(ContainerView->surface());
	}

	ultralight::RefPtr<ultralight::View> GetView()
	{
		return ContainerView;
	}

	void Update()
	{
		SoftwareRenderer->Update();
		SoftwareRenderer->Render();
	}

	void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;

	ultralight::RefPtr<ultralight::View> OnCreateChildView(ultralight::View* caller,
		const ultralight::String& opener_url,
		const ultralight::String& target_url,
		bool is_popup,
		const ultralight::IntRect& popup_rect) override;

	/////////////////////////////////////////////////////////////////////////////
	/// Inherited from LoadListener:                                          ///
	/////////////////////////////////////////////////////////////////////////////

	void OnFailLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url,
		const ultralight::String& description,
		const ultralight::String& error_domain,
		int error_code) override;

	/////////////////////////////////////////////////////////////////////////////
	/// Inherited from Logger:                                                ///
	/////////////////////////////////////////////////////////////////////////////

	virtual void LogMessage(ultralight::LogLevel log_level,
		const ultralight::String16& message) override;

	void LoadURL(const FAnsiString& url);
	void ExecuteScript(const FAnsiString& script);

	ultralight::RefPtr<ultralight::View> ContainerView;
	UXFRISurfaceFactory* surfaceFactory;
	ultralight::RefPtr<ultralight::Renderer> SoftwareRenderer;
	
	FRIContext* FriContext;
};