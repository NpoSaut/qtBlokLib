#include "aux_resource.h"

#include "QMetaType"
#include "qdebug.h"

AuxResource::AuxResource(Descriptor descriptor, QObject *parent) :
    CanBlokMessage(descriptor/0x20, descriptor & 0xF, parent)
{
    qRegisterMetaType<AuxResourceCode> ("AuxResourceCode");
}

void AuxResource::fillMessage(CanFrame &frame) const
{
    frame[1] = quint8 (getCode ());
    frame[2] = quint8 (getData () >> 24);
    frame[3] = quint8 (getData () >> 16);
    frame[4] = quint8 (getData () >> 8);
    frame[5] = quint8 (getData ());
}

bool AuxResource::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update = setCode (AuxResourceCode (frame[1])) || update;
    update = setData (  (quint32 (frame[2]) << 24)
                      + (quint32 (frame[3]) << 16)
                      + (quint32 (frame[4]) <<  8)
                      + (quint32 (frame[5]) <<  0)) || update;
    processData ();
    return update;
}

bool AuxResource::setCode(AuxResource::AuxResourceCode c)
{
    if ( code != c )
    {
        code = c;
        emit codeChanged (code);
        return true;
    }
    return false;
}

bool AuxResource::setData(quint32 d)
{
    if ( data != d)
    {
        data = d;
        emit dataChanged (data);
        return true;
    }
    return false;
}


AuxResourceVersion::AuxResourceVersion(AuxResource::Descriptor descriptor, QObject *parent)
    : AuxResource (descriptor, parent),
      version (0),
      subversion (0),
      checksum (0)
{ }


void AuxResourceVersion::encodeData()
{
    qDebug() << "code fields into data";
    setData( (quint32 (quint8 (version))    << 24)
            +(quint32 (quint8 (subversion)) << 16)
            +(quint32 (quint16(checksum))   <<  0) );
}


void AuxResourceVersion::processData()
{
    setVersion    ( quint8 (getData() >> 24) );
    setSubversion ( quint8 (getData() >> 16) );
    setChecksum   ( quint16(getData() >>  0) );
}


bool AuxResourceVersion::setVersion(int v)
{
    if ( version != v )
    {
        version = v;
        emit versionChanged (version);
        return true;
    }
    return false;
}

bool AuxResourceVersion::setSubversion(int subv)
{
    if ( subversion != subv )
    {
        subversion = subv;
        emit subversionChanged (subversion);
        return true;
    }
    return false;
}

bool AuxResourceVersion::setChecksum(int csum)
{
    if ( checksum != csum )
    {
        checksum = csum;
        emit checksumChanged (checksum);
        return true;
    }
    return false;
}
