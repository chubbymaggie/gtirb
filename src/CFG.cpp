#include <gtirb/CFG.hpp>
#include <gtirb/CFGNode.hpp>
#include <gtirb/CFGNodeInfo.hpp>
#include <gtirb/CFGSet.hpp>
#include <gtirb/EA.hpp>
#include <gtirb/Module.hpp>
#include <gtirb/RuntimeError.hpp>

using namespace gtirb;

BOOST_CLASS_EXPORT_IMPLEMENT(gtirb::CFG);

void CFG::setEA(EA x)
{
    this->ea = x;
}

EA CFG::getEA() const
{
    return this->ea;
}

void CFG::setProcedureName(std::string x)
{
    this->procedureName = x;
}

std::string CFG::getProcedureName() const
{
    return this->procedureName;
}

void CFG::setFlags(uint64_t x)
{
    this->flags = x;
}

uint64_t CFG::getFlags() const
{
    return this->flags;
}

const std::vector<std::shared_ptr<CFGNode>>& CFG::getNodes() const
{
    return this->nodes;
}
