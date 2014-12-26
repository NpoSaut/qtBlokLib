#ifndef AUX_RESOURCE_H
#define AUX_RESOURCE_H

#include <QMetaEnum>

#include "cDoodahLib/lowlevel.h"
#include "cDoodahLib/packing.h"

#include "canblokmessage.h"

// AUX_RESOURCE
// id: multiple
// len: 5
// desc:multiple
class AuxResource : public CanBlokMessage
{
    Q_OBJECT
    Q_ENUMS(Descriptor)
public:
    enum AuxResourceCode
    {
        RES_VERSION = 0,
        RES_INTERNAL_ERROR = 1,
        RES_INTERNAL_WARRING = 2,
        RES_CHANNEL = 4,
        RES_DATA = 208,
        RES_MAPCODE = 209,
        RES_BUSY = 251,
        RES_FAIL = 252,
        RES_REPEAT = 253,
        RES_UNKNOWN = 254,
        RES_OK = 255
    };

    enum Descriptor
    {
        MCO_A = 0x8805,
        MCO_B = 0x8A05,
        MP1_A = 0x9045,
        MP1_B = 0x9245,
        MP2_A = 0x9085,
        MP2_B = 0x9285,
        TSKBM_K_A = 0xD805,
        TSKBM_K_B = 0xD825,
        TSKBM_P_A = 0xDA05,
        TSKBM_P_B = 0xDA25,
        AMR = 0xCC05,
        SAUT_PTK_A = 0x9065,
        SAUT_PTK_B = 0x9265,
        BS_DPS_A = 0xC665,
        BS_DPS_B = 0xC685,
        IPD_A = 0x9805,
        IPD_B = 0x9A05,
        SAUT_A = 0xB805,
        SAUT_B = 0xBA05,
        BIL_A = 0xD005,
        BIL_B = 0xD205,
        BIL2_A = 0xD025,
        BIL2_B = 0xD225,
        MM = 0xC208,
        GT_A = 0xCB05,
        GT_B = 0xCB65,
        UKTOL = 0xBC05,
        MPSU_A = 0x4845,
        MPSU_B = 0x4865,
        MSUTP = 0xC445,
        MSUL_A = 0xC405,
        MSUL_B = 0xC425,
        EPK = 0x4E85,
        RC_A = 0xA205,
        RC_B = 0xA605,
        MV_A = 0xE005,
        MV_B = 0xE105,
        VDS_A = 0xDC25,
        VDS_B = 0xDC45,
        OUT_A = 0xDC65,
        OUT_B = 0xDC85
    };

    explicit AuxResource(Descriptor descriptor, QObject *parent = 0);

    AuxResourceCode getCode () const { return code; }
    quint32 getData () const { return data; }

signals:
    void codeChanged (AuxResourceCode code);
    void dataChanged (quint32 data);
    
public slots:
    bool setCode (AuxResourceCode c);
    bool setData (quint32 d);

protected:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    virtual void processData () { }

private:
    AuxResourceCode code;
    quint32 data;
};

class AuxResourceVersion : public AuxResource
{
    Q_OBJECT
public:
    explicit AuxResourceVersion (Descriptor descriptor, QObject *parent);

    int getVersion () const { return data->version; }
    int getSubversion () const { return data->subversion; }
    int getChecksum () const { return data->checksum; }

signals:
    void updated (int version, int subversion, int checksum);

public slots:
    bool setVersion (int v);
    bool setSubversion (int subv);
    bool setChecksum (int csum);

protected:
    void processData ();
    void encodeData ();

private:
    PACKED(
    struct DataFields
    {
        quint32 checksum    :16;
        quint32 subversion  :8;
        quint32 version     :8;
    });
    Bitfield<DataFields> data;
};

#endif // AUX_RESOURCE_H
