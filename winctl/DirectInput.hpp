#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <memory>

namespace winctl {
  
struct DIRelease {
    template<typename T>
    void operator()(T* ptr) {
        if (ptr) {
            ptr->Release();
        }
    }
};

using DirectInputPtr = std::unique_ptr<IDirectInput8, DIRelease>;
using DirectInputDevicePtr = std::unique_ptr<IDirectInputDevice8, DIRelease>;

}
