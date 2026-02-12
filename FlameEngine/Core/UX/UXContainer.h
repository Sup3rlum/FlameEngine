#pragma once

#include "UXFRISurface.h"
#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#include <AppCore/Platform.h>
#include <AppCore/AppCore.h>

#include <tuple>

template<typename T>
struct JSValueConverter;

template<>
struct JSValueConverter<bool>
{
	bool operator()(const ultralight::JSValue& val)
	{
		return val.ToBoolean();
	}
};

template<>
struct JSValueConverter<FString8>
{
	FString8 operator()(const ultralight::JSValue& val)
	{
		return ((ultralight::String)val.ToString()).utf8().data();
	}
};

template<>
struct JSValueConverter<int>
{
	int operator()(const ultralight::JSValue& val)
	{
		return val.ToInteger();
	}
};

template<>
struct JSValueConverter<float>
{
	float operator()(const ultralight::JSValue& val)
	{
		return val.ToNumber();
	}
};


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
	

	}

	virtual void OnFinishLoading(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const ultralight::String& url) override
	{

	}

	virtual void OnAddConsoleMessage(ultralight::View* caller,
		ultralight::MessageSource source,
		ultralight::MessageLevel level,
		const ultralight::String& message,
		uint32_t line_number,
		uint32_t column_number,
		const ultralight::String& source_id) override;

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


	template <typename TReturn, typename... TArgs>
	void SetCallback(const FString8& str, const FDelegate<TReturn(TArgs...)>& lambda) {
		// Factory that generates indices [0,1,2,...,N-1]
		auto makeWrapper = [=]<std::size_t... Is>(std::index_sequence<Is...>) {
			return [=](const ultralight::JSObject&, const ultralight::JSArgs& args) {
				// Expand args[Is] aligned with TArgs...
				lambda(JSValueConverter<TArgs>{}(args[Is])...);
				};
		};

		auto wrapper = makeWrapper(std::index_sequence_for<TArgs...>{});

		if (!Callbacks.Contains(str)) {
			Callbacks.Set(str, ultralight::JSCallback(wrapper));
		}

		ultralight::JSObject global = ultralight::JSGlobalObject();
		global[str.ToPlatformString()] = ultralight::JSCallback(wrapper);
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