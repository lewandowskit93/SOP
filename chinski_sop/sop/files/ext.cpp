#include <string>
#include <vector>
#include <cstdint>
#include ".\sop\files\ext.h"
#include ".\sop\files\block.h"
#include ".\sop\files\inode.h"
#include ".\sop\files\constev.h"
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>

sop::files::Ext::Ext()
{
  ConstEV x;
  this->dataBlocks = boost::make_shared<Block[]>(x.numOfBlocks);
}

sop::files::Ext::Ext(std::string driveFile)
{
}

sop::files::Ext::~Ext()
{
}