/**********************************************************************************************************************/
/*                                                MP3 PLAYER                                                          */
/* 14/12/00, M : creation of the file                                                                                 */
/*               uses DirectShow (DX7)                                                                                */
/**********************************************************************************************************************/

#if !defined(AFX_PLAYERMP3_H__5E8A1F22_D1D0_11D4_9849_00A0CC2C5341__INCLUDED_)
#define AFX_PLAYERMP3_H__5E8A1F22_D1D0_11D4_9849_00A0CC2C5341__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif

/************************************************ INCLUDE *************************************************************/

#include  <strmif.h>                                        // DirectShow
#include  <string>                                          // STL string

/************************************************ CLASSES *************************************************************/

/**********************************************************************************************************************/
/*                                                CPlayerMP3                                                          */
/**********************************************************************************************************************/

class     CPlayerMP3
  {
// public enums

  public:

    typedef enum 
    {
          _UNINITIALIZED_               = 0,
          _STOPPED_,
          _PAUSED_,
          _PLAYING_,
    } PLAYER_STATE;

    typedef enum
    {
          _OK_                          = 0,
          _MP3ERR_CANTPLAY_             = 0x80000000,
          _MP3ERR_CANTPAUSE_,
          _MP3ERR_CANTREWIND_,
          _MP3ERR_CANTSTOP_,
          _MP3ERR_NOVOLUME_,
    } MP3_ERROR;

// public methods

  public:

    // constructors & destructor

    CPlayerMP3                          (void);
   ~CPlayerMP3                          (void);

    HRESULT         hrLoad              (const TCHAR* lpszFile);

    // state

    bool            boCanPlay           (void) const;
    bool            boCanPause          (void) const;
    bool            boCanStop           (void) const;

    bool            boIsInitialized     (void) const;
    bool            boIsStopped         (void) const;
    bool            boIsPaused          (void) const;
    bool            boIsPlaying         (void) const;
    bool            boCompleted         (void) const;

    const TCHAR*    lpszGetFile         (void) const;

    // play

    HRESULT         hrPlay              (void);
    HRESULT         hrPause             (void);
    HRESULT         hrRewind            (void);
    HRESULT         hrStop              (void);
    HRESULT         hrSetVolume         (long lVolume);     // 0=full, -10000=silence, logarithmic

// protected methods

  protected:

    void            vInit               (void);
    void            vClean              (void);
    HRESULT         hrCreateFilterGraph (void);

// protected members

  protected:

    IGraphBuilder*  m_lpGraph;
    std::string     m_strFile;
    DWORD           m_dwState;
  };

/************************************************ INLINE **************************************************************/

#include  "PlayerMP3.inl"

/**********************************************************************************************************************/

#endif // !defined(AFX_PLAYERMP3_H__5E8A1F22_D1D0_11D4_9849_00A0CC2C5341__INCLUDED_)
