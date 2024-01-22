/**************************************
 *
 * University of Haifa
 *
 * Theory of compilation
 *
 * P machine compiler - code generation project
 *
 * Yosi Ben Asher
 * Email: yosibenasher@gmail.com
 *
 * Mariah Akree
 * Email: mariah.uni.haifa@gmail.com
 *
 **************************************/

#include "main.h"

extern int yydebug;
StackList intercase_stack;
StackList arrayDims;
StackList case_stack;
StackList arrayLows;
StackList DimsHelp_stack;
StackList DimsHelp_stackTwo;
 int address = 5;

 string coderl = "";
 int incdecflag = 0;
 int const_rightson_flag = 0;
 int condition_count = 0;
 //int loop_count = 0;
 int case_count = 0;
 int intercase_count = 0;
 int interelse_count = 0;
 int ifinsideloop = 0;
 int tempo = 0;
 int subafterdec = 0;
 int writeConstFlag = 0;
 int writeAddedVar = 0;
 int arraySize = 1;


 string case_tracker = "";
 string intercase_tracker = "0";
 string VarName = "";
 string VarType = "";
 int pointerFlag = 0;
 string pointerVal = "";
 int newStat = 0;
 int recordSize = 0;
 int definingRecord = 0;
 int definingArray = 0;
 int recordReference = 0;
 int addressReference = 0;
 string recordReferenceName = "";
 int declarationFlag = 1;
 int addressAlready = 0;
 int dimsMultiplie = 1;
 int dimsMultiplieFlag = 0;
 int flagRecordRef = 0;
 int dimsCount = 0;
 int arrayDimsClass = 0;
 string arrayNameDimsClass = "";
 int arrayIndexDimsClass = 0;
 int arrayReference = 0;
 int lastAccessedIndex = 0;
 int DimIsExp = 0;
 int assignFlag = 0;
 int assignArrayFlag = 1;

 int insideArrayofRec = 0;
 int assignRecordRefFlag = 0;
 int addressRefCounter = 0;
 string saveRecType = "";
 int DimClassXY = 0;
 int DimClassNameIsFree = 0;
 string previousIdeTypeName = "";
 int theFSResult = 0;

PCMemory ram;

PCMemory labels;

void writeAST(Object* r, fstream& file)
{
	//Perform recursive tree print
	r->print(file);
}
void Pcode(Object* r, fstream& file)
{
	//Perform recursive tree print
	r->pcodegen(file);
}

Object* getTree(char* progFile)
{

	yydebug = 1;
	assert(progFile);

	// yyin is an external variable that been used in yyparse as pointer to the source file.
	yyin = fopen(progFile, "r");
	if (!yyin) {
		cerr << "Error: file " << progFile << " does not exst. Aborting ..." << endl;;
		exit(1);
	}
	assert(yyin);

	// yyparse is yacc function that parse the program, checks syntax and builds the program AST.
	yyparse();

	fclose(yyin);

	// root was initialized in yyparse while it was building the tree.
	// root is the pointer of the returning tree.
	assert(root);
	return(root);
}



int main(int argc, char* argv[])
{
	CodeGenerator codeGen;
	//check input arguments.
	if (argc < 2) {
		cerr << endl << "Input file is missing. Aborting ..." << endl;
		exit(1);
	}

	Object* theProgram = getTree(argv[1]);
	assert(theProgram == root);

	fstream treeFile(TREE_OUTPUT_TEXT_FILE, ios::out);
	fstream pcodeFile(OUTPUT_CODE_TEXT, ios::out);
	treeFile << AST_FILE_HEADER << endl;
	pcodeFile << AST_FILE_HEADER << endl;
	writeAST(theProgram, treeFile);
	Pcode(theProgram, pcodeFile);
	treeFile.close();
	return (0);
}


