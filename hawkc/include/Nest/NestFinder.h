#pragma once

#include "Nest/NestConf.h"

#include <memory>

namespace NestFinder
{
  std::shared_ptr<NestConf> findInRunDir();
}
