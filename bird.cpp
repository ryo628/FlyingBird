//	Flying_Bird (��)
//
//	bonochof �� @ryo_628
//

#include<stdio.h>
#include<math.h>
#include "DxLib.h"
#include "struct.h"

//�e��萔
#define CHARA_X 50
#define CHARA_Y 50
#define ITEM_X 30
#define ITEM_Y 30

/* �y�O���[�o���ϐ��z */
grData grp = {0};		//�摜�f�[�^
seData se = {0};		//���f�[�^
int exitFlag = 0;		//�\�t�g�I���t���O
int scene = TITLE;		//���
int music = 1;			//���y
char Key[256];			//�L�[���͏��
int loopcount = 0;		//�A�\�r�p�J�E���^�[

/* �y�֐��v���g�^�C�v�z */
void InitGrp();										//�摜�ǂݍ���
void InitSe();										//�����ǂݍ���
void Input();										//����
void Fullscreen(bool num);							//�X�N���[���؂�ւ�
void InitBird(Bird *b);								//�ϐ�������
void InitObj(Object *o);							//�A�C�e��������
void Title(bool *winMode);							//��ʁF�^�C�g��
void Title_Draw(int, int);							//�`��F�^�C�g��

void Opening();
void Maingame(Bird *b, Object *o,int *starttime);	//��ʁF���C���Q�[��
void ItemCot(Object *o, int time);					//�A�C�e������
void MainGameMove(Bird *b);							//��ʁF���C���Q�[��
void ItemGet(Bird *b, Object *o);					//�A�C�e������
int CalcDistance(Position, Position);				//��_�ԋ����v�Z

void MainGame_Draw(Bird *b, Object *o, int time);	//�`��F���C���Q�[��
void Pause();										//��ʁF�|�[�Y
void Gameover();									//��ʁF�Q�[���I�[�o�[
void Gameclear();									//��ʁF�Q�[���N���A
void PlayMusic(int);								//���y�Đ�

/* �y���C���֐��z */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	//�L�����ϐ�
	Bird bird;
	Object item;
	//�E�B���h�E���[�h(1:�E�B���h�E, 0:�t���X�N���[��)
	bool winMode = true;
	//�J�n���ԕۑ�
	int starttime = 0;

	//��ʃ��[�h�̃Z�b�g
	SetGraphMode(640, 480, 16);
	//Log.txt�̏o�͂�}��
	SetOutApplicationLogValidFlag(false);
	//�^�C�g���o�[�̕ύX
	SetMainWindowText("Flying Bird!");
	//�E�B���h�E���[�h�ŋN��
	ChangeWindowMode(winMode);
	//�c�w���C�u��������������
	if (DxLib_Init() == -1) return -1;

	//�摜�f�[�^�̃��[�h
	InitGrp();
	//���ʉ��f�[�^�̃��[�h
	InitSe();
	//�ϐ�������
	InitBird(&bird);
	InitObj(&item);

	//���y�Đ�
	PlayMusic(0);
	//���ԏ�����
	starttime = GetNowCount();

	//���C�����[�v
	while(  ProcessMessage() == 0 && 
			CheckHitKey( KEY_INPUT_ESCAPE ) == 0 && 
			exitFlag == 0 )
	{
		Input();			//����
		ClearDrawScreen();	//��ʃN���A

		switch (scene)		//��ʕ���
		{
		case TITLE:			//�^�C�g��
			Title(&winMode);
			break;
		case OPNING:
			Opening();
			break;
		case MAINGAME:		//���C���Q�[��
			Maingame(&bird,&item,&starttime);
			break;
		case PAUSE:			//�|�[�Y���
			Pause();
			break;
		case GAMEOVER:		//�Q�[���I�[�o�[
			Gameover();
			InitBird(&bird);
			break;
		case GAMECLEAR:		//�Q�[���N���A
			Gameclear();
			InitBird(&bird);
			break;
		}
		//����ʏo��
		ScreenFlip();		

		//�J�E���g		
		loopcount = loopcount++ % 1000;
	}

	//DX���C�u�����I������
	DxLib_End();			
	return 0;
}

/* �摜�ǂݍ��� */
void InitGrp()
{
	//����ʐݒ�
	SetDrawScreen( DX_SCREEN_BACK );
	//���ߐF�̕ύX
	SetTransColor( 0,255,0 );	

	/* �摜�ǂݍ��� */
	//�w�i
	grp.Back = LoadGraph( "Data/Graphic/back.png" );

	//�^�C�g������
	grp.Title = LoadGraph( "Data/Graphic/back_title.png" );
	grp.Title_Start = LoadGraph( "Data/Graphic/back_start.png" );
	grp.Title_Menu = LoadGraph( "Data/Graphic/back_menu.png" );
	grp.Title_Option = LoadGraph( "Data/Graphic/back_option.png" );

	//������
	grp.Pause = LoadGraph( "Data/Graphic/pause.jpg" );
	grp.GameOver = LoadGraph( "Data/Graphic/gameover.png" );
	grp.GameClear = LoadGraph( "Data/Graphic/gameclear.png" );

	//�J�[�\������
	grp.Cursor = LoadGraph("Data/Graphic/cursor.png");
	grp.KeyBack = LoadGraph("Data/Graphic/keyback.png");

	//�L����
	LoadDivGraph("Data/Graphic/bird.png", 4, 4, 1, 300, 300, grp.Bird);
}

/* ���ǂݍ��� */
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

/* ���� */
void Input()
{
	//���ׂẴL�[�����擾
	GetHitKeyStateAll( Key );	
}

/* �X�N���[�����[�h�؂�ւ� */
void Fullscreen(bool num)
{	
	num = !num;
	ChangeWindowMode( num );
	InitGrp();
}

/* �e��ϐ�������*/
//���L����������
void InitBird(Bird *b)
{
	//���L�������W������
	b->c.x = 320;
	b->c.y = 240;
	b->lv = 1;
	b->bug = 0;
}

//�A�C�e��������
void InitObj(Object *o)
{
	//�ϐ�������
	o->height = 0;
	o->o.y = 0;
	o->exist_flag = 0;
	o->baloon_flag = 1;
}

/* �^�C�g�� */
void Title(bool *winMode)
{
	//�^�C�g���ł̏��
	static int titlescene = START;
	//�J�[�\���l
	static int menu = 0;				

	//�X�^�[�g���
	if( titlescene == START )			
	{
		if( Key[ KEY_INPUT_RETURN ] )
		{
			titlescene = MENU;
			PlaySoundMem( se.Start, DX_PLAYTYPE_NORMAL );
		}
	}
	//���j���[���
	else if( titlescene == MENU )		
	{
		//���莞
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
		//�L�[��ړ�
		else if( Key[ KEY_INPUT_UP ] )
		{
			menu--;
			if( menu < 0 )
				menu = 2;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
		//�L�[���ړ�
		else if( Key[ KEY_INPUT_DOWN ] )
		{
			menu++;
			if( menu > 2 )
				menu = 0;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
	}
	//�I�v�V�������
	else if( titlescene == OPTION )		
	{
		//���莞
		if( Key[ KEY_INPUT_RETURN ] )
		{
			switch( menu )
			{
			case 0:						//�t���X�N���[��
				Fullscreen(winMode);
				break;
			case 1:						//���y
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
		//�L�[��ړ�
		else if( Key[ KEY_INPUT_UP ] )
		{
			menu--;
			if( menu < 0 )
				menu = 2;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
		//�L�[���ړ�
		else if( Key[ KEY_INPUT_DOWN ] )
		{
			menu++;
			if( menu > 2 )
				menu = 0;
			PlaySoundMem( se.Select, DX_PLAYTYPE_NORMAL );
		}
	}

	//�`��
	Title_Draw( titlescene, menu );		
}

/* �^�C�g���`�� */
void Title_Draw( int titlescene, int menu )
{
	//�ǎ�
	DrawGraph( 0, 0, grp.Back, false );

	//�^�C�g�����S
	DrawGraph( 0, 0, grp.Title, true );							
	
	//�I����ʃ��S
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

	//�J�[�\��
	if( titlescene )
		DrawGraph( 450, 250 + (45 * menu), grp.Cursor, true );	
}

/* �I�[�v�j���O���o */
void Opening()
{

}

/* ���C���Q�[�� */
void Maingame(Bird *b,Object *o,int *starttime)
{
	//���Ԍv�Z
	static int time=0;
	time = GetNowCount() - *starttime;

	//����
	MainGameMove(b);

	//�A�C�e��
	ItemCot(o, time);

	//�A�C�e������
	ItemGet(b,o);

	//�`��
	MainGame_Draw(b,o,time);
}

//�A�C�e������
void ItemCot(Object *o,int time)
{
	//���݂��鎞
	if (o->exist_flag != 0)
	{
		//�A���鎞
		if (o->baloon_flag == 1)
		{
			//�������
			o->o.x--;

			//�Ղ��Ղ�
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
		//�A�Ȃ���
		else
		{
			//������
			o->o.y++;
		}

		//��ʊO�ɏo����
		if (o->o.x < 0)
		{
			//����
			o->exist_flag = 0;
		}
	}
	//�A�C�e�������݂��Ȃ���
	if (o->exist_flag == 0)
	{
		//����
		if (rand() % 15 < 3)
		{
			//���W����
			o->exist_flag = 1;
			o->height = 340 - rand() % 100;
			o->o.y = o->height;
			o->o.x = 640;
			o->baloon_flag = 1;
		}
	}
}

//�A�C�e������
void ItemGet(Bird *b, Object *o)
{
	//�A�Ƃ̓����蔻��
	if (CalcDistance(o->o, b->c) < 80) o->baloon_flag = 0;
	
	//�����蔻��
	if (o->baloon_flag == 0 && (b->c.x < (o->o.x + 30) && (b->c.x + 50) > o->o.x) && (b->c.y < (o->o.y + 30) && (b->c.y + 50) > o->o.y))
	{
		o->exist_flag = 0;
		b->bug++;
	}
}

//2�_�ԋ����v�Z
int CalcDistance(Position a, Position b)
{
	double dis;

	dis = pow(a.x + CHARA_X / 2 - b.x, 2.0) + pow(a.y + CHARA_Y / 2 - b.y, 2.0);
	dis = sqrt(dis);

	clsDx();
	printfDx("dis=%f",dis);

	return (int)dis;
}

/* ���C���Q�[������ */
void MainGameMove(Bird *b)
{
	/* ���L�����ړ� */
	//��ړ�
	if (Key[KEY_INPUT_UP])
	{
		b->c.y -= b->lv;
	}
	//����
	else
	{
		b->c.y++;
	}
	//�E�ړ�
	if (Key[KEY_INPUT_RIGHT])
	{
		b->c.x += b->lv * 2;
	}
	//���ړ�
	if (Key[KEY_INPUT_LEFT])
	{
		b->c.x -= b->lv * 2;
	}

	//����
	//�オ��Ȃ�
	if (b->c.y <= 0)
		b->c.y = 0;
	//�������玀��
	if (b->c.y + 32 >= 480)
		scene = GAMEOVER;
	//�߂�Ȃ�
	if (b->c.x <= 0)
		b->c.x = 0;
	//�s���߂��Ȃ�
	if (b->c.x >= 500)
		b->c.x = 500;
}

/* ���C���Q�[���`�� */
void MainGame_Draw(Bird *b, Object *o,int time)
{
	//�w�i�\��
	static int wallroll = 640;
	DrawGraph(wallroll, 0, grp.Back, false);
	DrawGraph(wallroll - 640, 0, grp.Back, false);

	//�ړ����x
	if (b->c.x >= 500)
		wallroll -= b->lv * 2;
	else
		wallroll--;
	//�I�[�o�[�����Ȃ�
	if (wallroll <=0)
		wallroll = 640;

	//���L�����\��
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
	//�H�΂���
	c_mode = c_mode % 4;
	DrawExtendGraph(b->c.x, b->c.y, b->c.x + CHARA_X - 1 , b->c.y + CHARA_Y - 1, grp.Bird[c_mode], true);

	//�A�C�e���\��
	if (o->baloon_flag == 1) DrawCircle(o->o.x + ITEM_X / 2, o->o.y + ITEM_Y / 2, 40, 40, GetColor(255, 0, 0), TRUE);	//�A
	DrawExtendGraph(o->o.x, o->o.y, o->o.x + ITEM_X - 1, o->o.y + ITEM_Y - 1 , grp.Bird[1], true);	//�a

	//���ԕ\��
	DrawFormatString(286, 440, GetColor(255, 255, 255), "%2d:%2d", time / 60000, time / 1000 % 60);
	
	DrawFormatString( 10, 40, GetColor( 255, 255, 255 ), "bug=%d", b->bug );
}

/* �|�[�Y��� */
void Pause()
{
	//��ʕ`��
	DrawGraph( 0, 0, grp.Pause, false );

	//�^�u�L�[�Ő؂�ւ�
	if( Key[ KEY_INPUT_TAB ] )	
	{
		PlaySoundMem( se.Back, DX_PLAYTYPE_NORMAL );
		scene = MAINGAME;
	}
}

/* �Q�[���I�[�o�[ */
void Gameover()
{
	if( Key[ KEY_INPUT_RETURN ] )
	{
		scene = TITLE;
		PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		PlayMusic( 0 );
	}

	//�w�i�\��
	DrawGraph( 0, 0, grp.GameOver, false );
}

/* �Q�[���N���A */
void Gameclear()
{
	if( Key[ KEY_INPUT_RETURN ] )
	{
		scene = TITLE;
		PlaySoundMem( se.Decide, DX_PLAYTYPE_NORMAL );
		PlayMusic( 0 );
	}

	//�w�i�\��
	DrawGraph( 0, 0, grp.GameClear, false );
}

/* ���y�Đ� */
void PlayMusic( int select )	//�����F���y�I����ʂ��ǂ���
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