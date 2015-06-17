#include <cassert>
#include <string.h>

#include "tomAL.h"

#pragma comment ( lib, "source/tomAL/OpenAL/lib/OpenAL32.lib" )

//#pragma comment ( lib, "source/tomAL/ogg/lib/libogg_static.lib" )
//#pragma comment ( lib, "source/tomAL/ogg/lib/libvorbis_static.lib" )
//#pragma comment ( lib, "source/tomAL/ogg/lib/libvorbisfile_static.lib" )

bool tomALInmemory::LoadWav( char* filename )
{
	FILE* fp = fopen( filename, "rb" );

	char temp;
	while( 1 )
	{
		fread( &temp, 1, 1, fp );
		if( temp == 'f' )
		{
			fread( &temp, 1, 1, fp );
			if( temp == 'm' )
			{
				fread( &temp, 1, 1, fp );
				if( temp == 't' )
				{
					fread( &temp, 1, 1, fp );
					if( temp == ' ' )
					{
						break;
					}
					fseek( fp, -1L, SEEK_CUR );
				}
				fseek( fp, -1L, SEEK_CUR );
			}
			fseek( fp, -1L, SEEK_CUR );
		}
	}

	fseek( fp, 6L, SEEK_CUR );

	unsigned short channel; //1：モノラル　2：ステレオ
	unsigned int rate;//サンプリングレート(Hz)
	unsigned int bps;//データ速度(byte/second)
	unsigned short blocksize;//ブロックサイズ(byte/sample*チャンネル数)
	unsigned short bpsam;//サンプルあたりのビット数

	fread( &channel, 1, 2, fp );
	fread( &rate, 1, 4, fp );
	fread( &bps, 1, 4, fp );
	fread( &blocksize, 1, 2, fp );
	fread( &bpsam, 1, 2, fp );

	if( channel == 1 )
	{
		if( bpsam == 8 )
		{
			playData.format = AL_FORMAT_MONO8;
		}
		else if( bpsam == 16 )
		{
			playData.format = AL_FORMAT_MONO16;
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
		}
		else if( bpsam == 16 )
		{
			playData.format = AL_FORMAT_STEREO16;
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

	playData.rate = rate;

	while( 1 )
	{
		fread( &temp, 1, 1, fp );
		if( temp == 'd' )
		{
			fread( &temp, 1, 1, fp );
			if( temp == 'a' )
			{
				fread( &temp, 1, 1, fp );
				if( temp == 't' )
				{
					fread( &temp, 1, 1, fp );
					if( temp == 'a' )
					{
						break;
					}
					fseek( fp, -1L, SEEK_CUR );
				}
				fseek( fp, -1L, SEEK_CUR );
			}
			fseek( fp, -1L, SEEK_CUR );
		}
	}

	fread( &playData.dataSize, 1, 4, fp );
	data = new char[playData.dataSize];
	fread( data, 1, playData.dataSize, fp );

	fclose( fp );

	return true;
}

//bool tomALInmemory::LoadOgg( char* filename )
//{
//	FILE* fp = fopen( filename, "rb" );
//	OggVorbis_File vf;
//	ov_open( fp, &vf, NULL, 0 );
//	vorbis_info* info = ov_info( &vf, -1 );
//
//	unsigned short channel; //1：モノラル　2：ステレオ
//	unsigned int rate;//サンプリングレート(Hz)
//	unsigned int bps;//データ速度(byte/second)
//	unsigned short blocksize;//ブロックサイズ(byte/sample*チャンネル数)
//	unsigned short bpsam;//サンプルあたりのビット数
//
//	channel = info->channels == 1 ? 1 : 2;
//	bpsam = 16;
//	playDate.rate = info->rate;
//	
//	if( channel == 1 )
//	{
//		playDate.format = AL_FORMAT_MONO16;
//	}
//	else if( channel == 2 )
//	{
//		playDate.format = AL_FORMAT_STEREO16;
//	}
//
//	playDate.dataSize = (int)ov_pcm_total( &vf, -1 ) * 2;
//	data.reset( new char[playDate.dataSize] );
//	int readsize = 0;
//	while( 1 )
//	{
//		int c = ov_read( &vf, &data.get( )[readsize], playDate.dataSize - readsize, 0, 2, 1, NULL );
//		readsize += c;
//
//		if( readsize >= playDate.dataSize ) break;
//	}
//	ov_clear( &vf );
//
//	return true;
//}

tomALInmemory::~tomALInmemory()
{
	if( data != nullptr )
	{
		delete[] data;
		data = nullptr;
	}
}

bool tomALInmemory::Load( char* filename )
{
	if( strstr( filename, ".wav" ) != NULL )
	{
		LoadWav( filename );
	}
	/*else if( strstr( filename, ".ogg" ) != NULL )
	{
		LoadOgg( filename );
	}*/
	else
	{
		return false;
	}
}

void tomALInmemory::Set( char* filename, bool _3D )
{
	is3D = _3D;

	Load( filename );

	alGenBuffers( 1, &buffer );
	alBufferData( buffer, playData.format, data, playData.dataSize, playData.rate );

	alGenSources( 1, &source );
	alSourcef( source, AL_GAIN, vol );
	alSourcei( source, AL_BUFFER, buffer );
}

void tomALInmemory::Play()
{
	ALint state;
	alGetSourcei( source, AL_SOURCE_STATE, &state );
	if( state != AL_PAUSED )
	{
		alSourceRewind( source );
	}
	switch( effect )
	{
	case SOUND_EFFECT::DISTORTION:
	{
		char* work;
		work = new char[playData.dataSize];
		memcpy( work, data, playData.dataSize );
		Distortion( work, playData.dataSize );

		//alDeleteSources( 1, &source );
		alDeleteBuffers( 1, &buffer );
		alGenBuffers( 1, &buffer );
		alBufferData( buffer, playData.format, work, playData.dataSize, playData.rate );

		alGenSources( 1, &source );
		alSourcef( source, AL_GAIN, vol );
		alSourcei( source, AL_BUFFER, buffer );
	}
	case SOUND_EFFECT::DELAY:
		Delay( data, playData.dataSize );
		break;
	}

	alSourcePlay( source );
}

void tomALInmemory::Pause()
{
	alSourcePause( source );
}

void tomALInmemory::Stop()
{
	alSourceStop( source );
}

bool tomALInmemory::IsPlay()
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