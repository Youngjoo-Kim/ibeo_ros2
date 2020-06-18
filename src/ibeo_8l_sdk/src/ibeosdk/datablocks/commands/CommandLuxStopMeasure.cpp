//======================================================================
/*! \file CommandLuxStopMeasure.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandLuxStopMeasure.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

CommandLuxStopMeasure::CommandLuxStopMeasure()
  : LuxCommand<CommandId::CmdLuxStopMeasure>(),
    m_reserved(0x0000)
{}

//======================================================================

bool CommandLuxStopMeasure::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const std::istream::pos_type StopPos = is.tellg();

	readLE(is, m_commandId);
	readLE(is, m_reserved);

	return !is.fail()
	       && ((is.tellg() - StopPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CommandLuxStopMeasure::serialize(std::ostream& os) const
{
	const std::istream::pos_type StopPos = os.tellp();

	writeLE(os, m_commandId);
	writeLE(os, m_reserved);

	return !os.fail() && ((os.tellp() - StopPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================