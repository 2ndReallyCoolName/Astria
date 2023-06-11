#pragma once
// error exception helper macro
#define ASWND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__, hr )
#define ASWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() ) 
#define ASWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )