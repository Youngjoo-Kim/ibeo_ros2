//======================================================================
/*! \file IbeoDevice.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 24, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEODEVICE_HPP_SEEN
#define IBEOSDK_IBEODEVICE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoTcpIpDeviceBase.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>

#ifdef _WIN32
#include <ibeosdk/DataBlockRegister.hpp>
#endif // _WIN32

#include <iostream>
#include <vector>
#include <list>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoDevice
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jan 30, 2014
 * \tparam DeviceImpl   Implementation of the Device.
 *///-------------------------------------------------------------------
template <class DeviceImpl>
class IbeoDevice : public IbeoTcpIpDeviceBase {
public:
	//========================================
	/*!\brief
	 *
	 *///-------------------------------------
	IbeoDevice(const std::string& ip, const unsigned short port = 12002)
	  : IbeoTcpIpDeviceBase(ip, port),
	    dbbp(DataBlockBufferProviderGlobal<DeviceImpl>::getInstance())
	{
		logDebug << "Create Device:" << std::endl;
		logDebug << "Registered DataTypes: " << std::endl;

		std::list<DataBlock::DataBlockRegisterId::Key> dtList = getIdsOfSupportedDataBlocks();
		std::list<DataBlock::DataBlockRegisterId::Key>::const_iterator iter = dtList.begin();
		for (; iter != dtList.end(); ++iter) {
			logDebug << std::hex << "0x" << *iter << std::dec << std::endl;
		}
	}

	virtual ~IbeoDevice() {}

public:
	//========================================
	/*!\brief This method will be called by the receive thread
	 *        when a new DataBlock has been received completely.
	 *
	 * This class will call notifyListeners and notifyStreamers.
	 *
	 * \param[in]       dh  Header that came along with that
	 *                      DataBlock.
	 * \param[in, out]  is  Stream that contains the contents of
	 *                      the DataBlock that has been received.
	 *///-------------------------------------
	virtual const DataBlock* onData(const IbeoDataHeader& dh, std::istream& is)
	{
		if (dh.getDataType() == DataTypeId::DataType_Unknown) {
			logError << "Received data block id unknown." <<  std::endl;
			return NULL;
		}

		DataBlock* db1 = dbbp.getBuffer(dh.getDataType());
		if (db1) {
			if (const DataBlock* db = deserialize(is, *db1, dh))
				return db;
			else
				return NULL;
		}

		logDebug2 << "Ignoring unknown DataBlock "
				<< "0x" << std::hex << dh.getDataType() << std::dec
				<< " of size: " << dh.getMessageSize() << " bytes."
				<< std::endl;
			return NULL;
	}

public:
	//========================================
	/*!\brief
	 *
	 *///-------------------------------------
	std::list<DataBlock::DataBlockRegisterId::Key>
	getIdsOfSupportedDataBlocks() const
	{
		std::list<DataBlock::DataBlockRegisterId::Key> keyList;
#ifdef _WIN32
		typedef DataBlockBufferProvider<DeviceImpl>::ProviderMap PM;
		const PM& pm = dbbp.getMap();
		PM::const_iterator iter = pm.begin();
#else // _WIN32
		typedef typename DataBlockBufferProvider<DeviceImpl>::ProviderMap PM;
		const PM& pm = dbbp.getMap();
		typename PM::const_iterator iter = pm.begin();
#endif // _WIN32
		for (; iter != dbbp.getMap().end(); ++iter) {
			keyList.push_back(iter->first);
		} // for iter
		keyList.sort();
		return keyList;
	}

protected:
	DataBlockBufferProvider<DeviceImpl> dbbp;
}; // IbeoDevice
//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEODEVICE_HPP_SEEN

//======================================================================

