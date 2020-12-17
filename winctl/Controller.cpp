#include <winctl/Controller.hpp>


#include <stdexcept>


using namespace winctl;


static IDirectInput8& getDirectInput() {
    static auto ptr = []() {
        IDirectInput8* buf = nullptr;
        if (FAILED(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, &reinterpret_cast<LPVOID>(buf), nullptr))) {
            throw std::runtime_error("Could not create DirectInput8");
        }

        return DirectInputPtr(buf);
    }();

    return *ptr;
}



namespace winctl {
    struct Controller::AcquireGuard {
        AcquireGuard(IDirectInputDevice8* ptr)
            :_device(ptr) {
            }
        ~AcquireGuard() {
            if (_device) {
                _device->Unacquire();
            }
        }
        IDirectInputDevice8* _device;
    };

    Controller::Controller(ControllerId id)
        :_id(std::move(id)) {
        IDirectInputDevice8* ptr = nullptr;
        if (FAILED(getDirectInput().CreateDevice(id.guidInstance, &ptr, nullptr))) {
            throw std::runtime_error("Could not initialize direct input device");
        }
        _device = DirectInputDevicePtr(ptr);

        if (FAILED(_device->SetDataFormat(&c_dfDIJoystick))) {
            throw std::runtime_error("Could not set data format");
        }
    }

    DIJOYSTATE Controller::getState() const {
        DIJOYSTATE buffer;

        // TODO: should we acquire on every frame? Or do we acquire, keep and just poll?
        auto guard = _acquire();

        if (FAILED(_device->Poll())) {
            throw std::runtime_error("Could not poll device");
        }
        if (FAILED(_device->GetDeviceState(sizeof(buffer), reinterpret_cast<void*>(&buffer)))) {
            throw std::runtime_error("Could not get device state");
        }

        return buffer;
    }

    [[no_discard]] Controller::AcquireGuard Controller::_acquire() const {
        if (FAILED(_device->Acquire())) {
            throw std::runtime_error("Could not acquire device");
        }
        return AcquireGuard(_device.get());
    }
}

static BOOL enumerateControllersCallback(LPCDIDEVICEINSTANCE device, LPVOID ref) {
    auto& buffer = *reinterpret_cast<std::vector<ControllerId>*>(ref);

    buffer.push_back(*device);

    return DIENUM_CONTINUE;
}

std::vector<ControllerId>(::winctl::enumerateControllers)() {
    std::vector<ControllerId> buffer;

    if (FAILED(getDirectInput().EnumDevices(DI8DEVCLASS_GAMECTRL, enumerateControllersCallback, &buffer, DIEDFL_ATTACHEDONLY))) {
        throw std::runtime_error("Could not enumerate controllers");
    }

    return buffer;

}
