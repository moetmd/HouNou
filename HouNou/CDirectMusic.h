/*
   Demo Name:  Direct Music Demo
      Author:  Allen Sherrod
     Chapter:  Ch 11
*/


#ifndef _UGP_DIRECTMUSIC_H_
#define _UGP_DIRECTMUSIC_H_

#define INITGUID

#include<windows.h>
#include<dmusicc.h>
#include<dmusici.h>
#include<cguid.h>

#define UGP_INFINITE -1


class CDirectMusic
{
   public:
      CDirectMusic();
      ~CDirectMusic() { Shutdown(); }

      bool CreateSound(char *soundfile,
                       int numRepeats);
      bool SetupSoundParameters(float dopplerFactor,
                                float rolloffFactor,
                                float minDist,
                                float maxDist);

      void Play();
      void UpdateSoundPosition(float x, float y, float z);
      void Stop();
      void Shutdown();


   private:
      bool Initialize(char *filename, int numRepeats);

      IDirectMusicLoader8 *m_soundLoader;
      IDirectMusicPerformance8 *m_soundPerformance;
      IDirectMusicSegment8 *m_audioSound;
      IDirectMusicAudioPath *m_audioPath;
      IDirectSound3DBuffer *m_audioBuffer;
      IDirectSound3DListener *m_audioListener;
      DS3DBUFFER m_bufferParams;
      DS3DLISTENER m_listenerParams;
};

#endif