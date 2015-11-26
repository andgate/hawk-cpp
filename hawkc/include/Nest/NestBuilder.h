#pragma once

#include "Nest.h"
#include "Nest/NestConf.h"

#include <memory>

namespace NestBuilder
{
  std::shared_ptr<Nest> build(std::shared_ptr<NestConf>);
}
