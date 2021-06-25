#ifndef WINDY_SETTINGS_H
#define WINDY_SETTINGS_H

#include "ImageFormat.h"

namespace windy {
	class Settings {
	public:
		static ImageFormat TextureFormat;
	};
}

windy::ImageFormat windy::Settings::TextureFormat = windy::ImageFormat::PVR;

#endif
