#pragma once

#include <winctl/ControllerId.hpp>

#include <vector>
#include <memory>

namespace winctl {

class Controller final {
public:
    Controller(ControllerId id);

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    Controller(Controller&&) = default;
    Controller& operator=(Controller&&) = default;

    DIJOYSTATE getState() const;

private:
    struct AcquireGuard;
    [[no_discard]] AcquireGuard _acquire() const;

    ControllerId _id;
    DirectInputDevicePtr _device;
};

std::vector<ControllerId> enumerateControllers();

}