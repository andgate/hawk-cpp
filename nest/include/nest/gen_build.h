#pragma once

#include "nest/Config.h"

#include <hkc/Build.h>

#include <memory>

namespace Nest	
{
	std::unique_ptr<hkc::Build> gen_build(Config_ptr& nest_conf);
}