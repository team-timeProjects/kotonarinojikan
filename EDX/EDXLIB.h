#pragma once

namespace EDX{
	///////////////////////////////////////////
	//										 //
	//				　定数　				 //
	//										 //
	///////////////////////////////////////////
	//円周率
#define EDX_PI_F 3.141592653589f
#define EDX_360RAD 6.283185307178f
#define EDX_90RAD 1.570796326794f
#define EDX_45RAD 0.785398163397f
#define EDX_1RAD 0.017453292519f
	//頂点フォーマット
#define FVF_VERTEX2DGRAPH (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3DGRAPH (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX2D   (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define FVF_VERTEX3D (D3DFVF_XYZ |  D3DFVF_DIFFUSE  )
#define FVF_VERTEX3DEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE  )
#define FVF_VERTEX3DMODEL (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
	//フレーム管理
#define EDX_FPS 60
#define FRAME_DIRAY ((DWORD)(1000/EDX_FPS))//1フレームあたりの最大処理時間
	//汎用型マクロ
#define _ARGB D3DCOLOR_ARGB
	//型宣言
	typedef char COLOR;
	//ネットワーク定数
#define EDX_TCP 0
#define EDX_UDP 1
#define EDX_TCP_NOTCONNECT -1
#define EDX_TCP_HOST 0
#define EDX_TCP_CLIENT 1
	//ファイル定数
#define EDX_STRSIZE 512
	//汎用処理マクロ
#define FOR(i,end) for(int i=0;i<end;i++)
#define E_STR(s) char s[EDX_STRSIZE];
	///////////////////////////////////////////
	//										 //
	//				　マクロ　				 //
	//										 //
	///////////////////////////////////////////
#define SAFE_DELETE(p){delete p;p=nullptr;}
#define SAFE_RELEASE(p){p->Release();delete p;p=NULL;}

	///////////////////////////////////////////
	//										 //
	//				構造体定義				 //
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
		DWORD    diffuse;  // 反射光
	};

	struct AF_VERTEX3DEX
	{
		union{ struct{ float x, y, z; }; Vector pos; };
		union{ struct{ float x, y, z; }; Vector normal; };

		DWORD    diffuse;  // 反射光
	};

	struct VERTEXGRAPH{
		union{ struct{ float x, y, z; }; Vector xyz; };
		float rhw; // 除算数 
		DWORD dwColor; // 頂点の色 
		float u, v; //　テクスチャ座標 
	};

	struct VERTEX3DGRAPH{
		union{ struct{ float x, y, z; }; Vector xyz; };
		DWORD dwColor; // 頂点の色 
		float u, v; //　テクスチャ座標 
	};

	struct EMFVERTEX{
		union{ struct{ float px, py, pz; }; Vector pos; };
		union{ struct{ float nx, ny, nz; }; Vector normal; };
		DWORD dwColor; // 頂点の色 
		float u, v; //　テクスチャ座標 
	};


	struct VERTEXINDEX{
		unsigned short index[3];
	};
	///////////////////////////////////////////
	//										 //
	//			プロトタイプ宣言			 //
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
		EDX_CLICK_L, EDX_CLICK_R
	};

	class EDX_Input{
		//マウス入力状態の保存
		static int Click[2];
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

		//描画ステートのセット
		static void SetRS(RS State);
		static void SetRenderType(RT RenderType);

		//表画面の更新
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
		//テクスチャと画像情報
		LPDIRECT3DTEXTURE9 pTexture;
		LPDIRECT3DSURFACE9 Surface;
		LPDIRECT3DSURFACE9 ZBuffer;
		D3DVIEWPORT9       ViewPort;

		//頂点情報とステータス
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

		//テクスチャ保存構造体
		struct TexSave {
			//パス
			char FilePass[EDX_STRSIZE];
			//カウンター
			unsigned int	UseObjCount;
			//テクスチャ
			LPDIRECT3DTEXTURE9 lpTexture;
			//インフォ
			D3DXIMAGE_INFO info;
		};

		//本体
		static TexSave TexSave[MAX_TEXTURE];

	public:
		//初期化
		static BOOL Init();
		//テクスチャ読み込み
		static LPDIRECT3DTEXTURE9* LoadTexture(char* filename, D3DXIMAGE_INFO* lpinfo);
		//テクスチャ消去
		static void	Release(LPDIRECT3DTEXTURE9 lpTexture);
	};

	void DrawBox(float x1, float y1, DWORD c1, float x2, float y2, DWORD c2, float x3, float y3, DWORD c3, float x4, float y4, DWORD c4);
}