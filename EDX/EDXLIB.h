#pragma once

namespace EDX{
	///////////////////////////////////////////
	//										 //
	//				�@�萔�@				 //
	//										 //
	///////////////////////////////////////////
	//�~����
#define EDX_PI_F 3.141592653589f
#define EDX_360RAD 6.283185307178f
#define EDX_90RAD 1.570796326794f
#define EDX_45RAD 0.785398163397f
#define EDX_1RAD 0.017453292519f
	//���_�t�H�[�}�b�g
#define FVF_VERTEX2DGRAPH (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3DGRAPH (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX2D   (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define FVF_VERTEX3D (D3DFVF_XYZ |  D3DFVF_DIFFUSE  )
#define FVF_VERTEX3DEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE  )
#define FVF_VERTEX3DMODEL (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
	//�t���[���Ǘ�
#define EDX_FPS 60
#define FRAME_DIRAY ((DWORD)(1000/EDX_FPS))//1�t���[��������̍ő又������
	//�ėp�^�}�N��
#define _ARGB D3DCOLOR_ARGB
	//�^�錾
	typedef char COLOR;
	//�l�b�g���[�N�萔
#define EDX_TCP 0
#define EDX_UDP 1
#define EDX_TCP_NOTCONNECT -1
#define EDX_TCP_HOST 0
#define EDX_TCP_CLIENT 1
	//�t�@�C���萔
#define EDX_STRSIZE 512
	//�ėp�����}�N��
#define FOR(i,end) for(int i=0;i<end;i++)
#define E_STR(s) char s[EDX_STRSIZE];
	///////////////////////////////////////////
	//										 //
	//				�@�}�N���@				 //
	//										 //
	///////////////////////////////////////////
#define SAFE_DELETE(p){delete p;p=nullptr;}
#define SAFE_RELEASE(p){p->Release();delete p;p=NULL;}

	///////////////////////////////////////////
	//										 //
	//				�\���̒�`				 //
	//										 //
	///////////////////////////////////////////
	struct Vector{
		float x;
		float y;
		float z;


		Vector operator =(const D3DXVECTOR3& v){ Vector a = { v.x, v.y, v.z }; return a; }

		Vector operator +(const Vector& v){ Vector a = { x + v.x, y + v.y, z + v.z }; return a; }
		Vector operator -(const Vector& v){ Vector a = { x - v.x, y - v.y, z - v.z }; return a; }
		Vector operator *(const Vector& v){ Vector a = { x * v.x, y * v.y, z * v.z }; return a; }
		Vector operator /(const Vector& v){ Vector a = { x / v.x, y / v.y, z / v.z }; return a; }

		void operator+=(const Vector &v){ x += v.x; y += v.y; z += v.z; }
		void operator-=(const Vector &v){ x -= v.x; y -= v.y; z -= v.z; }
		void operator*=(const Vector &v){ x *= v.x; y *= v.y; z *= v.z; }
		void operator/=(const Vector &v){ x /= v.x; y /= v.y; z /= v.z; }

		Vector operator +(const float& f){ Vector a = { x + f, y + f, z + f }; return a; }
		Vector operator -(const float& f){ Vector a = { x - f, y - f, z - f }; return a; }
		Vector operator *(const float& f){ Vector a = { x * f, y * f, z * f }; return a; }
		Vector operator /(const float& f){ Vector a = { x / f, y / f, z / f }; return a; }

		void operator+=(const float &f){ x += f; y += f; z += f; }
		void operator-=(const float &f){ x -= f; y -= f; z -= f; }
		void operator*=(const float &f){ x *= f; y *= f; z *= f; }
		void operator/=(const float &f){ x /= f; y /= f; z /= f; }

	};
	struct Quaternion{
		union{ struct{ float x, y, z; }; Vector xyz; };
		float w;
	};

	struct AF_VERTEX2D
	{
		union{ struct{ float x, y, z; }; Vector xyz; };
		float       rhw;
		DWORD    diffuse;
	};

	struct AF_VERTEX3D
	{
		union{ struct{ float x, y, z; }; Vector xyz; };
		DWORD    diffuse;  // ���ˌ�
	};

	struct AF_VERTEX3DEX
	{
		union{ struct{ float x, y, z; }; Vector pos; };
		union{ struct{ float x, y, z; }; Vector normal; };

		DWORD    diffuse;  // ���ˌ�
	};

	struct VERTEXGRAPH{
		union{ struct{ float x, y, z; }; Vector xyz; };
		float rhw; // ���Z�� 
		DWORD dwColor; // ���_�̐F 
		float u, v; //�@�e�N�X�`�����W 
	};

	struct VERTEX3DGRAPH{
		union{ struct{ float x, y, z; }; Vector xyz; };
		DWORD dwColor; // ���_�̐F 
		float u, v; //�@�e�N�X�`�����W 
	};

	struct EMFVERTEX{
		union{ struct{ float px, py, pz; }; Vector pos; };
		union{ struct{ float nx, ny, nz; }; Vector normal; };
		DWORD dwColor; // ���_�̐F 
		float u, v; //�@�e�N�X�`�����W 
	};


	struct VERTEXINDEX{
		unsigned short index[3];
	};
	///////////////////////////////////////////
	//										 //
	//			�v���g�^�C�v�錾			 //
	//										 //
	///////////////////////////////////////////
	class EDX_Shader;
	bool FloatCmp(float a, float b);
	D3DXMATRIX FreeAxisRotateDX(Vector Pos, Vector Axis, float RotateAngle);
	Vector FreeAxisRotate(Vector Pos, Vector Axis, float RotateAngle);
	Vector FreeAxisRotate(Vector Pos, Vector Offset, Vector Axis, float RotateAngle);
	Quaternion MulQuater(Quaternion Q1, Quaternion Q2);
	float DotVec(Vector v1, Vector v2);
	Vector CrossVec(Vector v1, Vector v2);
	Vector Normalize(Vector Vec);
	D3DXVECTOR3 Normalize(D3DXVECTOR3 Vec);
	Vector GetV(float vx, float vy, float vz);
	Vector GetV(D3DXVECTOR3 D3Vec);
	float Length(Vector Vec1, Vector Vec2);
	float Length(Vector Vec);
	Vector MulVM(Vector v, D3DXMATRIX mat);
	Vector MulVM(D3DXVECTOR3 v, D3DXMATRIX mat);
	//Vector GetScreenPos(Vector Pos);

	///////////////////////////////////////////
	//										 //
	//				EDX_Input				 //
	//										 //
	///////////////////////////////////////////
	int MouseGet(int butontype);
	enum{
		EDX_CLICK_L, EDX_CLICK_R, EDX_WHEEL
	};

	class EDX_Input{
	public:
		//�}�E�X���͏�Ԃ̕ۑ�
		static int Click[3];
	public:

		static BOOL GetState();
		static unsigned int GetClickModule(int BUTTON_TYPE);
	};
	///////////////////////////////////////////
	//										 //
	//		    EDX_RenderMgr				 //
	//										 //
	///////////////////////////////////////////
	typedef class EDX_RenderMgr{
	public:
		enum RS{
			ALPHA_ENABLE, ALPHA_DISABLE,

			DEPTHREAD_ENABLE, DEPTHREAD_DISABLE,

			DEPTHWRITE_ENABLE, DEPTHWRITE_DISABLE,

			ALPHATEST_ENABLE, ALPHATEST_DISABLE,

			DITHER_ENABLE, DITHER_DISABLE,

			CULL_ENABLE, CULL_DISABLE,

			NORMALIZE_ENABLE, NORMALIZE_DISABLE,

			LINEAR_ENABLE, POINT_ENABLE,

			ADD_ENABLE, ADD_DISABLE,
		};

		enum RT{
			//2D
			TYPE_DEFAULT,
			TYPE_ADD,
			TYPE_2DEFFECT,
			TYPE_2DADDEFFECT,
		};

		//�`��X�e�[�g�̃Z�b�g
		static void SetRS(RS State);
		static void SetRenderType(RT RenderType);

		//�\��ʂ̍X�V
		static BOOL ScreenUpdate();
		static BOOL BeginSceneModule();
		static BOOL EndSceneModule();

	}RM;


	///////////////////////////////////////////
	//										 //
	//				EDX_2DObj				 //
	//										 //
	///////////////////////////////////////////

	union COLORBUFFER{
		struct{ COLOR b, g, r, a; };
		DWORD dwColor = 0x00000000;
	};

	class EDX_2DObj{
	private:
		//�e�N�X�`���Ɖ摜���
		LPDIRECT3DTEXTURE9 pTexture;
		LPDIRECT3DSURFACE9 Surface;
		LPDIRECT3DSURFACE9 ZBuffer;
		D3DVIEWPORT9       ViewPort;

		//���_���ƃX�e�[�^�X
		VERTEXGRAPH v[4];
		Vector Scale;
		float Angle;
		BOOL LRTurnOver;
		float CenterX, CenterY;
		RM::RT RenderType;
	public:
		D3DXIMAGE_INFO info;
		EDX_2DObj(char * filename);
		EDX_2DObj(UINT w, UINT h);
		void Draw(int x, int y);
		void Draw(int x, int y, char* Techniquename);
		void Draw(int x, int y, int W, int H, int Tx, int Ty, int Tw, int Th);
		void Draw(int x, int y, int W, int H, int Tx, int Ty, int Tw, int Th, char* Techniquename);

		void Draw3D(float x, float y, float z);
		void Draw3D(float x, float y, float z, char* Techniquename);
		void Draw3D(float x, float y, float z, int Tx, int Ty, int Tw, int Th);

		LPDIRECT3DTEXTURE9 GetTex(){ return pTexture; }
		void SetScale(float Scale);
		void SetScale(Vector Scale);
		void SetScale(float x, float y, float z);
		void SetAngle(float Angle);
		void SetVtARGB(int VtNum, COLOR A, COLOR R, COLOR G, COLOR B);
		void SetARGB(COLOR A, COLOR R, COLOR G, COLOR B);
		void SetARGB(DWORD ARGB);
		void SetTurnOver(BOOL LR);
		void SetCenter(int X, int Y);
		void SetRenderTarget(DWORD Idx, bool IsClear);
		void SendTexture(char* FromGPU);
		void SetRenderType(RM::RT RenderType);
		~EDX_2DObj();
	};

	///////////////////////////////////////////
	//										 //
	//		    EDX_TextureMgr				 //
	//										 //
	///////////////////////////////////////////
	class EDX_TextureMgr{
	private:
		static const int MAX_TEXTURE = 512;

		//�e�N�X�`���ۑ��\����
		struct TexSave {
			//�p�X
			char FilePass[EDX_STRSIZE];
			//�J�E���^�[
			unsigned int	UseObjCount;
			//�e�N�X�`��
			LPDIRECT3DTEXTURE9 lpTexture;
			//�C���t�H
			D3DXIMAGE_INFO info;
		};

		//�{��
		static TexSave TexSave[MAX_TEXTURE];

	public:
		//������
		static BOOL Init();
		//�e�N�X�`���ǂݍ���
		static LPDIRECT3DTEXTURE9* LoadTexture(char* filename, D3DXIMAGE_INFO* lpinfo);
		//�e�N�X�`������
		static void	Release(LPDIRECT3DTEXTURE9 lpTexture);
	};

	void DrawBox(float x1, float y1, DWORD c1, float x2, float y2, DWORD c2, float x3, float y3, DWORD c3, float x4, float y4, DWORD c4);
}