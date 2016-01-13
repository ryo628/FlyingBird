/* 【構造体型】 */
//画像データ
typedef struct
{
	int Back, Title, Title_Start, Title_Menu, Title_Option;
	int Pause;
	int Cursor, KeyBack, Space;
	int Level, Star, Memory;
	int GameOver, GameClear;

	int Bird[4];
}grData;

//音データ
typedef struct
{
	int Start, Select, Decide, Back;
	int KeyPush;
	int Space;
	int LvUp;
	int GameOver, GameClear;
}seData;

//二次元座標
typedef struct
{
	int x, y;
}Position;

//自キャラ変数
typedef struct
{
	//座標
	Position c;
	//レベル
	int lv;
	//餌の持ち数
	int bug;
}Bird;

//アイテム変数
typedef struct
{
	//座標
	Position o;
	//高さ
	int height;
	//存在判定
	int exist_flag;
	// 風船存在判定
	int baloon_flag;
}Object;

/* 【列挙型】 */
enum eScene		//場面
{
	TITLE,
	OPNING,
	SELECT,
	MAINGAME,
	PAUSE,
	GAMEOVER,
	GAMECLEAR
};

enum eTitle		//タイトル場面
{
	START,
	MENU,
	OPTION
};