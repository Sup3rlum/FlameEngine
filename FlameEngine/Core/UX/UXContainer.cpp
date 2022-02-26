#include "UXContainer.h"

#include <AppCore/Platform.h>
#include <iostream>

#include "../Engine/GameSystem/ControlComponent.h"

using namespace ultralight;

ControlComponent control;

UXContainer::UXContainer(FRIContext* context) :
	FriContext(context)
{
	Config config;

	config.device_scale = 1.0;
	config.scroll_timer_delay = 1 / 165.0;
	config.animation_timer_delay = 1 / 165.0;

	config.resource_path = "./Assets/Resources/";
	config.use_gpu_renderer = false;

	Platform::instance().set_config(config);
	Platform::instance().set_font_loader(GetPlatformFontLoader());
	Platform::instance().set_file_system(GetPlatformFileSystem("./Assets"));

	Platform::instance().set_logger(this);
	Platform::instance().set_surface_factory(new UXFRISurfaceFactory(context));

	SoftwareRenderer = Renderer::Create();

	auto vp = context->GetViewport();
	ContainerView = SoftwareRenderer->CreateView(vp.Width, vp.Height, true, nullptr);

	ContainerView->set_view_listener(this);
	ContainerView->set_load_listener(this);

}

void UXContainer::LoadURL(const FString8& url)
{
	ContainerView->LoadURL(url.ToPlatformString());
}

void UXContainer::ExecuteScript(const FString8& script)
{
	ContainerView->EvaluateScript(script.ToPlatformString());
}

void UXContainer::LogMessage(LogLevel log_level, const String16& message) 
{
	String msg(message);
	std::cout << msg.utf8().data() << std::endl;
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
	int b = 5;

}

void UXContainer::OnFailLoading(ultralight::View* caller,
	uint64_t frame_id,
	bool is_main_frame,
	const String& url,
	const String& description,
	const String& error_domain,
	int error_code) 
{

	int a = 5;

}

void UXContainer::OnDOMReady(ultralight::View* caller,
	uint64_t frame_id,
	bool is_main_frame,
	const ultralight::String& url)
{

	RefPtr<JSContext> context = caller->LockJSContext();
	SetJSContext(context->ctx());

	JSObject global = JSGlobalObject();

	for (auto& [name, callback] : Callbacks)
	{
		global[name.ToPlatformString()] = callback;
	}
}


void UXContainer::HandleInput(FKeyboardKeys key, FKeyEvent keyEvent)
{
	// Synthesize a key press event for the 'Right Arrow' key
	KeyEvent evt;


	if (keyEvent == FKeyEvent::OnPress)
	{
		evt.type = KeyEvent::kType_RawKeyDown;
	}
	if (keyEvent == FKeyEvent::OnPress)
	{
		evt.type = KeyEvent::kType_KeyUp;
	}

	evt.virtual_key_code = (int)key;
	evt.native_key_code = 0;
	evt.modifiers = 0;

	// You'll need to generate a key identifier from the virtual key code
	// when synthesizing events. This function is provided in KeyEvent.h
	GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

	ContainerView->FireKeyEvent(evt);
}

void UXContainer::UpdateContainer()
{
	SoftwareRenderer->Render();
	SoftwareRenderer->Update();

	auto position = FriContext->GetCursorPosition();

	if (prevPosition != position)
	{
		MouseEvent evt;
		evt.type = MouseEvent::kType_MouseMoved;
		evt.x = position.x;
		evt.y = position.y - 28.0f;
		evt.button = MouseEvent::kButton_None;

		ContainerView->FireMouseEvent(evt);
	}

	prevPosition = position;


	if (control.IsMouseButtonDown(FMouseButton::Left) && !wasLMB)
	{
		MouseEvent evt;
		evt.type = MouseEvent::kType_MouseDown;
		evt.x = position.x;
		evt.y = position.y - 28.0f;
		evt.button = MouseEvent::kButton_Left;

		ContainerView->FireMouseEvent(evt);
	}
	if (!control.IsMouseButtonDown(FMouseButton::Left) && wasLMB)
	{
		MouseEvent evt;
		evt.type = MouseEvent::kType_MouseUp;
		evt.x = position.x;
		evt.y = position.y - 28.0f;
		evt.button = MouseEvent::kButton_Left;

		ContainerView->FireMouseEvent(evt);
	}

	wasLMB = control.IsMouseButtonDown(FMouseButton::Left);

}