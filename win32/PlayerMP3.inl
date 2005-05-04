/**********************************************************************************************************************/
/*                                                MP3 PLAYER                                                          */
/* 14/12/00, M : creation of the file                                                                                 */
/*               uses DirectShow (DX7)                                                                                */
/**********************************************************************************************************************/

#ifndef   _PLAYERMP3_INL_
#define   _PLAYERMP3_INL_

/**********************************************************************************************************************/
/*                                                STATE                                                               */
/**********************************************************************************************************************/

inline bool CPlayerMP3::boCanPlay() const
  {
  return((m_dwState == _STOPPED_) || (m_dwState == _PAUSED_));
  }

inline bool CPlayerMP3::boCanStop() const
  {
  return((m_dwState == _PLAYING_) || (m_dwState == _PAUSED_));
  }

inline bool CPlayerMP3::boCanPause() const
  {
  return((m_dwState == _PLAYING_) || (m_dwState == _STOPPED_));
  }

//

inline bool CPlayerMP3::boIsInitialized() const
  {
  return (m_dwState != _UNINITIALIZED_);
  }

inline bool CPlayerMP3::boIsStopped() const
  {
  return (m_dwState == _STOPPED_);
  }

inline bool CPlayerMP3::boIsPaused() const
  {
  return (m_dwState == _PAUSED_);
  }

inline bool CPlayerMP3::boIsPlaying() const
  {
  return (m_dwState == _PLAYING_);
  }

//

inline const TCHAR* CPlayerMP3::lpszGetFile() const
  {
  return m_strFile.c_str();
  }

/**********************************************************************************************************************/

#endif
