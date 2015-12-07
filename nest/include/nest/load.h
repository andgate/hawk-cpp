#include "nest/Config.h"

#include <string>

namespace Nest
{
	// Assumes config_path is sanitized and exists
	// Intended to receive filepath from the Find.h module.
	Config_ptr load(const std::string& config_path);
}