#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable: 4996)
//һ���򵥵�SNL���Դʷ�������
//����Ϊ�ʷ�������+�﷨������
#include"iostream"
#include"string"
#include"fstream"
#include"queue"
#include"math.h"
#include"string.h"
#include"vector"
#include"stack"
using namespace std;

//����Ϊ�ʷ����������ݺͺ�������

class Token
{
public:
	int Type;     // 1Ϊ�����֣�2Ϊ��ʶ����3Ϊ�޷���������4Ϊ�������,9Ϊ��������
	int Line;     //���ִ�����Ҫ����������δ���ִ��������䱣��Ϊ0
	string Item;
	Token() { Line = 0; }
};

string reserved[24];      //������
char character[19];          //�������
queue<string>  IDTable;      //��ʶ����
queue<int>     NumTable;     //���ֱ�
queue<Token>   TKTable;      //Token��
char   CurrentChar = '\0';   //��ŵ�ǰ������ַ�
string CurrentString = "";   //��ŵ�ǰ����ĵ���
int    CurrentLine = 1;      //��ŵ�ǰ������

Token TK1;                   //��������ǰ��token

void Judge();
void Judge1();
void Judge2();
void Judge3();
void Initiation(); //��ʼ������
bool IsReserved(); //�ж��Ƿ�Ϊ������
bool IsChar();     //�ж��Ƿ�Ϊ�����ַ�
bool IsID();       //�ж��Ƿ�Ϊ��ʶ��
bool IsNumber();   //�ж��Ƿ�Ϊ����
void getToken();
bool CheckStr();   //���Token�б����Ƿ��д���ĵ���


//����Ϊ�﷨���������ݺͺ�������
class Tree {       //������������ֵܵĽṹ�������
public:
	string Item;           //��ŵ��ַ���
	vector<Tree*> Children;//������Ŷ���
	Tree(string S) { Item = S; }
	void add(Tree* T) {
		Children.push_back(T);
	}
};
Tree* root;        //��������﷨��
int ERRCount = 0;
int ErrorFlag = 0;

void Display(Tree* T,int depth, vector<int>* v, bool buttom);//��ʾ���ɵ��﷨��
void UseToken();
void ERROR(string S);
Tree* MatchReserved(string S); // ƥ�䱣���� ������Ҷ�ӽڵ�
Tree* MatchID();               // ƥ���ʶ��
Tree* MatchNumber();           // ƥ���޷�������
Tree* MatchChar(char S);     // ƥ���ַ�
/**********************/
/*****�﷨��������*****/
/**********************/
Tree* Program();
Tree* ProgramHead();
Tree* DeclarePart();
Tree* ProgramBody();
//ProgramHead
//Tree* ProgramName();   ��ֱ�Ӽ��ɵ�ProgramHead��
//DeclarePart
Tree* TypeDecPart();
Tree* VarDecPart();
Tree* ProcDecpart();
//TypeDecPart
Tree* TypeDec();
Tree* TypeDecList();
Tree* TypeID();
Tree* TypeDef();
Tree* TypeDecMore();
//TypeDef
Tree* BaseType();
Tree* StructureType();
Tree* ArrayType();
Tree* RecType();
Tree* FieldDecList();
Tree* IDList();
Tree* FieldDecMore();
Tree* IDMore();
//VarDecPart
Tree* VarDec();
Tree* VarDecList();
Tree* VarIDList();
Tree* VarDecMore();
Tree* VarIDMore();
//ProcDecpart
Tree* ProcDec();
//Tree* ProcName();
Tree* ParamList();
Tree* ProcDecPart();
Tree* ProcBody();
Tree* ProcDeclaration();
//ParamList
Tree* ParamDecList();
Tree* Param();
Tree* ParamMore();
Tree* FormList();
Tree* FIDMore();
//ProcDecPart

//ProcBody

//ProgramBody
Tree* StmList();
//StmList
Tree* Stm();
Tree* StmMore();
//Stm
Tree* ConditionalStm();
Tree* LoopStm();
Tree* InputStm();
Tree* OutputStm();
Tree* ReturnStm();
Tree* AssCall();
Tree* AssignmentRest();
Tree* CallStmRest();
//AssignmentRest
Tree* VariMore();
Tree* Exp();
//ConditionalStm
Tree* RelExp();
//LoopStm

//InputStm
Tree* Invar();
//OutputStm

//ReturnStm

//CallStmRest
Tree* ActParamList();
Tree* ActParamMore();
//RelExp
Tree* OtherRelE();
Tree* CmpOp();
//Exp
Tree* Term();
Tree* OtherTerm();
Tree* AddOp();
//Term
Tree* Factor();
Tree* OtherFactor();
Tree* MultOp();
//Factor
Tree* Variable();
Tree* FieldVar();
Tree* FieldVarMore();


/*****�˴���ʼΪ������*****/

ifstream fin("program.txt");    // �ļ�������
int main()
{
	Initiation();
	getToken();

	if (!CheckStr()) {
		queue<Token> TK;
		cout << "�õ���Token���£�" << endl;
		cout << "���� ����" << endl;
		for (TK = TKTable; TK.size() > 0; TK.pop()) {
			cout << TK.front().Type << "  " << TK.front().Item << endl;
		}
		//OutputToken();
	}
	fin.close();
	root = Program();
	vector<int>* v = new vector<int>();
	v->push_back(0);
	 Display(root,0, v, false);
	
	return 0;

}
/*****����������*****/

void Judge()
{
	if ((CurrentChar >= 'A' && CurrentChar <= 'Z') ||     //�ж��Ƿ�Ϊ��ĸ��ͷ�ĵ���
		(CurrentChar >= 'a' && CurrentChar <= 'z')) {
		Judge1();
	}
	else
		if (CurrentChar >= '0' && CurrentChar <= '9') {     //�ж��Ƿ�Ϊ����
			Judge2();
		}
		else {                                             //�ж��Ƿ�Ϊ�����ַ��������ַ�
			Judge3();
		}

}

void Judge1()                                      // �ж��Ƿ�Ϊ�����ֻ��ʶ��
{
	CurrentString.append(1, CurrentChar);
	while (((CurrentChar >= 'A' && CurrentChar <= 'Z') ||
		(CurrentChar >= 'a' && CurrentChar <= 'z') ||
		(CurrentChar >= '0' && CurrentChar <= '9')) &&
		!fin.eof())
	{
		fin >> noskipws >> CurrentChar;
		if ((CurrentChar >= 'A' && CurrentChar <= 'Z') ||
			(CurrentChar >= 'a' && CurrentChar <= 'z') ||
			(CurrentChar >= '0' && CurrentChar <= '9'))
			CurrentString.append(1, CurrentChar);
	}//while

	if (!fin.eof()) fin.seekg(-1, ios::cur); //�����˲�����ĸ�����ֵ��ַ����˻�Դ�����ļ�ָ��һλ

	if (IsReserved()) {
		TK1.Type = 1;                    // ����1Ϊ������
		TK1.Item = CurrentString;
		TK1.Line = CurrentLine;
		TKTable.push(TK1);
	}
	else {
		TK1.Type = 2;                    // ����2Ϊ��ʶ��
		TK1.Item = CurrentString;
		TK1.Line = CurrentLine;
		TKTable.push(TK1);               // ����Token��
		IDTable.push(CurrentString);     // ����ʶ������IDTable��
	}//if
}

void Judge2()
{

	CurrentString.append(1, CurrentChar);
	while ((CurrentChar >= '0' && CurrentChar <= '9') &&
		!fin.eof())
	{
		fin >> noskipws >> CurrentChar;
		if (CurrentChar >= '0' && CurrentChar <= '9')
			CurrentString.append(1, CurrentChar);
	}//while

	if (!fin.eof()) fin.seekg(-1, ios::cur);//�����˲������ֵ��ַ����˻�Դ�����ļ�ָ��һλ

	int num = 0;
	for (int i = 0; i < CurrentString.length(); i++)
		num += ((int)CurrentString[CurrentString.length() - i - 1] - 48) * pow(10, i);

	NumTable.push(num);
	TK1.Type = 3;                    // ����3Ϊ�޷�������
	TK1.Item = CurrentString;
	TK1.Line = CurrentLine;
	TKTable.push(TK1);
}

void Judge3()
{
	if (!IsChar()) {

		CurrentString.append(1, CurrentChar);
		TK1.Type = 9;                               // ����9Ϊ��������
		TK1.Item = CurrentString;
		TK1.Line = CurrentLine;
		TKTable.push(TK1);
		return;
	}
	if (CurrentChar == ' ' || CurrentChar == '\n' || CurrentChar == '\r') return;  //����Ҫ���ո�س�����¼��Token��
	if (CurrentChar == ':' && !fin.eof())          //�����ð�Ż�Ҫ�ж��Ƿ���ð�ŵ���
	{
		CurrentString.append(1, CurrentChar);
		fin >> noskipws >> CurrentChar;
		if (CurrentChar == '=')
		{
			CurrentString.append(1, CurrentChar);
			TK1.Type = 4;                    // ����4Ϊ�������
			TK1.Item = CurrentString;
			TK1.Line = CurrentLine;
			TKTable.push(TK1);
			return;
		}
	}

	CurrentString.append(1, CurrentChar);
	TK1.Type = 4;                    // ����4Ϊ�������
	TK1.Item = CurrentString;
	TK1.Line = CurrentLine;
	TKTable.push(TK1);
}

bool IsReserved()
{
	for (int i = 0; i < 24; i++)
		if (!stricmp(CurrentString.c_str(), reserved[i].c_str())) // �ж��Ƿ�Ϊ������
			return true;
	return false;
}
bool IsChar()
{
	for (int i = 0; i < 19; i++)
		if (CurrentChar == character[i]) return true;              //�ж��Ƿ�Ϊ�����ַ�
	return false;
}
bool IsID() {
	queue<string> Q = IDTable;
	for (; Q.size() > 0; Q.pop())
		if (CurrentString == Q.front()) return true;
	return false;
}
bool IsNumber() {
	int num = 0;
	for (int i = 0; i < CurrentString.length(); i++)
		num += ((int)CurrentString[CurrentString.length() - i - 1] - 48) * pow(10, i);
	queue<int> Q = NumTable;
	for (; Q.size() > 0; Q.pop())
		if (num == Q.front()) return true;
	return false;
}
void Initiation() {     //���ñ����֡��������
	reserved[0] = "program";
	reserved[1] = "id";
	reserved[2] = "type";
	reserved[3] = "integer";
	reserved[4] = "char";
	reserved[5] = "array";
	reserved[6] = "of";
	reserved[7] = "intc";
	reserved[8] = "record";
	reserved[9] = "begin";
	reserved[10] = "end";
	reserved[11] = "var";
	reserved[12] = "procedure";
	reserved[13] = "if";
	reserved[14] = "then";
	reserved[15] = "else";
	reserved[16] = "fi";
	reserved[17] = "while";
	reserved[18] = "do";
	reserved[19] = "endwh";
	reserved[20] = "read";
	reserved[21] = "write";
	reserved[22] = "return";
	reserved[23] = "EOF";

	character[0] = '+';
	character[1] = '-';
	character[2] = '*';
	character[3] = '/';
	character[4] = '<';
	character[5] = '=';
	character[6] = '(';
	character[7] = ')';
	character[8] = '[';
	character[9] = ']';
	character[10] = '{';
	character[11] = '}';
	character[12] = '.';
	character[13] = ';';
	character[14] = ':';
	character[15] = '\'';
	character[16] = ' ';
	character[17] = '\n';
	character[18] = ',';
}

void getToken()
{
	while (fin.peek() != EOF)
	{
		CurrentString = "";
		fin >> noskipws >> CurrentChar;
		if (CurrentChar == '\n') CurrentLine++; //����������ǰ����+1
		Judge();
	}
}
bool CheckStr() {
	queue<Token> TK = TKTable;
	int flag = 0;
	for (TK = TKTable; TK.size() > 0; TK.pop()) {
		if (TK.front().Type == 9) {
			cout << "Error:line " << TK.front().Line << "  \"" << TK.front().Item << "\" is not a valid character." << endl;
			flag = 1;
		}
	}
	if (flag) return true;
	return false;
}


/*******************************************************************************/
/*******************                                          ******************/
/*******************      ����Ϊ�﷨�����ĺ���ʵ��            ******************/
/*******************                                          ******************/
/*******************************************************************************/
void UseToken() {
	CurrentString = TKTable.front().Item;
	CurrentLine = TKTable.front().Line;
	TKTable.pop();
}
void ERROR(string S) {
	ERRCount++;
	cout << "error in line: " << CurrentLine << endl;
	cout << S << endl;
	exit(0);
}
/***************************/
/***ƥ�䲢������ӦҶ�ڵ�****/
/***************************/
Tree* MatchReserved(string S) {
	UseToken();
	if (IsReserved() && !strcmpi(CurrentString.c_str(), S.c_str())) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	string s = "Reserved word error! " + S + " exepected";
	ERROR(s);
	return NULL;
}
Tree* MatchID() {
	UseToken();
	if (IsID()) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	ERROR("Identifier error! ");
	return NULL;
}
Tree* MatchNumber() {
	UseToken();
	if (IsNumber()) {
		Tree* t = new Tree(CurrentString);
		return t;
	}
	ERROR("Number error! ");
	return NULL;
}
Tree* MatchChar(char S) {
	UseToken();
	string str = "0";
	str[0] = S;
	if (IsChar() && CurrentString[0] == S) 
	{
		if (CurrentString.length() == 2&&CurrentString[1] == '=') 
		{
			if (CurrentString[1] == '=') 
			{
				Tree* t = new Tree(CurrentString);
				return t;
			}
		}
		else {
			Tree* t = new Tree(CurrentString);
			return t;
		}
	}
	string s = "Character error! " + str + " exepected";
	ERROR(s);
	return NULL;
}
/****************************/
/********�﷨��������********/
/****************************/
Tree* Program() {
	Tree* T = new Tree("Program");
	T->add(ProgramHead());
	T->add(DeclarePart());
	T->add(ProgramBody());
	return T;
}
Tree* ProgramHead() {
	Tree* T = new Tree("ProgramHead");
	T->add(MatchReserved("program"));
	T->add(MatchID());
	return T;
}
Tree* DeclarePart() {
	Tree* T = new Tree("DeclarePart");
	T->add(TypeDecPart());
	T->add(VarDecPart());
	T->add(ProcDecpart());
	return T;
}
Tree* ProgramBody() {
	Tree* T = new Tree("ProgramBody");
	T->add(MatchReserved("begin"));
	T->add(StmList());
	T->add(MatchReserved("end"));
	return T;
}
//ProgramHead
//Tree* ProgramName(){Tree* T = new Tree();return T;}   ��ֱ�Ӽ��ɵ�ProgramHead��
//DeclarePart
Tree* TypeDecPart() {
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "type")) {
		T = new Tree("TypeDecPart");
		T->add(TypeDec());
	}
	return T;
}
Tree* VarDecPart() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "var")) {
		T = new Tree("VarDecPart");
		T->add(VarDec());
	}
	return T;
}
Tree* ProcDecpart() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "procedure")) {
		T = new Tree("ProcDecpart");
		T->add(ProcDec());
	}
	return T;
}
//TypeDecPart
Tree* TypeDec() {
	Tree* T = new Tree("TypeDec");
	T->add(MatchReserved("type"));
	T->add(TypeDecList());
	return T;
}
Tree* TypeDecList() {
	Tree* T = new Tree("TypeDecList");
	T->add(TypeID());
	T->add(MatchChar('='));
	T->add(TypeDef());
	T->add(MatchChar(';'));
	T->add(TypeDecMore());
	return T;
}
Tree* TypeID() {
	Tree* T = new Tree("TypeID");
	T->add(MatchID());
	return T;
}
Tree* TypeDef() {
	Tree* T = new Tree("TypeDef");
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char")) {
		T->add(BaseType());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "array") ||
		!strcmp(TKTable.front().Item.c_str(), "record")) {
		T->add(StructureType());
		return T;
	}
	T->add(MatchID());
	return T;
}
Tree* TypeDecMore() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (IsID()) {
		T = new Tree("TypeDecMore");
		T->add(TypeDecList());
	}
	CurrentString = Temp;
	return T;
}
//TypeDef
Tree* BaseType() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "integer")) {
		T = new Tree("BaseType");
		T->add(MatchReserved("integer"));
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "char")) {
		T = new Tree("BaseType");
		T->add(MatchReserved("char"));
		return T;
	}
	UseToken();
	ERROR(CurrentString + " is not a base type! ");
	return T;
}
Tree* StructureType() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "array")) {
		T = new Tree("StructureType");
		T->add(ArrayType());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "record")) {
		T = new Tree("BaseType");
		T->add(RecType());
		return T;
	}
	return T;
}
Tree* ArrayType() {
	Tree* T = new Tree("ArrayType");
	T->add(MatchReserved("array"));
	T->add(MatchChar('['));
	T->add(MatchNumber());
	T->add(MatchChar('.'));
	T->add(MatchChar('.'));
	T->add(MatchNumber());
	T->add(MatchChar(']'));
	T->add(MatchReserved("of"));
	T->add(BaseType());
	return T;
}
Tree* RecType() {
	Tree* T = new Tree("RecType");
	T->add(MatchReserved("record"));
	T->add(FieldDecList());
	T->add(MatchReserved("end"));
	return T;
}
Tree* FieldDecList() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char")) {
		T = new Tree("FieldDecList");
		T->add(BaseType());
		T->add(IDList());
		T->add(MatchChar(';'));
		T->add(FieldDecMore());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "array")) {
		T = new Tree("FieldDecList");
		T->add(ArrayType());
		T->add(IDList());
		T->add(MatchChar(';'));
		T->add(FieldDecMore());
		return T;
	}
	ERROR("Field declare list error.");
	return T;
}
Tree* IDList() {
	Tree* T = new Tree("IDList");
	T->add(MatchID());
	T->add(IDMore());
	return T;
}
Tree* FieldDecMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char") ||
		!strcmp(TKTable.front().Item.c_str(), "array")) {
		T = new Tree("FieldDecMore");
		T->add(FieldDecList());
	}
	return T;
}
Tree* IDMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), ",")) {
		T = new Tree("IDMore");
		T->add(MatchChar(','));
		T->add(IDList());
	}
	return T;
}
//VarDecPart
Tree* VarDec() {
	Tree* T = new Tree("VarDec");
	T->add(MatchReserved("var"));
	T->add(VarDecList());
	return T;
}
Tree* VarDecList() {
	Tree* T = new Tree("VarDecList");
	T->add(TypeDef());
	T->add(VarIDList());
	T->add(MatchChar(';'));
	T->add(VarDecMore());
	return T;
}
Tree* VarIDList() {
	Tree* T = new Tree("VarIDList");
	T->add(MatchID());
	T->add(VarIDMore());
	return T;
}
Tree* VarDecMore() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char") ||
		!strcmp(TKTable.front().Item.c_str(), "array") ||
		!strcmp(TKTable.front().Item.c_str(), "record") ||
		IsID()) {
		T = new Tree("VarDecMore");
		T->add(VarDecList());
	}
	CurrentString = Temp;
	return T;
}
Tree* VarIDMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), ",")) {
		T = new Tree("VarIDMore");
		T->add(MatchChar(','));
		T->add(VarIDList());
	}
	return T;
}
//ProcDecpart
Tree* ProcDec() {
	Tree* T = new Tree("ProcDec");
	T->add(MatchReserved("procedure"));
	T->add(MatchID());
	T->add(MatchChar('('));
	T->add(ParamList());
	T->add(MatchChar(')'));
	T->add(MatchChar(';'));
	T->add(ProcDecPart());
	T->add(ProcBody());
	T->add(ProcDecpart());
	return T;
}
//Tree* ProcName();    ֱ�ӱ��ID
Tree* ParamList() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char") ||
		!strcmp(TKTable.front().Item.c_str(), "array") ||
		!strcmp(TKTable.front().Item.c_str(), "record") ||
		!strcmp(TKTable.front().Item.c_str(), "var") ||
		IsID()) {
		T = new Tree("ParamList");
		T->add(ParamDecList());
	}
	CurrentString = Temp;
	return T;
}
Tree* ProcDecPart() {
	Tree* T = new Tree("ProcDecPart");
	T->add(DeclarePart());
	return T;
}
Tree* ProcBody() {
	Tree* T = new Tree("ProcBody");
	T->add(ProgramBody());
	return T;
}
//ParamList
Tree* ParamDecList() {
	Tree* T = new Tree("ParamDecList");
	T->add(Param());
	T->add(ParamMore());
	return T;
}
Tree* Param() {
	Tree* T = NULL;
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (!strcmp(TKTable.front().Item.c_str(), "integer") ||
		!strcmp(TKTable.front().Item.c_str(), "char") ||
		!strcmp(TKTable.front().Item.c_str(), "array") ||
		!strcmp(TKTable.front().Item.c_str(), "record") ||
		IsID()) {
		T = new Tree("Param");
		T->add(TypeDef());
		T->add(FormList());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "var")) {
		T = new Tree("Param");
		T->add(MatchReserved("var"));
		T->add(TypeDef());
		T->add(FormList());
		return T;
	}
	CurrentString = Temp;
	ERROR("Param error! ");
	return T;
}
Tree* ParamMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), ";")) {
		T = new Tree("ParamMore");
		T->add(MatchChar(';'));
		T->add(ParamDecList());
	}
	return T;
}
Tree* FormList() {
	Tree* T = new Tree("FormList");
	T->add(MatchID());
	T->add(FIDMore());
	return T;
}
Tree* FIDMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), ",")) {
		T = new Tree("FIDMore");
		T->add(MatchChar(','));
		T->add(FormList());
	}
	return T;
}
//ProcDecPart

//ProcBody

//ProgramBody
Tree* StmList() {
	Tree* T = new Tree("StmList");
	T->add(Stm());
	T->add(StmMore());
	return T;
}
//StmList
Tree* Stm() {
	Tree* T = new Tree("Stm");
	if (!strcmp(TKTable.front().Item.c_str(), "if")) {
		T->add(ConditionalStm());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "while")) {
		T->add(LoopStm());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "read")) {
		T->add(InputStm());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "write")) {
		T->add(OutputStm());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "return")) {
		T->add(ReturnStm());
		return T;
	}
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (IsID()) {
		T->add(MatchID());
		T->add(AssCall());
	}
	CurrentString = Temp;
	delete T;
	return NULL;
}
Tree* StmMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), ";")) {
		T = new Tree("StmMore");
		T->add(MatchChar(';'));
		T->add(StmList());
	}
	return T;
}
//Stm
Tree* ConditionalStm() {
	Tree* T = new Tree("ConditionalStm");
	T->add(MatchReserved("if"));
	T->add(Exp());
	if (!strcmp(TKTable.front().Item.c_str(), "<"))
	{
		T->add(MatchChar('<'));
	}
	else if (!strcmp(TKTable.front().Item.c_str(), "="))
	{
		T->add(MatchChar('='));
	}
	else
	{
		ERROR("condition error\n");
	}
	T->add(Exp());
	T->add(MatchReserved("then"));
	T->add(StmList());
	T->add(MatchReserved("else"));
	T->add(StmList());
	T->add(MatchReserved("fi"));
	return T;
}
Tree* LoopStm() {
	Tree* T = new Tree("LoopStm");
	T->add(MatchReserved("while"));
	T->add(Exp());
	if (!strcmp(TKTable.front().Item.c_str(), "<"))
	{
		T->add(MatchChar('<'));
	}
	else if (!strcmp(TKTable.front().Item.c_str(), "="))
	{
		T->add(MatchChar('='));
	}
	else
	{
		ERROR("condition error\n");
	}
	T->add(Exp());
	T->add(MatchReserved("do"));
	T->add(StmList());
	T->add(MatchReserved("endwh"));
	return T;
}
Tree* InputStm() {
	Tree* T = new Tree("InputStm");
	T->add(MatchReserved("read"));
	T->add(MatchChar('('));
	T->add(MatchID());
	T->add(MatchChar(')'));
	return T;
}
Tree* OutputStm() {
	Tree* T = new Tree("OutputStm");
	T->add(MatchReserved("write"));
	T->add(MatchChar('('));
	T->add(Exp());
	T->add(MatchChar(')'));
	return T;
}
Tree* ReturnStm() {
	Tree* T = new Tree("ReturnStm");
	T->add(MatchReserved("return"));
	return T;
}
Tree* AssCall() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "[") ||
		!strcmp(TKTable.front().Item.c_str(), ".") ||
		!strcmp(TKTable.front().Item.c_str(), ":="))
	{
		T = new Tree("AssCall");
		T->add(AssignmentRest());
		return T;
	}
	if (!strcmp(TKTable.front().Item.c_str(), "("))
	{
		T = new Tree("AssCall");
		T->add(CallStmRest());
		return T;
	}
	ERROR("ass call error\n");
	return T;
}
Tree* AssignmentRest() {
	Tree* T = new Tree("AssignmentRest");
	if (!strcmp(TKTable.front().Item.c_str(), "[") ||
		!strcmp(TKTable.front().Item.c_str(), "."))
	{
		T->add(VariMore());
	}
	T->add(MatchChar(':'));
	//T->add(MatchChar('='));
	T->add(Exp());
	return T;
}
Tree* CallStmRest() {
	Tree* T = new Tree("CallStmRest");
	T->add(MatchChar('('));
	T->add(ActParamList());
	T->add(MatchChar(')'));
	return T;
}
//AssignmentRest
Tree* VariMore() {
	Tree* T = NULL;
	if (!strcmp(TKTable.front().Item.c_str(), "["))
	{
		T = new Tree("VariMore");
		T->add(MatchChar('['));
		T->add(Exp());
		T->add(MatchChar(']'));
	}
	if (!strcmp(TKTable.front().Item.c_str(), "."))
	{
		T = new Tree("VariMore");
		T->add(MatchChar('.'));
		T->add(FieldVar());
	}
	return T;
}
Tree* Exp() {
	Tree* T = new Tree("Exp");
	T->add(Term());
	T->add(OtherTerm());
	return T;
}
//ConditionalStm
#if(0)Tree* RelExp() {
	Tree* T = new Tree("RelExp");
	T->add(Exp());
	T->add(OtherRelE());
	return T;
}
#endif
//LoopStm

//InputStm
#if(0)
Tree* Invar() {
	Tree* T = new Tree();
	return T;
}
#endif
//OutputStm

//ReturnStm

//CallStmRest
Tree* ActParamList() { 
	Tree* T = NULL; 
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	if (!strcmpi(TKTable.front().Item.c_str(), "(")||
		IsNumber()||
		IsID()){ 
		T = new Tree("ActParamList");
		T->add(Exp());
		T->add(ActParamMore());
	}
	CurrentString = Temp;
	return T; 
}
Tree* ActParamMore() { 
	Tree* T = NULL; 
	if (!strcmpi(TKTable.front().Item.c_str(), ",")) {
		T = new Tree("ActParamMore");
		T->add(MatchChar(','));
		T->add(ActParamList());
	}
	return T; }
//RelExp
//Tree* OtherRelE() { Tree* T = new Tree(); return T; }
//Tree* CmpOp() { Tree* T = new Tree(); return T; }
//Exp
Tree* Term() { 
	Tree* T = new Tree("Term");
	T->add(Factor());
	T->add(OtherFactor());
	return T; }
Tree* OtherTerm() { 
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "+") ||
		!strcmpi(TKTable.front().Item.c_str(), "-")) {
		Tree* T = new Tree("OtherTerm");
		T->add(AddOp());
		T->add(Exp());
	}
	return T;
}
Tree* AddOp() { 
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "+"))
	{
		Tree* T = new Tree("AddOp");
		T->add(MatchChar('+'));
		return T;
	}
	if (!strcmpi(TKTable.front().Item.c_str(), "-"))
	{
		Tree* T = new Tree("AddOp");
		T->add(MatchChar('-'));
		return T;
	}
	ERROR("cmpop error\n");
	return T; }
//Term
Tree* Factor() { 
	Tree* T = new Tree("Factor");
	string Temp = CurrentString;
	CurrentString = TKTable.front().Item;
	
	if (IsNumber())
	{
		T->add(MatchNumber());
		return T;
	} 
	if (!strcmpi(TKTable.front().Item.c_str(),"("))
	{
		T->add(MatchChar('('));
		T->add(Exp());
		T->add(MatchChar(')'));
		return T;
	}
	if (IsID())
	{
		T->add(Variable());
		return T;
	}
	cout <<endl<< CurrentString << endl<<endl;
	CurrentString = Temp;
	delete T;
	cout << CurrentString << endl;
	ERROR("factor error\n");
	return NULL;
}
Tree* OtherFactor() { 
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "*") ||
		!strcmpi(TKTable.front().Item.c_str(), "/"))
	{
		Tree* T = new Tree("OtherFactor");
		T->add(MultOp());
		T->add(Term());
	}
	return T; 
}
Tree* MultOp() {
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "*"))
	{
		Tree* T = new Tree("MultOp");
		T->add(MatchChar('*'));
		return T;
	}
	if (!strcmpi(TKTable.front().Item.c_str(), "/"))
	{
		Tree* T = new Tree("MultOp");
		T->add(MatchChar('/'));
		return T;
	}
	ERROR("cmpop error\n");
	return T; 
}
//Factor
Tree* Variable() { 
	Tree* T = new Tree("Variable");
	T->add(MatchID());
	T->add(VariMore());
	return T; 
}
Tree* FieldVar() { 
	Tree* T = new Tree("FieldVar");
	T->add(MatchID());
	T->add(FieldVarMore());
	return T; 
}
Tree* FieldVarMore() { 
	Tree* T = NULL;
	if (!strcmpi(TKTable.front().Item.c_str(), "["))
	{
		Tree* T = new Tree("FieldVarMore");
		T->add(MatchChar('['));
		T->add(Exp());
		T->add(MatchChar(']'));

	}
	return T; }

void Display(Tree *T,int depth,vector<int>*v,bool buttom) {
	if (T == NULL)return;
	cout << endl;
	int i;
	for ( i = 0; i < depth; i++) {
		if (v->at(i) == 1)cout<<" ��";
		else printf("  ");
	}
	if (buttom) {
		cout<<" ����"<< T->Item;
		v->at(depth) = 0;
	}
	else cout<<" ����"<< T->Item;
	if (depth + 1 == (int)v->size())v->push_back(1);
	v->at(depth + 1) = 1;
	int len = T->Children.size();
	for ( i = 0; i < len; i++) {
		if (i == len - 1)Display(T->Children.at(i), depth + 1, v, true);
		else Display(T->Children.at(i), depth + 1, v, false);
	}
}
