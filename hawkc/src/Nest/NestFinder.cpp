#include "Nest/NestFinder.h"

std::shared_ptr<NestConf> NestFinder::findInRunDir()
{
  return std::make_shared<NestConf>();
}
