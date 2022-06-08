#include "result.h"

Result::Result(QObject *parent)
    : QObject{parent}
{

}

Result::Result(const Result &ref):QObject(nullptr)
{
    this->code=ref.code;
    this->conf=ref.conf;
    this->msg=ref.msg;
    this->type=ref.type;
}

int Result::getCode() const
{
    return code;
}

void Result::setCode(int newCode)
{
    code = newCode;
}

const QString &Result::getMsg() const
{
    return msg;
}

void Result::setMsg(const QString &newMsg)
{
    msg = newMsg;
}

const QString &Result::getType() const
{
    return type;
}

void Result::setType(const QString &newType)
{
    type = newType;
}

const QString &Result::getConf() const
{
    return conf;
}

void Result::setConf(const QString &newConf)
{
    conf = newConf;
}

void Result::operator=(const Result &ref)
{
    this->code=ref.code;
    this->conf=ref.conf;
    this->msg=ref.msg;
    this->type=ref.type;
}
