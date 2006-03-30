/**********************************************************************************************************************/
/*                                                MP3 PLAYER                                                          */
/* 14/12/00, M : creation of the file                                                                                 */
/*               uses DirectShow (DX7)                                                                                */
/**********************************************************************************************************************/

/************************************************ INCLUDE *************************************************************/

//#include  "stdafx.h"
#include  "PlayerMP3.h"

#include  <control.h>                                       // IMediaControl
#include  <uuids.h>
#include  <evcode.h>                                        // EC_COMPLETE

/**********************************************************************************************************************/
/*                                                CONSTRUCTORS / DESTRUCTOR                                           */
/**********************************************************************************************************************/

CPlayerMP3::CPlayerMP3()
  {
  vInit();
  }

//

CPlayerMP3::~CPlayerMP3()
  {
  vClean();
  }

/************************************************ vInit ***************************************************************/
// initialize members & resources
// 14/12/00, M
// in :
// out:
/**********************************************************************************************************************/

void CPlayerMP3::vInit()
  {
  m_lpGraph = NULL;
  m_dwState = _UNINITIALIZED_;
  }

/************************************************ vClean **************************************************************/
// release memory & resources
// 14/12/00, M
// in :
// out:
/**********************************************************************************************************************/

void CPlayerMP3::vClean()
  {
  if(m_lpGraph) m_lpGraph->Release();
  m_strFile = "";

  vInit();
  }

/************************************************ hrCreateFilterGraph *************************************************/
// initialize DShow object
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrCreateFilterGraph()
  {
  if(m_lpGraph) return true;

  HRESULT hrErr = CoCreateInstance(CLSID_FilterGraph,
                                   NULL,
                                   CLSCTX_INPROC_SERVER,
                                   IID_IGraphBuilder,
                                   (void**)&m_lpGraph);
  if(FAILED(hrErr))
    {
    m_lpGraph = NULL;
    return hrErr;
    }

  return _OK_;
  }

/************************************************ hrLoad **************************************************************/
// load a file with graph object
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrLoad(const TCHAR* lpszFile)
  {
  vClean();

  HRESULT hrErr = hrCreateFilterGraph();
  if(FAILED(hrErr)) return hrErr;

  WCHAR wszFile[MAX_PATH];
  MultiByteToWideChar(CP_ACP,0,lpszFile,-1,wszFile,MAX_PATH);

  hrErr = m_lpGraph->RenderFile(wszFile,NULL);
  if(FAILED(hrErr)) return hrErr;

  m_strFile = lpszFile;
  m_dwState = _STOPPED_;
  return _OK_;
}

/**********************************************************************************************************************/
/*                                                PLAY                                                                */
/**********************************************************************************************************************/

/************************************************ hrPlay **************************************************************/
// start playing
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrPlay()
  {
  if(!boCanPlay()) return _MP3ERR_CANTPLAY_;

  IMediaControl* pMC;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IMediaControl,(void**)&pMC);
  if(FAILED(hrErr)) return hrErr;

  hrErr = pMC->Run();
  pMC->Release();
  if(FAILED(hrErr)) return hrErr;

  m_dwState = _PLAYING_;
  return _OK_;
  }

/************************************************ hrPause *************************************************************/
// pause replay
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrPause()
  {
  if(!boCanPause()) return _MP3ERR_CANTPAUSE_;

  IMediaControl* pMC;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IMediaControl,(void**)&pMC);
  if(FAILED(hrErr)) return hrErr;

  hrErr = pMC->Pause();
  pMC->Release();
  if(FAILED(hrErr)) return hrErr;

  m_dwState = _PAUSED_;
  return _OK_;
  }

/************************************************ hrRewind ************************************************************/
// rewind to the beginning
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrRewind()
  {
  if(!boIsInitialized()) return _MP3ERR_CANTREWIND_;
  //hrStop();

  IMediaPosition* pMP;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IMediaPosition,(void**)&pMP);
  if(FAILED(hrErr)) return hrErr;
  
  hrErr = pMP->put_CurrentPosition(0);
  pMP->Release();
  if(FAILED(hrErr)) return hrErr;

  return _OK_;
  }

/************************************************ hrStop **************************************************************/
// stop playing
// 14/12/00, M
// in :
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrStop()
  {
  if(!boCanStop()) return _MP3ERR_CANTSTOP_;

  IMediaControl* pMC;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IMediaControl,(void**)&pMC);
  if(FAILED(hrErr)) return hrErr;

  hrErr = pMC->Stop();
  pMC->Release();
  if(FAILED(hrErr)) return hrErr;

  m_dwState = _STOPPED_;
  return _OK_;
  }

/************************************************ hrSetVolume *********************************************************/
// change volume
// 27/02/02, M
// in : volume (dB*100)
// out: _OK_ or error ID
/**********************************************************************************************************************/

HRESULT CPlayerMP3::hrSetVolume(long lVolume)
  {
  if(!boIsInitialized()) return _MP3ERR_NOVOLUME_;

  IBasicAudio* pBA;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IBasicAudio,(void**)&pBA);
  if(FAILED(hrErr)) return hrErr;

  if(lVolume >      0) lVolume = 0;
  if(lVolume < -10000) lVolume = -10000;
  hrErr = pBA->put_Volume(lVolume);
  pBA->Release();
  if(FAILED(hrErr)) return hrErr;

  return _OK_;
  }

/************************************************ boCompleted *********************************************************/
// check if end of stream is reached
// 28/02/02, M
// in :
// out: end reached ?
/**********************************************************************************************************************/

bool CPlayerMP3::boCompleted() const
  {
  if(!boIsPlaying()) return false;

  IMediaEvent* pME;
  HRESULT hrErr = m_lpGraph->QueryInterface(IID_IMediaEvent,(void**)&pME);
  if(FAILED(hrErr)) return false;

  long lEvent;
  hrErr = pME->WaitForCompletion(0,&lEvent);
  pME->Release();
  if(FAILED(hrErr)) return false;

  return(lEvent == EC_COMPLETE);
  }
