#pragma once

#include "nest/config.h"

#include <hkc/build.h>

#include <memory>

namespace Nest	
{
    hkc::Build_ptr gen_build(Config_ptr nest_conf);
}
