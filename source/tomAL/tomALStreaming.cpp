#include <windows.h>
#include <process.h>
#include <cassert>
#include <string.h>

#include "tomAL.h"

#pragma comment ( lib, "source/tomAL/OpenAL/lib/OpenAL32.lib" )

unsigned int WINAPI Thread( void* lpdwParam )
{
	tomALStreaming* streaming = (tomALStreaming*)lpdwParam;

	while( 1 )
	{
		if( tomALStreaming::isEndThread == false )
		{
			_endthread( );
			return 0;
		}
		if( streaming == nullptr )
		{
			_endthread();
			return 0;
		}
		streaming->Stream();
	}

	return 0;
}

bool tomALStreaming::LoadWav( char* data )
{
	FILE* fp = fopen( dataName, "rb" );
	if( fp == NULL )
	{
		return false;
	}

	fseek( fp, dataStartPoint + readDataSize, SEEK_SET );

	int readSize = fread( data, 1, inMemoryDataSize, fp );
	if( readSize == inMemoryDataSize )
	{
		readDataSize += readSize;
	}
	else if( readSize < inMemoryDataSize )
	{
		if( loop )
		{
			fseek( fp, dataStartPoint, SEEK_SET );

			fread( &data[readSize], 1, inMemoryDataSize - readSize, fp );

			readDataSize = inMemoryDataSize - readSize;
		}
		else
		{
			return false;
		}
	}
	else
	{
		assert( "ストリーミングがヤバい" );
	}

	fclose( fp );

	return true;
}

void tomALStreaming::Set( char* filename, int mode, int param )
{
	strcpy( dataName, filename );

	FILE* fp = fopen( filename, "rb" );

	char temp;
	while( 1 )
	{
		fread( &temp, 1, 1, fp );
		dataStartPoint++;
		if( temp == 'f' )
		{
			fread( &temp, 1, 1, fp );
			dataStartPoint++;
			if( temp == 'm' )
			{
				fread( &temp, 1, 1, fp );
				dataStartPoint++;
				if( temp == 't' )
				{
					fread( &temp, 1, 1, fp );
					dataStartPoint++;
					if( temp == ' ' )
					{
						break;
					}
					fseek( fp, -1L, SEEK_CUR );
					dataStartPoint--;
				}
				fseek( fp, -1L, SEEK_CUR );
				dataStartPoint--;
			}
			fseek( fp, -1L, SEEK_CUR );
			dataStartPoint--;
		}
	}

	fseek( fp, 6L, SEEK_CUR );
	dataStartPoint += 6;

	unsigned short channel; //1：モノラル　2：ステレオ
	unsigned int rate;//サンプリングレート(Hz)
	unsigned int bps;//データ速度(byte/second)
	unsigned short blocksize;//ブロックサイズ(byte/sample*チャンネル数)
	unsigned short bpsam;//サンプルあたりのビット数

	fread( &channel, 1, 2, fp );
	dataStartPoint += 2;
	fread( &rate, 1, 4, fp );
	dataStartPoint += 4;
	fread( &bps, 1, 4, fp );
	dataStartPoint += 4;
	fread( &blocksize, 1, 2, fp );
	dataStartPoint += 2;
	fread( &bpsam, 1, 2, fp );
	dataStartPoint += 2;

	if( channel == 1 )
	{
		if( bpsam == 8 )
		{
			playData.format = AL_FORMAT_MONO8;
			inMemoryDataSize = rate;
		}
		else if( bpsam == 16 )
		{
			playData.format = AL_FORMAT_MONO16;
			inMemoryDataSize = rate;
		}
		else
		{
			assert( "対応してないフォーマットです！" );
		}
	}
	else if( channel == 2 )
	{
		if( bpsam == 8 )
		{
			playData.format = AL_FORMAT_STEREO8;
			inMemoryDataSize = rate;
		}
		else if( bpsam == 16 )
		{
			playData.format = AL_FORMAT_STEREO16;
			inMemoryDataSize = rate;
		}
		else
		{
			assert( "対応してないフォーマットです！" );
		}
	}
	else
	{
		assert( "対応してないフォーマットです！" );
	}

	inMemoryDataSize = 44100;

	playData.rate = rate;

	while( 1 )
	{
		fread( &temp, 1, 1, fp );
		dataStartPoint++;
		if( temp == 'd' )
		{
			fread( &temp, 1, 1, fp );
			dataStartPoint++;
			if( temp == 'a' )
			{
				fread( &temp, 1, 1, fp );
				dataStartPoint++;
				if( temp == 't' )
				{
					fread( &temp, 1, 1, fp );
					dataStartPoint++;
					if( temp == 'a' )
					{
						break;
					}
					fseek( fp, -1L, SEEK_CUR );
					dataStartPoint--;
				}
				fseek( fp, -1L, SEEK_CUR );
				dataStartPoint--;
			}
			fseek( fp, -1L, SEEK_CUR );
			dataStartPoint--;
		}
	}

	fread( &playData.dataSize, 1, 4, fp );
	dataStartPoint += 4;

	data = new char[inMemoryDataSize];
	alGenBuffers( 2, bgmbuffer );
	fread( data, 1, inMemoryDataSize, fp );
	if( effect == SOUND_EFFECT::DISTORTION )
	{
		Distortion( data, inMemoryDataSize );
	}
	alBufferData( bgmbuffer[0], playData.format, data, inMemoryDataSize, playData.rate );
	fread( data, 1, inMemoryDataSize, fp );
	if( effect == SOUND_EFFECT::DISTORTION )
	{
		Distortion( data, inMemoryDataSize );
	}
	alBufferData( bgmbuffer[1], playData.format, data, inMemoryDataSize, playData.rate );
	readDataSize = inMemoryDataSize * 2;

	alGenSources( 1, &source );
	alSourcef( source, AL_GAIN, vol );
	alSourceQueueBuffers( source, 2, bgmbuffer );

	setMode( mode, param );

	fclose( fp );

	/*	管理スレッドの作成	*/
	hStrThread = (HANDLE)_beginthreadex( NULL, 0, Thread, this, 0, &dwThreadId );
	if( hStrThread == NULL )
	{
		return;
	}
}

tomALStreaming::~tomALStreaming()
{
	if( data != nullptr )
	{
		delete[] data;
		data = nullptr;
	}
}

void tomALStreaming::Play( )
{
	ALint state;
	alGetSourcei( source, AL_SOURCE_STATE, &state );
	if( state != AL_PAUSED )
	{
		alSourceRewind( source );
	}
	alSourcePlay( source );
}

void tomALStreaming::Pause()
{
	alSourcePause( source );
}

void tomALStreaming::Stop()
{
	alSourceStop( source );
}

bool tomALStreaming::IsPlay()
{
	ALint state;
	alGetSourcei( source, AL_SOURCE_STATE, &state );
	if( state == AL_PLAYING )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void tomALStreaming::Stream()
{
	ALint n;
	alGetSourcei( source, AL_BUFFERS_PROCESSED, &n );

	if( n > 0 )
	{
		if( LoadWav( data ) )
		{
			if( effect == SOUND_EFFECT::DISTORTION )
			{
				Distortion( data, inMemoryDataSize );
			}


			alGenBuffers( 1, &buffer );
			alBufferData( buffer, playData.format, data, inMemoryDataSize, playData.rate );

			alSourceQueueBuffers( source, 1, &buffer );
		}
		else
		{
			return;
		}
	}
}

void tomALStreaming::setMode( int m, int param )
{
	mode = m;
	switch( mode )
	{
	case STREAMING_MODE::NORMAL:
		break;
	case STREAMING_MODE::FADE_IN:
		fadePalam = (vol - workVol) / param;
		break;
	case STREAMING_MODE::FADE_OUT:
		workVol = vol;
		fadePalam = vol / param;
		break;
	}
}

void tomALStreaming::Update()
{
	//tomALBase::Update();
	switch( mode )
	{
	case STREAMING_MODE::NORMAL:
		alSourcef( source, AL_GAIN, vol );
		break;
	case STREAMING_MODE::FADE_IN:
		alSourcef( source, AL_GAIN, workVol );
		workVol += fadePalam;
		if( workVol >= vol )
		{
			mode = STREAMING_MODE::NORMAL;
		}
		break;
	case STREAMING_MODE::FADE_OUT:
		alSourcef( source, AL_GAIN, workVol );
		workVol -= fadePalam;
		if( 0 >= workVol )
		{
			mode = STREAMING_MODE::NORMAL;
			alSourceStop( source );
		}
		break;
	}
}

bool tomALStreaming::isEndThread = true;
