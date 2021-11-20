#include "expression.h"

#include <stdexcept>
#include "math.h"
#include "../modules/module.h"

Type get_common_number_type(Expression *left, Expression *right)
{
    if (left->type == integer && right->type == integer)
    {
        return integer;
    }
    else if (left->is_numbery() && right->is_numbery())
    {
        return number;
    }
    else
    {
        throw std::runtime_error("invalid type for arithmetic operation");
    }
}

void check_number_types(Expression *left, Expression *right)
{
    if (!left->is_numbery() || !right->is_numbery())
    {
        throw std::runtime_error("invalid type for comparison");
    }
}

void check_boolean_types(Expression *left, Expression *right)
{
    if (left->type != boolean || !right->type != boolean)
    {
        throw std::runtime_error("invalid type for logical operation");
    }
}

bool Expression::evaluate_boolean()
{
    throw std::runtime_error("not implemented");
}

int Expression::evaluate_integer()
{
    throw std::runtime_error("not implemented");
}

double Expression::evaluate_number()
{
    throw std::runtime_error("not implemented");
}

std::string Expression::evaluate_identifier()
{
    throw std::runtime_error("not implemented");
}

std::string Expression::evaluate_string()
{
    throw std::runtime_error("not implemented");
}

bool Expression::is_numbery()
{
    return this->type == number || this->type == integer;
}

StringExpression::StringExpression(std::string value)
{
    this->type = string;
    this->value = value;
}

std::string StringExpression::evaluate_string()
{
    return this->value;
}

IntegerExpression::IntegerExpression(int value)
{
    this->type = integer;
    this->value = value;
}

int IntegerExpression::evaluate_integer()
{
    return this->value;
}

double IntegerExpression::evaluate_number()
{
    return this->value;
}

NumberExpression::NumberExpression(double value)
{
    this->type = number;
    this->value = value;
}

double NumberExpression::evaluate_number()
{
    return this->value;
}

VariableExpression::VariableExpression(Variable *variable)
{
    this->type = variable->type;
    this->variable = variable;
}

bool VariableExpression::evaluate_boolean()
{
    return this->type == boolean ? this->variable->boolean_value : Expression::evaluate_boolean();
}

int VariableExpression::evaluate_integer()
{
    return this->type == integer ? this->variable->integer_value : Expression::evaluate_integer();
}

double VariableExpression::evaluate_number()
{
    return this->type == number ? this->variable->number_value : Expression::evaluate_number();
}

std::string VariableExpression::evaluate_string()
{
    return this->type == string ? this->variable->string_value : Expression::evaluate_string();
}

PropertyExpression::PropertyExpression(Module *module, std::string property_name)
{
    this->type = number; // TODO: more flexible property types
    this->module = module;
    this->property_name = property_name;
}

double PropertyExpression::evaluate_number()
{
    return this->module->get(this->property_name);
}

PowerExpression::PowerExpression(Expression *left, Expression *right)
{
    this->type = get_common_number_type(left, right);
    this->left = left;
    this->right = right;
}

int PowerExpression::evaluate_integer()
{
    return pow(this->left->evaluate_integer(), this->right->evaluate_integer());
}

double PowerExpression::evaluate_number()
{
    return pow(this->left->evaluate_number(), this->right->evaluate_number());
}

NegateExpression::NegateExpression(Expression *operand)
{
    this->type = get_common_number_type(operand, operand);
    this->operand = operand;
}

int NegateExpression::evaluate_integer()
{
    return -this->operand->evaluate_integer();
}

double NegateExpression::evaluate_number()
{
    return -this->operand->evaluate_number();
}

MultiplyExpression::MultiplyExpression(Expression *left, Expression *right)
{
    this->type = get_common_number_type(left, right);
    this->left = left;
    this->right = right;
}

int MultiplyExpression::evaluate_integer()
{
    return this->left->evaluate_integer() * this->right->evaluate_integer();
}

double MultiplyExpression::evaluate_number()
{
    return this->left->evaluate_number() * this->right->evaluate_number();
}

DivideExpression::DivideExpression(Expression *left, Expression *right)
{
    this->type = get_common_number_type(left, right);
    this->left = left;
    this->right = right;
}

int DivideExpression::evaluate_integer()
{
    return this->left->evaluate_integer() / this->right->evaluate_integer();
}

double DivideExpression::evaluate_number()
{
    return this->left->evaluate_number() / this->right->evaluate_number();
}

AddExpression::AddExpression(Expression *left, Expression *right)
{
    this->type = get_common_number_type(left, right);
    this->left = left;
    this->right = right;
}

int AddExpression::evaluate_integer()
{
    return this->left->evaluate_integer() + this->right->evaluate_integer();
}

double AddExpression::evaluate_number()
{
    return this->left->evaluate_number() + this->right->evaluate_number();
}

SubtractExpression::SubtractExpression(Expression *left, Expression *right)
{
    this->type = get_common_number_type(left, right);
    this->left = left;
    this->right = right;
}

int SubtractExpression::evaluate_integer()
{
    return this->left->evaluate_integer() - this->right->evaluate_integer();
}

double SubtractExpression::evaluate_number()
{
    return this->left->evaluate_number() - this->right->evaluate_number();
}

GreaterExpression::GreaterExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool GreaterExpression::evaluate_boolean()
{
    return this->left->evaluate_number() > this->right->evaluate_number();
}

LessExpression::LessExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool LessExpression::evaluate_boolean()
{
    return this->left->evaluate_number() < this->right->evaluate_number();
}

GreaterEqualExpression::GreaterEqualExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool GreaterEqualExpression::evaluate_boolean()
{
    return this->left->evaluate_number() >= this->right->evaluate_number();
}

LessEqualExpression::LessEqualExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool LessEqualExpression::evaluate_boolean()
{
    return this->left->evaluate_number() <= this->right->evaluate_number();
}

EqualExpression::EqualExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool EqualExpression::evaluate_boolean()
{
    return this->left->evaluate_number() == this->right->evaluate_number();
}

UnequalExpression::UnequalExpression(Expression *left, Expression *right)
{
    check_number_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool UnequalExpression::evaluate_boolean()
{
    return this->left->evaluate_number() != this->right->evaluate_number();
}

NotExpression::NotExpression(Expression *operand)
{
    check_boolean_types(operand, operand);
    this->type = boolean;
    this->operand = operand;
}

bool NotExpression::evaluate_boolean()
{
    return !this->operand->evaluate_boolean();
}

AndExpression::AndExpression(Expression *left, Expression *right)
{
    check_boolean_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool AndExpression::evaluate_boolean()
{
    return this->left->evaluate_boolean() && this->right->evaluate_boolean();
}

OrExpression::OrExpression(Expression *left, Expression *right)
{
    check_boolean_types(left, right);
    this->type = boolean;
    this->left = left;
    this->right = right;
}

bool OrExpression::evaluate_boolean()
{
    return this->left->evaluate_boolean() || this->right->evaluate_boolean();
}