#ifndef SOUND_COMMAND_H
#define SOUND_COMMAND_H

#include "canblokmessage.h"

// SOUND_COMMAND
// id: 0x280
// len: 5
// desc:0x5005
class SoundCommand : public CanBlokMessage
{
    Q_OBJECT
public:
    explicit SoundCommand(QObject *parent = 0);

    int getPhraseNumber () const { return phraseNumber; }

signals:
    void sayPhraseNumber (int n);

public slots:
    bool setPhraseNumber (int n);

private:
    void fillMessage (CanFrame &frame) const;
    bool parseSuitableMessage (const CanFrame &frame);

    int phraseNumber;
};

#endif // SOUND_COMMAND_H
