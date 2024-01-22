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
 /*   PROGRAM = 258,
      BBEGIN = 259,
      END = 260,
      DECLARE = 261,
      PROCEDURE = 262,
      FUNCTION = 263,
      LABEL = 264,
      INTEGER = 265,
      REAL = 266,
      RECORD = 267,
      BOOLEAN = 268,
      ARRAY = 269,
      OF = 270,
      ASSIGN = 271,
      LC = 272,
      RC = 273,
      IF = 274,
      THEN = 275,
      ELSE = 276,
      WHILE = 277,
      REPEAT = 278,
      FI = 279,
      DO = 280,
      OD = 281,
      READ = 282,
      WRITE = 283,
      TRUE = 284,
      FALSE = 285,
      ADD = 286,
      MIN = 287,
      MUL = 288,
      DIV = 289,
      GOTO = 290,
      MOD = 291,
      LES = 292,
      LEQ = 293,
      EQU = 294,
      NEQ = 295,
      GRE = 296,
      GEQ = 297,
      AND = 298,
      OR = 299,
      NOT = 300,
      CASE = 301,
      FOR = 302,
      FIN = 303,
      IDENTICAL = 304,
      FROM = 305,
      BY = 306,
      TO = 307,
      NEW = 308,
      INTCONST = 309,
      IDE = 310,
      REALCONST = 311,
      STRING = 312,
      DUMMY = 313*/

#ifndef AST_H 
#define AST_H

#include <iostream>
#include <assert.h>
#include <string>
#include <iomanip>

using namespace std;
extern int incdecflag = 0;
extern int muldivflag = 0;
extern int const_rightson_flag = 0;
extern int address;
extern int condition_count = 0;
extern int loop_count = 0; // Counts Loops
extern int case_count = 0; // Counts Cases
extern int intercase_count = 0; // Counts Local InterCases Number
extern int interelse_count = 0;
extern int ifinsideloop = 0;
extern int global_case_count = 0;
extern string case_tracker = "";
extern string intercase_tracker = "";
extern int tempo = 0;
extern int writeConstFlag = 0;
extern int writeAddedVar = 0;

extern string coderl;


struct stackNode
{
    int data;
    stackNode* next;
};

class StackList
{
    stackNode* top;
public:
    StackList()
    {
        top = NULL;
    }
    void push(int x);
    int pop();
    int peek();
    ~StackList();
};

void StackList::push(int x)
{
    stackNode* temp;
    temp = new stackNode;
    temp->data = x;
    temp->next = top;
    top = temp;
}

int StackList::pop()
{
    if (top != NULL)
    {
        stackNode* temp = top;
        top = top->next;
        int value = temp->data;
        delete temp;
        return value;
    }
    else
        return -1;
}

int StackList::peek() {
    if (top != NULL)
    {
        return top->data;
    }
    else
        return -1;
}

StackList::~StackList()
{
    while (top != NULL)
    {
        stackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

extern StackList intercase_stack;
StackList intercase_stack;

extern StackList case_stack;
StackList case_stack;


struct MemoryNode
{
    int _size;
    int _address;
    string _type;
    int _data;
    MemoryNode* next;
    MemoryNode()
    {

    }
public: void setValues(int size, int address, int data, string type)
{
    this->_size = size;
    this->_address = address;
    this->_type = type;
    this->_data = data;
    this->next = NULL;
}
};

class PCMemory
{
private:
    MemoryNode* head, * tail;
public:
    PCMemory()
    {
        head = NULL;
        tail = NULL;
    }

public: void add_node(MemoryNode* n)
    {
        MemoryNode* tmp = new MemoryNode;
        tmp->_size = n->_size;
        tmp->_address = n->_address;
        tmp->_type = n->_type;
        tmp->_data = n->_data;
        tmp->next = NULL;

        if (head == NULL)
        {
            head = tmp;
            tail = tmp;
        }
        else
        {
            tail->next = tmp;
            tail = tail->next;
        }
    }
    int find_node(string type) // Returns the address of the needed node;
    {
        MemoryNode* tmp = new MemoryNode;
        tmp = head;
        while (tmp != NULL)
        {
            if (tmp->_type == type)
                return tmp->_address;
            else
                tmp = tmp->next;
        }
        return -1;
    }
};

extern PCMemory ram;


/**
 * classes 
 */
// base class for tree nodes
class Object {
public:
 virtual void print (ostream& os) = 0;
 virtual void pcodegen(ostream& os) = 0;
 virtual Object * clone () const {return NULL;}
 virtual ~Object () {}
};

class Expr : public Object {
public :
  // Unary operations
  Expr (int op, Object * atom) : op_(op), atom_(atom), left_(NULL), right_(NULL), unary_(true) {}
  // Binary operations
  Expr (int op, Object * left, Object * right) : op_(op), left_(left), right_(right), atom_(NULL), unary_(false) {}

  Expr(const Expr & exp){
    unary_= exp.unary_; 
    op_ = exp.op_;
    left_ = exp.left_->clone();
    right_ = exp.right_->clone();
    atom_ = exp.atom_->clone();
  }

  virtual ~Expr(){
    if (left_) delete left_;
    if (right_) delete right_;
    if (atom_) delete atom_;
  }

  
  void print (ostream& os) {
    os<<"Node name : Expr"<<endl;
    assert(op_);
    if (unary_){
      os<<"Unary op is : "<<op_;
      assert(atom_);
      atom_->print(os);
    }
    else
    {
      os<<"Binary op is : "<<op_;
      assert(left_ && right_);
      right_->print(os);
      left_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      int temp = tempo;
      //coderl = "x";
      assert(op_);
      tempo = 1;
      if (unary_) {
          assert(atom_);
          atom_->pcodegen(os);
          if (op_ == 287)
          {
              os << "neg" << endl;
          }
          if (op_ == 300)
          {
              os << "not" << endl;
          }
      }
      else {
          assert(left_ && right_);
          if (op_ == 296)
          {
              coderl = "left";
          }
          if (op_ >= 292 && op_ <= 297)
          {
              coderl = "left";
          }
          //os << "OP BEFORE IS " << op_ << endl;
          if (op_ == 288 || op_ == 289)
          {
              muldivflag = 1;
          }
          const_rightson_flag = 1;
          right_->pcodegen(os); // Right is from the symbol table
          const_rightson_flag = 0;
          incdecflag = op_;
          tempo = 0;
          left_->pcodegen(os); // Left is the constant values ( ma2lobat )
          tempo = temp;
          //os << "COMING IN WITH CONST & OP " << const_rightson_flag << "||" << op_ <<  endl;
          if (op_ == 287)
          {
              //os << "rightsonflag " << const_rightson_flag << endl;
              if(const_rightson_flag == 0 )
              os << "sub" << endl;
          }
          else if (op_ == 286)
          {
                  if (const_rightson_flag == 0)
                      os << "add" << endl;
          }
          else if (op_ == 296)
          {
              os << "grt" << endl;
          }
          else if (op_ == 292)
          {
              os << "les" << endl;
          }
          else if (op_ == 293)
          {
              os << "leq" << endl;
          }
          else if (op_ == 294)
          {
              os << "equ" << endl;
          }
          else if (op_ == 295)
          {
              os << "neq" << endl;
          }
          else if (op_ == 297)
          {
              os << "geq" << endl;
          }
          else if (op_ == 298)
          {
              os << "and" << endl;
          }
          else if (op_ == 299)
          {
              os << "or" << endl;
          }
          else if (op_ == 288)
          {
              os << "mul" << endl;
              const_rightson_flag = 0;
          }
          else if (op_ == 289)
          {
              os << "div" << endl;
              const_rightson_flag = 0;
          }
      }
  }
  virtual Object * clone () const { return new Expr(*this);}
  
private:
  bool unary_;
  int op_;
  Object * left_;
  Object * right_;
  Object * atom_;
};

class ExprList : public Object {
public :
  ExprList (Object * expr) : expr_(expr),expr_list_(NULL){assert(expr_);}
  ExprList (Object * expr, Object * expr_list) : expr_(expr),expr_list_(expr_list) {assert(expr_ && expr_list_);}

  ExprList(const ExprList& exp){
    expr_ = exp.expr_->clone();
    expr_list_ = exp.expr_list_->clone();
  }

  virtual ~ExprList(){
    if (expr_) delete expr_;
    if (expr_list_) delete expr_list_;
  }

  void print (ostream& os) {
		os<<"Node name : ExprList";
		assert( expr_);
    expr_->print(os);
    if (expr_list_){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(expr_);
      expr_->pcodegen(os);
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ExprList(*this);}
  
private:
  Object * expr_;
	Object * expr_list_;
};

class Dim : public Object {
public:
  Dim (Object * exp) : exp_(exp), dim_(NULL) {assert(exp_);}
  Dim (Object * exp, Object * dim) : exp_(exp),dim_(dim) {assert(exp_ && dim_);}

  Dim(const Dim& d){
    exp_ = d.exp_->clone();
    dim_ = d.dim_->clone();
  }

  virtual ~Dim(){
    if (exp_) delete exp_;
    if (dim_) delete dim_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Dim"<<endl;
    assert(exp_);
    exp_->print(os);
    if (dim_){
      dim_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      assert(exp_);
      exp_->pcodegen(os);
      if (dim_) {
          dim_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new Dim(*this);}

private:
  Object * exp_;
  Object * dim_;
};

class Atom : public Object {
};

class IntConst : public Atom {
public:
  IntConst(const int i) : i_(i) {}
  IntConst(const IntConst& in) : i_(in.i_) {}
  
  void print (ostream& os) {
    os<<"Node name : IntConst. Value is :"<<i_<<endl;
  }
  void pcodegen(ostream& os) 
  {

      if (incdecflag == 287 && tempo == 0) {
          os << "dec " << i_ << endl;
          incdecflag = 0;
          const_rightson_flag = 1;
      }
      else
      {
          //os << incdecflag << "|" << tempo << "|" << writeConstFlag << "|" << writeAddedVar << endl;
          if ( (incdecflag == 286 && tempo == 0 && writeConstFlag == 0) || (writeAddedVar == 1))
          {
              os << "inc " << i_ << endl;
              incdecflag = 0;
              const_rightson_flag = 1;
          }
          else
          {

              os << "ldc " << i_ << endl;
          }
      }
            
  }
  virtual Object * clone () const { return new IntConst(*this);}

private:
  const int i_;
};

class RealConst : public Atom {
public:
  RealConst(const double r) : r_(r) {}
  RealConst(const RealConst& in) : r_(in.r_) {}  

  void print (ostream& os) {
    os<<"Node name : RealConst. Value is :"<<r_<<endl;
  }
  void pcodegen(ostream& os) 
  {
          if (incdecflag == 287 && tempo == 0) {
              os << "dec " << std::fixed << setprecision(1) << r_ << endl;
              incdecflag = 0;
              const_rightson_flag = 1;
          }
          else
          {
              if ((incdecflag == 286 && tempo == 0 && writeConstFlag == 0) || (writeAddedVar == 1))
              {
                  os << "inc " << std::fixed << setprecision(1) << r_ << endl;
                  incdecflag = 0;
                  const_rightson_flag = 1;
              }
              else
              {
                  os << "ldc " << std::fixed << setprecision(1) << r_ << endl;
              }
          }

      }
  virtual Object * clone () const { return new RealConst(*this);}

private:
  const double r_;
};

class True : public Atom {
public:
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is true"<<endl;
  }
  void pcodegen(ostream& os) {
      os << "ldc 1" << endl;
  }
  virtual Object * clone () const { return new True();}

};

class False : public Atom {
public :
  void print (ostream& os) {
    os<<"Node name : trueConst. Value is false"<<endl;
  }
  void pcodegen(ostream& os) {
      os << "ldc 0 " << endl;
  }
  virtual Object * clone () const { return new False();}
};

class Var : public Atom {
};

class ArrayRef : public Var {
public :
  ArrayRef (Object * var, Object * dim) : var_(var),dim_(dim) {assert(var_ && dim_);}
  ArrayRef(const ArrayRef& arr){
    var_ = arr.var_->clone();
    dim_ = arr.dim_->clone();
  }

  virtual ~ArrayRef(){
    if (var_) delete var_;
    if (dim_) delete dim_;
  }

  void print (ostream& os) {
    os<<"Node name : ArrayRef"<<endl;
    assert(var_ && dim_);
    var_->print(os);
    dim_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_ && dim_);
      var_->pcodegen(os);
      dim_->pcodegen(os);
  }
  virtual Object * clone () const { return new ArrayRef(*this);}

private:
  Object * var_;
  Object * dim_;
};

class RecordRef : public Var {
public :
  RecordRef (Object * varExt, Object * varIn) : varExt_(varExt),varIn_(varIn) {assert(varExt_ && varIn_);}
  RecordRef(const RecordRef& rec){
    varExt_ = rec.varExt_->clone();
    varIn_ = rec.varIn_->clone();
  }
  
  virtual ~RecordRef(){
    if (varExt_) delete varExt_;
    if (varIn_) delete varIn_;
  }

  void print (ostream& os) {
    os<<"Node name : RecordRef"<<endl;
    assert(varExt_ && varIn_);
    varExt_->print(os);
    varIn_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(varExt_ && varIn_);
      varExt_->pcodegen(os);
      varIn_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordRef(*this);}

private:
  Object * varExt_;
  Object * varIn_;
};

class AddressRef : public Var {
public :
  AddressRef (Object * var) : var_(var) {assert(var_);}
  AddressRef(const AddressRef& addre){
    var_ = addre.var_->clone();
  }

  virtual ~AddressRef(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
    os<<"Node name : AddressRef"<<endl;
    assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      var_->pcodegen(os);
  }
  virtual Object * clone () { return new AddressRef(*this);}

private:
  Object * var_;
};

class Statement : public Object {
};

class NewStatement : public Statement {
public :
  NewStatement (Object * var) : var_(var) {assert(var_);}
  NewStatement(const NewStatement& ns){
    var_ = ns.var_->clone();
  }

  virtual ~NewStatement(){
    if (var_) delete var_;
  }

  void print (ostream& os) {
		os<<"Node name : NewStatement";
		assert(var_);
    var_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(var_);
      var_->pcodegen(os);
  }
  virtual Object * clone () { return new NewStatement(*this);}
  
private:
  Object * var_;
};

class WriteStrStatement : public Statement {
public :
  WriteStrStatement (const char * str) { 
		str_ = new string(str); 
	}
  
  WriteStrStatement(const WriteStrStatement& ns){
    str_ = new string(*ns.str_); 
  }

  virtual ~WriteStrStatement () {
		if (str_) delete str_;
	}
  void print (ostream& os) {
		os<<"Node name : WriteStrStatement";
		assert(str_);
    os<<"Str statement is: "<<str_<<endl;
  }
  void pcodegen(ostream& os) {
      assert(str_);
  }
  virtual Object * clone () { return new WriteStrStatement(*this);}
  
private:
  string * str_;
};

class WriteVarStatement : public Statement {
public :
  WriteVarStatement (Object * exp) : exp_(exp) {assert(exp_);}

  WriteVarStatement(const WriteVarStatement& ex){
    exp_ = ex.clone();
  }

  virtual ~WriteVarStatement(){
    if (exp_) delete exp_;
  }

  void print (ostream& os) {
		os<<"Node name : WriteVarStatement";
		assert(exp_);
    exp_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(exp_);
      coderl = "left";
      writeConstFlag = 1;
      exp_->pcodegen(os);
      writeConstFlag = 0;
      writeAddedVar = 0;
      os << "print" << endl;
      coderl = "x";
  }
  virtual Object * clone () const { return new WriteVarStatement(*this);}
  
private:
  Object * exp_;
};

class ProcedureStatement : public Statement {
public :
  ProcedureStatement (const char * str) { 
		str_ = new string(str); 
	}

  ProcedureStatement (Object * expr_list, const char * str) : expr_list_(expr_list) {
    assert(expr_list_);
    str_ = new string(str);
  }

  ProcedureStatement(const ProcedureStatement& ps){
    expr_list_ = ps.expr_list_->clone();
    str_ = new string(*ps.str_);
  }

   virtual ~ProcedureStatement () {
		if (str_) delete str_;
    if (expr_list_) delete expr_list_;
	} 

  void print (ostream& os) {
    os<<"Node name : ProcedureStatement. Proc name : "<<str_<<endl;
    if (expr_list_ ){
      expr_list_->print(os);
    }
  }
  void pcodegen(ostream& os) {
      if (expr_list_) {
          expr_list_->pcodegen(os);
      }
  }
  virtual Object * clone () const { return new ProcedureStatement(*this);}
  
private:
	Object * expr_list_;
  string * str_;
};

class Case : public Object {
public :
  Case (Object * stat_list, int val) : leafChild_ (NULL), stat_list_(stat_list) {
		// note the special treatment in miny.y for this case (makenode)
		leafChild_ = new IntConst(val);
		assert(stat_list_);
	}

  Case(const Case& c){
    stat_list_ = c.stat_list_->clone();
    leafChild_ = c.leafChild_->clone();
  }

  virtual ~Case () {
    if (stat_list_) delete stat_list_;
    if (leafChild_) delete leafChild_;
	} 
  
  void print (ostream& os) {
		os<<"Node name : Case";
		assert(stat_list_);
	  stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
      int case_TempCount = case_stack.peek();
      assert(stat_list_);
      stat_list_->pcodegen(os);
      os << "ujp end_switch_" << case_stack.peek() << endl;


  }
  virtual Object * clone () const { return new Case(*this);}

private:
  Object * stat_list_;
	Object * leafChild_;
};


class CaseList : public Object {
public :
  CaseList (Object * ccase) : case_(ccase),case_list_(NULL) {assert(case_);}
  CaseList (Object * ccase, Object * case_list) : case_(ccase),case_list_(case_list) {assert(case_ && case_list_);}

  CaseList(const CaseList& cl){
    case_ = cl.case_->clone();
    case_list_ = cl.case_list_->clone();
  }

  virtual ~CaseList () {
    if (case_) delete case_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) 
  {
		os<<"Node name : CaseList";
		assert( case_ );
    case_->print(os);
    if (case_list_){
      case_list_->print(os);
    }
    
  }
  void pcodegen(ostream& os) {
      int current_intercase = 0; 

      intercase_count++;
      current_intercase = intercase_count;
      os << "case_" << current_intercase << "_" << case_stack.peek() << ":" << endl;
      assert(case_);
      case_->pcodegen(os);
      if (case_list_) {
          case_list_->pcodegen(os);

      }
      os << "ujp case_" << current_intercase << "_" << case_stack.peek() << endl;

  }
  virtual Object * clone () const { return new CaseList(*this);}

private:
  Object * case_;
	Object * case_list_;
};

class CaseStatement : public Statement {
public :
  CaseStatement (Object * exp, Object * case_list) : exp_(exp),case_list_(case_list) {assert(exp_ && case_list_);}

  CaseStatement(const CaseStatement& cs){
    exp_ = cs.exp_->clone();
    case_list_ = cs.case_list_->clone();
  }

  virtual ~CaseStatement () {
    if (exp_) delete exp_;
    if (case_list_) delete case_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : CaseStatement";
		assert( exp_ && case_list_);
    exp_->print(os);
		case_list_->print(os);
  }
  void pcodegen(ostream& os) 
  {
      //os << "PUSHED << " << intercase_count << endl;
      intercase_stack.push(intercase_count);
      intercase_count = 0;
      int current_case = 0;
      coderl = "left";
      case_count++;
      current_case = case_count;
      case_stack.push(case_count);
      assert(exp_ && case_list_);
      exp_->pcodegen(os);
      os << "neg" << endl;
      os << "ixj end_switch_" << current_case << endl;

      case_list_->pcodegen(os);

      os << "end_switch_" << current_case << ":" <<endl;
      case_stack.pop();

      coderl = "x";
      intercase_count = intercase_stack.pop(); // ME Here
      //os << "POPED << " << intercase_count << endl;

  }
  virtual Object * clone () const { return new CaseStatement(*this);}
  
private:
  Object * exp_;
	Object * case_list_;
};

class LoopStatement : public Statement {
public :
  LoopStatement (Object * exp, Object * stat_list) : exp_(exp),stat_list_(stat_list) {assert(exp_ && stat_list_);}

  LoopStatement(const LoopStatement& ls){
    exp_ = ls.exp_->clone();
    stat_list_ = ls.stat_list_->clone();
  }

  virtual ~LoopStatement () {
    if (exp_) delete exp_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
		os<<"Node name : LoopStatement";
		assert( exp_ && stat_list_);
    exp_->print(os);
		stat_list_->print(os);
  }
  void pcodegen(ostream& os) {
      ifinsideloop++;
      coderl = "left";
      int current_loop;
      assert(exp_ && stat_list_); 
      loop_count++;
      current_loop = loop_count;
      os << "loop_" << current_loop << ":" << endl;

      exp_->pcodegen(os);
      os << "fjp end_loop_" << current_loop << endl;
      //os << "C" << endl;
      stat_list_->pcodegen(os);
      os << "ujp loop_" << current_loop << endl;
      os << "end_loop_" << current_loop << ":" << endl;
      //os << "D" << endl;
      coderl = "x";
      //loop_count = 0;
      ifinsideloop--;
  }
  virtual Object * clone () const { return new LoopStatement(*this);}

private:
  Object * exp_;
	Object * stat_list_;
};


class ConditionalStatement : public Statement {
public :
  ConditionalStatement (Object * exp, Object * stat_list_if) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(NULL) {assert(exp_ && stat_list_if_);}
  ConditionalStatement (Object * exp, Object * stat_list_if, Object * stat_list_else) : exp_(exp),stat_list_if_(stat_list_if), stat_list_else_(stat_list_else) {assert(exp_ && stat_list_if_ && stat_list_else_ );}

  ConditionalStatement(const ConditionalStatement& cs){
    exp_ = cs.exp_->clone();
    stat_list_if_ = cs.stat_list_if_->clone();
    stat_list_else_ = cs.stat_list_else_->clone();
  }

  virtual ~ConditionalStatement () {
    if (exp_) delete exp_;
    if (stat_list_if_) delete stat_list_if_;
    if (stat_list_else_) delete stat_list_else_;
	} 

  void print (ostream& os) {
		os<<"Node name : ConditionalStatement";
		assert( exp_ && stat_list_if_);
        exp_->print(os);
		stat_list_if_->print(os);
		if (stat_list_else_){
			stat_list_else_->print(os);
		}
  }
  void pcodegen(ostream& os) {
      int thereelse = 0;
      int current_iter;
      int current_else;
      assert(exp_ && stat_list_if_);
      //os << "A" << endl; // Always Goes In
      coderl = "left";
      exp_->pcodegen(os); // Checks expression inside the condition.
      condition_count++;
      current_iter = condition_count;
      if (!stat_list_else_)
      {
          os << "fjp end_if_" << current_iter << endl;
      }
      else
      {
          interelse_count++;
          os << "fjp else_if_" << current_iter << endl; // Changed here
      }
      //os << "B" << endl;
      stat_list_if_->pcodegen(os); // Jumps to other If's inside , Line under is after it is done
      if (stat_list_else_) 
      {
          thereelse = 1;
          current_else = interelse_count;
          os << "ujp end_if_" << current_iter << endl;
          os << "else_if_" << current_iter << ":" << endl; // IDk here
          stat_list_else_->pcodegen(os);
      }
      os << "end_if_" << current_iter << ":" << endl;
      coderl = "x";
      //os << "D" << endl;
  }
  virtual Object * clone () const { return new ConditionalStatement(*this);}
  
private:
  Object * exp_;
	Object * stat_list_if_;
	Object * stat_list_else_;
};


class Assign : public Statement {
public :
  Assign (Object * var, Object * exp) : var_(var), exp_(exp) {assert(var_ && exp_);}
  
  Assign(const Assign& as){
    var_ = as.var_->clone();
    exp_ = as.exp_->clone();
  }

  virtual ~Assign () {
    if (exp_) delete exp_;
    if (var_) delete var_;
	} 

  void print (ostream& os) {
		os<<"Node name : Assign";
		assert(var_ && exp_);
        exp_->print(os);
        var_->print(os);
  }
  void pcodegen(ostream& os) {
      incdecflag = 0;
      assert(var_ && exp_);
      coderl = "right";
      exp_->pcodegen(os);
      coderl = "left";
      var_->pcodegen(os);
      os << "sto" << endl;
      coderl = "x";
  }
  virtual Object * clone () const { return new Assign(*this);}

private:
  Object * var_;
  Object * exp_;
};

class StatementList : public Object {
public :
  StatementList (Object * stat) : stat_list_(NULL), stat_(stat)  { assert(stat_);}
  StatementList (Object * stat_list, Object * stat) : stat_list_(stat_list),stat_(stat)  {assert(stat_list_ && stat);}

  StatementList (const StatementList& sl){
    stat_ = sl.stat_->clone();
    stat_list_ = sl.stat_list_->clone();
  }

  virtual ~StatementList () {
    if (stat_) delete stat_;
    if (stat_list_) delete stat_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : StatementList"<<endl;
	  if (stat_list_){
		  stat_list_->print(os);
	  }
	  assert(stat_);
	  stat_->print(os);
  }
  void pcodegen(ostream& os) 
  {
      if (stat_list_) {
          stat_list_->pcodegen(os);
      }

      assert(stat_);
      stat_->pcodegen(os);
  }
  virtual Object * clone () const { return new StatementList(*this);}
  
private:
  Object * stat_;
  Object * stat_list_;
};

class RecordList : public Object {
public :
  RecordList (Object * var_decl) : record_list_(NULL), var_decl_(var_decl)  { assert(var_decl_);}
  RecordList (Object * record_list, Object * var_decl) : record_list_(record_list),var_decl_(var_decl)  {assert(record_list_ && var_decl);}
  
  RecordList(const RecordList& li){
    var_decl_= li.var_decl_->clone();
    record_list_ = li.record_list_->clone();
  }

  virtual ~RecordList () {
    if (var_decl_) delete var_decl_;
    if (record_list_) delete record_list_;
	} 

  void print (ostream& os) {
	  os<<"Node name : RecordList"<<endl;
	  if (record_list_){
		  record_list_->print(os);
	  }
	  assert(var_decl_);
	  var_decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (record_list_) {
          record_list_->pcodegen(os);
      }
      assert(var_decl_);
      var_decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordList(*this);}
  
private:
  Object * var_decl_;
  Object * record_list_;
};

class Type : public Object {
};

class SimpleType : public Type {
public:
  SimpleType (const char * name) { 
		name_ = new string(name); 
	}

  virtual ~SimpleType () {
		if (name_ )delete name_;
	}

  SimpleType(const SimpleType& s){
    name_ = new string(*s.name_);
  }

  void print (ostream& os) {
		os<<"Node name : SimpleType"<<endl;;
		os<<"Type is : "<< (*name_) <<endl;
	}
  void pcodegen(ostream& os) {
  }
  virtual Object * clone () const { return new SimpleType(*this);}

private:
  string * name_;
};

class IdeType : public Type {
public:
  IdeType (const char * name) { 
    name_ = new string(name); 
  }

  virtual ~IdeType () {
    if (name_) delete name_;
  }

  IdeType(const IdeType& s){
    name_ = new string(*s.name_);
  }

  void print (ostream& os) {
    os<<"Node name : IdeType"<<endl;
  }
  void pcodegen(ostream& os) 
  {
      os << "ldc " << ram.find_node(*name_) << endl;
      if (coderl == "left")
      {
          os << "ind" << endl;
          if (writeConstFlag == 1)
              writeAddedVar = 1;
      }
  }
  virtual Object * clone () const { return new IdeType(*this);}

private:
  string * name_;
};

class ArrayType : public Type {
public :
  ArrayType (int l,int u, Object * type) : low_(l),up_(u),type_(type)  { assert(type_);}
  
  ArrayType(const ArrayType& a) : low_(a.low_), up_(a.up_){
    type_ = a.type_->clone();
  }

  virtual ~ArrayType () {
    if (type_) delete type_;
  }

  void print (ostream& os) {
		os<<"Node name : ArrayType: low bound is: "<<low_<<", up bound is: "<<up_<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);
  }
  virtual Object * clone () const { return new ArrayType(*this);}

private:
  Object * type_;
	int low_,up_;
};

class RecordType : public Type {
public :
  RecordType (Object * record_list) : record_list_(record_list)  { assert(record_list_);}

  RecordType(const RecordType& y){
    record_list_ = y.record_list_->clone();
  }

  virtual ~RecordType () {
    if (record_list_) delete record_list_;
  }
  
  void print (ostream& os) {
		os<<"Node name : RecordType"<<endl;
	  assert(record_list_);
	  record_list_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(record_list_);
      record_list_->pcodegen(os);
  }
  virtual Object * clone () const { return new RecordType(*this);}

private:
  Object * record_list_;
};


class AddressType : public Type {
public :
  AddressType (Object * type) : type_(type)  { assert(type_);}

  AddressType(const AddressType& t){
    type_ = t.type_->clone();
  }

  virtual ~AddressType () {
    if (type_) delete type_;
  }
  
  void print (ostream& os) {
		os<<"Node name : AddressType"<<endl;
	  assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);
  }
  virtual Object * clone () const { return new AddressType(*this);}

private:
  Object * type_;
};


class Declaration : public Object {
};

class VariableDeclaration : public Declaration {
public:
  VariableDeclaration (Object * type, const char * str) : type_(type){
    assert(type_);
    name_ = new string(str);
  }

  VariableDeclaration(const VariableDeclaration& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_);
  }

  virtual ~VariableDeclaration () {
    if (type_) delete type_;
		delete name_;
	}

  void print (ostream& os) {
    os<<"Node name : VariableDeclaration. Var name is: "<<*name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_);
      type_->pcodegen(os);

      MemoryNode newNode;
      newNode.setValues(1, address, -1, *name_);
      //os << "ADDED VARIABLE " << *name_ << " AT " << address << endl;
      address++;
      ram.add_node(&newNode);
  }
  virtual Object * clone () const { return new VariableDeclaration(*this);}

private:
  Object * type_;
  string * name_;
};

class Parameter : public Object {
public :
  Parameter (Object * type, const char * name) : type_(type){
    assert(type_);
    name_ = new string(name);
  }
  
  Parameter(const Parameter& p){
    type_ = p.type_->clone();
    name_ = new string(*p.name_);
  }

  virtual ~Parameter () {
    if (type_) delete type_;
		delete name_;
	}
  
  void print (ostream& os) {
		printWayOfPassing(os);
    os<<"Parameter name :" <<name_<<endl;
		assert(type_);
	  type_->print(os);
  }
  void pcodegen(ostream& os) {
      printWayOfPassing(os);
      assert(type_);
      type_->pcodegen(os);
  }
protected:
	virtual void printWayOfPassing (ostream& os) const = 0;
  
private:
  Object * type_;
  string * name_;
};

class ByReferenceParameter : public Parameter {
public :
	ByReferenceParameter (Object * type, const char * name) : Parameter (type,name) {}
  virtual Object * clone () const { return new ByReferenceParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByReferenceParameter ";
	}
};

class ByValueParameter : public Parameter {
public :
	ByValueParameter (Object * type, const char * name) : Parameter(type,name) {}
  virtual Object * clone () const { return new ByValueParameter(*this);}
protected:
  void printWayOfPassing (ostream& os) const{
	  os<<"Node name : ByValueParameter ";
	}
};

class ParameterList : public Object {
public :
  ParameterList (Object * formal) : formal_(formal),  formal_list_(NULL) { assert(formal_);}
  ParameterList (Object * formal, Object * formal_list) : formal_(formal), formal_list_(formal_list) {assert(formal_ && formal_list_);}
  
  ParameterList(const ParameterList& pl){
    formal_ = pl.formal_->clone();
    formal_list_ = pl.formal_list_->clone();
  }
  
  virtual ~ParameterList () {
    if (formal_) delete formal_;
    if (formal_list_) delete formal_list_;
	}

  void print (ostream& os) {
	  os<<"Node name : ParameterList"<<endl;
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  assert(formal_);
	  formal_->print(os);
  }
  void pcodegen(ostream& os) {
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      assert(formal_);
      formal_->pcodegen(os);
  }
  virtual Object * clone () const { return new ParameterList(*this);}
  
private:
  Object * formal_;
  Object * formal_list_;
};

class FunctionDeclaration : public Declaration {
public :
  FunctionDeclaration (Object * type, Object * block, const char * name) : type_(type), block_(block), formal_list_(NULL) {
    assert(type_ && block_);
    name_ = new string(name);
  }

  FunctionDeclaration (Object * type, Object * formal_list, Object * block,  const char * name) : type_(type), formal_list_(formal_list),block_(block) {
    assert(type_ && formal_list_ && block_ );
    name_ = new string(name);
  }

  virtual ~FunctionDeclaration () {
    if (type_) delete type_;
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  FunctionDeclaration(const FunctionDeclaration& fd){
    type_ = fd.type_->clone();
    block_ = fd.block_->clone();
    formal_list_ = fd.formal_list_->clone();
    name_ = new string(*fd.name_);
  }

  void print (ostream& os) {
    os<<"Node name : FunctionDeclaration. Func name is: "<<*name_<<endl;
	  assert(type_ && block_);
	  type_->print(os);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(type_ && block_);
      type_->pcodegen(os);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new FunctionDeclaration(*this);}
  
private:
  Object * type_;
  Object * block_;
  Object * formal_list_;
  string * name_;
};

class ProcedureDeclaration : public Declaration {
public :
  ProcedureDeclaration (Object * block, const char * name) : formal_list_(NULL), block_(block) { 
    assert(block_);
    name_ = new string(name);
  }

  ProcedureDeclaration (Object * formal_list, Object * block, const char * name) : formal_list_(formal_list),block_(block)  {
    assert(formal_list_ && block_);
    name_ = new string(name);
  }

  virtual ~ProcedureDeclaration () {
    if (block_) delete block_;
    if (formal_list_) delete formal_list_;
    if (name_) delete name_;
  }

  ProcedureDeclaration(const ProcedureDeclaration& pd){
    block_ = pd.block_->clone();
    formal_list_ = pd.formal_list_->clone();
    name_ = new string(*pd.name_);
  }

  void print (ostream& os) {
	  os<<"Node name : ProcedureDeclaration. Proc name is: "<<*name_<<endl;
	  assert(block_);
	  if (formal_list_){
		  formal_list_->print(os);
	  }
	  block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(block_);
      if (formal_list_) {
          formal_list_->pcodegen(os);
      }
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new ProcedureDeclaration(*this);}
  
private:
  Object * block_;
  Object * formal_list_;
  string * name_;
};

class DeclarationList : public Object {
public :
  DeclarationList (Object * decl) : decl_(decl), decl_list_(NULL) { assert(decl_);}
  DeclarationList (Object * decl_list, Object * decl) : decl_list_(decl_list),decl_(decl)  {assert(decl_list_ && decl);}

  DeclarationList(const DeclarationList& dl){
    decl_ = dl.decl_->clone();
    decl_list_ = dl.decl_list_->clone();
  }

  virtual ~DeclarationList () {
    if (decl_) delete decl_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : DeclarationList"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(decl_);
	  decl_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(decl_);
      decl_->pcodegen(os);
  }
  virtual Object * clone () const { return new DeclarationList(*this);}

private:
  Object * decl_;
  Object * decl_list_;
};

class Block : public Object {
public :
  Block (Object * stat_seq) : stat_seq_(stat_seq), decl_list_(NULL)  { assert(stat_seq_);}
  Block (Object * decl_list, Object * stat_seq) : decl_list_(decl_list), stat_seq_(stat_seq)  {assert(decl_list_ && stat_seq_);}

  Block(const Block& b){
    decl_list_ = b.decl_list_->clone();
    stat_seq_ = b.stat_seq_->clone();
  }

  virtual ~Block () {
    if (stat_seq_) delete stat_seq_;
    if (decl_list_) delete decl_list_;
  }
  
  void print (ostream& os) {
	  os<<"Node name : Begin"<<endl;
	  if (decl_list_){
		  decl_list_->print(os);
	  }
	  assert(stat_seq_);
	  stat_seq_->print(os);
  }
  void pcodegen(ostream& os) {
      if (decl_list_) {
          decl_list_->pcodegen(os);
      }
      assert(stat_seq_);
      stat_seq_->pcodegen(os);
  }

  virtual Object * clone () const { return new Block(*this);}
  
private:
  Object * decl_list_;
  Object * stat_seq_;
};

class Program : public Object {
public :
  Program (Object * block, const char * str) : block_(NULL) {
    block_ = dynamic_cast<Block *>(block);
    assert(block_);
    name_ = new string(str);
  }

  Program(const Program& prog){
    block_ = dynamic_cast<Block *>(prog.block_->clone());
    assert(block_);
    name_ = new string(*prog.name_);
  }

  virtual ~Program () {
    if (block_) delete block_;
    delete name_;
  }
  
  void print (ostream& os) {
    os<<"Node name : Root/Program. Program name is: "<<*name_<<endl;
	  assert(block_);
    block_->print(os);
  }
  void pcodegen(ostream& os) {
      assert(block_);
      block_->pcodegen(os);
  }
  virtual Object * clone () const { return new Program(*this);}
  
private:
  Block * block_;
  string * name_;
};



#endif //AST_H