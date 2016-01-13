//	Flying_Bird (仮)
//
//	bonochof と @ryo_628
//

#include<stdio.h>
#include<math.h>
#include "DxLib.h"
#include "struct.h"

//各種定数
#define CHARA_X 50
#define CHARA_Y 50
#define ITEM_X 30
#define ITEM_Y 30

/* 【グローバル変数】 */
grData grp = {0};		//画像データ
seData se = {0};		//音データ
int exitFlag = 0;		//ソフト終了フラグ
int scene = TITLE;		//場面
int music = 1;			//音楽
char Key[256];			//キー入力状態
int loopcount = 0;		//アソビ用カウンター

/* 【関数プロトタイプ】 */
void InitGrp();										//画像読み込み
void InitSe();										//音声読み込み
void Input();										//入力
void Fullscreen(bool num);							//スクリーン切り替え
void InitBird(Bird *b);								//変数初期化
void InitObj(Object *o);							//アイテム初期化
void Title(bool *winMode);							//場面：タイトル
void Title_Draw(int, int);							//描画：タイトル

void Opening();
void Maingame(Bird *b, Object *o,int *starttime);	//場面：メインゲーム
void ItemCot(Object *o, int time);					//アイテム制御
void MainGameMove(Bird *b);							//場面：メインゲーム
void ItemGet(Bird *b, Object *o);					//アイテム入手
int CalcDistance(Position, Position);				//二点間距離計算

void MainGame_Draw(Bird *b, Object *o, int time);	//描画：メインゲーム
void Pause();										//場面：ポーズ
void Gameover();									//場面：ゲームオーバー
void Gameclear();									//場面：ゲームクリア
void PlayMusic(int);								//音楽再生

/* 【メイン関数】 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//キャラ変数
	Bird bird;
	Object item;
	//ウィンドウモード(1:ウィンドウ, 0:フルスクリーン)
	bool winMode = true;
	//開始時間保存
	int starttime = 0;

	//画面モードのセット
	SetGraphMode(640, 480, 16);
	//Log.txtの出力を抑制
	SetOutApplicationLogValidFlag(false);
	//タイトルバーの変更
	SetMainWindowText("Flying Bird!");
	//ウィンドウモードで起動
	ChangeWindowMode(winMode);
	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) return -1;

	//画像データのロード
	InitGrp();
	//効果音データのロード
	InitSe();
	//変数初期化
	InitBird(&bird);
	InitObj(&item);

	//音楽再生
	PlayMusic(0);
	//時間初期化
	starttime = GetNowCount();

	//メインループ
	while(  ProcessMessage() == 0 && 
			CheckHitKey( KEY_INPUT_ESCAPE ) == 0 && 
			exitFlag == 0 )
	{
		Input();			//入力
		ClearDrawScreen();	//画面クリア

		switch (scene)		//画面分岐
		{
		case TITLE:			//タイトル
			Title(&winMode);
			break;
		case OPNING:
			Opening();
			break;
		case MAINGAME:		//メインゲーム
			Maingame(&bird,&item,&starttime);
			break;
		case PAUSE:			//ポーズ画面
			Pause();
			break;
		case GAMEOVER:		//ゲームオーバー
			Gameover();
			InitBird(&bird);
			break;
		case GAMECLEAR:		//ゲームクリア
			Gameclear();
			InitBird(&bird);
			break;
		}
		//裏画面出力
		ScreenFlip();		

		//カウント		
		loopcount = loopcount++ % 1000;
	}

	//DXライブラリ終了処理
	DxLib_End();			
	return 0;
}

/* 画像読み込み */
void InitGrp()
{
	//裏画面設定
	SetDrawScreen( DX_SCREEN_BACK );
	//透過色の変更
	SetTransColor( 0,255,0 );	

	/* 画像読み込み */
	//背景
	grp.Back = LoadGraph( "Data/Graphic/back.png" );

	//タイトル分岐
	grp.Title = LoadGraph( "Data/Graphic/back_title.png" );
	grp.Title_Start = LoadGraph( "Data/Graphic/back_start.png" );
	grp.Title_Menu = LoadGraph( "Data/Graphic/back_menu.png" );
	grp.Title_Option = LoadGraph( "Data/Graphic/back_option.png" );

	//特殊画面
	grp.Pause = LoadGraph( "Data/Graphic/pause.jpg" );
	grp.GameOver = LoadGraph( "Data/Graphic/gameover.png" );
	grp.GameClear = LoadGraph( "Data/Graphic/gameclear.png" );

	//カーソル周り
	grp.Cursor = LoadGraph("Data/Graphic/cursor.png");
	grp.KeyBack = LoadGraph("Data/Graphic/keyback.png");

	//キャラ
	LoadDivGraph("Data/Graphic/bird.png", 4, 4, 1, 300, 300, grp.Bird);
}

/* 音読み込み */
void InitSe()
{
	se.Start = LoadSoundMem( "Data/Sound/start.wav" );

	se.Select = LoadSoundMem( "Data/Sound/select.wav" );
	se.Decide = LoadSoundMem( "Data/Sound/decide.wav" );
	se.Back = LoadSoundMem( "Data/Sound/back.wav" );
	se.KeyPush = LoadSoundMem( "Data/Sound/keypush.wav" );

	se.LvUp = LoadSoundMem( "Data/Sound/lvup.wav" );

	se.GameOver = LoadSoundMem( "Data/Sound/gameover.wav" );
	se.GameClear = LoadSoundMem( "Data/Sound/gameclear.wav" );
}

/* 入力 */
void Input()
{
	//すべてのキー情報を取得
	GetHitKeyStateAll( Key );	
}

/* スクリーンモード切り替え */
void Fullscreen(bool num)
{	
	num = !num;
	ChangeWindowMode( num );
	InitGrp();
}

/* 各種変数初期化*/
//自キャラ初期化
void InitBird(Bird *b)
{
	//自キャラ座標初期化
	b->c.x = 320;
	b->c.y = 240;
	b->lv = 1;
	b->bug = 0;
}

//アイテム初期化
void InitObj(Object *o)
{
	//変数初期化
	o->height = 0;
	o->o.y = 0;
	o->exist_flag = 0;
	o->baloon_flag = 1;
}

/* タイトル */
void Title(bool *winMode)
{
	//タイトルでの場面
	static int titlescene = START;
	//カーソル値
	static int menu = 0;				

	//スタート画面
	if( titlescene == START )			
	{
		if( Key[ KEY_INPUT_RETURN ] )
		{
			titlescene = MENU;
			PlaySoundMem( se.Start, DX_PLAYTYPE_NORMAL );
		}
	}
	//メニュー画面
	else if( titlescene == MENU )		
	{
		//決定時
		if( Key[ KEY_INPUT_RETURN ] )
		{
			switch( menu )
			{
			case 0:						//SELECT
				scene = MAINGAME;
				break;
			case 1:						//OPTION
				titlescene = OPTION;
				menu = 0;
				break;
			case 2:						//EXIT
				exitFlag = 1;
				break;
			}
			PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		}
		//キー上移動
		else if( Key[ KEY_INPUT_UP ] )
		{
			menu--;
			if( menu < 0 )
				menu = 2;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
		//キー下移動
		else if( Key[ KEY_INPUT_DOWN ] )
		{
			menu++;
			if( menu > 2 )
				menu = 0;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
	}
	//オプション画面
	else if( titlescene == OPTION )		
	{
		//決定時
		if( Key[ KEY_INPUT_RETURN ] )
		{
			switch( menu )
			{
			case 0:						//フルスクリーン
				Fullscreen(winMode);
				break;
			case 1:						//音楽
				music++;
				if( music > 3 )
					music = 0;
				PlayMusic( 1 );
				break;
			case 2:						//BACK
				titlescene = MENU;
				menu = 0;
				PlayMusic( 0 );
				break;
			}
			PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		}
		//キー上移動
		else if( Key[ KEY_INPUT_UP ] )
		{
			menu--;
			if( menu < 0 )
				menu = 2;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
		//キー下移動
		else if( Key[ KEY_INPUT_DOWN ] )
		{
			menu++;
			if( menu > 2 )
				menu = 0;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
	}

	//描画
	Title_Draw( titlescene, menu );		
}

/* タイトル描画 */
void Title_Draw( int titlescene, int menu )
{
	//壁紙
	DrawGraph( 0, 0, grp.Back, false );

	//タイトルロゴ
	DrawGraph( 0, 0, grp.Title, true );							
	
	//選択画面ロゴ
	switch( titlescene )										
	{
	case START:
		DrawGraph( 0, 0, grp.Title_Start, true );
		break;
	case MENU:
		DrawGraph( 0, 0, grp.Title_Menu, true );
		break;
	case OPTION:
		DrawGraph( 0, 0, grp.Title_Option, true );
		break;
	}

	//カーソル
	if( titlescene )
		DrawGraph( 450, 250 + (45 * menu), grp.Cursor, true );	
}

/* オープニング演出 */
void Opening()
{

}

/* メインゲーム */
void Maingame(Bird *b,Object *o,int *starttime)
{
	//時間計算
	static int time=0;
	time = GetNowCount() - *starttime;

	//操作
	MainGameMove(b);

	//アイテム
	ItemCot(o, time);

	//アイテム入手
	ItemGet(b,o);

	//描写
	MainGame_Draw(b,o,time);
}

//アイテム操作
void ItemCot(Object *o,int time)
{
	//存在する時
	if (o->exist_flag != 0)
	{
		//泡ある時
		if (o->baloon_flag == 1)
		{
			//流される
			o->o.x--;

			//ぷかぷか
			if (o->exist_flag % 2 == 1)
			{
				o->o.y -= 2;
				if (o->o.y <= o->height - 150)o->exist_flag++;
			}
			else
			{
				o->o.y += 2;
				if (o->o.y >= o->height + 150)o->exist_flag++;
			}
		}
		//泡ない時
		else
		{
			//落ちる
			o->o.y++;
		}

		//画面外に出た時
		if (o->o.x < 0)
		{
			//消す
			o->exist_flag = 0;
		}
	}
	//アイテムが存在しない時
	if (o->exist_flag == 0)
	{
		//乱数
		if (rand() % 15 < 3)
		{
			//座標生成
			o->exist_flag = 1;
			o->height = 340 - rand() % 100;
			o->o.y = o->height;
			o->o.x = 640;
			o->baloon_flag = 1;
		}
	}
}

//アイテム入手
void ItemGet(Bird *b, Object *o)
{
	//泡との当たり判定
	if (CalcDistance(o->o, b->c) < 80) o->baloon_flag = 0;
	
	//当たり判定
	if (o->baloon_flag == 0 && (b->c.x < (o->o.x + 30) && (b->c.x + 50) > o->o.x) && (b->c.y < (o->o.y + 30) && (b->c.y + 50) > o->o.y))
	{
		o->exist_flag = 0;
		b->bug++;
	}
}

//2点間距離計算
int CalcDistance(Position a, Position b)
{
	double dis;

	dis = pow(a.x + CHARA_X / 2 - b.x, 2.0) + pow(a.y + CHARA_Y / 2 - b.y, 2.0);
	dis = sqrt(dis);

	clsDx();
	printfDx("dis=%f",dis);

	return (int)dis;
}

/* メインゲーム操作 */
void MainGameMove(Bird *b)
{
	/* 自キャラ移動 */
	//上移動
	if (Key[KEY_INPUT_UP])
	{
		b->c.y -= b->lv;
	}
	//落下
	else
	{
		b->c.y++;
	}
	//右移動
	if (Key[KEY_INPUT_RIGHT])
	{
		b->c.x += b->lv * 2;
	}
	//左移動
	if (Key[KEY_INPUT_LEFT])
	{
		b->c.x -= b->lv * 2;
	}

	//判定
	//上がれない
	if (b->c.y <= 0)
		b->c.y = 0;
	//落ちたら死ぬ
	if (b->c.y + 32 >= 480)
		scene = GAMEOVER;
	//戻れない
	if (b->c.x <= 0)
		b->c.x = 0;
	//行き過ぎない
	if (b->c.x >= 500)
		b->c.x = 500;
}

/* メインゲーム描画 */
void MainGame_Draw(Bird *b, Object *o,int time)
{
	//背景表示
	static int wallroll = 640;
	DrawGraph(wallroll, 0, grp.Back, false);
	DrawGraph(wallroll - 640, 0, grp.Back, false);

	//移動速度
	if (b->c.x >= 500)
		wallroll -= b->lv * 2;
	else
		wallroll--;
	//オーバーさせない
	if (wallroll <=0)
		wallroll = 640;

	//自キャラ表示
	static int c_mode = 0;
	if (b->c.y >= 300)
	{
		if (loopcount % 3 == 0) c_mode++;
	}
	else if (b->c.y >= 150)
	{
		if (loopcount % 5 == 0) c_mode++;
	}
	else
	{
		if (loopcount % 10 == 0) c_mode++;
	}
	//羽ばたき
	c_mode = c_mode % 4;
	DrawExtendGraph(b->c.x, b->c.y, b->c.x + CHARA_X - 1 , b->c.y + CHARA_Y - 1, grp.Bird[c_mode], true);

	//アイテム表示
	if (o->baloon_flag == 1) DrawCircle(o->o.x + ITEM_X / 2, o->o.y + ITEM_Y / 2, 40, 40, GetColor(255, 0, 0), TRUE);	//泡
	DrawExtendGraph(o->o.x, o->o.y, o->o.x + ITEM_X - 1, o->o.y + ITEM_Y - 1 , grp.Bird[1], true);	//餌

	//時間表示
	DrawFormatString(286, 440, GetColor(255, 255, 255), "%2d:%2d", time / 60000, time / 1000 % 60);
	
	DrawFormatString( 10, 40, GetColor( 255, 255, 255 ), "bug=%d", b->bug );
}

/* ポーズ画面 */
void Pause()
{
	//画面描写
	DrawGraph( 0, 0, grp.Pause, false );

	//タブキーで切り替え
	if( Key[ KEY_INPUT_TAB ] )	
	{
		PlaySoundMem( se.Back, DX_PLAYTYPE_NORMAL );
		scene = MAINGAME;
	}
}

/* ゲームオーバー */
void Gameover()
{
	if( Key[ KEY_INPUT_RETURN ] )
	{
		scene = TITLE;
		PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		PlayMusic( 0 );
	}

	//背景表示
	DrawGraph( 0, 0, grp.GameOver, false );
}

/* ゲームクリア */
void Gameclear()
{
	if( Key[ KEY_INPUT_RETURN ] )
	{
		scene = TITLE;
		PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		PlayMusic( 0 );
	}

	//背景表示
	DrawGraph( 0, 0, grp.GameClear, false );
}

/* 音楽再生 */
void PlayMusic( int select )	//引数：音楽選択画面かどうか
{
	if( select )
	{
		if( music == 0 )
			StopMusic();
		else if( music == 1 )
			PlayMusic( "KeyBoarDefense_Data/Sound/bgm01.mp3", DX_PLAYTYPE_LOOP );
		else if( music == 2 )
			PlayMusic( "KeyBoarDefense_Data/Sound/bgm02.mp3", DX_PLAYTYPE_LOOP );
		else if( music == 3 )
			PlayMusic( "KeyBoarDefense_Data/Sound/bgm03.mp3", DX_PLAYTYPE_LOOP );
	}
	else
	{
		switch( scene )
		{
		case TITLE:
		case SELECT:
			PlayMusic( "KeyBoarDefense_Data/Sound/titlebgm.mp3", DX_PLAYTYPE_LOOP );
			break;
		case MAINGAME:
			if( music == 0 )
				StopMusic();
			else if( music == 1 )
				PlayMusic( "KeyBoarDefense_Data/Sound/bgm01.mp3", DX_PLAYTYPE_LOOP );
			else if( music == 2 )
				PlayMusic( "KeyBoarDefense_Data/Sound/bgm02.mp3", DX_PLAYTYPE_LOOP );
			else if( music == 3 )
				PlayMusic( "KeyBoarDefense_Data/Sound/bgm03.mp3", DX_PLAYTYPE_LOOP );
			break;
		}
	}
}