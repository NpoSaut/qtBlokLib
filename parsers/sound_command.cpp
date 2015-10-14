#include "sound_command.h"

SoundCommand::SoundCommand(QObject *parent) :
    CanBlokMessage(0x280, 5, parent),
    phraseNumber (0)
{
}

void SoundCommand::fillMessage(CanFrame &frame) const
{
    frame[2] = getPhraseNumber();
}

bool SoundCommand::setPhraseNumber(int n)
{
    if (n < 0 || n > 255)
        n = 255;

    phraseNumber = n;
    emit sayPhraseNumber(n);
    return true;
}


bool SoundCommand::parseSuitableMessage(const CanFrame &frame)
{
    bool update = false;
    update =  setPhraseNumber (frame[2]) || update;
    return update;
}
