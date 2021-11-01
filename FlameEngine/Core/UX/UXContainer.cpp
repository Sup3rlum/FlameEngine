#include "UXContainer.h"

#include <AppCore/Platform.h>

using namespace ultralight;

UXContainer::UXContainer(FRIContext* context) :
	FriContext(context)
{
	Config config;

	config.device_scale = 1.0;
	config.scroll_timer_delay = 1 / 165.0;
	config.animation_timer_delay = 1 / 165.0;

	config.resource_path = "./resources/";
	config.use_gpu_renderer = false;

	Platform::instance().set_config(config);
	Platform::instance().set_font_loader(GetPlatformFontLoader());
	Platform::instance().set_file_system(GetPlatformFileSystem("./assets"));

	Platform::instance().set_logger(this);
	Platform::instance().set_surface_factory(new UXFRISurfaceFactory(context));

	SoftwareRenderer = Renderer::Create();

	auto vp = context->GetViewport();
	ContainerView = SoftwareRenderer->CreateView(vp.Width, vp.Height, true, nullptr);

	ContainerView->set_view_listener(this);
	ContainerView->set_load_listener(this);


}

void UXContainer::LoadURL(const FAnsiString& url)
{
	ContainerView->LoadURL(url.ToPlatformString());
}

void UXContainer::ExecuteScript(const FAnsiString& script)
{
	ContainerView->EvaluateScript(script.ToPlatformString());
}

void UXContainer::LogMessage(LogLevel log_level, const String16& message) 
{
	String msg(message);
	//std::cout << msg.utf8().data() << std::endl;
}

RefPtr<View>  UXContainer::OnCreateChildView(ultralight::View* caller,
	const String& opener_url,
	const String& target_url,
	bool is_popup,
	const IntRect& popup_rect) 
{



	auto vp = FriContext->GetViewport();
	RefPtr<View> NewView = SoftwareRenderer->CreateView(vp.Width, vp.Height, true, nullptr);

	NewView->set_view_listener(this);
	NewView->set_load_listener(this);

	return NewView;
}

void  UXContainer::OnChangeCursor(ultralight::View* caller,
	Cursor cursor) 
{
	
}

void UXContainer::OnFailLoading(ultralight::View* caller,
	uint64_t frame_id,
	bool is_main_frame,
	const String& url,
	const String& description,
	const String& error_domain,
	int error_code) 
{
}

