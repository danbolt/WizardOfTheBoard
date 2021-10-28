#include "gameaudio.h"


#include "segmentinfo.h"

#include "audio/bgm/sequence/tracknumbers.h"
#include "audio/sfx/sfx.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

#define DEFAULT_INGAME_VOLUME 0x2fff

static u32 isAudioInitialized = 0;

void setAudioData(void) {
  nuAuSeqPlayerBankSet(_midibankSegmentRomStart, _midibankSegmentRomEnd - _midibankSegmentRomStart, _miditableSegmentRomStart);
  nuAuSeqPlayerSeqSet(_seqSegmentRomStart);

  nuAuSndPlayerBankSet(_sfxbankSegmentRomStart, _sfxbankSegmentRomEnd - _sfxbankSegmentRomStart, _sfxtableSegmentRomStart);

  nuAuSeqPlayerSetVol(0, 0x2fff);
}

void initializeAudio() {
  if (isAudioInitialized) {
    return;
  }

  nuAuInit();
  setAudioData();

  isAudioInitialized = 1;
}



void playSound(u32 soundId) {
  nuAuSndPlayerPlay(soundId % SFX_COUNT);
}
