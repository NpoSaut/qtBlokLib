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
      data (0)
{
    setCode (RES_VERSION);
    QObject::connect(this, SIGNAL(whateverChanged()), this, SLOT(processData()), Qt::DirectConnection);
}

void AuxResourceVersion::processData()
{
    if (getCode() == RES_VERSION)
    {
        if (data != getData())
        {
            data = getData();
            emit updated(data->version, data->subversion, data->checksum);
        }
    }
}


bool AuxResourceVersion::setVersion(int v)
{
    if ( data->version != v )
    {
        data->version = v;
        setData (data);
        return true;
    }
    return false;
}

bool AuxResourceVersion::setSubversion(int subv)
{
    if ( data->subversion != subv )
    {
        data->subversion = subv;
        setData (data);
        return true;
    }
    return false;
}

bool AuxResourceVersion::setChecksum(int csum)
{
    if ( data->checksum != csum )
    {
        data->checksum = csum;
        setData (data);
        return true;
    }
    return false;
}
