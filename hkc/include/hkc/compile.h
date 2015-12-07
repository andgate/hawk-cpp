#pragma once

#include "hkc/build.h"

#include <string>
#include <memory>

namespace hkc
{
	void compile(std::unique_ptr<Build>& build);
}
