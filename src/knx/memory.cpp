#include "memory.h"

#include <string.h>

#include "bits.h"

uint8_t storedData[512]; // Array zum Zwischenspeichern der Daten

Memory::Memory(Platform& platform, DeviceObject& deviceObject)
    : _platform(platform), _deviceObject(deviceObject)
{}


Memory::~Memory()
{}


void Memory::readMemory()
{
    //println("readMemory");

    uint8_t* flashStart = _platform.getNonVolatileMemoryStart();
    size_t flashSize = _platform.getNonVolatileMemorySize();
    
    // Debug-Ausgabe für flashStart
    //Serial.print("flashStart: ");
    //Serial.println((uintptr_t)flashStart, HEX); // Ausgabe der Speicheradresse in Hex

    // Debug-Ausgabe für flashSize
    //Serial.print("flashSize: ");
    //Serial.println(flashSize); // Ausgabe der Größe
    
    if (flashStart == nullptr)
    {
        //println("no user flash available;");
        return;
            }
        
        /*       
           
RESTORED 00 01 00 FA 00 00 00 00 00 00 00 03 13 07 00 01 04 00 01 60 00 01 00 00 00 00 00 00 00 01 00 00 00 01 01 00 00 00 01 C0 A8 B2 AC 00 01 FF FF FF 00 00 01 C0 A8 B2 01 00 01 E0 00 17 0C 00 01 10 00 1E 52 61 64 6F 6E 73 65 6E 73 6F 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FA AB CD 
02 01 00 00 00 08 00 00 00 98 00 08 01 00 00 00 0A 00 00 00 A0 00 0C 01 00 00 00 0A 00 00 00 AC 00 0C 01 00 00 00 12 00 00 00 B8 00 14 FF FF FF FF FF FF FF 
// ohne einträge
RESTORED 00 01 00 FA 00 00 00 00 00 00 00 03 13 07 00 01 04 00 01 60 00 01 00 00 00 00 00 00 00 01 00 00 00 01 01 00 00 00 01 C0 A8 B2 AC 00 01 FF FF FF 00 00 01 C0 A8 B2 01 00 01 E0 00 17 0C 00 01 10 00 1E 52 61 64 6F 6E 73 65 6E 73 6F 72 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FA AB CD 
78 01 00 00 00 08 00 00 00 98 00 08 01 00 00 00 08 00 00 00 A0 00 08 01 00 00 00 08 00 00 00 A8 00 08 01 00 00 00 0E 00 00 00 B0 00 10 FF FF FF FF FF FF FF           
   */        
             /*  

                flashStart[0] = 0x00; // 0
                flashStart[1] = 0x01; // 1
                flashStart[2] = 0x00; // 0
                flashStart[3] = 0xFA; // 250
                flashStart[4] = 0x00; // 0
                flashStart[5] = 0x00; // 0
                flashStart[6] = 0x00; // 0
                flashStart[7] = 0x00; // 0
                flashStart[8] = 0x00; // 0
                flashStart[9] = 0x00; // 0
                flashStart[10] = 0x00; // 0
                flashStart[11] = 0x03; // 3

                //-************saveRestores*************************************************************
                flashStart[12] = 0x13; // 19    // 1.3.6   Bereichsnummer: 0001    Liniennummer: 0011   Kombiniert: 00010011 (in hexadezimal: 13)     
                flashStart[13] = 0x07; // 7     //         Teilnehmernummer: 00000110 (in hexadezimal: 06)
                                                
                // Konfigurationsparameter
                flashStart[14] = 0x00; // 0
                flashStart[15] = 0x01; // 1
                flashStart[16] = 0x04; // 4
                flashStart[17] = 0x00; // 0
                flashStart[18] = 0x01; // 1
                flashStart[19] = 0x60; // 96

                flashStart[20] = 0x00; // 0
                flashStart[21] = 0x01; // 1
                flashStart[22] = 0x00; // 0
                flashStart[23] = 0x00; // 0
                flashStart[24] = 0x00; // 0
                flashStart[25] = 0x00; // 0
                flashStart[26] = 0x00; // 0
                flashStart[27] = 0x00; // 0
                //-*************************************************************************

                //-************saveRestores*************************************************************
                flashStart[28] = 0x00; // 0
                flashStart[29] = 0x01; // 1
                flashStart[30] = 0x00; // 0
                flashStart[31] = 0x00; // 0
                flashStart[32] = 0x00; // 0
                flashStart[33] = 0x01; // 1
                flashStart[34] = 0x01; // 1
                flashStart[35] = 0x00; // 0
                flashStart[36] = 0x00; // 0
                flashStart[37] = 0x00; // 0
                flashStart[38] = 0x01; // 1

                flashStart[39] = 0xC0; // 192
                flashStart[40] = 0xA8; // 168
                flashStart[41] = 0xB2; // 178
                flashStart[42] = 0xAC; // 172

                flashStart[43] = 0x00; // 0
                flashStart[44] = 0x01; // 1

                flashStart[45] = 0xFF; // 255
                flashStart[46] = 0xFF; // 255
                flashStart[47] = 0xFF; // 255
                flashStart[48] = 0x00; // 0

                flashStart[49] = 0x00; // 0
                flashStart[50] = 0x01; // 1

                flashStart[51] = 0xC0; // 192
                flashStart[52] = 0xA8; // 168
                flashStart[53] = 0xB2; // 178
                flashStart[54] = 0x01; // 1

                flashStart[55] = 0x00; // 0
                flashStart[56] = 0x01; // 1

                flashStart[57] = 0xE0; // 224
                flashStart[58] = 0x00; // 0
                flashStart[59] = 0x17; // 23
                flashStart[60] = 0x0C; // 12

                flashStart[61] = 0x00; // 0
                flashStart[62] = 0x01; // 1

                // Geräteinformationen
                flashStart[63] = 0x10; // 16
                flashStart[64] = 0x00; // 0
                flashStart[65] = 0x1E; // 30
                flashStart[66] = 0x52; // 82
                flashStart[67] = 0x61; // 97
                flashStart[68] = 0x64; // 100
                flashStart[69] = 0x6F; // 111
                flashStart[70] = 0x6E; // 110
                flashStart[71] = 0x73; // 115
                flashStart[72] = 0x65; // 101
                flashStart[73] = 0x6E; // 110
                flashStart[74] = 0x73; // 115
                flashStart[75] = 0x6F; // 111
                flashStart[76] = 0x72; // 114

                flashStart[77] = 0x00; // 0
                flashStart[78] = 0x00; // 0
                flashStart[79] = 0x00; // 0
                flashStart[80] = 0x00; // 0
                flashStart[81] = 0x00; // 0
                flashStart[82] = 0x00; // 0
                flashStart[83] = 0x00; // 0
                flashStart[84] = 0x00; // 0
                flashStart[85] = 0x00; // 0
                flashStart[86] = 0x00; // 0
                flashStart[87] = 0x00; // 0
                flashStart[88] = 0x00; // 0
                flashStart[89] = 0x00; // 0
                flashStart[90] = 0x00; // 0
                flashStart[91] = 0x00; // 0
                flashStart[92] = 0x00; // 0
                flashStart[93] = 0x00; // 0
                flashStart[94] = 0x00; // 0
                flashStart[95] = 0x00; // 0
                //-*************************************************************************

                //-************tableobjs*************************************************************
                // Zusätzliche Konfigurationswerte
                flashStart[96] = 0x00; // 0
                flashStart[97] = 0xFA; // 250
                flashStart[98] = 0xAB; // 171
                flashStart[99] = 0xCD; // 205
                flashStart[100] = 0x02; // 2

                flashStart[101] = 0x01; // 1
                flashStart[102] = 0x00; // 0
                flashStart[103] = 0x00; // 0
                flashStart[104] = 0x00; // 0
                flashStart[105] = 0x08; // 8
                flashStart[106] = 0x00; // 0
                flashStart[107] = 0x00; // 0
                flashStart[108] = 0x00; // 0
                flashStart[109] = 0x98; // 152
                //-*************************************************************************

                flashStart[110] = 0x00; // 0
                flashStart[111] = 0x08; // 8

                //-************tableobjs********************************
                flashStart[112] = 0x01; // 1
                flashStart[113] = 0x00; // 0
                flashStart[114] = 0x00; // 0
                flashStart[115] = 0x00; // 0
                flashStart[116] = 0x0A; // 10        // 2 mehr mit einem eintrag mehr
                flashStart[117] = 0x00; // 0
                flashStart[118] = 0x00; // 0
                flashStart[119] = 0x00; // 0
                flashStart[120] = 0xA0; // 160
                //-********************************************

                flashStart[121] = 0x00; // 0
                flashStart[122] = 0x0C; // 12      // 4 mehr mit einem eintrag  mehr

                //-************tableobjs*******************
                flashStart[123] = 0x01; // 1
                flashStart[124] = 0x00; // 0
                flashStart[125] = 0x00; // 0
                flashStart[126] = 0x00; // 0


                flashStart[127] = 0x0A; // 10     // 2 = nur ID geschrieben    4 = mit erster GA      6 mit 2 GA's     8 mit 3 GA's   10 mit 4 GA

                flashStart[128] = 0x00; // 0
                flashStart[129] = 0x00; // 0
                flashStart[130] = 0x00; // 0
                flashStart[131] = 0xAC; // 172       // 4 mehr mit einem eintrag mehr
                //-******************************************
                flashStart[132] = 0x00; // 0
                flashStart[133] = 0x0C; // 12        // 4 mehr mit einem eintrag  mehr

                //-************tableobjs************
                flashStart[134] = 0x01; // 1
                flashStart[135] = 0x00; // 0
                flashStart[136] = 0x00; // 0
                flashStart[137] = 0x00; // 0


                flashStart[138] = 0x12; // 18          // 4 mehr mit einem eintrag  mehr
                //-*******************************

                flashStart[139] = 0x00; // 0
                flashStart[140] = 0x00; // 0
                flashStart[141] = 0x00; // 0

                flashStart[142] = 0xB8; // 184       // 8 mehr mit einem eintrag mehr

                flashStart[143] = 0x00; // 0
                flashStart[144] = 0x14; // 20       // 4 mehr mit einem eintrag  mehr

                flashStart[145] = 0xFF; // 255
                flashStart[146] = 0xFF; // 255
                flashStart[147] = 0xFF; // 255
                flashStart[148] = 0xFF; // 255
                flashStart[149] = 0xFF; // 255
                flashStart[150] = 0xFF; // 255
                flashStart[151] = 0xFF; // 255




               //----------------------







       */
    
    uint16_t _metadataSize_t = _metadataSize + 58;
    //print("_metadataSize ");println(_metadataSize_t);

    // Daten aus dem Flash-Speicher lesen und in storedData speichern
    for (int i = 0; i < _metadataSize_t; i++) {
        storedData[i] = *(flashStart + i);
    }

    //printHex("RESTORED ", flashStart, _metadataSize_t);

    uint16_t metadataBlockSize = alignToPageSize(_metadataSize_t);

    _freeList = new MemoryBlock(flashStart + metadataBlockSize, flashSize - metadataBlockSize);

    uint16_t apiVersion = 0;
    const uint8_t* buffer = popWord(apiVersion, flashStart);
    //print("apiVersion ");println(apiVersion);

    uint16_t manufacturerId = 0;
    buffer = popWord(manufacturerId, buffer);
    //print("manufacturerId ");println(manufacturerId);

    uint8_t hardwareType[LEN_HARDWARE_TYPE] = {0};
    buffer = popByteArray(hardwareType, LEN_HARDWARE_TYPE, buffer);
    //printHex("hardwareType ", hardwareType, LEN_HARDWARE_TYPE);

    uint16_t version = 0;
    buffer = popWord(version, buffer);
    //print("version ");println(version);

    // 00 01    00 FA 00 00 00 00 00    00 00

    VersionCheckResult versionCheck = FlashAllInvalid;

    // first check correct format of deviceObject-API
    //print("apiVersion ");println(apiVersion);
    //print("_deviceObject.apiVersion ");println(_deviceObject.apiVersion);
    if (_deviceObject.apiVersion == apiVersion) 
    {
        
       
        if (_versionCheckCallback != 0) {
            versionCheck = _versionCheckCallback(manufacturerId, hardwareType, version);
            //print("_versionCheckCallback: ");println(versionCheck);
            // callback should provide infomation about version check failure reasons
        }
        else if (_deviceObject.manufacturerId() == manufacturerId &&
                 memcmp(_deviceObject.hardwareType(), hardwareType, LEN_HARDWARE_TYPE) == 0) 
        {
            if (_deviceObject.version() == version) {
                versionCheck = FlashValid;
                //println("FlashValid");
            } 
            else
            {
                versionCheck = FlashTablesInvalid;
                //println("FlashTablesInvalid");
            }
        } 
        else 
        {
            //println("manufacturerId or hardwareType are different");
            //print("expexted manufacturerId: ");
            //print(_deviceObject.manufacturerId(), HEX);
            //print(", stored manufacturerId: ");
            //println(manufacturerId, HEX);
            //print("expexted hardwareType: ");
            //printHex("", _deviceObject.hardwareType(), LEN_HARDWARE_TYPE);
            //print(", stored hardwareType: ");
            //printHex("", hardwareType, LEN_HARDWARE_TYPE);
            //println("");
        }
    } 
    else 
    {
        //println("DataObject api changed, any data stored in flash is invalid.");
        //print("expexted DataObject api version: ");
        //print(_deviceObject.apiVersion, HEX);
        //print(", stored api version: ");
        //println(apiVersion, HEX);
    }

    if (versionCheck == FlashAllInvalid)
    {
        //println("ETS has to reprogram PA and application!");
        return;
    }

    //println("restoring data from flash...");
    //print("saverestores ");println(_saveCount);
    for (int i = 0; i < _saveCount; i++)
    {
        //println(flashStart - buffer);
        //println(".");
        buffer = _saveRestores[i]->restore(buffer);

            //-------------------------------
            // Ausgabe der Daten aus dem Speicher
            //uint8_t* data = buffer;
            /*
            for (int j = 0; j < _saveRestores[i]->saveSize(); j++)
            {
                print(buffer[j]);
                print(" ");
            }
            println("");
            */
            //-------------------------------

    }
    
    
    
    //println("restored saveRestores");
    if (versionCheck == FlashTablesInvalid) 
    {
        //println("TableObjects are referring to an older firmware version and are not loaded");
        return;
    }
    
    
    
    //print("tableObjs ");println(_tableObjCount);
    for (int i = 0; i < _tableObjCount; i++)
    {
        //println(flashStart - buffer);
        //println(".");
        buffer = _tableObjects[i]->restore(buffer);
        uint16_t memorySize = 0;
        buffer = popWord(memorySize, buffer);
        //print("buffer index after _saveRestores restore: ");
        //println(flashStart - buffer);

        if (memorySize == 0)
            continue;

        // this works because TableObject saves a relative addr and restores it itself
        addNewUsedBlock(_tableObjects[i]->_data, memorySize);

          //--------------------------
            // Ausgabe der Daten aus dem Speicher
            /*
            uint8_t* data = _tableObjects[i]->_data;
            for (int j = 0; j < memorySize; j++)
            {
                print(data[j]);
                print(" ");
            }
            println("");
             */
         //--------------------------

    }
    //println("restored Tableobjects");
}




void Memory::writeMemory()
{
        uint32_t t_count = 0;
        //uint32_t res_count = 0;
    
        //print("_metadataSize: ");
        //println(_metadataSize);
        //print("_tableObjCount: ");
        //println(_tableObjCount);

    // first get the necessary size of the writeBuffer
    uint16_t writeBufferSize = _metadataSize;
    //for (int i = 0; i < _saveCount; i++)
       // writeBufferSize = MAX(writeBufferSize, _saveRestores[i]->saveSize());

        for (int i = 0; i < _saveCount; i++)
        {
            uint16_t currentSaveSize = _saveRestores[i]->saveSize();
            writeBufferSize = MAX(writeBufferSize, currentSaveSize);

            // Debug-Ausgaben
            //Serial.print("Iteration _saveRestores: ");
            //Serial.println(i);
            //Serial.print("Aktuelle saveSize: ");
            //Serial.println(currentSaveSize);
            //Serial.print("Aktuelle writeBufferSize: ");
            //Serial.println(writeBufferSize);
        }

        for (int i = 0; i < _saveCount; i++)
        {
            // Ausgabe des aktuellen Zustands des _saveRestores-Arrays
            //Serial.print("_saveRestores[");
            //Serial.print(i);
            //Serial.print("]: ");
            //Serial.println(_saveRestores[i]->saveSize());
        }



    //for (int i = 0; i < _tableObjCount; i++)
       // writeBufferSize = MAX(writeBufferSize, _tableObjects[i]->saveSize() + 2 /*for memory pos*/);
      
        for (int i = 0; i < _tableObjCount; i++)
        {
            uint16_t currentSaveSize = _tableObjects[i]->saveSize() + 2;
            writeBufferSize = MAX(writeBufferSize, currentSaveSize);

            // Debug-Ausgaben
            //Serial.print("Iteration _tableObjCount: ");
            //Serial.println(i);
            //Serial.print("Aktuelle saveSize (plus 2 für Speicherposition): ");
            //Serial.println(currentSaveSize);
            //Serial.print("Aktuelle writeBufferSize: ");
            //Serial.println(writeBufferSize);
        }
        


            for (int i = 0; i < _tableObjCount; i++)
            {
                // Ausgabe des aktuellen Zustands des _saveRestores-Arrays
                //Serial.print("_tableObjCount[");
                //Serial.print(i);
                //Serial.print("]: ");
                //Serial.println(_tableObjects[i]->saveSize());
            }



 
        
        
        
            uint8_t buffer[writeBufferSize];
            uint32_t flashPos = 0;
            uint8_t* bufferPos = buffer;

                //print("writeBufferSize: ");
                //println(writeBufferSize);

            bufferPos = pushWord(_deviceObject.apiVersion, bufferPos);
                //print("bufferPos after pushing apiVersion: ");
                //println((uintptr_t)bufferPos);

            bufferPos = pushWord(_deviceObject.manufacturerId(), bufferPos);
                //print("bufferPos after pushing manufacturerId: ");
                //println((uintptr_t)bufferPos);

            bufferPos = pushByteArray(_deviceObject.hardwareType(), LEN_HARDWARE_TYPE, bufferPos);
                //print("bufferPos after pushing hardwareType: ");
                //println((uintptr_t)bufferPos);

            bufferPos = pushWord(_deviceObject.version(), bufferPos);
                //print("bufferPos after pushing version: ");
                //println((uintptr_t)bufferPos);

            flashPos = _platform.writeNonVolatileMemory(flashPos, buffer, bufferPos - buffer);
            //print("Writing to address--->>: "); println(flashPos);


            //print("save saveRestores ");
            //println(_saveCount);
            for (int i = 0; i < _saveCount; i++)
            {
                bufferPos = _saveRestores[i]->save(buffer);
                t_count = 0;
                //res_count = 0;
                //print("Writing to address: ");
                //println((uintptr_t)(flashPos));
                //print("Data: ");
                for (int j = 0; j < (bufferPos - buffer); j++)
                {
                    //print(buffer[j]);
                    //print(" ");
                    t_count ++;
                }
                //res_count = flashPos + t_count;
                //println("");
                //print("end address: ");
                //println((uintptr_t)(res_count));
                //println("");
                flashPos = _platform.writeNonVolatileMemory(flashPos, buffer, bufferPos - buffer);
            }



            //print("save tableobjs ");
            //println(_tableObjCount);
            for (int i = 0; i < _tableObjCount; i++)
            {
                bufferPos = _tableObjects[i]->save(buffer);

                // Ausgabe der Adresse und des Werts, die geschrieben werden sollen
                t_count = 0;
                //res_count = 0;
                //print("Writing to address: ");
                //println((uintptr_t)(flashPos));
                //print("Data: ");
                for (int j = 0; j < (bufferPos - buffer); j++)
                {
                    //print(buffer[j]);
                    //print(" ");
                    t_count ++;
                }
                //res_count = flashPos + t_count;
                //println("");
                //print("end address: ");
                //println((uintptr_t)(res_count));
                //println("");

                //save to size of the memoryblock for tableobject too, so that we can rebuild the usedList and freeList
                if (_tableObjects[i]->_data != nullptr)
                {
                    MemoryBlock* block = findBlockInList(_usedList, _tableObjects[i]->_data);
                    if (block == nullptr)
                    {
                        //println("_data of TableObject not in _usedList");
                        _platform.fatalError();
                    }
                    bufferPos = pushWord(block->size, bufferPos);
                }
                else
                    bufferPos = pushWord(0, bufferPos);

                flashPos = _platform.writeNonVolatileMemory(flashPos, buffer, bufferPos - buffer);
            }




                 //printHex("buffer ", buffer, 152);
            
    _platform.commitNonVolatileMemory();
}


void Memory::editMemory(size_t offset, uint8_t value)
{
    // Fügen Sie den Offset zur Startadresse des nicht-flüchtigen Speichers hinzu

     //Serial.print("offset: ");Serial.println(offset);
     //Serial.print("value: ");Serial.println(value);
     
   
     //uint8_t* startAddress = _platform.getNonVolatileMemoryStart();
     uint8_t* address = _platform.getNonVolatileMemoryStart() + offset;
   
   
 
    //uint8_t* address = _platform.getNonVolatileMemoryStart() + offset;
    if (address >= _platform.getNonVolatileMemoryStart() && address < _platform.getNonVolatileMemoryStart() + _platform.getNonVolatileMemorySize())
    {
        *address = value; // Ändern Sie den Wert an der gegebenen Adresse
        _platform.commitNonVolatileMemory();
    }
    else
    {
        //println("Ungültige Adresse!");
    }
  
     /*
    offset = 150; value = 254;
     address = _platform.getNonVolatileMemoryStart() + offset;
    *address = value; 
     */
    _platform.commitNonVolatileMemory();

}

void Memory::editMemoryarray(size_t offset, uint8_t* values, size_t length)
{
    uint8_t* address = _platform.getNonVolatileMemoryStart() + offset;

    for(size_t i = 0; i < length; i++)
    {
        *(address + i) = values[i]; // Ändern Sie den Wert an der gegebenen Adresse
    }

    _platform.commitNonVolatileMemory();
}


void Memory::saveMemory()
{
    _platform.commitNonVolatileMemory();
}

void Memory::addSaveRestore(SaveRestore* obj)
{
    if (_saveCount >= MAXSAVE - 1)
        return;

    _saveRestores[_saveCount] = obj;
    _saveCount += 1;
    _metadataSize += obj->saveSize();
}

void Memory::addSaveRestore(TableObject* obj)
{
    if (_tableObjCount >= MAXTABLEOBJ)
        return;

    _tableObjects[_tableObjCount] = obj;
    _tableObjCount += 1;
    _metadataSize += obj->saveSize();
    _metadataSize += 2; // for size
}

uint8_t* Memory::allocMemory(size_t size)
{
    // always allocate aligned to pagesize
    size = alignToPageSize(size);

    MemoryBlock* freeBlock = _freeList;
    MemoryBlock* blockToUse = nullptr;
    
    // find the smallest possible block that is big enough
    while (freeBlock)
    {
        if (freeBlock->size >= size)
        {
            if (blockToUse != nullptr && (blockToUse->size - size) > (freeBlock->size - size))
                blockToUse = freeBlock;
            else if (blockToUse == nullptr)
                blockToUse = freeBlock;
        }
        freeBlock = freeBlock->next;
    }
    if (!blockToUse)
    {
        //println("No available non volatile memory!");
        _platform.fatalError();
    }

    if (blockToUse->size == size)
    {
        // use whole block
        removeFromFreeList(blockToUse);
        addToUsedList(blockToUse);
        return blockToUse->address;
    }
    else
    {
        // split block
        MemoryBlock* newBlock = new MemoryBlock(blockToUse->address, size);
        addToUsedList(newBlock);

        blockToUse->address += size;
        blockToUse->size -= size;

        return newBlock->address;
    }
}


void Memory::freeMemory(uint8_t* ptr)
{
    MemoryBlock* block = _usedList;
    MemoryBlock* found = nullptr;
    while (block)
    {
        if (block->address == ptr)
        {
            found = block;
            break;
        }
        block = block->next;
    }
    if(!found)
    {
        //println("freeMemory for not used pointer called");
        _platform.fatalError();
    }
    removeFromUsedList(block);
    addToFreeList(block);
}

void Memory::writeMemory(uint32_t relativeAddress, size_t size, uint8_t* data)
{
    _platform.writeNonVolatileMemory(relativeAddress, data, size);
}


uint8_t* Memory::toAbsolute(uint32_t relativeAddress)
{
    return _platform.getNonVolatileMemoryStart() + (ptrdiff_t)relativeAddress;
}


uint32_t Memory::toRelative(uint8_t* absoluteAddress)
{
    return absoluteAddress - _platform.getNonVolatileMemoryStart();
}

MemoryBlock* Memory::removeFromList(MemoryBlock* head, MemoryBlock* item)
{
    if (head == item)
    {
        MemoryBlock* newHead = head->next;
        head->next = nullptr;
        return newHead;
    }

    if (!head || !item)
    {
        //println("invalid parameters of Memory::removeFromList");
        _platform.fatalError();
    }

    bool found = false;
    MemoryBlock* block = head;
    while (block)
    {
        if (block->next == item)
        {
            found = true;
            block->next = item->next;
            break;
        }
        block = block->next;
    }

    if (!found)
    {
        //println("tried to remove block from list not in it");
        _platform.fatalError();
    }
    item->next = nullptr;
    return head;
}

void Memory::removeFromFreeList(MemoryBlock* block)
{
    _freeList = removeFromList(_freeList, block);
}


void Memory::removeFromUsedList(MemoryBlock* block)
{
    _usedList = removeFromList(_usedList, block);
}


void Memory::addToUsedList(MemoryBlock* block)
{
    block->next = _usedList;
    _usedList = block;
}


void Memory::addToFreeList(MemoryBlock* block)
{
    if (_freeList == nullptr)
    {
        _freeList = block;
        return;
    }

    // first insert free block in list
    MemoryBlock* current = _freeList;
    while (current)
    {
        if (current->address <= block->address && (current->next == nullptr || block->address < current->next->address))
        {
            //add after current
            block->next = current->next;
            current->next = block;
            break;
        }
        else if (current->address > block->address)
        {
            //add before current
            block->next = current;

            if (current == _freeList)
                _freeList = block;

            // swap current and block for merge
            MemoryBlock* tmp = current;
            current = block;
            block = tmp;

            break;
        }

        current = current->next;
    }
    // now check if we can merge the blocks
    // first current an block
    if ((current->address + current->size) == block->address)
    {
        current->size += block->size;
        current->next = block->next;
        delete block;
        // check further if now current can be merged with current->next
        block = current;
    }

    // if block is the last one, we are done 
    if (block->next == nullptr)
        return;

    // now check block and block->next
    if ((block->address + block->size) == block->next->address)
    {
        block->size += block->next->size;
        block->next = block->next->next;
        delete block->next;
    }
}

uint16_t Memory::alignToPageSize(size_t size)
{
    size_t pageSize = 4; //_platform.flashPageSize(); // align to 32bit for now, as aligning to flash-page-size causes side effects in programming
    // pagesize should be a multiply of two
    return (size + pageSize - 1) & (-1*pageSize);
}

MemoryBlock* Memory::findBlockInList(MemoryBlock* head, uint8_t* address)
{
    while (head != nullptr)
    {
        if (head->address == address)
            return head;

        head = head->next;
    }
    return nullptr;
}

void Memory::addNewUsedBlock(uint8_t* address, size_t size)
{
    MemoryBlock* smallerFreeBlock = _freeList;
    // find block in freeList where the new used block is contained in
    while (smallerFreeBlock)
    {
        if (smallerFreeBlock->next == nullptr ||
            (smallerFreeBlock->next != nullptr && smallerFreeBlock->next->address > address))
            break;
        
        smallerFreeBlock = smallerFreeBlock->next;
    }

    if (smallerFreeBlock == nullptr)
    {
        //println("addNewUsedBlock: no smallerBlock found");
        _platform.fatalError();
    }

    if ((smallerFreeBlock->address + smallerFreeBlock->size) < (address + size))
    {
        //println("addNewUsedBlock: found block can't contain new block");
        _platform.fatalError();
    }

    if (smallerFreeBlock->address == address && smallerFreeBlock->size == size)
    {
        // we take thow whole block
        removeFromFreeList(smallerFreeBlock);
        addToUsedList(smallerFreeBlock);
        return;
    }

    if (smallerFreeBlock->address == address)
    {
        // we take a front part of the block
        smallerFreeBlock->address += size;
        smallerFreeBlock->size -= size;
    }
    else
    {
        // we take a middle or end part of the block
        uint8_t* oldEndAddr = smallerFreeBlock->address + smallerFreeBlock->size;
        smallerFreeBlock->size = (address - smallerFreeBlock->address);

        if (address + size < oldEndAddr)
        {
            // we take the middle part of the block, so we need a new free block for the end part
            MemoryBlock* newFreeBlock = new MemoryBlock();
            newFreeBlock->next = smallerFreeBlock->next;
            newFreeBlock->address = address + size;
            newFreeBlock->size = oldEndAddr - newFreeBlock->address;
            smallerFreeBlock->next = newFreeBlock;
        }
    }

    MemoryBlock* newUsedBlock = new MemoryBlock(address, size);
    addToUsedList(newUsedBlock);
}

void Memory::versionCheckCallback(VersionCheckCallback func)
{
    _versionCheckCallback = func;
}

VersionCheckCallback Memory::versionCheckCallback()
{
    return _versionCheckCallback;
}



