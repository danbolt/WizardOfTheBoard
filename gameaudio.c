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

#ifdef NO_COMPILED_AUDIO

void initializeAudio() {
  //
}

void playSound(u32 soundId) {
  //
}

void stopLastPlayedSound() {
  //
}

void playMusic(u32 musicId) {
  //
}

void stopPlayingMusic() {
  //
}

void fadeOutMusic() {
  //
}

int isMusicPlaying() {
  return 1;
}


#else

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

void stopLastPlayedSound() {
  nuAuSndPlayerStop();
}

void playMusic(u32 musicId) {
  nuAuSeqPlayerStop(0);
  nuAuSeqPlayerSetNo(0, musicId % TRACK_COUNT);
  nuAuSeqPlayerPlay(0);
}

void stopPlayingMusic()  {
  if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
    nuAuSeqPlayerFadeOut(0, 25);
  }
}

void fadeOutMusic() {
  if (nuAuSeqPlayerGetState(0) == AL_PLAYING) {
    nuAuSeqPlayerFadeOut(0, 25);
  }
}

int isMusicPlaying() {
  return (nuAuSeqPlayerGetState(0) == AL_PLAYING);
}

#endif