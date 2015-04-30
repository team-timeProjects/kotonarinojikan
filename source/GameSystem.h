
struct _VB
{
public:
	LPDIRECT3DVERTEXBUFFER9 p;
	_VB( LPDIRECT3DDEVICE9 d3dd, UINT length ) : p( 0 ) { d3dd->CreateVertexBuffer( length, 0, D3DFVF_XYZ, D3DPOOL_SYSTEMMEM, &p, NULL ); }
	virtual ~_VB( void ) { if ( p ) p->Release(); } 
	operator LPDIRECT3DVERTEXBUFFER9( void ) { return p; }
	LPDIRECT3DVERTEXBUFFER9 operator ->( void ) { return p; }
};


//****************************************************************************************************
//
//	GameSystemクラス
//
//****************************************************************************************************
class GameSystem
{
private:
	//	描画関数(本体)
	static void DrawLine( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, D3DCOLOR c );
	static void DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c );
	static void DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, FLOAT r, D3DCOLOR c );
	
public:
	//	座標変換
	static	Vector3	TransCoordScreenToWorld( const Vector3& _ScreenCoord );
	static	Vector3	TransCoordScreenToWorld( const Vector3& _ScreenCoord, const D3DVIEWPORT9* _pViewPort, const Matrix* _pProjectionMatrix, const Matrix* _pViewMatrix );
	static	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat );

	//	当たり判定
	static bool Capsule( const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Pos3, const Vector3& Pos4, float r2 );
	static bool Capsule2( const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Pos3, const Vector3& Pos4, float r2 );
	static bool CapsuleVsBall( const Vector3& Pos1, const Vector3& Pos2, float r1, const Vector3& Ball, float r2 );
	static bool Ball( const Vector3& Pos1, float r1, const Vector3& Pos2, float r2 );

	//	描画関数
	static void DrawLine( const Vector3& Pos1, const Vector3& Pos2 );
	static void DrawSphere( const Vector3& Pos, float r1 );
	static void DrawCapsule( const Vector3& Pos1, const Vector3& Pos2, float r1 );

	//	ベジェ曲線(適当)
	static void	BezierCurveZ( Vector3& out, Vector3& p1, Vector3& p2, Vector3& p3, float t );

};

//****************************************************************************************************