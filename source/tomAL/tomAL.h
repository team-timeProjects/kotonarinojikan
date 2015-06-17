#ifndef _tomAL_h_
#define _tomAL_h_

#include "OpenAL\include\al.h"
#include "OpenAL\include\alc.h"

//#include"ogg\include\ogg\ogg.h"
//#include"ogg\include\vorbis\vorbisfile.h"

#include <math.h>
#include <memory>
#include <windows.h>
#include "iextreme.h"

class tomALBase;

namespace STREAMING_MODE
{
	enum
	{
		NORMAL = 0,
		FADE_IN,
		FADE_OUT,
	};
}

namespace SOUND_EFFECT
{
	enum
	{
		NONE = 0,
		DISTORTION,
		DELAY,
	};
}

typedef struct Lisner
{
	Vector3 pos;//位置
	Vector3 vel;//視線
	Vector3 ori;//姿勢
}Lisner;

typedef struct DISTORTION
{
	float level; //1～0(1がマックス、潰れきって何も聞こえないので注意)
}DISTORTION;

class DELAY
{
private:
	ALuint* delaySource = nullptr;
	int timer = 0;//カウンタ
	int playCount = 0;
	bool isPlay = false;
public:
	int delayTime = 0;//ディレイ音がなるまでの時間(単位:フレーム)
	float attenuation = 0;//減衰値(1～0)
	int time = 0;//ディレイ音の回数

	friend tomALBase;
};

class tomALBase
{
protected:
	ALuint buffer = 0;
	ALuint source = 0;
	float vol = 1.0f;
	float workVol = 0;
	unsigned int rate = 0;
	unsigned int dataSize = 0;

	int effect = 0;
	float distortionParam = 0;

	DELAY* delayParam = nullptr;
	

	struct PlayData
	{
		unsigned int rate = 0;
		unsigned int dataSize = 0;
		ALenum format = 0;
	}playData;

	void Distortion( char* data, int dataSize );

	void Delay( char* data, int dataSize );

	void DelayPlay()
	{
		delayParam->isPlay = true;
	}

	static Lisner lisner;

	bool is3D = false;

	Vector3 pos;
public:
	tomALBase() {};
	virtual ~tomALBase();

	virtual bool Load( char* filename ) = 0;

	void setEffect( int type, void* param );

	void setPos( Vector3& pos )
	{
		this->pos = pos;
	}

	void setVolume( float vol )
	{
		this->vol = vol;
	}

	virtual void Update();

	static void setLisner( Vector3& pos, Vector3& vel, Vector3& ori );
	static void setLisner( Lisner& lisner );
};

class tomALInmemory : public tomALBase
{
private:
	bool LoadWav( char* filename );
	//bool LoadOgg( char* filename );

	bool Load( char* filename );

	char* data = nullptr;
public:
	tomALInmemory(){};
	~tomALInmemory();

	void Set( char* filename, bool _3D = false );

	void Play();
	void Pause();
	void Stop();

	bool IsPlay();
};

class tomALStreaming : public tomALBase
{
private:
	ALuint bgmbuffer[2];

	bool LoadWav( char* data );
	//bool LoadOgg( char* filename );

	bool Load( char* filename ){ return true; }

	HANDLE	hStrThread;
	unsigned int dwThreadId;
	unsigned int dwThrdParam;

	char* data;
	unsigned int inMemoryDataSize = 0;

	char dataName[64];

	unsigned int readDataSize = 0;
	unsigned int dataStartPoint = 0;

	bool loop = true;

	int mode = 0;
	float fadePalam = 0;
public:
	tomALStreaming( ){};
	~tomALStreaming( );

	void Set( char* filename, int mode = STREAMING_MODE::NORMAL, int param = 60 );

	void Play();
	void Pause();
	void Stop();

	void Stream();

	void Update();

	bool IsPlay( );

	void setMode( int m, int param = 60 );

	static bool isEndThread;
};

class tomALManager
{
private:
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

	tomALInmemory** inmemory = nullptr;
	tomALStreaming** streaming = nullptr;

	int inmemoryNum = 0;
	int streamingNum = 0;
public:
	tomALManager(){};
	~tomALManager();

	void Init( int inmemorySound = 32, int streamingSound = 5 );

	static void setLisner( Vector3& pos, Vector3& vel, Vector3& ori )
	{
		tomALBase::setLisner( pos, vel, ori );
	}

	static void setLisner( Lisner& lisner )
	{
		tomALBase::setLisner( lisner );
	}

	void CreateStreamingPlayer( int No );

	void SetInmemory( int No, char* filename, bool _3D = false );
	void SetStreaming( int No, char* filename, int mode = STREAMING_MODE::NORMAL, int param = 60 );

	void PlayInmemory( int No );
	void PauseInmemory( int No );
	void StopInmemory( int No );
	bool IsPlayInmemory( int No );
	void SetEffectInmemory( int No, int type, void* param );
	void SetPos( int No, Vector3& pos )
	{
		inmemory[No]->setPos( pos );
	}
	void setVolumeInmemory( int No, float vol )
	{
		inmemory[No]->setVolume( vol );
	}

	void PlayStreaming( int No );
	void PauseStreaming( int No );
	void StopStreaming( int No );
	bool IsPlayStreaming( int No );
	void SetModeStreaming( int No, int m, int param = 60 );
	void SetEffectStreaming( int No, int type, void* param );
	void setVolumeStreaming( int No, float vol )
	{
		streaming[No]->setVolume( vol );
	}

	void Update();
};

#endif