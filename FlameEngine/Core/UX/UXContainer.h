#pragma once

#include "UXFRISurface.h"
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#include <AppCore/Platform.h>
#include <AppCore/AppCore.h>

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

	void UpdateContainer();
	void HandleInput(FKeyboardKeys key, FKeyEvent keyEvent);

	void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;

	ultralight::RefPtr<ultralight::View> OnCreateChildView(ultralight::View* caller,
		const ultralight::String& opener_url,
		const ultralight::String& target_url,
		bool is_popup,
		const ultralight::IntRect& popup_rect) override;

	/////////////////////////////////////////////////////////////////////////////
	/// Inherited from LoadListener:                                          ///
	/////////////////////////////////////////////////////////////////////////////

	virtual void OnFailLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url,
		const ultralight::String& description,
		const ultralight::String& error_domain,
		int error_code) override;
	

	virtual void OnBeginLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url) override
	{
	
		int c = 5;
	}


	virtual void OnFinishLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url) override
	{
		int d = 5;
	}

	virtual void OnDOMReady(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url) override;

	/////////////////////////////////////////////////////////////////////////////
	/// Inherited from Logger:                                                ///
	/////////////////////////////////////////////////////////////////////////////

	virtual void LogMessage(ultralight::LogLevel log_level,
		const ultralight::String16& message) override;

	void LoadURL(const FString8& url);
	void ExecuteScript(const FString8& script);

	void SetCallback(const FString8& str, const FDelegate<void(void)>& lambda)
	{
		if (!Callbacks.Contains(str))
		{
			Callbacks.Set(str, ultralight::JSCallback([=](const ultralight::JSObject& obj, const ultralight::JSArgs& arg) { lambda(); }));
		}

		ultralight::JSObject global = ultralight::JSGlobalObject();
		global[str.ToPlatformString()] = ultralight::JSCallback([=](const ultralight::JSObject& obj, const ultralight::JSArgs& arg) { lambda(); });

	}
	ultralight::RefPtr<ultralight::View> ContainerView;
	UXFRISurfaceFactory* surfaceFactory;
	ultralight::RefPtr<ultralight::Renderer> SoftwareRenderer;
	
	FRIContext* FriContext;

	FVector2 prevPosition;

	bool wasLMB;
	bool wasRMB;

	FHashMap<FString8, ultralight::JSCallback> Callbacks;

};