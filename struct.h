/* �y�\���̌^�z */
//�摜�f�[�^
typedef struct
{
	int Back, Title, Title_Start, Title_Menu, Title_Option;
	int Pause;
	int Cursor, KeyBack, Space;
	int Level, Star, Memory;
	int GameOver, GameClear;

	int Bird[4];
}grData;

//���f�[�^
typedef struct
{
	int Start, Select, Decide, Back;
	int KeyPush;
	int Space;
	int LvUp;
	int GameOver, GameClear;
}seData;

//�񎟌����W
typedef struct
{
	int x, y;
}Position;

//���L�����ϐ�
typedef struct
{
	//���W
	Position c;
	//���x��
	int lv;
	//�a�̎�����
	int bug;
}Bird;

//�A�C�e���ϐ�
typedef struct
{
	//���W
	Position o;
	//����
	int height;
	//���ݔ���
	int exist_flag;
	// ���D���ݔ���
	int baloon_flag;
}Object;

/* �y�񋓌^�z */
enum eScene		//���
{
	TITLE,
	OPNING,
	SELECT,
	MAINGAME,
	PAUSE,
	GAMEOVER,
	GAMECLEAR
};

enum eTitle		//�^�C�g�����
{
	START,
	MENU,
	OPTION
};