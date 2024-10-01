#ifndef _ROUTERS_HPP_
#define _ROUTERS_HPP_

#include "HttpService.h"
#include "User.hpp"

namespace route
{
    void RegisterResources(hv::HttpService &router);
}

#endif