#include "data_link_layer.h"

#include "bits.h"
#include "platform.h"
#include "device_object.h"
#include "cemi_server.h"
#include "cemi_frame.h"


void DataLinkLayerCallbacks::activity(uint8_t info)
{
    if(_activityCallback)
        _activityCallback(info);
}

void DataLinkLayerCallbacks::setActivityCallback(ActivityCallback activityCallback)
{
    _activityCallback = activityCallback;
}

DataLinkLayer::DataLinkLayer(DeviceObject& devObj, NetworkLayerEntity& netLayerEntity, Platform& platform) :
    _deviceObject(devObj), _networkLayerEntity(netLayerEntity), _platform(platform)
{
#ifdef KNX_ACTIVITYCALLBACK
    _netIndex = netLayerEntity.getEntityIndex();
#endif
}

#ifdef USE_CEMI_SERVER

void DataLinkLayer::cemiServer(CemiServer& cemiServer)
{
    _cemiServer = &cemiServer;
}

void DataLinkLayer::dataRequestFromTunnel(CemiFrame& frame)
{
    _cemiServer->dataConfirmationToTunnel(frame);

    frame.messageCode(L_data_ind);
    
    // Send to local stack
    frameReceived(frame);

    // Send to KNX medium
    sendFrame(frame);
}
#endif

void DataLinkLayer::dataRequest(AckType ack, AddressType addrType, uint16_t destinationAddr, uint16_t sourceAddr, FrameFormat format, Priority priority, NPDU& npdu)
{
    // Normal data requests and broadcasts will always be transmitted as (domain) broadcast with domain address for open media (e.g. RF medium) 
    // The domain address "simulates" a closed medium (such as TP) on an open medium (such as RF or PL)
    // See 3.2.5 p.22
    sendTelegram(npdu, ack, destinationAddr, addrType, sourceAddr, format, priority, Broadcast);
}

void DataLinkLayer::systemBroadcastRequest(AckType ack, FrameFormat format, Priority priority, NPDU& npdu, uint16_t sourceAddr)
{
    // System Broadcast requests will always be transmitted as broadcast with KNX serial number for open media (e.g. RF medium) 
    // See 3.2.5 p.22
    sendTelegram(npdu, ack, 0, GroupAddress, sourceAddr, format, priority, SysBroadcast);
}

void DataLinkLayer::dataConReceived(CemiFrame& frame, bool success)
{
    MessageCode backupMsgCode = frame.messageCode();
    frame.messageCode(L_data_con);
    frame.confirm(success ? ConfirmNoError : ConfirmError);
    AckType ack = frame.ack();
    AddressType addrType = frame.addressType();
    uint16_t destination = frame.destinationAddress();
    uint16_t source = frame.sourceAddress();
    FrameFormat type = frame.frameType();
    Priority priority = frame.priority();
    NPDU& npdu = frame.npdu();
    SystemBroadcast systemBroadcast = frame.systemBroadcast();

    //print("dataConReceived: ");Serial.println(frame.toString().c_str());

#ifdef USE_CEMI_SERVER
    // if the confirmation was caused by a tunnel request then
    // do not send it to the local stack
    if (frame.sourceAddress() == _cemiServer->clientAddress())
    {
        // Stop processing here and do NOT send it the local network layer
        return;
    }
#endif    

    if (addrType == GroupAddress && destination == 0)
            if (systemBroadcast == SysBroadcast)
                _networkLayerEntity.systemBroadcastConfirm(ack, type, priority, source, npdu, success);
            else
                _networkLayerEntity.broadcastConfirm(ack, type, priority, source, npdu, success);
    else
        _networkLayerEntity.dataConfirm(ack, addrType, destination, type, priority, source, npdu, success);

    frame.messageCode(backupMsgCode);
}

void DataLinkLayer::frameReceived(CemiFrame& frame)
{
    AckType ack = frame.ack();
    AddressType addrType = frame.addressType();
    uint16_t destination = frame.destinationAddress();
    //destination = 0;
    uint16_t source = frame.sourceAddress();
    FrameFormat type = frame.frameType();
    Priority priority = frame.priority();
    NPDU& npdu = frame.npdu();
    uint16_t ownAddr = _deviceObject.individualAddress();
    SystemBroadcast systemBroadcast = frame.systemBroadcast();


    // Debug statements
    /*
    Serial.println(" ");    
    Serial.println("--------------------------------------------");
    Serial.println("CemiFrame Received: ");
    printf("AckType: %d\n", ack);
    printf("AddressType: %d\n", addrType);
    printf("Destination: %u\n", destination);
    printf("Source: %u\n", source);
    printf("FrameFormat: %d\n", type);
    printf("Priority: %d\n", priority);
    printf("NPDU: %p\n", &npdu); // Assuming NPDU is a complex type, printing the address
    printf("Own Address: %u\n", ownAddr);
    printf("SystemBroadcast: %d\n", systemBroadcast);
    */
    //-------------------------------------
        // Debug-Print für den Frame
          /*
        
        for (int i = 0; i < frame.dataLength(); ++i) {
            if (frame.data()[i] < 0x10) {
                Serial.print("0");
            }
            Serial.print(frame.data()[i], HEX);
            Serial.print(" ");
        }
        Serial.println(); 
         */
    //-------------------------------------

    // Debug-Print für den Frame
      /*
    Serial.println("CemiFrame Received: ");
    Serial.println(frame.toStringdecode().c_str());
    Serial.println("--------------------------------------------");
    Serial.println(" ");    
     */
    //-------------------------------------




#ifdef USE_CEMI_SERVER
    // Do not send our own message back to the tunnel
    if (frame.sourceAddress() != _cemiServer->clientAddress())
    {
        _cemiServer->dataIndicationToTunnel(frame);
    }
#endif

    if (source == ownAddr) {
        //printf("Source is own address: %u\n", source);
        _deviceObject.individualAddressDuplication(true);
    }

    if (addrType == GroupAddress && destination == 0) {
        //printf("AddressType is GroupAddress and Destination is 0\n");
        if (systemBroadcast == SysBroadcast) {
            //printf("SystemBroadcast is SysBroadcast\n");
            _networkLayerEntity.systemBroadcastIndication(ack, type, npdu, priority, source);
        } else {
            //printf("SystemBroadcast is not SysBroadcast\n");
            _networkLayerEntity.broadcastIndication(ack, type, npdu, priority, source);
        }
    } else {
        //printf("AddressType is not GroupAddress or Destination is not 0\n");
        _networkLayerEntity.dataIndication(ack, addrType, destination, type, npdu, priority, source);
    }
}

bool DataLinkLayer::sendTelegram(NPDU & npdu, AckType ack, uint16_t destinationAddr, AddressType addrType, uint16_t sourceAddr, FrameFormat format, Priority priority, SystemBroadcast systemBroadcast)
{
    CemiFrame& frame = npdu.frame();
    frame.messageCode(L_data_ind);
    frame.destinationAddress(destinationAddr);
    frame.sourceAddress(sourceAddr);
    frame.addressType(addrType);
    frame.priority(priority);
    frame.repetition(RepetitionAllowed);
    frame.systemBroadcast(systemBroadcast);

    // Debug statements
    /*
    Serial.println(" ");
    Serial.println("--------------------------------------------");
    Serial.println("CemiFrame sendTelegram: ");
    printf("Destination Address: %u\n", destinationAddr);
    printf("Source Address: %u\n", sourceAddr);
    printf("Address Type: %d\n", addrType);
    printf("Priority: %d\n", priority);
    printf("System Broadcast: %d\n", systemBroadcast);
    printf("NPDU Octet Count: %d\n", npdu.octetCount());

         */

    //-------------------------------------
        //print("sendTelegram: ");Serial.println(frame.toString().c_str());
        // Debug-Print für den Frame
           /*
        for (int i = 0; i < frame.dataLength(); ++i) {
            if (frame.data()[i] < 0x10) {
                Serial.print("0");
            }
            Serial.print(frame.data()[i], HEX);
            Serial.print(" ");
        }
        Serial.println();   
          */
    //-------------------------------------

    // Debug-Print für den Frame
      /*
    Serial.println("CemiFrame sendTelegram: ");
    Serial.println(frame.toStringdecode().c_str());
    Serial.println("--------------------------------------------");
    Serial.println(" ");   
    */
    //-------------------------------------


    if (npdu.octetCount() <= 15) {
        frame.frameType(StandardFrame);
        //printf("Frame Type set to StandardFrame\n");
    } else {
        //printf("Frame Type before setting: %d\n", format);
        frame.frameType(format);
        //printf("Frame Type after setting: %d\n", frame.frameType());
    }

    if (!frame.valid())
    {
        //printf("Invalid frame\n");
        return false;
    }

//    if (frame.npdu().octetCount() > 0)
//    {
//        _print("<- DLL ");
//        frame.apdu().printPDU();
//    }

    // The data link layer might be an open media link layer
    // and will setup rfSerialOrDoA, rfInfo and rfLfn that we also 
    // have to send through the cEMI server tunnel
    // Thus, reuse the modified cEMI frame as "frame" is only passed by reference here!
    bool success = sendFrame(frame);

#ifdef USE_CEMI_SERVER
    CemiFrame tmpFrame(frame.data(), frame.totalLenght());
    // We can just copy the pointer for rfSerialOrDoA as sendFrame() sets
    // a pointer to const uint8_t data in either device object (serial) or
    // RF medium object (domain address)
#ifdef USE_RF
    tmpFrame.rfSerialOrDoA(frame.rfSerialOrDoA()); 
    tmpFrame.rfInfo(frame.rfInfo());
    tmpFrame.rfLfn(frame.rfLfn());
#endif
    tmpFrame.confirm(ConfirmNoError);
    _cemiServer->dataIndicationToTunnel(tmpFrame);
#endif

    //printf("Send Frame Success: %d\n", success);
    return success;
}

uint8_t* DataLinkLayer::frameData(CemiFrame& frame)
{
   
    // Debug-Ausgabe des Frames
    //print("frameData: ");Serial.println(frame.toString().c_str());
    return frame._data;
}


