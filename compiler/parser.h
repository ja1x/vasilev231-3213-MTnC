#pragma once

#include "lexer.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

// Все типы узлов
enum class NodeType
{
    PROGRAM,
    FUNCTION_DECL,
    PARAM_DECL,
    BLOCK_STMT,
    VAR_DECL_STMT,
    ASSIGN_STMT,
    IF_STMT,
    WHILE_STMT,
    FOR_STMT,
    RETURN_STMT,
    EXPR_STMT,
    IDENTIFIER_EXPR,
    LITERAL_EXPR,
    BINARY_EXPR,
    UNARY_EXPR,
    CALL_EXPR,
    MEMBER_CALL_EXPR,
    ARRAY_ACCESS_EXPR
};

string nodeTypeName(NodeType t);

// Базовый класс ASTNode
struct ASTNode
{
    NodeType type;
    int      line = 0;

    explicit ASTNode(NodeType t, int ln = 0) : type(t), line(ln) {}
    virtual ~ASTNode() = default;

    virtual void print(const string& prefix = "", bool isLast = true) const = 0;
};

using NodePtr = shared_ptr<ASTNode>;

inline void printBranch(const string& prefix, bool isLast, const string& label)
{
    cout << prefix << (isLast ? "'-- " : "+-- ") << label << "\n";
}
inline string childPrefix(const string& prefix, bool isLast)
{
    return prefix + (isLast ? "    " : "|   ");
}

// Классы узлов AST
// Выражения
struct IdentifierExpr : ASTNode
{
    string name;
    IdentifierExpr(const string& n, int ln)
        : ASTNode(NodeType::IDENTIFIER_EXPR, ln), name(n) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct LiteralExpr : ASTNode
{
    string value;
    LiteralExpr(const string& v, int ln)
        : ASTNode(NodeType::LITERAL_EXPR, ln), value(v) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct BinaryExpr : ASTNode
{
    string  op;
    NodePtr left;
    NodePtr right;
    BinaryExpr(const string& o, NodePtr l, NodePtr r, int ln)
        : ASTNode(NodeType::BINARY_EXPR, ln), op(o),
        left(move(l)), right(move(r)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct UnaryExpr : ASTNode
{
    string  op;
    NodePtr operand;
    UnaryExpr(const string& o, NodePtr operand, int ln)
        : ASTNode(NodeType::UNARY_EXPR, ln), op(o), operand(move(operand)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct CallExpr : ASTNode
{
    string          callee;
    vector<NodePtr> arguments;
    CallExpr(const string& c, vector<NodePtr> args, int ln)
        : ASTNode(NodeType::CALL_EXPR, ln), callee(c), arguments(move(args)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct MemberCallExpr : ASTNode
{
    NodePtr         object;
    string          method;
    vector<NodePtr> arguments;
    MemberCallExpr(NodePtr obj, const string& m, vector<NodePtr> args, int ln)
        : ASTNode(NodeType::MEMBER_CALL_EXPR, ln),
        object(move(obj)), method(m), arguments(move(args)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct ArrayAccessExpr : ASTNode
{
    NodePtr array;
    NodePtr index;
    ArrayAccessExpr(NodePtr arr, NodePtr idx, int ln)
        : ASTNode(NodeType::ARRAY_ACCESS_EXPR, ln),
        array(move(arr)), index(move(idx)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

// Тело программы
struct BlockStmt : ASTNode
{
    vector<NodePtr> statements;
    explicit BlockStmt(int ln) : ASTNode(NodeType::BLOCK_STMT, ln) {}
    void print(const string& prefix, bool isLast) const override;
};

// Операторы
struct VarDeclStmt : ASTNode
{
    string  varType;
    string  name;
    NodePtr initExpr;
    VarDeclStmt(const string& t, const string& n, NodePtr init, int ln)
        : ASTNode(NodeType::VAR_DECL_STMT, ln),
        varType(t), name(n), initExpr(move(init)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct AssignStmt : ASTNode
{
    string  op;
    NodePtr left;
    NodePtr right;
    AssignStmt(const string& o, NodePtr l, NodePtr r, int ln)
        : ASTNode(NodeType::ASSIGN_STMT, ln), op(o),
        left(move(l)), right(move(r)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct IfStmt : ASTNode
{
    NodePtr condition;
    NodePtr thenBranch;
    NodePtr elseBranch;
    IfStmt(NodePtr cond, NodePtr thenB, NodePtr elseB, int ln)
        : ASTNode(NodeType::IF_STMT, ln),
        condition(move(cond)), thenBranch(move(thenB)), elseBranch(move(elseB)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct WhileStmt : ASTNode
{
    NodePtr condition;
    NodePtr body;
    WhileStmt(NodePtr cond, NodePtr body, int ln)
        : ASTNode(NodeType::WHILE_STMT, ln),
        condition(move(cond)), body(move(body)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct ForStmt : ASTNode
{
    NodePtr init;
    NodePtr condition;
    NodePtr update;
    NodePtr body;
    ForStmt(NodePtr i, NodePtr c, NodePtr u, NodePtr b, int ln)
        : ASTNode(NodeType::FOR_STMT, ln),
        init(move(i)), condition(move(c)), update(move(u)), body(move(b)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct ReturnStmt : ASTNode
{
    NodePtr value;
    ReturnStmt(NodePtr v, int ln)
        : ASTNode(NodeType::RETURN_STMT, ln), value(move(v)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct ExprStmt : ASTNode
{
    NodePtr expression;
    ExprStmt(NodePtr expr, int ln)
        : ASTNode(NodeType::EXPR_STMT, ln), expression(move(expr)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

// Объявления функций
struct ParamDecl : ASTNode
{
    string paramType;
    string name;
    ParamDecl(const string& t, const string& n, int ln)
        : ASTNode(NodeType::PARAM_DECL, ln), paramType(t), name(n) {
    }
    void print(const string& prefix, bool isLast) const override;
};

struct FunctionDecl : ASTNode
{
    string          returnType;
    string          name;
    vector<NodePtr> parameters;
    NodePtr         body;
    FunctionDecl(const string& rt, const string& n,
        vector<NodePtr> params, NodePtr b, int ln)
        : ASTNode(NodeType::FUNCTION_DECL, ln),
        returnType(rt), name(n), parameters(move(params)), body(move(b)) {
    }
    void print(const string& prefix, bool isLast) const override;
};

// Корневой узел
struct ProgramNode : ASTNode
{
    vector<NodePtr> functions;
    explicit ProgramNode(int ln) : ASTNode(NodeType::PROGRAM, ln) {}
    void print(const string& prefix, bool isLast) const override;
};

// Структура ошибки
struct ParseError
{
    string message;
    int    line;
};

// Класс анализатора
class Parser
{
public:
    explicit Parser(const vector<Token>& tokens);
    void parse();
    const vector<ParseError>& getErrors() const { return errors; }

private:
    const vector<Token>& tokens;
    size_t pos; // индекс текущего токена
    vector<ParseError> errors;

    const Token& cur() const;
    const Token& peek(int off = 1) const;
    bool   atEnd() const;
    Token  consume(); 
    bool   match(TokenType t, const string& val = "") const;
    Token  expect(TokenType t, const string& val = "");
    bool   isType(const string& v) const;

    void addError(const string& msg, int line);
    void skipTo(initializer_list<string> stopValues);
    void checkFunctionReturn(const FunctionDecl& fn);

    NodePtr parseProgram();
    NodePtr parseFunctionDecl();
    NodePtr parseParamDecl();
    NodePtr parseBlock();
    NodePtr parseStatement();
    NodePtr parseVarDeclStmt();
    NodePtr parseIfStmt();
    NodePtr parseWhileStmt();
    NodePtr parseForStmt();
    NodePtr parseReturnStmt();
    NodePtr parseExprStmt();
    NodePtr parseForInit();

    NodePtr parseExpr();
    NodePtr parseAssignExpr();
    NodePtr parseOrExpr();
    NodePtr parseAndExpr();
    NodePtr parseEqualityExpr();
    NodePtr parseRelExpr();
    NodePtr parseAddExpr();
    NodePtr parseMulExpr();
    NodePtr parseUnaryExpr();
    NodePtr parsePostfixExpr();
    NodePtr parsePrimaryExpr();

    vector<NodePtr> parseArgList();
};